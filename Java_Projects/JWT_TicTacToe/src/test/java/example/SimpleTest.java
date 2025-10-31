package example;

import example.domain.model.Role;
import example.domain.model.User;
import example.domain.service.JwtProvider;
import example.utils.JwtUtil;
import example.web.filter.JwtAuthentication;
import org.junit.jupiter.api.Test;
import org.springframework.test.util.ReflectionTestUtils;

import java.util.Map;
import java.util.Set;
import java.util.UUID;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Простой тест для проверки основной функциональности
 */
public class SimpleTest {

    @Test
    void jwtProvider_ShouldGenerateAndValidateTokens() {
        // Given
        JwtProvider jwtProvider = new JwtProvider();
        ReflectionTestUtils.setField(jwtProvider, "secret", "test-secret-key-for-jwt-token-generation-minimum-256-bits");
        ReflectionTestUtils.setField(jwtProvider, "accessExpiration", 3600000L);
        ReflectionTestUtils.setField(jwtProvider, "refreshExpiration", 2592000000L);

        User user = new User();
        user.setId(UUID.randomUUID());
        user.setLogin("testuser");
        user.setRole(Role.USER);

        // When
        String accessToken = jwtProvider.generateAccessToken(user);
        String refreshToken = jwtProvider.generateRefreshToken(user);

        // Then
        assertNotNull(accessToken);
        assertNotNull(refreshToken);
        assertTrue(jwtProvider.validateAccessToken(accessToken));
        assertTrue(jwtProvider.validateRefreshToken(refreshToken));
    }

    @Test
    void jwtUtil_ShouldCreateAuthentication() {
        // Given
        JwtUtil jwtUtil = new JwtUtil();
        UUID userId = UUID.randomUUID();
        Map<String, Object> claims = Map.of(
                "sub", userId.toString(),
                "role", "USER"
        );

        // When
        JwtAuthentication auth = jwtUtil.generate(claims);

        // Then
        assertNotNull(auth);
        assertEquals(userId, auth.getPrincipal());
        assertEquals(userId.toString(), auth.getName());
        assertTrue(auth.isAuthenticated());
        assertEquals(1, auth.getAuthorities().size());
    }

    @Test
    void jwtAuthentication_ShouldWorkCorrectly() {
        // Given
        UUID userId = UUID.randomUUID();
        Set<Role> roles = Set.of(Role.USER);

        // When
        JwtAuthentication auth = new JwtAuthentication(userId, roles);

        // Then
        assertEquals(userId, auth.getPrincipal());
        assertEquals(userId.toString(), auth.getName());
        assertTrue(auth.isAuthenticated());
        assertEquals(1, auth.getAuthorities().size());
        assertTrue(auth.getAuthorities().contains(Role.USER));
    }

    @Test
    void role_ShouldImplementGrantedAuthority() {
        // When & Then
        assertEquals("USER", Role.USER.getAuthority());
    }

    @Test
    void user_ShouldImplementUserDetails() {
        // Given
        User user = new User();
        user.setId(UUID.randomUUID());
        user.setLogin("testuser");
        user.setPassword("password");
        user.setRole(Role.USER);

        // Then
        assertEquals("testuser", user.getUsername());
        assertEquals("password", user.getPassword());
        assertTrue(user.isAccountNonExpired());
        assertTrue(user.isAccountNonLocked());
        assertTrue(user.isCredentialsNonExpired());
        assertTrue(user.isEnabled());
        assertEquals(1, user.getAuthorities().size());
    }
}








