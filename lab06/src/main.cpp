#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

// ==========================================
// 1. 基础联网配置 (请修改)
// ==========================================
const char* ssid = "YOUR_WIFI_SSID";         // 您的 WiFi 名称
const char* password = "YOUR_WIFI_PASSWORD"; // 您的 WiFi 密码

// ==========================================
// 2. OpenClaw 配置 (请根据您的电脑 IP 修改)
// ==========================================
// 注意：必须填写您电脑的【局域网 IP】，例如 "192.168.1.100"
// 您可以在电脑命令行输入 `ipconfig` 查看 IPv4 地址
const String openclawIP = "192.168.x.x"; 
const String openclawUrl = "http://" + openclawIP + ":18789/hooks/esp32";

// 在 openclaw.json 中设置的用于验证的 Token
const char* token = "your-secret-token"; 

// ==========================================
// 3. 硬件与阈值配置
// ==========================================
const int TOUCH_PIN = 4;    // 触摸引脚 D4 (Touch 0)
const int LED_PIN = 2;      // 板载 LED
const int THRESHOLD = 35;   // 触摸阈值 (越小越灵敏)

unsigned long lastTriggerTime = 0;
const unsigned long cooldownTime = 5000; // 5秒内不重复触发

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // 连接 WiFi
    Serial.println("\n--- Lab06: OpenClaw Bridge ---");
    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // 闪烁表示连接中
    }

    Serial.println("\nWiFi Connected!");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
    
    digitalWrite(LED_PIN, HIGH); // 联网成功后常亮一秒
    delay(1000);
    digitalWrite(LED_PIN, LOW);
}

void sendToOpenClaw(int val) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(openclawUrl);
        
        // 关键：添加 OpenClaw 要求的请求头
        http.addHeader("Content-Type", "application/json");
        http.addHeader("Authorization", String("Bearer ") + token);

        // 构建符合 OpenClaw 格式的 JSON
        // text: 展示在飞书上的消息
        // sessionKey: 标识设备
        String jsonPayload = "{";
        jsonPayload += "\"text\": \"🚨 警报：您的 ESP32 触摸传感器被触发！数值: " + String(val) + "\",";
        jsonPayload += "\"sessionKey\": \"hook:esp32-demo\",";
        jsonPayload += "\"metadata\": { \"device\": \"ESP32-D0WD\", \"sensor\": \"touch\", \"val\": " + String(val) + " }";
        jsonPayload += "}";

        Serial.println("Pushing to OpenClaw...");
        int httpResponseCode = http.POST(jsonPayload);

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.printf("HTTP %d Success!\n", httpResponseCode);
            Serial.println("OpenClaw Says: " + response);
            
            // 成功提示信号：快闪两次
            digitalWrite(LED_PIN, HIGH); delay(100); digitalWrite(LED_PIN, LOW); delay(100);
            digitalWrite(LED_PIN, HIGH); delay(100); digitalWrite(LED_PIN, LOW);
        } else {
            Serial.printf("HTTP Error: %s\n", http.errorToString(httpResponseCode).c_str());
            Serial.println("请检查：1.电脑IP是否正确  2.OpenClaw是否开启了 bind: lan");
        }
        http.end();
    } else {
        Serial.println("Error: WiFi lost!");
    }
}

void loop() {
    int touchValue = touchRead(TOUCH_PIN);
    
    // 逻辑：如果触摸值低于阈值（表示被碰），且过了冷却时间
    if (touchValue < THRESHOLD && (millis() - lastTriggerTime > cooldownTime)) {
        Serial.printf("Touch detected! (Value: %d)\n", touchValue);
        lastTriggerTime = millis();
        
        digitalWrite(LED_PIN, HIGH); // 处理中灯亮
        sendToOpenClaw(touchValue);
        digitalWrite(LED_PIN, LOW);
    }
    
    delay(50); // 提高采样频率到 20Hz
}
