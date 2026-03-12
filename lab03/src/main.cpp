#include <Arduino.h>

// ============================================================
// 实验3: LED呼吸灯实验 - PWM控制
// 功能：通过PWM实现LED呼吸灯效果（渐亮渐暗）
// ============================================================

// 定义LED引脚
const int ledPin = 2;

// 设置PWM属性
const int pwmChannel = 0;    // PWM通道 0
const int freq = 5000;       // 频率 5000Hz
const int resolution = 8;    // 分辨率 8位 (0-255)

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("================================");
    Serial.println("实验3: LED呼吸灯实验 - PWM控制");
    Serial.println("================================");

    // 配置PWM通道
    ledcSetup(pwmChannel, freq, resolution);
    // 将LED引脚绑定到PWM通道
    ledcAttachPin(ledPin, pwmChannel);

    Serial.println("PWM 初始化完成");
    Serial.println("频率: 5000Hz, 分辨率: 8位 (0-255)\n");
}

void loop() {
    // 逐渐变亮
    Serial.println("渐亮...");
    for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
        ledcWrite(pwmChannel, dutyCycle);
        delay(15);
    }

    delay(300);  // 最亮停留

    // 逐渐变暗
    Serial.println("渐暗...");
    for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
        ledcWrite(pwmChannel, dutyCycle);
        delay(15);
    }

    delay(300);  // 最暗停留

    Serial.println("Breathing cycle completed\n");
}