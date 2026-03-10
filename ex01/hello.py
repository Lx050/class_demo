"""
HelloWorld 程序 - 用户注册登录系统 (命令行版)

功能说明：
  1. 输出 Hello World 欢迎信息
  2. 使用 input() 函数实现键盘输入
  3. 用户注册：输入用户名、密码、确认密码
  4. 密码强度校验：长度>=6、包含大写字母、包含数字
  5. 用户登录：输入用户名、密码进行身份验证

运行方式：
  cd ex01
  python hello.py
"""

# === 用户数据存储（使用字典模拟数据库） ===
user_database = {}


def validate_password(password):
    """
    密码强度校验函数
    检查密码是否满足以下规则：
      1. 长度至少 6 位
      2. 包含至少一个大写字母
      3. 包含至少一个数字

    参数:
        password (str): 待校验的密码字符串

    返回:
        list: 未通过的校验规则描述列表，为空表示密码合格
    """
    errors = []

    # 规则1：检查密码长度
    if len(password) < 6:
        errors.append("长度至少6位")

    # 规则2：检查是否包含大写字母
    has_upper = any(c.isupper() for c in password)
    if not has_upper:
        errors.append("需包含至少一个大写字母")

    # 规则3：检查是否包含数字
    has_digit = any(c.isdigit() for c in password)
    if not has_digit:
        errors.append("需包含至少一个数字")

    return errors


def register():
    """
    用户注册功能
    通过 input() 函数从键盘读取用户名和密码
    将注册信息存储到 user_database 字典中
    """
    print("\n===== 用户注册 =====")

    # 键盘输入：读取用户名
    username = input("请输入用户名: ")

    # 检查用户名是否为空
    if not username.strip():
        print("注册失败: 用户名不能为空！")
        return

    # 检查用户名是否已存在
    if username in user_database:
        print("注册失败: 用户名已存在！")
        return

    # 键盘输入：读取密码
    password = input("请输入密码: ")

    # 键盘输入：读取确认密码
    confirm_password = input("请再次输入密码: ")

    # 校验两次密码是否一致
    if password != confirm_password:
        print("注册失败: 两次输入的密码不一致！")
        return

    # 校验密码强度
    password_errors = validate_password(password)
    if password_errors:
        print("注册失败: 密码不符合要求:")
        for error in password_errors:
            print(f"  - {error}")
        return

    # 注册成功：将用户信息存入字典
    user_database[username] = password
    print(f"注册成功！欢迎 {username}")


def login():
    """
    用户登录功能
    通过 input() 函数从键盘读取用户名和密码
    验证输入的用户名和密码是否与存储的数据匹配
    """
    print("\n===== 用户登录 =====")

    # 键盘输入：读取用户名
    username = input("请输入用户名: ")

    # 校验：用户是否存在
    if username not in user_database:
        print("登录失败: 用户不存在，请先注册！")
        return

    # 键盘输入：读取密码
    password = input("请输入密码: ")

    # 校验：密码是否正确
    if user_database[username] != password:
        print("登录失败: 密码错误！")
        return

    # 登录成功
    print(f"登录成功！欢迎回来, {username}")


# ============================================================
# 主程序入口 - Main Program
# ============================================================
print("Hello, World!")
print("欢迎使用用户注册登录系统 (命令行版)")

while True:
    # 显示菜单选项
    print("\n请选择操作:")
    print("  1. 注册")
    print("  2. 登录")
    print("  3. 退出")

    # 键盘输入：读取用户选择
    choice = input("请输入选项 (1/2/3): ")

    if choice == "1":
        register()
    elif choice == "2":
        login()
    elif choice == "3":
        print("再见！感谢使用。")
        break
    else:
        # 输出：提示无效输入
        print("无效选项，请重新输入")
