#include <WiFi.h>
#include <WebServer.h>

// ============================================================
// 实验5: 简易 Web Server
// 功能：通过网页按钮控制板载 LED
// ============================================================

// --- 模式选择 ---
// 若要连接自家路由器，请填写 ssid/password 并确保手机也在同一个 WiFi
// 若不方便，程序将默认开启热点 (AP 模式)，手机连接 ESP32 即可
const char* ssid     = "你的WiFi名称"; 
const char* password = "你的WiFi密码";

// AP 模式配置 (热点模式)
const char* ap_ssid = "ESP32-Web-Lab";
const char* ap_pass = "12345678";

// 引脚定义
const int LED_PIN = 2; // 板载 LED

// 创建 Web 服务实例，端口 80
WebServer server(80);

// 生成网页内容
String makePage() {
    String state = digitalRead(LED_PIN) ? "点亮 (ON)" : "熄灭 (OFF)";
    String color = digitalRead(LED_PIN) ? "#00FF00" : "#FF0000";

    String html = R"rawliteral(
    <!DOCTYPE html>
    <html lang="zh-CN">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>ESP32 控制面板</title>
        <style>
            body { font-family: Arial, sans-serif; text-align: center; background-color: #f4f4f9; margin-top: 50px; }
            .container { background: white; padding: 30px; border-radius: 15px; display: inline-block; box-shadow: 0 4px 8px rgba(0,0,0,0.1); }
            h1 { color: #333; }
            .status { font-size: 1.5em; margin: 20px 0; }
            .btn { display: inline-block; padding: 15px 30px; font-size: 1.2em; color: white; border: none; border-radius: 8px; cursor: pointer; text-decoration: none; margin: 10px; transition: 0.3s; }
            .btn-on { background-color: #28a745; }
            .btn-off { background-color: #dc3545; }
            .btn:hover { opacity: 0.8; }
        </style>
    </head>
    <body>
        <div class="container">
            <h1>Lab 05: ESP32 控制面包</h1>
            <p class="status">当前 LED 状态: <b style="color: )rawliteral" + color + R"rawliteral(;">)rawliteral" + state + R"rawliteral(</b></p>
            <a href="/on" class="btn btn-on">点亮 LED</a>
            <a href="/off" class="btn btn-off">熄灭 LED</a>
        </div>
        <p style="margin-top:20px; color:#666;">访问地址: http://192.168.4.1 (若在 AP 模式)</p>
    </body>
    </html>
    )rawliteral";
    return html;
}

// 根目录处理函数
void handleRoot() {
    server.send(200, "text/html; charset=UTF-8", makePage());
}

// 处理 "点亮" 请求
void handleOn() {
    digitalWrite(LED_PIN, HIGH);
    server.sendHeader("Location", "/");
    server.send(303); // 重定向回首页
}

// 处理 "熄灭" 请求
void handleOff() {
    digitalWrite(LED_PIN, LOW);
    server.sendHeader("Location", "/");
    server.send(303);
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Serial.println("\n--- 实验5: 简易 Web Server ---");

    // 尝试开启热点模式 (最简单，无需配置)
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ap_ssid, ap_pass);
    
    Serial.println("热点已开启 (AP Mode)");
    Serial.print("WiFi 名称: "); Serial.println(ap_ssid);
    Serial.print("WiFi 密码: "); Serial.println(ap_pass);
    Serial.print("访问地址: http://"); Serial.println(WiFi.softAPIP());

    // 绑定路径
    server.on("/", handleRoot);
    server.on("/on", handleOn);
    server.on("/off", handleOff);

    // 启动服务
    server.begin();
    Serial.println("HTTP 服务器已启动");
}

void loop() {
    server.handleClient();
}