package example.utils;

import example.domain.model.Role;
import example.web.filter.JwtAuthentication;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.junit.jupiter.MockitoExtension;
import org.springframework.security.core.GrantedAuthority;

import java.util.Collection;
import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

import static org.junit.jupiter.api.Assertions.*;

@ExtendWith(MockitoExtension.class)
class JwtUtilTest {

    private JwtUtil jwtUtil;
    private UUID testUserId;

    @BeforeEach
    void setUp() {
        jwtUtil = new JwtUtil();
        testUserId = UUID.randomUUID();
    }

    @Test
    void generate_WithValidClaims_ShouldReturnJwtAuthentication() {
        // Given
        Map<String, Object> claims = new HashMap<>();
        claims.put("sub", testUserId.toString());
        claims.put("role", "USER");

        // When
        JwtAuthentication auth = jwtUtil.generate(claims);

        // Then
        assertNotNull(auth);
        assertEquals(testUserId, auth.getPrincipal());
        assertEquals(testUserId.toString(), auth.getName());
        assertTrue(auth.isAuthenticated());
    }

    @Test
    void generate_WithValidRole_ShouldSetCorrectAuthorities() {
        // Given
        Map<String, Object> claims = new HashMap<>();
        claims.put("sub", testUserId.toString());
        claims.put("role", "USER");

        // When
        JwtAuthentication auth = jwtUtil.generate(claims);
        Collection<? extends GrantedAuthority> authorities = auth.getAuthorities();

        // Then
        assertEquals(1, authorities.size());
        assertTrue(authorities.contains(Role.USER));
    }

    @Test
    void generate_WithoutRole_ShouldDefaultToUserRole() {
        // Given
        Map<String, Object> claims = new HashMap<>();
        claims.put("sub", testUserId.toString());
        // Роль не указана

        // When
        JwtAuthentication auth = jwtUtil.generate(claims);
        Collection<? extends GrantedAuthority> authorities = auth.getAuthorities();

        // Then
        assertEquals(1, authorities.size());
        assertTrue(authorities.contains(Role.USER));
    }

    @Test
    void generate_WithInvalidRole_ShouldDefaultToUserRole() {
        // Given
        Map<String, Object> claims = new HashMap<>();
        claims.put("sub", testUserId.toString());
        claims.put("role", "INVALID_ROLE");

        // When
        JwtAuthentication auth = jwtUtil.generate(claims);
        Collection<? extends GrantedAuthority> authorities = auth.getAuthorities();

        // Then
        assertEquals(1, authorities.size());
        assertTrue(authorities.contains(Role.USER));
    }

    @Test
    void generate_WithNullRole_ShouldDefaultToUserRole() {
        // Given
        Map<String, Object> claims = new HashMap<>();
        claims.put("sub", testUserId.toString());
        claims.put("role", null);

        // When
        JwtAuthentication auth = jwtUtil.generate(claims);
        Collection<? extends GrantedAuthority> authorities = auth.getAuthorities();

        // Then
        assertEquals(1, authorities.size());
        assertTrue(authorities.contains(Role.USER));
    }

    @Test
    void generate_WithInvalidUUID_ShouldThrowException() {
        // Given
        Map<String, Object> claims = new HashMap<>();
        claims.put("sub", "invalid-uuid");
        claims.put("role", "USER");

        // When & Then
        assertThrows(IllegalArgumentException.class, () -> jwtUtil.generate(claims));
    }

    @Test
    void generate_WithNullSubject_ShouldThrowException() {
        // Given
        Map<String, Object> claims = new HashMap<>();
        claims.put("sub", null);
        claims.put("role", "USER");

        // When & Then
        assertThrows(Exception.class, () -> jwtUtil.generate(claims));
    }

    @Test
    void generate_WithMissingSubject_ShouldThrowException() {
        // Given
        Map<String, Object> claims = new HashMap<>();
        claims.put("role", "USER");
        // sub отсутствует

        // When & Then
        assertThrows(Exception.class, () -> jwtUtil.generate(claims));
    }

    @Test
    void generate_WithEmptyClaims_ShouldThrowException() {
        // Given
        Map<String, Object> claims = new HashMap<>();

        // When & Then
        assertThrows(Exception.class, () -> jwtUtil.generate(claims));
    }

    @Test
    void generate_WithDifferentUserIds_ShouldProduceDifferentAuthentications() {
        // Given
        UUID userId1 = UUID.randomUUID();
        UUID userId2 = UUID.randomUUID();

        Map<String, Object> claims1 = new HashMap<>();
        claims1.put("sub", userId1.toString());
        claims1.put("role", "USER");

        Map<String, Object> claims2 = new HashMap<>();
        claims2.put("sub", userId2.toString());
        claims2.put("role", "USER");

        // When
        JwtAuthentication auth1 = jwtUtil.generate(claims1);
        JwtAuthentication auth2 = jwtUtil.generate(claims2);

        // Then
        assertNotEquals(auth1.getPrincipal(), auth2.getPrincipal());
        assertEquals(userId1, auth1.getPrincipal());
        assertEquals(userId2, auth2.getPrincipal());
    }

    @Test
    void generate_ShouldAlwaysReturnAuthenticatedTrue() {
        // Given
        Map<String, Object> claims = new HashMap<>();
        claims.put("sub", testUserId.toString());
        claims.put("role", "USER");

        // When
        JwtAuthentication auth = jwtUtil.generate(claims);

        // Then
        assertTrue(auth.isAuthenticated());
    }

    @Test
    void generate_ShouldHandleRoleExtraction() {
        // Given
        Map<String, Object> claims = new HashMap<>();
        claims.put("sub", testUserId.toString());
        claims.put("role", "USER");

        // When
        JwtAuthentication auth = jwtUtil.generate(claims);

        // Then
        Collection<? extends GrantedAuthority> authorities = auth.getAuthorities();
        assertEquals(1, authorities.size());

        GrantedAuthority authority = authorities.iterator().next();
        assertEquals("USER", authority.getAuthority());
    }

    @Test
    void generate_WithRoleAsInteger_ShouldHandleGracefully() {
        // Given
        Map<String, Object> claims = new HashMap<>();
        claims.put("sub", testUserId.toString());
        claims.put("role", 123); // Неправильный тип

        // When
        JwtAuthentication auth = jwtUtil.generate(claims);
        Collection<? extends GrantedAuthority> authorities = auth.getAuthorities();

        // Then - должен fallback к USER
        assertEquals(1, authorities.size());
        assertTrue(authorities.contains(Role.USER));
    }
}








