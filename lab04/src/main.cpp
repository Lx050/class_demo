#include <Arduino.h>

// ============================================================
// 实验4: 触摸引脚实验
// 功能：使用触摸引脚 (GPIO4) 控制板载 LED (GPIO2)
// ============================================================

// 定义触摸引脚 (T0 对应 GPIO4)
#define TOUCH_PIN 4
// 定义 LED 引脚 (板载 LED 通常是 GPIO2)
#define LED_PIN 2

// 触摸阈值 - 需要根据实际环境调整（触摸时值会减小）
// 建议：触摸杜邦线时观察串口输出的值，设定一个略高于触摸状态的值
int threshold = 30;
int touchValue;

void setup() {
  // 初始化串口通信，波特率 115200
  Serial.begin(115200);
  delay(1000); // 等待串口稳定
  
  // 初始化 LED 引脚为输出模式
  pinMode(LED_PIN, OUTPUT);
  
  Serial.println("================================");
  Serial.println("实验4: 触摸引脚控制 LED");
  Serial.println("================================");
  Serial.print("检测引脚: GPIO");
  Serial.println(TOUCH_PIN);
  Serial.print("预设阈值: ");
  Serial.println(threshold);
  Serial.println("请触摸 GPIO4 引脚对应的杜邦线...\n");
}

void loop() {
  // 读取触摸传感器的数值 (0-100 左右，触摸时值减小)
  touchValue = touchRead(TOUCH_PIN);
  
  // 定期在串口打印当前的触摸值，方便调试阈值
  Serial.print("当前触摸值: ");
  Serial.println(touchValue);
  
  // 阈值判断逻辑
  if (touchValue < threshold) {
    // 当触摸值小于阈值时，认为被触摸，点亮 LED
    digitalWrite(LED_PIN, HIGH);
    Serial.println("状态: 已触摸 [LED ON]");
  } else {
    // 否则熄灭 LED
    digitalWrite(LED_PIN, LOW);
    Serial.println("状态: 未触摸 [LED OFF]");
  }
  
  // 适当延时，避免串口输出过快
  delay(200);
}