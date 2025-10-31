package example.web.controller;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HomeController {

    @GetMapping("/")
    public String home() {
        return "Добро пожаловать в Tic-Tac-Toe API! Используйте /auth для авторизации и /game для игры.";
    }
}