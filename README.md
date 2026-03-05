<div align="center">

# class_demo

![Python](https://img.shields.io/badge/Python-3.x-3776AB?logo=python&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-green.svg)
![GitHub last commit](https://img.shields.io/github/last-commit/Lx050/class_demo)
![GitHub repo size](https://img.shields.io/github/repo-size/Lx050/class_demo)

**A collection of Python programming exercises for classroom assignments.**

</div>

---

## Table of Contents

- [Overview](#overview)
- [Prerequisites](#prerequisites)
- [Project Structure](#project-structure)
- [Exercises](#exercises)
  - [ex01 - HelloWorld & Keyboard Input](#ex01---helloworld--keyboard-input)
- [Git Workflow](#git-workflow)
- [Changelog](#changelog)
- [License](#license)

## Overview

This repository contains a series of Python programming exercises designed for classroom learning. Each exercise is organized in its own directory (`ex01`, `ex02`, ...) and focuses on specific programming concepts, progressing from basic I/O to more advanced topics.

## Prerequisites

| Requirement | Version |
|-------------|---------|
| Python      | >= 3.6  |
| Git         | >= 2.0  |

## Project Structure

```
class_demo/
├── LICENSE
├── README.md
└── ex01/
    └── hello.py        # Exercise 01: HelloWorld & Keyboard Input
```

## Exercises

### ex01 - HelloWorld & Keyboard Input

> **Objective:** Master standard output and keyboard input in Python.

#### Concepts Covered

| Concept | Function | Description |
|---------|----------|-------------|
| Standard Output | `print()` | Output formatted text to the console |
| Standard Input | `input()` | Read user input from the keyboard as a string |
| Variable Assignment | `=` | Store and reference data using named variables |

#### Source Code

```python
# ex01/hello.py

print("Hello, World!")

user_input = input("Please enter something: ")
print("You entered:", user_input)
```

#### Usage

```bash
cd class_demo
python ex01/hello.py
```

#### Sample Session

```
$ python ex01/hello.py
Hello, World!
Please enter something: Python is great
You entered: Python is great
```

#### Implementation Notes

- **v1** — Implemented basic `print()` output to display "Hello, World!"
- **v2** — Extended with `input()` to capture keyboard input and echo it back using `print()`

## Git Workflow

This project follows the **Feature Branch Workflow** with pull request reviews:

```
main
 └── feature/ex01-helloworld    ──► PR #1 ──► merge
      └── feature/ex01-input    ──► PR #2 ──► merge
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
| v1.0 | 2026-03-05 | Initial HelloWorld program |
| v1.1 | 2026-03-05 | Added keyboard input/output feature |

## License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

---

<div align="center">

*Built for classroom learning and programming practice.*

</div>
