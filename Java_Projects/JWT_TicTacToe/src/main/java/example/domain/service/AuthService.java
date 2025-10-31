package example.domain.service;

import example.web.model.JwtRequest;
import example.web.model.JwtResponse;
import example.web.model.SignUpRequest;
import org.springframework.security.core.AuthenticationException;

import java.util.UUID;

public interface AuthService {

    // Старый метод для обратной совместимости:
    UUID authenticate(String authHeader) throws AuthenticationException;

    // JWT
    JwtResponse authenticate(JwtRequest request) throws AuthenticationException;
    JwtResponse refreshAccessToken(String refreshToken);
    JwtResponse refreshRefreshToken(String refreshToken);

    UUID register(SignUpRequest request) throws IllegalStateException;

    boolean hasActiveSecondPlayer();
    void initializeDefaultUser();
}
