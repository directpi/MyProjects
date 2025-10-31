package s21.example.domain.service;

import org.springframework.security.core.AuthenticationException;
import s21.example.web.model.SignUpRequest;

import java.util.UUID;

public interface AuthService {
    boolean register(SignUpRequest request);
    UUID authenticate(String authHeader) throws AuthenticationException;
    boolean hasActiveSecondPlayer();
    void initializeDefaultUser();
}
