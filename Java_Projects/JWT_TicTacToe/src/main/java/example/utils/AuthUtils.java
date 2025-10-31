package example.utils;

import org.springframework.security.core.Authentication;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.stereotype.Component;

import java.util.UUID;

@Component
public class AuthUtils {
    public UUID getCurrentPlayerId() {
        Authentication auth = SecurityContextHolder.getContext().getAuthentication();
        if (auth != null && auth.isAuthenticated()) {
            if (auth.getPrincipal() instanceof UUID) {
                return (UUID) auth.getPrincipal();
            }
            if (auth.getPrincipal() instanceof String) {
                return UUID.fromString((String) auth.getPrincipal());
            }
        }
        throw new SecurityException("Пользователь не аутентифицирован");
    }
}
