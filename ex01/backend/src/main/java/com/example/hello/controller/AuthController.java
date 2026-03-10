package com.example.hello.controller;

import org.springframework.web.bind.annotation.*;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * 用户认证控制器 - Authentication Controller
 *
 * 提供用户注册和登录的 REST API 接口
 * 使用 HashMap 在内存中存储用户数据（仅用于课堂演示）
 *
 * API 接口列表：
 *   POST /api/auth/register  - 用户注册（含密码强度校验）
 *   POST /api/auth/login     - 用户登录
 */
@RestController
@RequestMapping("/api/auth")
@CrossOrigin(origins = "http://localhost:5173")
public class AuthController {

    /**
     * 用户存储 - User Storage
     * 使用 HashMap 模拟数据库，key 为用户名，value 为密码
     * 注意：实际项目中应使用数据库，并对密码进行加密存储
     */
    private final Map<String, String> userStore = new HashMap<>();

    /**
     * 用户注册接口 - Register Endpoint
     *
     * 接收 JSON 格式的请求体，包含 username 和 password 字段
     * 校验用户名是否为空、是否已存在
     *
     * @param request 包含 username 和 password 的 Map
     * @return 包含 success 状态和 message 提示的 Map
     *
     * 请求示例：
     *   POST /api/auth/register
     *   { "username": "张三", "password": "123456" }
     *
     * 成功响应：{ "success": true, "message": "注册成功！欢迎 张三" }
     * 失败响应：{ "success": false, "message": "用户名已存在" }
     */
    @PostMapping("/register")
    public Map<String, Object> register(@RequestBody Map<String, String> request) {
        // 从请求体中获取用户名和密码
        String username = request.getOrDefault("username", "").trim();
        String password = request.getOrDefault("password", "");

        // 校验：用户名不能为空
        if (username.isEmpty()) {
            return Map.of("success", false, "message", "用户名不能为空");
        }

        // 校验：密码不能为空
        if (password.isEmpty()) {
            return Map.of("success", false, "message", "密码不能为空");
        }

        // 校验：密码强度检查
        List<String> passwordErrors = validatePassword(password);
        if (!passwordErrors.isEmpty()) {
            String errorMessage = "密码不符合要求: " + String.join("; ", passwordErrors);
            return Map.of("success", false, "message", errorMessage);
        }

        // 校验：用户名是否已被注册
        if (userStore.containsKey(username)) {
            return Map.of("success", false, "message", "用户名已存在");
        }

        // 将用户信息存入内存（模拟数据库写入）
        userStore.put(username, password);

        return Map.of("success", true, "message", "注册成功！欢迎 " + username);
    }

    /**
     * 密码强度校验方法 - Password Validation
     *
     * 校验规则：
     *   1. 密码长度至少 6 位
     *   2. 必须包含至少一个大写字母
     *   3. 必须包含至少一个数字
     *
     * @param password 待校验的密码字符串
     * @return 错误信息列表，为空表示密码合格
     */
    private List<String> validatePassword(String password) {
        List<String> errors = new ArrayList<>();

        // 规则1：检查密码长度
        if (password.length() < 6) {
            errors.add("长度至少6位");
        }

        // 规则2：检查是否包含大写字母
        boolean hasUpperCase = false;
        for (char c : password.toCharArray()) {
            if (Character.isUpperCase(c)) {
                hasUpperCase = true;
                break;
            }
        }
        if (!hasUpperCase) {
            errors.add("需包含至少一个大写字母");
        }

        // 规则3：检查是否包含数字
        boolean hasDigit = false;
        for (char c : password.toCharArray()) {
            if (Character.isDigit(c)) {
                hasDigit = true;
                break;
            }
        }
        if (!hasDigit) {
            errors.add("需包含至少一个数字");
        }

        return errors;
    }

    /**
     * 用户登录接口 - Login Endpoint
     *
     * 接收 JSON 格式的请求体，验证用户名和密码是否匹配
     *
     * @param request 包含 username 和 password 的 Map
     * @return 包含 success 状态和 message 提示的 Map
     *
     * 请求示例：
     *   POST /api/auth/login
     *   { "username": "张三", "password": "123456" }
     *
     * 成功响应：{ "success": true, "message": "登录成功！欢迎回来, 张三" }
     * 失败响应：{ "success": false, "message": "密码错误" }
     */
    @PostMapping("/login")
    public Map<String, Object> login(@RequestBody Map<String, String> request) {
        // 从请求体中获取用户名和密码
        String username = request.getOrDefault("username", "").trim();
        String password = request.getOrDefault("password", "");

        // 校验：用户是否存在
        if (!userStore.containsKey(username)) {
            return Map.of("success", false, "message", "用户不存在，请先注册");
        }

        // 校验：密码是否正确
        if (!userStore.get(username).equals(password)) {
            return Map.of("success", false, "message", "密码错误");
        }

        // 登录成功
        return Map.of("success", true, "message", "登录成功！欢迎回来, " + username);
    }
}
