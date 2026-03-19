#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiManager.h>

// ==========================================
// 1. OpenClaw 配置 (云服务器公网 IP)
// ==========================================
const String openclawIP = "115.190.2.253"; 
const String openclawUrl = "http://" + openclawIP + ":18789/hooks/esp32";
const char* token = "6638cdf0eb815b0defec637d0d1ee120d03ea99cc471415d5eb612f0640a6303"; 

// ==========================================
// 2. 硬件与阈值配置
// ==========================================
const int TOUCH_PIN = 4;    // 触摸引脚 D4 (Touch 0)
const int LED_PIN = 2;      // 板载 LED
const int THRESHOLD = 35;   // 触摸阈值 (越小越灵敏)

unsigned long lastTriggerTime = 0;
const unsigned long cooldownTime = 5000; 

// 用于输出调试数值的变量
unsigned long lastDebugTime = 0;

void sendToOpenClaw(int val) {
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n>>> [HTTP] Launching request to OpenClaw...");
        HTTPClient http;
        http.begin(openclawUrl);
        http.addHeader("Content-Type", "application/json");
        http.addHeader("Authorization", String("Bearer ") + token);

        String jsonPayload = "{";
        jsonPayload += "\"text\": \"🚨 警报：ESP32 触摸报警！监测值: " + String(val) + "\",";
        jsonPayload += "\"sessionKey\": \"hook:esp32-final\",";
        jsonPayload += "\"metadata\": { \"device\": \"ESP32-D0WD\", \"sensor\": \"touch\", \"val\": " + String(val) + " }";
        jsonPayload += "}";

        int httpResponseCode = http.POST(jsonPayload);

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.printf(">>> [HTTP] Code: %d\n", httpResponseCode);
            Serial.println(">>> [HTTP] Response: " + response);
            
            if (httpResponseCode == 200 || httpResponseCode == 201) {
                // 成功快闪
                for(int i=0; i<3; i++) {
                    digitalWrite(LED_PIN, HIGH); delay(100);
                    digitalWrite(LED_PIN, LOW); delay(100);
                }
            }
        } else {
            Serial.printf(">>> [HTTP] Failed! Error: %s\n", http.errorToString(httpResponseCode).c_str());
        }
        http.end();
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Serial.println("\n--- Lab06: OpenClaw Bridge (FINAL VERIFY) ---");

    WiFiManager wm;
    // wm.resetSettings(); // <-- 已经注释掉，防止每次都进入配网模式

    wm.setConfigPortalTimeout(180);

    if (!wm.autoConnect("ESP32-Lab06-Config")) {
        Serial.println("Failed to connect, restarting...");
        delay(3000);
        ESP.restart();
    }

    Serial.println("\nWiFi Connected! Ready for touch trigger.");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
    
    // 成功连接后亮灯提示
    digitalWrite(LED_PIN, HIGH); delay(1000); digitalWrite(LED_PIN, LOW);
}

void loop() {
    int touchValue = touchRead(TOUCH_PIN);
    
    // 每 500ms 输出一次当前数值，帮你调试灵敏度
    if (millis() - lastDebugTime > 500) {
        Serial.printf("Touch Raw Value: %d (Threshold: %d)\n", touchValue, THRESHOLD);
        lastDebugTime = millis();
    }
    
    // 触发逻辑
    if (touchValue < THRESHOLD && (millis() - lastTriggerTime > cooldownTime)) {
        Serial.printf("\n*** TRIGGER! Low value detected: %d ***\n", touchValue);
        lastTriggerTime = millis();
        
        digitalWrite(LED_PIN, HIGH);
        sendToOpenClaw(touchValue);
        digitalWrite(LED_PIN, LOW);
    }
    
    delay(50);
}
