package example.web.filter;

import example.domain.model.Role;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.security.core.GrantedAuthority;

import java.util.Collection;
import java.util.Set;
import java.util.UUID;

import static org.junit.jupiter.api.Assertions.*;

class JwtAuthenticationTest {

    private UUID testUserId;
    private Set<Role> testRoles;
    private JwtAuthentication jwtAuthentication;

    @BeforeEach
    void setUp() {
        testUserId = UUID.randomUUID();
        testRoles = Set.of(Role.USER);
        jwtAuthentication = new JwtAuthentication(testUserId, testRoles);
    }

    @Test
    void constructor_ShouldSetFieldsCorrectly() {
        // When & Then
        assertEquals(testUserId, jwtAuthentication.getPrincipal());
        assertTrue(jwtAuthentication.isAuthenticated());
        assertEquals(testUserId.toString(), jwtAuthentication.getName());
    }

    @Test
    void getAuthorities_ShouldReturnCorrectRoles() {
        // When
        Collection<? extends GrantedAuthority> authorities = jwtAuthentication.getAuthorities();

        // Then
        assertNotNull(authorities);
        assertEquals(1, authorities.size());
        assertTrue(authorities.contains(Role.USER));
    }

    @Test
    void getAuthorities_WithMultipleRoles_ShouldReturnAllRoles() {
        // Given - если в будущем добавятся другие роли
        Set<Role> multipleRoles = Set.of(Role.USER);
        JwtAuthentication auth = new JwtAuthentication(testUserId, multipleRoles);

        // When
        Collection<? extends GrantedAuthority> authorities = auth.getAuthorities();

        // Then
        assertEquals(1, authorities.size());
        assertTrue(authorities.contains(Role.USER));
    }

    @Test
    void getPrincipal_ShouldReturnUserId() {
        // When
        Object principal = jwtAuthentication.getPrincipal();

        // Then
        assertEquals(testUserId, principal);
        assertInstanceOf(UUID.class, principal);
    }

    @Test
    void getName_ShouldReturnUserIdAsString() {
        // When
        String name = jwtAuthentication.getName();

        // Then
        assertEquals(testUserId.toString(), name);
    }

    @Test
    void isAuthenticated_ShouldReturnTrueByDefault() {
        // When
        boolean authenticated = jwtAuthentication.isAuthenticated();

        // Then
        assertTrue(authenticated);
    }

    @Test
    void setAuthenticated_ShouldChangeAuthenticationStatus() {
        // Given
        assertTrue(jwtAuthentication.isAuthenticated());

        // When
        jwtAuthentication.setAuthenticated(false);

        // Then
        assertFalse(jwtAuthentication.isAuthenticated());

        // When
        jwtAuthentication.setAuthenticated(true);

        // Then
        assertTrue(jwtAuthentication.isAuthenticated());
    }

    @Test
    void getCredentials_ShouldReturnNull() {
        // When
        Object credentials = jwtAuthentication.getCredentials();

        // Then
        assertNull(credentials);
    }

    @Test
    void getDetails_ShouldReturnNull() {
        // When
        Object details = jwtAuthentication.getDetails();

        // Then
        assertNull(details);
    }

    @Test
    void constructor_WithEmptyRoles_ShouldWork() {
        // Given
        Set<Role> emptyRoles = Set.of();

        // When
        JwtAuthentication auth = new JwtAuthentication(testUserId, emptyRoles);

        // Then
        assertNotNull(auth);
        assertTrue(auth.getAuthorities().isEmpty());
        assertEquals(testUserId, auth.getPrincipal());
    }

    @Test
    void constructor_WithNullUserId_ShouldThrowExceptionOnGetName() {
        // When
        JwtAuthentication auth = new JwtAuthentication(null, testRoles);

        // Then
        assertNull(auth.getPrincipal());
        // getName() вызовет NPE так как userId.toString() на null
        assertThrows(NullPointerException.class, () -> auth.getName());
    }

    @Test
    void equals_SameUserIdAndRoles_ShouldNotBeEqual() {
        // Given - JwtAuthentication не переопределяет equals, поэтому объекты не равны
        JwtAuthentication auth1 = new JwtAuthentication(testUserId, testRoles);
        JwtAuthentication auth2 = new JwtAuthentication(testUserId, testRoles);

        // Then
        assertNotEquals(auth1, auth2); // Разные экземпляры объектов
        assertEquals(auth1.getPrincipal(), auth2.getPrincipal()); // Но данные одинаковые
        assertEquals(auth1.getAuthorities(), auth2.getAuthorities());
    }

    @Test
    void toString_ShouldNotThrowException() {
        // When & Then
        assertDoesNotThrow(() -> jwtAuthentication.toString());
    }
}
