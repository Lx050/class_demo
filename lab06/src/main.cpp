#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

// ==========================================
// 请修改以下配置以适配您的环境
// ==========================================
const char* ssid = "YOUR_WIFI_SSID";         // 您的 WiFi 名称
const char* password = "YOUR_WIFI_PASSWORD"; // 您的 WiFi 密码

// 飞书机器人或 OpenClaw 的 Webhook 地址
// 格式通常为: https://open.feishu.cn/open-apis/bot/v2/hook/xxxxxxxxxxxx
const char* webhook_url = "YOUR_WEBHOOK_URL_HERE";

// ==========================================
// 硬件引脚定义
// ==========================================
const int TOUCH_PIN = 4;    // 触摸引脚 D4 (Touch 0)
const int LED_PIN = 2;      // 板载 LED
const int THRESHOLD = 30;   // 触摸阈值 (越小越灵敏)

// 运行状态变量
unsigned long lastTriggerTime = 0;
const unsigned long cooldownTime = 5000; // 5秒冷却时间，防止消息刷屏

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // 连接 WiFi
    Serial.println("\nConnecting to WiFi...");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        // 闪烁 LED 表示正在后台尝试连接
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }

    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
    // 连接成功后常亮一秒提示
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
}

void sendFeishuNotification() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(webhook_url);
        http.addHeader("Content-Type", "application/json");

        // 构建飞书机器人消息格式 (文本消息)
        String jsonPayload = "{\"msg_type\":\"text\",\"content\":{\"text\":\"📢 实验报告：小龙虾实验室检测到物理触摸！主人，有人在摸我的 ESP32！🦞\"}}";

        Serial.println("Sending notification to Feishu...");
        int httpResponseCode = http.POST(jsonPayload);

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.print("HTTP Success, Response Code: ");
            Serial.println(httpResponseCode);
            Serial.println("Response: " + response);
            
            // 发送成功，快闪三次
            for(int i=0; i<3; i++) {
                digitalWrite(LED_PIN, HIGH); delay(100);
                digitalWrite(LED_PIN, LOW); delay(100);
            }
        } else {
            Serial.print("Error sending POST: ");
            Serial.println(httpResponseCode);
        }
        http.end();
    } else {
        Serial.println("WiFi Disconnected!");
    }
}

void loop() {
    int touchValue = touchRead(TOUCH_PIN);
    
    // 如果数值低于阈值且不在冷却时间内
    if (touchValue < THRESHOLD && (millis() - lastTriggerTime > cooldownTime)) {
        Serial.printf("Touch detected! Value: %d\n", touchValue);
        
        lastTriggerTime = millis();
        
        // 简单指示灯，表示正在处理
        digitalWrite(LED_PIN, HIGH);
        
        sendFeishuNotification();
        
        digitalWrite(LED_PIN, LOW);
    }

    // 每隔 100ms 采样一次
    delay(100);
}
