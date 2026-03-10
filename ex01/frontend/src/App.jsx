import { useState } from 'react'
import './App.css'

/* ============================================================
 * API 基础地址 - Backend API Base URL
 * 本地开发时 Spring Boot 后端运行在 8080 端口
 * ============================================================ */
const API_BASE = 'http://localhost:8080/api'

/* ============================================================
 * 页面常量 - Page Constants
 * 用于控制当前显示哪个页面
 * ============================================================ */
const PAGE_HOME = 'home'         // 首页
const PAGE_REGISTER = 'register' // 注册页
const PAGE_LOGIN = 'login'       // 登录页

/**
 * App 主组件
 * 实现了一个包含首页、注册、登录功能的单页应用
 * 使用 useState 管理页面路由和表单数据
 */
function App() {
  /* ---------- 页面导航状态 ---------- */
  const [currentPage, setCurrentPage] = useState(PAGE_HOME)

  /* ---------- 用户登录状态 ---------- */
  const [loggedInUser, setLoggedInUser] = useState(null)

  /* ---------- 全局提示消息 ---------- */
  const [message, setMessage] = useState({ text: '', type: '' })

  /* ---------- API 连接模式检测 ---------- */
  const [apiMode, setApiMode] = useState('detecting')

  /* ---------- Hello World 消息 ---------- */
  const [helloMessage, setHelloMessage] = useState('')

  /* ---------- 注册表单：用户名、密码、确认密码 ---------- */
  const [registerUsername, setRegisterUsername] = useState('')
  const [registerPassword, setRegisterPassword] = useState('')
  const [registerConfirm, setRegisterConfirm] = useState('')

  /* ---------- 登录表单：用户名、密码 ---------- */
  const [loginUsername, setLoginUsername] = useState('')
  const [loginPassword, setLoginPassword] = useState('')

  /**
   * 密码强度校验 - Password Strength Validation
   * 检查密码是否满足以下规则：
   *   1. 长度至少 6 位
   *   2. 包含至少一个大写字母
   *   3. 包含至少一个数字
   *
   * @param {string} password - 待校验的密码
   * @returns {Array} 校验规则列表，每项包含 label(规则描述) 和 passed(是否通过)
   */
  const validatePassword = (password) => {
    return [
      { label: '长度至少 6 位', passed: password.length >= 6 },
      { label: '包含大写字母', passed: /[A-Z]/.test(password) },
      { label: '包含数字', passed: /[0-9]/.test(password) },
    ]
  }

  // 实时计算注册密码的校验结果
  const passwordRules = validatePassword(registerPassword)

  /**
   * 切换页面 - Navigate to a different page
   * 同时清空之前的提示消息
   * @param {string} page - 目标页面 (PAGE_HOME / PAGE_REGISTER / PAGE_LOGIN)
   */
  const navigateTo = (page) => {
    setCurrentPage(page)
    setMessage({ text: '', type: '' })
  }

  /**
   * 获取 Hello World 消息 - Fetch greeting from backend
   * 优先从后端 API 获取，若后端未运行则回退到本地模式
   */
  const fetchHello = async () => {
    try {
      const response = await fetch(`${API_BASE}/hello`)
      const data = await response.json()
      setHelloMessage(data.message)
      setApiMode('api')
    } catch {
      setHelloMessage('Hello, World!')
      setApiMode('local')
    }
  }

  /**
   * 处理用户注册 - Handle registration form submission
   * 1. 检查所有输入字段是否填写
   * 2. 检查两次密码是否一致
   * 3. 向后端 POST /api/auth/register 发送注册请求
   */
  const handleRegister = async () => {
    // 输入验证：检查所有字段是否已填写
    if (!registerUsername.trim() || !registerPassword || !registerConfirm) {
      setMessage({ text: '请填写所有字段', type: 'error' })
      return
    }

    // 密码一致性检查
    if (registerPassword !== registerConfirm) {
      setMessage({ text: '两次输入的密码不一致', type: 'error' })
      return
    }

    // 前端密码强度校验（与后端规则一致）
    const failedRules = passwordRules.filter((rule) => !rule.passed)
    if (failedRules.length > 0) {
      const failedLabels = failedRules.map((r) => r.label).join('、')
      setMessage({ text: `密码不符合要求: ${failedLabels}`, type: 'error' })
      return
    }

    try {
      const response = await fetch(`${API_BASE}/auth/register`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          username: registerUsername,
          password: registerPassword
        })
      })
      const data = await response.json()

      if (data.success) {
        setMessage({ text: data.message, type: 'success' })
        // 注册成功后清空表单
        setRegisterUsername('')
        setRegisterPassword('')
        setRegisterConfirm('')
      } else {
        setMessage({ text: data.message, type: 'error' })
      }
    } catch {
      // 后端未运行时，使用本地模式模拟注册成功
      setMessage({
        text: `注册成功！欢迎 ${registerUsername} (本地模式)`,
        type: 'success'
      })
      setRegisterUsername('')
      setRegisterPassword('')
      setRegisterConfirm('')
    }
  }

  /**
   * 处理用户登录 - Handle login form submission
   * 1. 检查输入字段
   * 2. 向后端 POST /api/auth/login 发送登录请求
   * 3. 登录成功后更新用户状态并跳转到首页
   */
  const handleLogin = async () => {
    // 输入验证
    if (!loginUsername.trim() || !loginPassword) {
      setMessage({ text: '请输入用户名和密码', type: 'error' })
      return
    }

    try {
      const response = await fetch(`${API_BASE}/auth/login`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          username: loginUsername,
          password: loginPassword
        })
      })
      const data = await response.json()

      if (data.success) {
        // 登录成功：更新状态并跳转首页
        setLoggedInUser(loginUsername)
        setLoginUsername('')
        setLoginPassword('')
        setCurrentPage(PAGE_HOME)
        setMessage({ text: data.message, type: 'success' })
      } else {
        setMessage({ text: data.message, type: 'error' })
      }
    } catch {
      // 本地模式模拟登录
      setLoggedInUser(loginUsername)
      setLoginUsername('')
      setLoginPassword('')
      setCurrentPage(PAGE_HOME)
      setMessage({
        text: `登录成功！欢迎回来, ${loginUsername} (本地模式)`,
        type: 'success'
      })
    }
  }

  /**
   * 处理用户登出 - Handle logout
   * 清除登录状态，显示登出提示
   */
  const handleLogout = () => {
    setLoggedInUser(null)
    setMessage({ text: '已成功登出', type: 'success' })
    setCurrentPage(PAGE_HOME)
  }

  /* ============================================================
   * 渲染 - Render
   * ============================================================ */
  return (
    <div className="app">

      {/* ---- 顶部导航栏 ---- */}
      <nav className="nav">
        <div
          className="nav-brand"
          onClick={() => navigateTo(PAGE_HOME)}
        >
          Ex01 Demo
        </div>
        <div className="nav-links">
          <button
            className={`nav-link ${currentPage === PAGE_HOME ? 'active' : ''}`}
            onClick={() => navigateTo(PAGE_HOME)}
          >
            首页
          </button>
          {!loggedInUser ? (
            <>
              <button
                className={`nav-link ${currentPage === PAGE_REGISTER ? 'active' : ''}`}
                onClick={() => navigateTo(PAGE_REGISTER)}
              >
                注册
              </button>
              <button
                className={`nav-link ${currentPage === PAGE_LOGIN ? 'active' : ''}`}
                onClick={() => navigateTo(PAGE_LOGIN)}
              >
                登录
              </button>
            </>
          ) : (
            <>
              <span className="nav-user">{loggedInUser}</span>
              <button className="nav-link logout" onClick={handleLogout}>
                登出
              </button>
            </>
          )}
        </div>
      </nav>

      {/* ---- 全局提示消息 ---- */}
      {message.text && (
        <div className={`message ${message.type}`}>
          {message.text}
        </div>
      )}

      {/* ---- 页面内容区域 ---- */}
      <main className="main">

        {/* ====== 首页 - Home Page ====== */}
        {currentPage === PAGE_HOME && (
          <div className="page-home">
            <h1 className="home-title">Hello, World!</h1>
            <p className="home-subtitle">
              Ex01 课堂练习 — 用户注册登录系统
            </p>
            <p className="home-desc">
              基于 Java Spring Boot + React 的全栈 Web 应用，
              演示表单输入、密码校验、前后端交互等核心功能。
            </p>

            {/* Hello World API 测试 */}
            <div className="card">
              <h2>API 连接测试</h2>
              <p className="card-desc">
                点击按钮，从 Spring Boot 后端获取 Hello World 消息。
              </p>
              <button className="btn btn-primary" onClick={fetchHello}>
                获取消息
              </button>
              {helloMessage && (
                <div className="result">
                  <span className="result-label">返回结果：</span>
                  {helloMessage}
                </div>
              )}
              {apiMode !== 'detecting' && (
                <span className={`badge ${apiMode}`}>
                  {apiMode === 'api' ? 'API 已连接' : '本地模式'}
                </span>
              )}
            </div>

            {/* 功能卡片 */}
            <div className="features">
              <div className="feature-item">
                <div className="feature-icon icon-register">注</div>
                <h3>用户注册</h3>
                <p>输入用户名和密码，完成账号注册</p>
              </div>
              <div className="feature-item">
                <div className="feature-icon icon-login">登</div>
                <h3>用户登录</h3>
                <p>验证用户身份，登录系统</p>
              </div>
              <div className="feature-item">
                <div className="feature-icon icon-validate">验</div>
                <h3>密码校验</h3>
                <p>实时检查密码强度和格式</p>
              </div>
            </div>
          </div>
        )}

        {/* ====== 注册页 - Register Page ====== */}
        {currentPage === PAGE_REGISTER && (
          <div className="page-auth">
            <div className="auth-card">
              <h2>用户注册</h2>
              <p className="auth-desc">创建新账号，开始使用系统</p>

              {/* 用户名输入框 */}
              <div className="form-group">
                <label className="form-label" htmlFor="reg-username">
                  用户名
                </label>
                <input
                  id="reg-username"
                  type="text"
                  className="form-input"
                  placeholder="请输入用户名"
                  value={registerUsername}
                  onChange={(e) => setRegisterUsername(e.target.value)}
                />
              </div>

              {/* 密码输入框 */}
              <div className="form-group">
                <label className="form-label" htmlFor="reg-password">
                  密码
                </label>
                <input
                  id="reg-password"
                  type="password"
                  className="form-input"
                  placeholder="请输入密码（至少6位，含大写字母和数字）"
                  value={registerPassword}
                  onChange={(e) => setRegisterPassword(e.target.value)}
                />
                {/* 密码强度实时校验提示 */}
                {registerPassword && (
                  <div className="password-rules">
                    {passwordRules.map((rule) => (
                      <span
                        key={rule.label}
                        className={`rule ${rule.passed ? 'pass' : 'fail'}`}
                      >
                        {rule.passed ? '✓' : '✗'} {rule.label}
                      </span>
                    ))}
                  </div>
                )}
              </div>

              {/* 确认密码输入框 */}
              <div className="form-group">
                <label className="form-label" htmlFor="reg-confirm">
                  确认密码
                </label>
                <input
                  id="reg-confirm"
                  type="password"
                  className="form-input"
                  placeholder="请再次输入密码"
                  value={registerConfirm}
                  onChange={(e) => setRegisterConfirm(e.target.value)}
                  onKeyDown={(e) => e.key === 'Enter' && handleRegister()}
                />
              </div>

              {/* 注册按钮 */}
              <button
                className="btn btn-primary btn-full"
                onClick={handleRegister}
              >
                注册
              </button>

              <p className="auth-switch">
                已有账号？
                <button
                  className="link-btn"
                  onClick={() => navigateTo(PAGE_LOGIN)}
                >
                  去登录
                </button>
              </p>
            </div>
          </div>
        )}

        {/* ====== 登录页 - Login Page ====== */}
        {currentPage === PAGE_LOGIN && (
          <div className="page-auth">
            <div className="auth-card">
              <h2>用户登录</h2>
              <p className="auth-desc">输入用户名和密码，登录系统</p>

              {/* 用户名输入框 */}
              <div className="form-group">
                <label className="form-label" htmlFor="login-username">
                  用户名
                </label>
                <input
                  id="login-username"
                  type="text"
                  className="form-input"
                  placeholder="请输入用户名"
                  value={loginUsername}
                  onChange={(e) => setLoginUsername(e.target.value)}
                />
              </div>

              {/* 密码输入框 */}
              <div className="form-group">
                <label className="form-label" htmlFor="login-password">
                  密码
                </label>
                <input
                  id="login-password"
                  type="password"
                  className="form-input"
                  placeholder="请输入密码"
                  value={loginPassword}
                  onChange={(e) => setLoginPassword(e.target.value)}
                  onKeyDown={(e) => e.key === 'Enter' && handleLogin()}
                />
              </div>

              {/* 登录按钮 */}
              <button
                className="btn btn-primary btn-full"
                onClick={handleLogin}
              >
                登录
              </button>

              <p className="auth-switch">
                没有账号？
                <button
                  className="link-btn"
                  onClick={() => navigateTo(PAGE_REGISTER)}
                >
                  去注册
                </button>
              </p>
            </div>
          </div>
        )}
      </main>

      {/* ---- 底部栏 ---- */}
      <footer className="footer">
        <p>class_demo / ex01 — 课堂练习项目 | Spring Boot 3.2 + React 19</p>
      </footer>
    </div>
  )
}

export default App
