package example.utils;

import example.domain.model.Role;
import example.web.filter.JwtAuthentication;
import org.springframework.stereotype.Component;

import java.util.Map;
import java.util.Set;
import java.util.UUID;

@Component
public class JwtUtil {

    public JwtAuthentication generate(Map<String, Object> claims) {
        // Извлекаем userId из subject токена
        String userIdStr = (String) claims.get("sub");
        UUID userId = UUID.fromString(userIdStr);

        // Извлекаем роли из claims (правильно!)
        Set<Role> roles = extractRolesFromClaims(claims);

        return new JwtAuthentication(userId, roles);
    }

    private Set<Role> extractRolesFromClaims(Map<String, Object> claims) {
        try {
            String roleStr = (String) claims.get("role");
            if (roleStr != null) {
                return Set.of(Role.valueOf(roleStr));
            }
        } catch (Exception e) {
            System.err.println("Error extracting role from claims: " + e.getMessage());
        }
        return Set.of(Role.USER);
    }
}