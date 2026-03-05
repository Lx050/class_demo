package com.example.hello.controller;

import org.springframework.web.bind.annotation.*;

import java.util.Map;

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
