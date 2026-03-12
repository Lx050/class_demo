#include <Arduino.h>
#include <map>
#include <string>

// 定义板载LED引脚
#define LED_PIN 2

// 用户数据库（使用 std::map 存储，键为用户名，值为密码）
std::map<std::string, std::string> user_database;

// 密码强度校验函数
bool validate_password(std::string password, std::string &error_msg) {
    if (password.length() < 6) {
        error_msg = "长度至少6位";
        return false;
    }
    
    bool has_upper = false;
    bool has_digit = false;
    
    for (char c : password) {
        if (isupper(c)) has_upper = true;
        if (isdigit(c)) has_digit = true;
    }
    
    if (!has_upper) {
        error_msg = "需包含至少一个大写字母";
        return false;
    }
    
    if (!has_digit) {
        error_msg = "需包含至少一个数字";
        return false;
    }
    
    return true;
}

// 串口读取字符串函数
std::string readString() {
    while (!Serial.available()) {
        delay(10);
    }
    std::string input = "";
    while (Serial.available()) {
        char c = Serial.read();
        if (c == '\n' || c == '\r') {
            delay(10); // 等待可能的 \r\n
            while(Serial.available() && (Serial.peek() == '\n' || Serial.peek() == '\r')) Serial.read();
            break;
        }
        input += c;
        delay(2);
    }
    return input;
}

void register_user() {
    Serial.println("\n===== 用户注册 =====");
    Serial.print("请输入用户名: ");
    std::string username = readString();
    Serial.println(username.c_str());

    if (username.empty()) {
        Serial.println("注册失败: 用户名不能为空！");
        return;
    }

    if (user_database.find(username) != user_database.end()) {
        Serial.println("注册失败: 用户名已存在！");
        return;
    }

    Serial.print("请输入密码: ");
    std::string password = readString();
    Serial.println("******");

    Serial.print("请再次输入密码: ");
    std::string confirm_password = readString();
    Serial.println("******");

    if (password != confirm_password) {
        Serial.println("注册失败: 两次输入的密码不一致！");
        return;
    }

    std::string error_msg;
    if (!validate_password(password, error_msg)) {
        Serial.print("注册失败: 密码不符合要求: ");
        Serial.println(error_msg.c_str());
        return;
    }

    user_database[username] = password;
    Serial.print("注册成功！欢迎 ");
    Serial.println(username.c_str());
}

void login_user() {
    Serial.println("\n===== 用户登录 =====");
    Serial.print("请输入用户名: ");
    std::string username = readString();
    Serial.println(username.c_str());

    if (user_database.find(username) == user_database.end()) {
        Serial.println("登录失败: 用户不存在，请先注册！");
        return;
    }

    Serial.print("请输入密码: ");
    std::string password = readString();
    Serial.println("******");

    if (user_database[username] != password) {
        Serial.println("登录失败: 密码错误！");
        return;
    }

    Serial.print("登录成功！欢迎回来, ");
    Serial.println(username.c_str());
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    delay(1000);
    Serial.println("Hello, World!");
    Serial.println("欢迎使用 ESP32 用户注册登录系统");
}

void loop() {
    Serial.println("\n请选择操作:");
    Serial.println("  1. 注册");
    Serial.println("  2. 登录");
    Serial.println("  3. 退出");
    Serial.print("请输入选项 (1/2/3): ");

    std::string choice = readString();
    Serial.println(choice.c_str());

    if (choice == "1") {
        register_user();
    } else if (choice == "2") {
        login_user();
    } else if (choice == "3") {
        Serial.println("再见！感谢使用。");
        while (true) {
            digitalWrite(LED_PIN, HIGH);
            delay(500);
            digitalWrite(LED_PIN, LOW);
            delay(500);
        }
    } else {
        Serial.println("无效选项，请重新输入");
    }
}