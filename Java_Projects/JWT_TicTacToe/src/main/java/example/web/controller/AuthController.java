package example.web.controller;

import example.domain.model.User;
import example.domain.service.AuthService;
import example.domain.service.UserService;
import example.web.filter.JwtAuthentication;
import example.web.model.*;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.validation.Valid;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.security.web.authentication.logout.SecurityContextLogoutHandler;
import org.springframework.web.bind.annotation.*;

import java.util.UUID;

@RestController
@RequestMapping("/auth")
public class AuthController {
    private final AuthService authService;
    private final UserService userService;

    @Autowired
    public AuthController(AuthService authService, UserService userService) {
        this.authService = authService;
        this.userService = userService;
    }

    @PostMapping("/register")
    public ResponseEntity<AuthStatusResponse> register(@Valid @RequestBody SignUpRequest request) {
        try {
            UUID userId = authService.register(request);
            User user = userService.getUserById(userId);

            return ResponseEntity.ok(
                    new AuthStatusResponse(true, userId, request.getLogin(), user.getRole().name(),
                            "Пользователь зарегистрирован"));

        } catch (IllegalStateException e) {
            return ResponseEntity.badRequest()
                    .body(new AuthStatusResponse(false, null, request.getLogin(), null, e.getMessage()));
        }
    }

    @PostMapping("/login")
    public ResponseEntity<JwtResponse> login(@RequestBody @Valid JwtRequest request) {
        try {
            JwtResponse jwtResponse = authService.authenticate(request);
            return ResponseEntity.ok(jwtResponse);
        } catch (org.springframework.security.core.AuthenticationException ex) {
            return ResponseEntity.status(HttpStatus.UNAUTHORIZED).build();
        }
    }

    @GetMapping("/check")
    public ResponseEntity<AuthStatusResponse> checkAuth() {

        // ПРОВЕРЯЕМ Spring Security Context
        Authentication auth = SecurityContextHolder.getContext().getAuthentication();
        if (auth == null || !auth.isAuthenticated() || !(auth instanceof JwtAuthentication)) {
            return ResponseEntity.status(HttpStatus.UNAUTHORIZED)
                    .body(new AuthStatusResponse(false, null, null, null, "Пользователь не аутентифицирован"));
        }

        try {
            JwtAuthentication jwtAuth = (JwtAuthentication) auth;
            UUID serverPlayerId = (UUID) jwtAuth.getPrincipal(); // Исправлено: getPrincipal() возвращает UUID

            // Получаем username из базы данных
            User user = userService.getUserById(serverPlayerId);
            String username = user.getLogin();

            return ResponseEntity.ok(
                    new AuthStatusResponse(true, serverPlayerId, username, user.getRole().name(),
                            "Пользователь авторизован"));

        } catch (Exception e) {
            return ResponseEntity.status(HttpStatus.UNAUTHORIZED)
                    .body(new AuthStatusResponse(false, null, null, null, "Ошибка авторизации: " + e.getMessage()));
        }
    }

    @PostMapping("/logout")
    public ResponseEntity<String> logout(HttpServletRequest request, HttpServletResponse response) {
        try {
            // Используем Spring Security logout: очищает cookies, сессии, security context
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

    @PostMapping("/refresh")
    public ResponseEntity<JwtResponse> refreshAccessToken(@RequestBody @Valid RefreshJwtRequest request) {
        try {
            JwtResponse response = authService.refreshAccessToken(request.refreshToken());
            return ResponseEntity.ok(response);
        } catch (org.springframework.security.core.AuthenticationException ex) {
            return ResponseEntity.status(HttpStatus.UNAUTHORIZED).build();
        }
    }

    @PostMapping("/refresh-token")
    public ResponseEntity<JwtResponse> refreshRefreshToken(@RequestBody @Valid RefreshJwtRequest request) {
        try {
            JwtResponse response = authService.refreshRefreshToken(request.refreshToken());
            return ResponseEntity.ok(response);
        } catch (org.springframework.security.core.AuthenticationException ex) {
            return ResponseEntity.status(HttpStatus.UNAUTHORIZED).build();
        }
    }
}