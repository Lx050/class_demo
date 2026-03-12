#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiManager.h>

// ==========================================
// 1. OpenClaw 配置 (云服务器公网 IP)
// ==========================================
const String openclawIP = "115.190.2.253"; 
const String openclawUrl = "http://" + openclawIP + ":18789/hooks/esp32";

// 在 openclaw.json 中设置的用于验证的 Token
const char* token = "6638cdf0eb815b0defec637d0d1ee120d03ea99cc471415d5eb612f0640a6303"; 

// ==========================================
// 2. 硬件与阈值配置
// ==========================================
const int TOUCH_PIN = 4;    // 触摸引脚 D4 (Touch 0)
const int LED_PIN = 2;      // 板载 LED
const int THRESHOLD = 35;   // 触摸阈值 (越小越灵敏)

unsigned long lastTriggerTime = 0;
const unsigned long cooldownTime = 5000; // 5秒内不重复触发

void sendToOpenClaw(int val) {
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println(">>> Preparing HTTP request...");
        HTTPClient http;
        http.begin(openclawUrl);
        http.addHeader("Content-Type", "application/json");
        http.addHeader("Authorization", String("Bearer ") + token);

        // 构建符合 OpenClaw 格式的 JSON
        String jsonPayload = "{";
        jsonPayload += "\"text\": \"🚨 ESP32 触摸报警！监测值: " + String(val) + "\",";
        jsonPayload += "\"sessionKey\": \"hook:esp32-debug\",";
        jsonPayload += "\"metadata\": { \"device\": \"ESP32-D0WD\", \"sensor\": \"touch\", \"val\": " + String(val) + " }";
        jsonPayload += "}";

        Serial.println(">>> Sending POST to: " + openclawUrl);
        int httpResponseCode = http.POST(jsonPayload);

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.printf(">>> HTTP Status: %d\n", httpResponseCode);
            Serial.println(">>> Server Response: " + response);
            
            if (httpResponseCode == 200 || httpResponseCode == 201) {
                // 成功：快闪三次
                for(int i=0; i<3; i++) {
                    digitalWrite(LED_PIN, HIGH); delay(100);
                    digitalWrite(LED_PIN, LOW); delay(100);
                }
            } else {
                // 失败：长亮 1 秒提示
                digitalWrite(LED_PIN, HIGH); delay(1000);
                digitalWrite(LED_PIN, LOW);
                Serial.println(">>> Tip: Check if the Token or Mapping is correct in OpenClaw.");
            }
        } else {
            Serial.printf(">>> HTTP Failed! Error: %s\n", http.errorToString(httpResponseCode).c_str());
            Serial.println(">>> Tip: Check Firewall or Port forwarding on Cloud Server.");
        }
        http.end();
    } else {
        Serial.println(">>> Error: WiFi NOT connected!");
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Serial.println("\n--- Lab06: OpenClaw Bridge (DEBUG MODE) ---");

    WiFiManager wm;
    // wm.resetSettings(); // 如果已经连上了，可以注释掉这行来加快启动过程

    wm.setConfigPortalTimeout(180);

    if (!wm.autoConnect("ESP32-Lab06-Config")) {
        Serial.println("Failed to connect or hit timeout");
        ESP.restart();
    }

    Serial.println("\nWiFi Connected!");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
    
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
}

void loop() {
    int touchValue = touchRead(TOUCH_PIN);
    
    if (touchValue < THRESHOLD && (millis() - lastTriggerTime > cooldownTime)) {
        Serial.printf("\n--- Touch Triggered! Value: %d ---\n", touchValue);
        lastTriggerTime = millis();
        digitalWrite(LED_PIN, HIGH);
        sendToOpenClaw(touchValue);
        digitalWrite(LED_PIN, LOW);
    }
    
    delay(50);
}
