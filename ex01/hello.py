# HelloWorld 程序 - 带键盘输入功能
# 功能1: 输出 Hello World
# 功能2: 使用 input() 函数实现键盘输入，显示输出键盘输入内容

# === 第一部分: HelloWorld 输出 ===
print("Hello, World!")
print("欢迎来到 Python 编程世界！")
print()

# === 第二部分: 使用 input 函数实现键盘输入 ===
# input() 函数用于从键盘读取用户输入
name = input("请输入你的名字: ")
print(f"你好, {name}! 欢迎学习 Python 编程！")
print()

age = input("请输入你的年龄: ")
print(f"{name}, 你今年 {age} 岁。")
print()

message = input("请输入一段话: ")
print(f"你输入的内容是: {message}")
print()

print("程序结束，感谢使用！")
