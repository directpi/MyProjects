package example.domain.service;

import example.domain.model.Role;
import example.domain.model.User;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.junit.jupiter.MockitoExtension;
import org.springframework.test.util.ReflectionTestUtils;

import java.util.Map;
import java.util.UUID;

import static org.junit.jupiter.api.Assertions.*;

@ExtendWith(MockitoExtension.class)
class JwtProviderTest {

    private JwtProvider jwtProvider;
    private User testUser;

    @BeforeEach
    void setUp() {
        jwtProvider = new JwtProvider();

        // Устанавливаем тестовые значения через рефлексию
        ReflectionTestUtils.setField(jwtProvider, "secret",
                "test-secret-key-for-jwt-token-generation-minimum-256-bits");
        ReflectionTestUtils.setField(jwtProvider, "accessExpiration", 3600000L); // 1 час
        ReflectionTestUtils.setField(jwtProvider, "refreshExpiration", 2592000000L); // 30 дней

        // Создаем тестового пользователя
        testUser = new User();
        testUser.setId(UUID.randomUUID());
        testUser.setLogin("testuser");
        testUser.setPassword("password");
        testUser.setRole(Role.USER);
    }

    @Test
    void generateAccessToken_ShouldReturnValidToken() {
        // When
        String token = jwtProvider.generateAccessToken(testUser);

        // Then
        assertNotNull(token);
        assertFalse(token.isEmpty());
        assertTrue(token.split("\\.").length == 3); // JWT должен состоять из 3 частей
    }

    @Test
    void generateRefreshToken_ShouldReturnValidToken() {
        // When
        String token = jwtProvider.generateRefreshToken(testUser);

        // Then
        assertNotNull(token);
        assertFalse(token.isEmpty());
        assertTrue(token.split("\\.").length == 3);
    }

    @Test
    void validateAccessToken_WithValidToken_ShouldReturnTrue() {
        // Given
        String token = jwtProvider.generateAccessToken(testUser);

        // When
        boolean isValid = jwtProvider.validateAccessToken(token);

        // Then
        assertTrue(isValid);
    }

    @Test
    void validateAccessToken_WithInvalidToken_ShouldReturnFalse() {
        // Given
        String invalidToken = "invalid.jwt.token";

        // When
        boolean isValid = jwtProvider.validateAccessToken(invalidToken);

        // Then
        assertFalse(isValid);
    }

    @Test
    void validateAccessToken_WithNullToken_ShouldReturnFalse() {
        // When
        boolean isValid = jwtProvider.validateAccessToken(null);

        // Then
        assertFalse(isValid);
    }

    @Test
    void validateRefreshToken_WithValidToken_ShouldReturnTrue() {
        // Given
        String token = jwtProvider.generateRefreshToken(testUser);

        // When
        boolean isValid = jwtProvider.validateRefreshToken(token);

        // Then
        assertTrue(isValid);
    }

    @Test
    void validateRefreshToken_WithInvalidToken_ShouldReturnFalse() {
        // Given
        String invalidToken = "invalid.jwt.token";

        // When
        boolean isValid = jwtProvider.validateRefreshToken(invalidToken);

        // Then
        assertFalse(isValid);
    }

    @Test
    void getClaims_WithValidAccessToken_ShouldReturnCorrectClaims() {
        // Given
        String token = jwtProvider.generateAccessToken(testUser);

        // When
        Map<String, Object> claims = jwtProvider.getClaims(token);

        // Then
        assertNotNull(claims);
        assertEquals(testUser.getId().toString(), claims.get("sub"));
        assertEquals(Role.USER.name(), claims.get("role"));
    }

    @Test
    void getClaims_WithValidRefreshToken_ShouldReturnSubject() {
        // Given
        String token = jwtProvider.generateRefreshToken(testUser);

        // When
        Map<String, Object> claims = jwtProvider.getClaims(token);

        // Then
        assertNotNull(claims);
        assertEquals(testUser.getId().toString(), claims.get("sub"));
        // Refresh токен не должен содержать роль
        assertNull(claims.get("role"));
    }

    @Test
    void getClaims_WithInvalidToken_ShouldThrowException() {
        // Given
        String invalidToken = "invalid.jwt.token";

        // When & Then
        assertThrows(RuntimeException.class, () -> jwtProvider.getClaims(invalidToken));
    }

    @Test
    void generateTokens_WithDifferentUsers_ShouldProduceDifferentTokens() {
        // Given
        User user1 = new User();
        user1.setId(UUID.randomUUID());
        user1.setLogin("user1");
        user1.setRole(Role.USER);

        User user2 = new User();
        user2.setId(UUID.randomUUID());
        user2.setLogin("user2");
        user2.setRole(Role.USER);

        // When
        String token1 = jwtProvider.generateAccessToken(user1);
        String token2 = jwtProvider.generateAccessToken(user2);

        // Then
        assertNotEquals(token1, token2);
    }

    @Test
    void accessToken_ShouldContainRole() {
        // Given
        String token = jwtProvider.generateAccessToken(testUser);

        // When
        Map<String, Object> claims = jwtProvider.getClaims(token);

        // Then
        assertEquals("USER", claims.get("role"));
    }

    @Test
    void refreshToken_ShouldNotContainRole() {
        // Given
        String token = jwtProvider.generateRefreshToken(testUser);

        // When
        Map<String, Object> claims = jwtProvider.getClaims(token);

        // Then
        assertNull(claims.get("role"));
    }
}








