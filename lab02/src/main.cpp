#include <Arduino.h>

// ============================================================
// 实验2: 基础IO实验 - LED闪烁控制
// 功能：LED基础闪烁 + SOS求救信号
// ============================================================

// 定义LED引脚，ESP32板载LED连接在 GPIO2
#define LED_PIN 2
#define LED_R 18

// SOS 求救信号（摩尔斯电码：三短、三长、三短）
void playSOS() {
    Serial.println("=== SOS 求救信号 ===");

    // S: 短闪3次（点）
    Serial.print("S: ");
    for (int i = 0; i < 3; i++) {
        digitalWrite(LED_PIN, HIGH);
        Serial.print(".");
        delay(200);
        digitalWrite(LED_PIN, LOW);
        delay(200);
    }
    delay(500);  // 字母间隔

    // O: 长闪3次（划）
    Serial.print(" O: ");
    for (int i = 0; i < 3; i++) {
        digitalWrite(LED_PIN, HIGH);
        Serial.print("-");
        delay(600);
        digitalWrite(LED_PIN, LOW);
        delay(200);
    }
    delay(500);  // 字母间隔

    // S: 短闪3次（点）
    Serial.print(" S: ");
    for (int i = 0; i < 3; i++) {
        digitalWrite(LED_PIN, HIGH);
        Serial.print(".");
        delay(200);
        digitalWrite(LED_PIN, LOW);
        delay(200);
    }
    Serial.println();

    delay(2000);  // 单词间隔
}

void setup() {
    // 初始化串口通信，设置波特率为115200
    Serial.begin(115200);
    // 将LED引脚设置为输出模式
    pinMode(LED_PIN, OUTPUT);
    delay(1000);

    Serial.println("================================");
    Serial.println("实验2: 基础IO实验 - LED闪烁控制");
    Serial.println("================================");
    Serial.println("\n开始 LED 闪烁...");
    Serial.println("基础闪烁 5 次后进入 SOS 模式\n");
}

void loop() {
    // === 基础闪烁：LED亮1秒，灭1秒，循环5次 ===
    for (int i = 0; i < 5; i++) {
        digitalWrite(LED_PIN, HIGH);   // 点亮LED
        Serial.println("LED ON");
        delay(1000);                   // 保持1秒

        digitalWrite(LED_PIN, LOW);    // 熄灭LED
        Serial.println("LED OFF");
        delay(1000);                   // 保持1秒
    }

    Serial.println("\n--- 进入 SOS 模式 ---\n");

    // === 进阶实验：SOS 求救信号，循环3次 ===
    for (int i = 0; i < 3; i++) {
        playSOS();
    }

    Serial.println("\n--- 返回基础闪烁 ---\n");
}