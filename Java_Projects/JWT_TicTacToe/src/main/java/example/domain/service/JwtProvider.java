package example.domain.service;

import example.domain.model.User;
import io.jsonwebtoken.Claims;
import io.jsonwebtoken.Jwts;
import io.jsonwebtoken.security.Keys;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Component;

import javax.crypto.SecretKey;
import java.nio.charset.StandardCharsets;
import java.util.Date;
import java.util.HashMap;
// import java.util.List;
import java.util.Map;

@Component
public class JwtProvider {

    @Value("${jwt.secret:default-secret-key:3600000}")
    private String secret;

    @Value("${jwt.access.expiration}")
    private Long accessExpiration;

    @Value("${jwt.refresh.expiration:2592000000}")
    private Long refreshExpiration;

    private SecretKey getSigningKey() {
        return Keys.hmacShaKeyFor(secret.getBytes(StandardCharsets.UTF_8));
    }

    public String generateAccessToken(User user) {
        return Jwts.builder()
                .setSubject(user.getId().toString()) // ← .setSubject() вместо .subject()
                .claim("role", user.getRole().name())
                .setIssuedAt(new Date())
                .setExpiration(new Date(System.currentTimeMillis() + accessExpiration))
                .signWith(getSigningKey())
                .compact();
        // return "access-token-" + user.getId(); // Заглушка
    }

    public String generateRefreshToken(User user) {
        return Jwts.builder()
                .setSubject(user.getId().toString()) // ← .setSubject()
                .setIssuedAt(new Date())
                .setExpiration(new Date(System.currentTimeMillis() + refreshExpiration))
                .signWith(getSigningKey())
                .compact();
        // return "refresh-token-" + user.getId(); // Заглушка
    }

    public boolean validateAccessToken(String token) {
        try {
            // ✅ ПРАВИЛЬНЫЙ СИНТАКСИС для JJWT 0.12.6
            Jwts.parserBuilder()
                    .setSigningKey(getSigningKey()) // ← .setSigningKey() вместо .verifyWith()
                    .build()
                    .parseClaimsJws(token); // ← .parseClaimsJws() вместо .parseSignedClaims()
            return true;
        } catch (Exception e) {
            return false;
        }
        // return token != null && token.startsWith("access-token-"); // Заглушка
    }

    public boolean validateRefreshToken(String token) {
        try {
            Jwts.parserBuilder()
                    .setSigningKey(getSigningKey())
                    .build()
                    .parseClaimsJws(token);
            return true;
        } catch (Exception e) {
            return false;
        }
    }

    public Map<String, Object> getClaims(String token) {
        try {
            Claims claims = Jwts.parserBuilder()
                    .setSigningKey(getSigningKey())
                    .build()
                    .parseClaimsJws(token)
                    .getBody();

            Map<String, Object> result = new HashMap<>();
            result.put("sub", claims.getSubject());
            result.put("role", claims.get("role", String.class));
            return result;
        } catch (Exception e) {
            throw new RuntimeException("Invalid token", e);
        }
    }
}