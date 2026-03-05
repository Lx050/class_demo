# class_demo

Five class assignments

## ex01 - HelloWorld & Keyboard Input

### Description

This exercise demonstrates basic Python programming concepts:
1. **HelloWorld**: Use `print()` to output "Hello, World!" to the console.
2. **Keyboard Input**: Use `input()` to read user input from the keyboard and `print()` to display it.

### File Structure

```
ex01/
└── hello.py    # Main program file
```

### How to Run

```bash
python ex01/hello.py
```

### Expected Output

```
Hello, World!
Please enter something: <user types here>
You entered: <displays what user typed>
```

### Key Concepts

- `print()` - Output text to the console
- `input()` - Read keyboard input as a string
- Variable assignment - Store user input in a variable for later use

### Version History

| Version | Description |
|---------|-------------|
| v1 | Initial HelloWorld program (`print("Hello, World!")`) |
| v2 | Added keyboard input feature using `input()` function |

### Git Workflow

This project follows a feature-branch workflow:
- `feature/ex01-helloworld` - HelloWorld program implementation
- `feature/ex01-input` - Keyboard input feature addition
- Feature branches are merged into `main` via pull requests
