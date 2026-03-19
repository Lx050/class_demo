#include <Arduino.h>


// ============================================================
// 实验3: LED呼吸灯实验 - PWM控制
// 功能：通过PWM实现LED呼吸灯效果（渐亮渐暗）
// ============================================================
#define LED_PIN 2
#define LED_R 18
#define LED_G 22
#define LED_Y 23
// 定义LED引脚（分别命名，避免重定义）
const int ledPinR = LED_R;
const int ledPinG = LED_G;
const int ledPinY = LED_Y;
// 设置PWM属性：每个LED使用单独通道
const int pwmChannelR = 0;    // PWM通道 0
const int pwmChannelG = 1;    // PWM通道 1
const int pwmChannelY = 2;    // PWM通道 2
const int freq = 5000;       // 频率 5000Hz
const int resolution = 8;    // 分辨率 8位 (0-255)

// 亮度缩放系数：用于补偿不同颜色LED的感知亮度差
constexpr float scaleR = 1.6f; // 可根据实际情况微调
constexpr float scaleG = 1.5f;
constexpr float scaleY = 1.0f;

// 将线性占空比乘以缩放并裁剪到最大值
static inline int scaledDuty(int duty, float scale) {
    const int maxDuty = (1 << resolution) - 1;
    int v = int(duty * scale + 0.5f);
    if (v > maxDuty) v = maxDuty;
    if (v < 0) v = 0;
    return v;
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("================================");
    Serial.println("实验3: LED呼吸灯实验 - PWM控制");
    Serial.println("================================");

    // 配置PWM通道并绑定到对应引脚
    ledcSetup(pwmChannelR, freq, resolution);
    ledcSetup(pwmChannelG, freq, resolution);
    ledcSetup(pwmChannelY, freq, resolution);
    ledcAttachPin(ledPinR, pwmChannelR);
    ledcAttachPin(ledPinG, pwmChannelG);
    ledcAttachPin(ledPinY, pwmChannelY);

    Serial.println("PWM 初始化完成");
    Serial.println("频率: 5000Hz, 分辨率: 8位 (0-255)\n");
}

void loop() {
    // 分别呼吸：每次只有一颗LED做呼吸效果，其他两颗保持熄灭
    // breatheChannel: channel, scale, stepDelay(ms per step), holdPeakMs
    auto breatheChannel = [](int channel, float scale, int stepDelay, int holdPeakMs) {
        Serial.println("渐亮...");
        for (int duty = 0; duty <= 255; duty++) {
            ledcWrite(channel, scaledDuty(duty, scale));
            delay(stepDelay);
        }
        delay(holdPeakMs);
        Serial.println("渐暗...");
        for (int duty = 255; duty >= 0; duty--) {
            ledcWrite(channel, scaledDuty(duty, scale));
            delay(stepDelay);
        }
        delay(200);
    };

    // LED R 单独呼吸
    ledcWrite(pwmChannelG, 0);
    ledcWrite(pwmChannelY, 0);
    Serial.println("LED R 呼吸...");
    // 红色视觉偏暗：放慢步进并延长峰值停留
    breatheChannel(pwmChannelR, scaleR, 10, 800);

    // LED G 单独呼吸
    ledcWrite(pwmChannelR, 0);
    ledcWrite(pwmChannelY, 0);
    Serial.println("LED G 呼吸...");
    // 绿色也放慢并延长峰值
    breatheChannel(pwmChannelG, scaleG, 10, 800);

    // LED Y 单独呼吸
    ledcWrite(pwmChannelR, 0);
    ledcWrite(pwmChannelG, 0);
    Serial.println("LED Y 呼吸...");
    // 黄色保持原节奏
    breatheChannel(pwmChannelY, scaleY, 8, 300);

    Serial.println("Breathing sequence completed\n");
}