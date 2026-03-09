<div align="center">

# class_demo

![Java](https://img.shields.io/badge/Java-17-ED8B00?logo=openjdk&logoColor=white)
![Spring Boot](https://img.shields.io/badge/Spring%20Boot-3.2-6DB33F?logo=springboot&logoColor=white)
![React](https://img.shields.io/badge/React-19-61DAFB?logo=react&logoColor=black)
![License](https://img.shields.io/badge/License-MIT-green.svg)
![GitHub last commit](https://img.shields.io/github/last-commit/Lx050/class_demo)
![Deploy](https://img.shields.io/github/actions/workflow/status/Lx050/class_demo/deploy.yml?label=Deploy&logo=githubactions&logoColor=white)

**A full-stack web application exercise built with Java Spring Boot and React.**

[**Live Demo**](https://lx050.github.io/class_demo/) | [**Source Code**](https://github.com/Lx050/class_demo)

</div>

---

## Table of Contents

- [Live Demo](#live-demo)
- [Overview](#overview)
- [Tech Stack](#tech-stack)
- [Prerequisites](#prerequisites)
- [Project Structure](#project-structure)
- [Exercises](#exercises)
  - [ex01 - HelloWorld Web App](#ex01---helloworld-web-app)
- [Quick Start](#quick-start)
- [API Documentation](#api-documentation)
- [Architecture](#architecture)
- [CI/CD Pipeline](#cicd-pipeline)
  - [Issue Auto-resolve](#issue-auto-resolve)
- [Git Workflow](#git-workflow)
- [Changelog](#changelog)
- [License](#license)

## Live Demo

The React frontend is automatically deployed to GitHub Pages via CI/CD:

**https://lx050.github.io/class_demo/**

> The demo runs in **Standalone Mode** (frontend-only). To experience the full-stack version with Spring Boot backend, follow the [Quick Start](#quick-start) guide for local development.

| Mode | Environment | Backend | Description |
|------|-------------|---------|-------------|
| Standalone | GitHub Pages | N/A | Frontend runs independently with local fallback |
| Full-Stack | Local Dev | Spring Boot | Frontend communicates with backend REST API |

## Overview

This repository contains a series of full-stack web application exercises designed for classroom learning. Each exercise is organized in its own directory (`ex01`, `ex02`, ...) with a **Java Spring Boot** backend and a **React** frontend, demonstrating modern web development practices from basic I/O to more advanced topics.

## Tech Stack

| Layer | Technology | Version | Purpose |
|-------|-----------|---------|---------|
| Backend | Java | 17 | Programming language |
| Backend | Spring Boot | 3.2.0 | REST API framework |
| Backend | Maven | 3.9+ | Build & dependency management |
| Frontend | React | 19 | UI component library |
| Frontend | Vite | 6.x | Build tool & dev server |

## Prerequisites

| Requirement | Version | Installation |
|-------------|---------|-------------|
| JDK | >= 17 | [Download](https://adoptium.net/) |
| Maven | >= 3.9 | [Download](https://maven.apache.org/) |
| Node.js | >= 18 | [Download](https://nodejs.org/) |
| npm | >= 9 | Included with Node.js |
| Git | >= 2.0 | [Download](https://git-scm.com/) |

## Project Structure

```
class_demo/
├── LICENSE
├── README.md
├── scripts/
│   └── resolve-issue.sh              # Issue auto-resolve logic
├── .github/workflows/
│   ├── deploy.yml                    # CI/CD: build & deploy to GitHub Pages
│   └── auto-resolve-issue.yml        # Issue auto-resolve & auto-merge
└── ex01/
    ├── backend/                          # Spring Boot REST API
    │   ├── pom.xml                       # Maven configuration
    │   ├── .gitignore
    │   └── src/main/
    │       ├── java/com/example/hello/
    │       │   ├── HelloWorldApplication.java      # Application entry point
    │       │   └── controller/
    │       │       └── HelloController.java        # REST endpoints
    │       └── resources/
    │           └── application.properties          # Server configuration
    └── frontend/                         # React SPA
        ├── package.json                  # npm configuration
        ├── vite.config.js                # Vite build config
        ├── index.html                    # HTML entry point
        ├── .gitignore
        └── src/
            ├── main.jsx                  # React entry point
            ├── App.jsx                   # Main application component
            ├── App.css                   # Component styles
            └── index.css                 # Global styles
```

## Exercises

### ex01 - HelloWorld Web App

> **Objective:** Build a full-stack HelloWorld web application with user input functionality using Java Spring Boot as the backend API and React as the frontend UI. Also includes a Python command-line version demonstrating basic I/O operations.

#### Features

| Feature | Description | API Endpoint |
|---------|-------------|-------------|
| Hello World | Fetch a greeting message from the backend | `GET /api/hello` |
| Keyboard Input | Submit user input and display the echoed response | `POST /api/echo` |

#### Python Command-Line Version

**`hello.py`** - A Python script demonstrating `print()` 输出和 `input()` 键盘输入功能:

| 功能 | 说明 | 使用的函数 |
|------|------|-----------|
| 输出 Hello World | 在终端打印欢迎信息 | `print()` |
| 键盘输入姓名 | 读取用户输入的姓名并显示 | `input()` |
| 键盘输入年龄 | 读取用户输入的年龄并显示 | `input()` |
| 键盘输入任意内容 | 读取用户输入的文本并回显 | `input()` |

```python
# 使用 input 函数实现键盘输入
name = input("请输入你的名字: ")
print(f"你好, {name}! 欢迎学习 Python 编程！")

age = input("请输入你的年龄: ")
print(f"{name}, 你今年 {age} 岁。")

message = input("请输入一段话: ")
print(f"你输入的内容是: {message}")
```

Run with:
```bash
cd ex01
python hello.py
```

#### Backend - Key Code

**`HelloController.java`** - REST API endpoints:

```java
@RestController
@RequestMapping("/api")
@CrossOrigin(origins = "http://localhost:5173")
public class HelloController {

    @GetMapping("/hello")
    public Map<String, String> hello() {
        return Map.of("message", "Hello, World!");
    }

    @PostMapping("/echo")
    public Map<String, String> echo(@RequestBody Map<String, String> request) {
        String input = request.getOrDefault("input", "");
        return Map.of("echo", "You entered: " + input);
    }
}
```

#### Frontend - Key Code

**`App.jsx`** - React component with `fetch` API calls:

```jsx
const fetchHello = async () => {
  const res = await fetch('http://localhost:8080/api/hello')
  const data = await res.json()
  setHelloMessage(data.message)  // "Hello, World!"
}

const submitInput = async () => {
  const res = await fetch('http://localhost:8080/api/echo', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ input: userInput }),
  })
  const data = await res.json()
  setEchoResult(data.echo)  // "You entered: ..."
}
```

#### Concepts Covered

| Concept | Technology | Description |
|---------|-----------|-------------|
| REST API | Spring Boot | Define `@GetMapping` / `@PostMapping` endpoints |
| CORS | Spring Boot | `@CrossOrigin` for cross-origin resource sharing |
| Component State | React | `useState` hook for reactive data binding |
| HTTP Client | React | `fetch` API for GET/POST requests |
| Event Handling | React | `onClick` / `onChange` / `onKeyDown` handlers |
| JSON Serialization | Both | Request/Response data exchange in JSON format |

## Quick Start

### 1. Clone the Repository

```bash
git clone git@github.com:Lx050/class_demo.git
cd class_demo
```

### 2. Start Backend (Terminal 1)

```bash
cd ex01/backend
mvn spring-boot:run
```

> Backend runs on http://localhost:8080

### 3. Start Frontend (Terminal 2)

```bash
cd ex01/frontend
npm install
npm run dev
```

> Frontend runs on http://localhost:5173

### 4. Open Browser

Navigate to http://localhost:5173 to use the application.

## API Documentation

### `GET /api/hello`

Returns a greeting message.

**Response:**

```json
{
  "message": "Hello, World!"
}
```

### `POST /api/echo`

Echoes back the user input.

**Request:**

```json
{
  "input": "Spring Boot is great"
}
```

**Response:**

```json
{
  "echo": "You entered: Spring Boot is great"
}
```

## Architecture

```
┌─────────────────┐     HTTP/JSON     ┌──────────────────────┐
│                 │   ─────────────►  │                      │
│   React App     │   GET /api/hello  │  Spring Boot Server  │
│   (Port 5173)   │   POST /api/echo  │  (Port 8080)         │
│                 │   ◄─────────────  │                      │
└─────────────────┘     JSON Response └──────────────────────┘
     Frontend                              Backend
   ┌───────────┐                      ┌────────────────┐
   │ App.jsx   │                      │ HelloController│
   │ App.css   │                      │ Application    │
   │ main.jsx  │                      │ pom.xml        │
   └───────────┘                      └────────────────┘
```

## CI/CD Pipeline

This project uses **GitHub Actions** for continuous integration and deployment:

```
develop ──► PR ──► main ──► GitHub Actions ──► GitHub Pages
                     │
                     ├── npm ci          (Install dependencies)
                     ├── npm run build   (Build React app)
                     └── deploy-pages    (Deploy to GitHub Pages)
```

| Trigger | Jobs | Description |
|---------|------|-------------|
| PR to `main` | Build | Validate that the React app builds successfully |
| Push to `main` | Build + Deploy | Build and deploy to GitHub Pages |

Workflow configuration: [`.github/workflows/deploy.yml`](.github/workflows/deploy.yml)

### Issue Auto-resolve

Issues are automatically analyzed and resolved via GitHub Actions:

```
Issue opened ──► GitHub Actions ──► Analyze ──► Create branch
                                                    │
                      Auto-merge ◄── PR ◄── Commit & Push
```

| Step | Action | Description |
|------|--------|-------------|
| 1 | Trigger | New Issue is opened |
| 2 | Analyze | `scripts/resolve-issue.sh` parses issue title & body |
| 3 | Fix | Applies matched pattern (update readme, fix bug, etc.) |
| 4 | PR | Creates `fix/issue-{N}` branch and pull request |
| 5 | Merge | Auto-merge via squash after CI passes |

**Supported Issue Patterns:**

| Keyword (EN / CN) | Action |
|-------------------|--------|
| `update readme` / `修改文档` | Appends content to README.md |
| `update footer` / `修改页脚` | Modifies footer text in App.jsx |
| `change title` / `修改标题` | Updates page heading |
| `update style` / `颜色` / `样式` / `不好看` | Modifies CSS theme colors (hex in body or auto) |
| `add feature` / `添加功能` | Inserts new feature card component |
| `fix bug` / `修复` / `错误` | Applies fix to file mentioned in body |

Workflow configuration: [`.github/workflows/auto-resolve-issue.yml`](.github/workflows/auto-resolve-issue.yml)
Resolve script: [`scripts/resolve-issue.sh`](scripts/resolve-issue.sh)

## Git Workflow

This project follows the **Feature Branch Workflow** with pull request reviews:

```
main
 └── feature/ex01-helloworld    ──► PR ──► merge
      └── feature/ex01-input    ──► PR ──► merge
```

| Branch | Purpose | Status |
|--------|---------|--------|
| `main` | Production-ready code | Protected |
| `develop` | Integration branch | Active |
| `feature/ex01-helloworld` | HelloWorld implementation | Merged |
| `feature/ex01-input` | Keyboard input feature | Merged |

## Changelog

| Version | Date | Description |
|---------|------|-------------|
| v2.2 | 2026-03-05 | Added Issue auto-resolve workflow with auto-merge |
| v2.1 | 2026-03-05 | Added GitHub Actions CI/CD, auto-deploy to GitHub Pages |
| v2.0 | 2026-03-05 | Migrated to Java Spring Boot + React full-stack architecture |
| v1.1 | 2026-03-05 | Added keyboard input/output feature (Python) |
| v1.0 | 2026-03-05 | Initial HelloWorld program (Python) |

## License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

---

<div align="center">

*Built for classroom learning and modern web development practice.*

</div>
