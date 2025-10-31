package example.domain.service;

import example.datasource.repository.UserRepository;
import example.domain.model.Role;
import example.domain.model.User;
import example.web.model.JwtRequest;
import example.web.model.JwtResponse;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;
import org.springframework.security.core.AuthenticationException;
import org.springframework.security.crypto.password.PasswordEncoder;

import java.util.Map;
import java.util.Optional;
import java.util.UUID;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.*;

@ExtendWith(MockitoExtension.class)
class AuthServiceImplTest {

    @Mock
    private UserRepository userRepository;

    @Mock
    private PasswordEncoder passwordEncoder;

    @Mock
    private JwtProvider jwtProvider;

    @InjectMocks
    private AuthServiceImpl authService;

    private User testUser;
    private JwtRequest testJwtRequest;

    @BeforeEach
    void setUp() {
        testUser = new User();
        testUser.setId(UUID.randomUUID());
        testUser.setLogin("testuser");
        testUser.setPassword("encodedPassword");
        testUser.setRole(Role.USER);

        testJwtRequest = new JwtRequest();
        testJwtRequest.setLogin("testuser");
        testJwtRequest.setPassword("rawPassword");
    }

    @Test
    void authenticate_WithValidCredentials_ShouldReturnJwtResponse() {
        // Given
        String accessToken = "access-token";
        String refreshToken = "refresh-token";

        when(userRepository.findByLogin("testuser")).thenReturn(Optional.of(testUser));
        when(passwordEncoder.matches("rawPassword", "encodedPassword")).thenReturn(true);
        when(jwtProvider.generateAccessToken(testUser)).thenReturn(accessToken);
        when(jwtProvider.generateRefreshToken(testUser)).thenReturn(refreshToken);

        // When
        JwtResponse response = authService.authenticate(testJwtRequest);

        // Then
        assertNotNull(response);
        assertEquals("Bearer", response.type());
        assertEquals(accessToken, response.accessToken());
        assertEquals(refreshToken, response.refreshToken());

        verify(userRepository).findByLogin("testuser");
        verify(passwordEncoder).matches("rawPassword", "encodedPassword");
        verify(jwtProvider).generateAccessToken(testUser);
        verify(jwtProvider).generateRefreshToken(testUser);
    }

    @Test
    void authenticate_WithNonExistentUser_ShouldThrowException() {
        // Given
        when(userRepository.findByLogin("testuser")).thenReturn(Optional.empty());

        // When & Then
        assertThrows(AuthenticationException.class, () -> authService.authenticate(testJwtRequest));

        verify(userRepository).findByLogin("testuser");
        verify(passwordEncoder, never()).matches(any(), any());
        verify(jwtProvider, never()).generateAccessToken(any());
        verify(jwtProvider, never()).generateRefreshToken(any());
    }

    @Test
    void authenticate_WithWrongPassword_ShouldThrowException() {
        // Given
        when(userRepository.findByLogin("testuser")).thenReturn(Optional.of(testUser));
        when(passwordEncoder.matches("rawPassword", "encodedPassword")).thenReturn(false);

        // When & Then
        assertThrows(AuthenticationException.class, () -> authService.authenticate(testJwtRequest));

        verify(userRepository).findByLogin("testuser");
        verify(passwordEncoder).matches("rawPassword", "encodedPassword");
        verify(jwtProvider, never()).generateAccessToken(any());
        verify(jwtProvider, never()).generateRefreshToken(any());
    }

    @Test
    void refreshAccessToken_WithValidRefreshToken_ShouldReturnNewAccessToken() {
        // Given
        String refreshToken = "valid-refresh-token";
        String newAccessToken = "new-access-token";
        Map<String, Object> claims = Map.of("sub", testUser.getId().toString());

        when(jwtProvider.validateRefreshToken(refreshToken)).thenReturn(true);
        when(jwtProvider.getClaims(refreshToken)).thenReturn(claims);
        when(userRepository.findById(testUser.getId())).thenReturn(Optional.of(testUser));
        when(jwtProvider.generateAccessToken(testUser)).thenReturn(newAccessToken);

        // When
        JwtResponse response = authService.refreshAccessToken(refreshToken);

        // Then
        assertNotNull(response);
        assertEquals("Bearer", response.type());
        assertEquals(newAccessToken, response.accessToken());
        assertEquals(refreshToken, response.refreshToken()); // Старый refresh токен остается

        verify(jwtProvider).validateRefreshToken(refreshToken);
        verify(jwtProvider).getClaims(refreshToken);
        verify(userRepository).findById(testUser.getId());
        verify(jwtProvider).generateAccessToken(testUser);
    }

    @Test
    void refreshAccessToken_WithInvalidRefreshToken_ShouldThrowException() {
        // Given
        String invalidRefreshToken = "invalid-refresh-token";
        when(jwtProvider.validateRefreshToken(invalidRefreshToken)).thenReturn(false);

        // When & Then
        assertThrows(AuthenticationException.class, () -> authService.refreshAccessToken(invalidRefreshToken));

        verify(jwtProvider).validateRefreshToken(invalidRefreshToken);
        verify(jwtProvider, never()).getClaims(any());
        verify(userRepository, never()).findById(any());
    }

    @Test
    void refreshAccessToken_WithNonExistentUser_ShouldThrowException() {
        // Given
        String refreshToken = "valid-refresh-token";
        Map<String, Object> claims = Map.of("sub", testUser.getId().toString());

        when(jwtProvider.validateRefreshToken(refreshToken)).thenReturn(true);
        when(jwtProvider.getClaims(refreshToken)).thenReturn(claims);
        when(userRepository.findById(testUser.getId())).thenReturn(Optional.empty());

        // When & Then
        assertThrows(AuthenticationException.class, () -> authService.refreshAccessToken(refreshToken));

        verify(jwtProvider).validateRefreshToken(refreshToken);
        verify(jwtProvider).getClaims(refreshToken);
        verify(userRepository).findById(testUser.getId());
        verify(jwtProvider, never()).generateAccessToken(any());
    }

    @Test
    void refreshRefreshToken_WithValidRefreshToken_ShouldReturnNewTokens() {
        // Given
        String oldRefreshToken = "old-refresh-token";
        String newRefreshToken = "new-refresh-token";
        String newAccessToken = "new-access-token";
        Map<String, Object> claims = Map.of("sub", testUser.getId().toString());

        when(jwtProvider.validateRefreshToken(oldRefreshToken)).thenReturn(true);
        when(jwtProvider.getClaims(oldRefreshToken)).thenReturn(claims);
        when(userRepository.findById(testUser.getId())).thenReturn(Optional.of(testUser));
        when(jwtProvider.generateRefreshToken(testUser)).thenReturn(newRefreshToken);
        when(jwtProvider.generateAccessToken(testUser)).thenReturn(newAccessToken);

        // When
        JwtResponse response = authService.refreshRefreshToken(oldRefreshToken);

        // Then
        assertNotNull(response);
        assertEquals("Bearer", response.type());
        assertEquals(newAccessToken, response.accessToken());
        assertEquals(newRefreshToken, response.refreshToken());

        verify(jwtProvider).validateRefreshToken(oldRefreshToken);
        verify(jwtProvider).getClaims(oldRefreshToken);
        verify(userRepository).findById(testUser.getId());
        verify(jwtProvider).generateRefreshToken(testUser);
        verify(jwtProvider).generateAccessToken(testUser);
    }

    @Test
    void refreshRefreshToken_WithInvalidRefreshToken_ShouldThrowException() {
        // Given
        String invalidRefreshToken = "invalid-refresh-token";
        when(jwtProvider.validateRefreshToken(invalidRefreshToken)).thenReturn(false);

        // When & Then
        assertThrows(AuthenticationException.class, () -> authService.refreshRefreshToken(invalidRefreshToken));

        verify(jwtProvider).validateRefreshToken(invalidRefreshToken);
        verify(jwtProvider, never()).getClaims(any());
        verify(userRepository, never()).findById(any());
        verify(jwtProvider, never()).generateRefreshToken(any());
        verify(jwtProvider, never()).generateAccessToken(any());
    }

    @Test
    void refreshRefreshToken_WithNonExistentUser_ShouldThrowException() {
        // Given
        String refreshToken = "valid-refresh-token";
        Map<String, Object> claims = Map.of("sub", testUser.getId().toString());

        when(jwtProvider.validateRefreshToken(refreshToken)).thenReturn(true);
        when(jwtProvider.getClaims(refreshToken)).thenReturn(claims);
        when(userRepository.findById(testUser.getId())).thenReturn(Optional.empty());

        // When & Then
        assertThrows(AuthenticationException.class, () -> authService.refreshRefreshToken(refreshToken));

        verify(jwtProvider).validateRefreshToken(refreshToken);
        verify(jwtProvider).getClaims(refreshToken);
        verify(userRepository).findById(testUser.getId());
        verify(jwtProvider, never()).generateRefreshToken(any());
        verify(jwtProvider, never()).generateAccessToken(any());
    }

    @Test
    void authenticate_ShouldGenerateBothTokens() {
        // Given
        when(userRepository.findByLogin("testuser")).thenReturn(Optional.of(testUser));
        when(passwordEncoder.matches("rawPassword", "encodedPassword")).thenReturn(true);
        when(jwtProvider.generateAccessToken(testUser)).thenReturn("access-token");
        when(jwtProvider.generateRefreshToken(testUser)).thenReturn("refresh-token");

        // When
        JwtResponse response = authService.authenticate(testJwtRequest);

        // Then
        verify(jwtProvider).generateAccessToken(testUser);
        verify(jwtProvider).generateRefreshToken(testUser);
        assertNotNull(response.accessToken());
        assertNotNull(response.refreshToken());
    }

    @Test
    void refreshAccessToken_ShouldKeepSameRefreshToken() {
        // Given
        String refreshToken = "refresh-token";
        Map<String, Object> claims = Map.of("sub", testUser.getId().toString());

        when(jwtProvider.validateRefreshToken(refreshToken)).thenReturn(true);
        when(jwtProvider.getClaims(refreshToken)).thenReturn(claims);
        when(userRepository.findById(testUser.getId())).thenReturn(Optional.of(testUser));
        when(jwtProvider.generateAccessToken(testUser)).thenReturn("new-access-token");

        // When
        JwtResponse response = authService.refreshAccessToken(refreshToken);

        // Then
        assertEquals(refreshToken, response.refreshToken()); // Тот же refresh token
        verify(jwtProvider, never()).generateRefreshToken(any()); // Refresh token не генерируется
    }

    @Test
    void refreshRefreshToken_ShouldGenerateBothNewTokens() {
        // Given
        String oldRefreshToken = "old-refresh-token";
        Map<String, Object> claims = Map.of("sub", testUser.getId().toString());

        when(jwtProvider.validateRefreshToken(oldRefreshToken)).thenReturn(true);
        when(jwtProvider.getClaims(oldRefreshToken)).thenReturn(claims);
        when(userRepository.findById(testUser.getId())).thenReturn(Optional.of(testUser));
        when(jwtProvider.generateRefreshToken(testUser)).thenReturn("new-refresh-token");
        when(jwtProvider.generateAccessToken(testUser)).thenReturn("new-access-token");

        // When
        JwtResponse response = authService.refreshRefreshToken(oldRefreshToken);

        // Then
        assertNotEquals(oldRefreshToken, response.refreshToken()); // Новый refresh token
        verify(jwtProvider).generateRefreshToken(testUser);
        verify(jwtProvider).generateAccessToken(testUser);
    }
}








