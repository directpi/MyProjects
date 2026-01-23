package s21.example.web.controller;

import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.AuthenticationException;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.security.web.authentication.logout.SecurityContextLogoutHandler;
import org.springframework.web.bind.annotation.*;
import s21.example.domain.service.AuthService;
import s21.example.web.model.AuthStatusResponse;
import s21.example.web.model.SignUpRequest;

import java.nio.charset.StandardCharsets;
import java.util.Base64;
import java.util.UUID;

@RestController
@RequestMapping("/auth")
public class AuthController {
    private final AuthService authService;

    @Autowired
    public AuthController(AuthService authService) {
        this.authService = authService;
    }

    @PostMapping("/register")
    public ResponseEntity<String> register(@RequestBody SignUpRequest request) {
        if (authService.register(request)) {
            return ResponseEntity.ok("Пользователь зарегистрирован");
        }
        return ResponseEntity.badRequest().body("Пользователь уже существует");
    }


    @PostMapping("/login")
    public ResponseEntity<AuthStatusResponse> login(@RequestHeader("Authorization") String authHeader) {
        try {
            UUID userId = authService.authenticate(authHeader);

            // Извлекаем логин из заголовка
            String base64Credentials = authHeader.substring("Basic ".length()).trim();
            String credentials = new String(Base64.getDecoder().decode(base64Credentials), StandardCharsets.UTF_8);
            String username = credentials.split(":", 2)[0];

            return ResponseEntity.ok(
                    new AuthStatusResponse(true, userId, username, "Аутентификация успешна")
            );
        } catch (AuthenticationException e) {
            return ResponseEntity.status(HttpStatus.UNAUTHORIZED)
                    .body(new AuthStatusResponse(false, null, null, "Ошибка аутентификации: " + e.getMessage()));
        }
    }

    @GetMapping("/check")
    public ResponseEntity<AuthStatusResponse> checkAuth(@RequestHeader("Authorization") String authHeader) {
        try {
            UUID userId = authService.authenticate(authHeader);

            // Извлекаем логин из заголовка Authorization
            String base64Credentials = authHeader.substring("Basic ".length()).trim();
            String credentials = new String(Base64.getDecoder().decode(base64Credentials), StandardCharsets.UTF_8);
            String username = credentials.split(":", 2)[0];

            return ResponseEntity.ok(
                    new AuthStatusResponse(true, userId, username, "Пользователь авторизован")
            );
        } catch (AuthenticationException e) {
            return ResponseEntity.status(HttpStatus.UNAUTHORIZED)
                    .body(new AuthStatusResponse(false, null, null, "Ошибка авторизации: " + e.getMessage()));
        }
    }
    @PostMapping("/logout")
    public ResponseEntity<String> logout(HttpServletRequest request, HttpServletResponse response) {
        try {
            // Используем Spring Security logout
            Authentication auth = SecurityContextHolder.getContext().getAuthentication();
            if (auth != null) {
                new SecurityContextLogoutHandler().logout(request, response, auth);
            }
            return ResponseEntity.ok("Выход выполнен успешно");
        } catch (Exception e) {
            return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR)
                    .body("Ошибка при выходе: " + e.getMessage());
        }
    }
}