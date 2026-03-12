#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiManager.h> // 引入 WiFiManager 库

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
        HTTPClient http;
        http.begin(openclawUrl);
        http.addHeader("Content-Type", "application/json");
        http.addHeader("Authorization", String("Bearer ") + token);

        String jsonPayload = "{";
        jsonPayload += "\"text\": \"🚨 警报：您的 ESP32 触摸传感器被触发！数值: " + String(val) + "\",";
        jsonPayload += "\"sessionKey\": \"hook:esp32-dynamic-wifi\",";
        jsonPayload += "\"metadata\": { \"device\": \"ESP32-D0WD\", \"sensor\": \"touch\", \"val\": " + String(val) + " }";
        jsonPayload += "}";

        Serial.println("Pushing to OpenClaw...");
        int httpResponseCode = http.POST(jsonPayload);

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.printf("HTTP %d Success!\n", httpResponseCode);
            digitalWrite(LED_PIN, HIGH); delay(100); digitalWrite(LED_PIN, LOW); delay(100);
            digitalWrite(LED_PIN, HIGH); delay(100); digitalWrite(LED_PIN, LOW);
        } else {
            Serial.printf("HTTP Error: %s\n", http.errorToString(httpResponseCode).c_str());
        }
        http.end();
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Serial.println("\n--- Lab06: Dynamic WiFi Bridge ---");

    // ==========================================
    // 3. 使用 WiFiManager 替代硬编码 WiFi
    // ==========================================
    WiFiManager wm;

    // 如果 180 秒内没有连接上 WiFi，则重置 ESP32
    wm.setConfigPortalTimeout(180);

    // 尝试连接之前的 WiFi，如果失败，则启动一个 AP
    // AP 名称为 "ESP32-Lab06-Config"
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
        Serial.printf("Touch detected! (Value: %d)\n", touchValue);
        lastTriggerTime = millis();
        
        digitalWrite(LED_PIN, HIGH);
        sendToOpenClaw(touchValue);
        digitalWrite(LED_PIN, LOW);
    }
    
    delay(50);
}
