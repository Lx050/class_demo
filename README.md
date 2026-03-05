<div align="center">

# class_demo

![Java](https://img.shields.io/badge/Java-17-ED8B00?logo=openjdk&logoColor=white)
![Spring Boot](https://img.shields.io/badge/Spring%20Boot-3.2-6DB33F?logo=springboot&logoColor=white)
![React](https://img.shields.io/badge/React-19-61DAFB?logo=react&logoColor=black)
![License](https://img.shields.io/badge/License-MIT-green.svg)
![GitHub last commit](https://img.shields.io/github/last-commit/Lx050/class_demo)

**A full-stack web application exercise built with Java Spring Boot and React.**

</div>

---

## Table of Contents

- [Overview](#overview)
- [Tech Stack](#tech-stack)
- [Prerequisites](#prerequisites)
- [Project Structure](#project-structure)
- [Exercises](#exercises)
  - [ex01 - HelloWorld Web App](#ex01---helloworld-web-app)
- [Quick Start](#quick-start)
- [API Documentation](#api-documentation)
- [Architecture](#architecture)
- [Git Workflow](#git-workflow)
- [Changelog](#changelog)
- [License](#license)

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

> **Objective:** Build a full-stack HelloWorld web application with user input functionality using Java Spring Boot as the backend API and React as the frontend UI.

#### Features

| Feature | Description | API Endpoint |
|---------|-------------|-------------|
| Hello World | Fetch a greeting message from the backend | `GET /api/hello` |
| Keyboard Input | Submit user input and display the echoed response | `POST /api/echo` |

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
| v2.0 | 2026-03-05 | Migrated to Java Spring Boot + React full-stack architecture |
| v1.1 | 2026-03-05 | Added keyboard input/output feature (Python) |
| v1.0 | 2026-03-05 | Initial HelloWorld program (Python) |

## License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

---

<div align="center">

*Built for classroom learning and modern web development practice.*

</div>
