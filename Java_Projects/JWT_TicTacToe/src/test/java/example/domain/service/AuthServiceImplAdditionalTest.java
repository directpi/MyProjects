package example.domain.service;

import example.datasource.repository.UserRepository;
import example.domain.model.Role;
import example.domain.model.User;
import example.web.model.SignUpRequest;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;
import org.springframework.security.core.AuthenticationException;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.test.util.ReflectionTestUtils;

import java.util.Optional;
import java.util.UUID;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.*;
import org.mockito.junit.jupiter.MockitoSettings;
import org.mockito.quality.Strictness;

@ExtendWith(MockitoExtension.class)
@MockitoSettings(strictness = Strictness.LENIENT)
class AuthServiceImplAdditionalTest {

    @Mock
    private UserRepository userRepository;

    @Mock
    private PasswordEncoder passwordEncoder;

    @Mock
    private JwtProvider jwtProvider;

    @InjectMocks
    private AuthServiceImpl authService;

    private SignUpRequest validSignUpRequest;
    private User testUser;

    @BeforeEach
    void setUp() {
        validSignUpRequest = new SignUpRequest();
        validSignUpRequest.setLogin("testuser");
        validSignUpRequest.setPassword("testpassword");

        testUser = new User();
        testUser.setId(UUID.randomUUID());
        testUser.setLogin("testuser");
        testUser.setPassword("encodedPassword");
        testUser.setRole(Role.USER);

        // Устанавливаем значения через рефлексию для @Value полей
        ReflectionTestUtils.setField(authService, "defaultUserLogin", "defaultuser");
        ReflectionTestUtils.setField(authService, "defaultUserPassword", "defaultpass");
    }

    @Test
    void register_WithValidRequest_ShouldReturnUserId() {
        // Given
        when(userRepository.findByLogin("testuser")).thenReturn(Optional.empty());
        when(passwordEncoder.encode("testpassword")).thenReturn("encodedPassword");
        when(userRepository.save(any(User.class))).thenReturn(testUser);

        // When
        UUID result = authService.register(validSignUpRequest);

        // Then
        assertEquals(testUser.getId(), result);
        verify(userRepository).findByLogin("testuser");
        verify(passwordEncoder).encode("testpassword");
        verify(userRepository).save(any(User.class));
    }

    @Test
    void register_WithExistingUser_ShouldThrowException() {
        // Given
        when(userRepository.findByLogin("testuser")).thenReturn(Optional.of(testUser));

        // When & Then
        IllegalStateException exception = assertThrows(IllegalStateException.class,
                () -> authService.register(validSignUpRequest));
        assertEquals("Пользователь testuser уже существует", exception.getMessage());

        verify(userRepository).findByLogin("testuser");
        verify(passwordEncoder, never()).encode(any());
        verify(userRepository, never()).save(any());
    }

    @Test
    void register_ShouldCreateUserWithCorrectData() {
        // Given
        when(userRepository.findByLogin("testuser")).thenReturn(Optional.empty());
        when(passwordEncoder.encode("testpassword")).thenReturn("encodedPassword");
        when(userRepository.save(any(User.class))).thenAnswer(invocation -> {
            User savedUser = invocation.getArgument(0);
            savedUser.setId(UUID.randomUUID());
            return savedUser;
        });

        // When
        authService.register(validSignUpRequest);

        // Then
        verify(userRepository).save(argThat(user -> "testuser".equals(user.getLogin()) &&
                "encodedPassword".equals(user.getPassword()) &&
                Role.USER.equals(user.getRole())));
    }

    @Test
    void register_WithNullLogin_ShouldThrowException() {
        // Given
        SignUpRequest invalidRequest = new SignUpRequest();
        invalidRequest.setLogin(null);
        invalidRequest.setPassword("password");

        // When & Then
        assertThrows(Exception.class, () -> authService.register(invalidRequest));
    }

    @Test
    void register_WithEmptyLogin_ShouldAttemptToSave() {
        // Given
        SignUpRequest requestWithEmptyLogin = new SignUpRequest();
        requestWithEmptyLogin.setLogin("");
        requestWithEmptyLogin.setPassword("password");

        when(userRepository.findByLogin("")).thenReturn(Optional.empty());
        when(passwordEncoder.encode("password")).thenReturn("encodedPassword");
        when(userRepository.save(any(User.class))).thenReturn(testUser);

        // When
        UUID result = authService.register(requestWithEmptyLogin);

        // Then
        assertNotNull(result);
        verify(userRepository).findByLogin("");
        verify(userRepository).save(any(User.class));
    }

    @Test
    void hasActiveSecondPlayer_WithLessThanTwoUsers_ShouldReturnFalse() {
        // Given
        when(userRepository.count()).thenReturn(1L);

        // When
        boolean result = authService.hasActiveSecondPlayer();

        // Then
        assertFalse(result);
        verify(userRepository).count();
    }

    @Test
    void hasActiveSecondPlayer_WithTwoOrMoreUsers_ShouldReturnTrue() {
        // Given
        when(userRepository.count()).thenReturn(2L);

        // When
        boolean result = authService.hasActiveSecondPlayer();

        // Then
        assertTrue(result);
        verify(userRepository).count();
    }

    @Test
    void hasActiveSecondPlayer_WithExactlyTwoUsers_ShouldReturnTrue() {
        // Given
        when(userRepository.count()).thenReturn(2L);

        // When
        boolean result = authService.hasActiveSecondPlayer();

        // Then
        assertTrue(result);
        verify(userRepository).count();
    }

    @Test
    void hasActiveSecondPlayer_WithManyUsers_ShouldReturnTrue() {
        // Given
        when(userRepository.count()).thenReturn(10L);

        // When
        boolean result = authService.hasActiveSecondPlayer();

        // Then
        assertTrue(result);
        verify(userRepository).count();
    }

    @Test
    void initializeDefaultUser_WhenUserDoesNotExist_ShouldCreateUser() {
        // Given
        when(userRepository.findByLogin("defaultuser")).thenReturn(Optional.empty());
        when(passwordEncoder.encode("defaultpass")).thenReturn("encodedDefaultPass");

        // When
        authService.initializeDefaultUser();

        // Then
        verify(userRepository).findByLogin("defaultuser");
        verify(passwordEncoder).encode("defaultpass");
        verify(userRepository).save(argThat(user -> "defaultuser".equals(user.getLogin()) &&
                "encodedDefaultPass".equals(user.getPassword()) &&
                Role.USER.equals(user.getRole())));
    }

    @Test
    void initializeDefaultUser_WhenUserExists_ShouldNotCreateUser() {
        // Given
        User existingUser = new User();
        existingUser.setLogin("defaultuser");
        when(userRepository.findByLogin("defaultuser")).thenReturn(Optional.of(existingUser));

        // When
        authService.initializeDefaultUser();

        // Then
        verify(userRepository).findByLogin("defaultuser");
        verify(passwordEncoder, never()).encode(any());
        verify(userRepository, never()).save(any());
    }

    // Примечание: Basic Auth тесты удалены, так как этот функционал устарел
    // и конфликтует с новой JWT архитектурой. Оставлены только JWT тесты.

    @Test
    void authenticate_JwtRequest_WithNonAsciiLogin_ShouldThrowException() {
        // Given
        example.web.model.JwtRequest requestWithNonAscii = new example.web.model.JwtRequest();
        requestWithNonAscii.setLogin("тестuser"); // Кириллица
        requestWithNonAscii.setPassword("password");

        // When & Then
        assertThrows(AuthenticationException.class,
                () -> authService.authenticate(requestWithNonAscii));
    }

    @Test
    void authenticate_JwtRequest_WithAsciiLogin_ShouldProceed() {
        // Given
        example.web.model.JwtRequest requestWithAscii = new example.web.model.JwtRequest();
        requestWithAscii.setLogin("testuser123"); // Только ASCII
        requestWithAscii.setPassword("password");

        when(userRepository.findByLogin("testuser123")).thenReturn(Optional.of(testUser));
        when(passwordEncoder.matches("password", "encodedPassword")).thenReturn(true);
        when(jwtProvider.generateAccessToken(testUser)).thenReturn("access-token");
        when(jwtProvider.generateRefreshToken(testUser)).thenReturn("refresh-token");

        // When & Then
        assertDoesNotThrow(() -> authService.authenticate(requestWithAscii));
        verify(userRepository).findByLogin("testuser123");
    }

    @Test
    void containsNonAscii_WithNullString_ShouldReturnFalse() {
        // Given & When & Then
        // Используем рефлексию для доступа к приватному методу
        try {
            java.lang.reflect.Method method = AuthServiceImpl.class.getDeclaredMethod("containsNonAscii", String.class);
            method.setAccessible(true);
            boolean result = (boolean) method.invoke(authService, (String) null);
            assertFalse(result);
        } catch (Exception e) {
            fail("Не удалось протестировать приватный метод containsNonAscii");
        }
    }

    @Test
    void containsNonAscii_WithAsciiString_ShouldReturnFalse() {
        // Given & When & Then
        try {
            java.lang.reflect.Method method = AuthServiceImpl.class.getDeclaredMethod("containsNonAscii", String.class);
            method.setAccessible(true);
            boolean result = (boolean) method.invoke(authService, "testuser123");
            assertFalse(result);
        } catch (Exception e) {
            fail("Не удалось протестировать приватный метод containsNonAscii");
        }
    }

    @Test
    void containsNonAscii_WithNonAsciiString_ShouldReturnTrue() {
        // Given & When & Then
        try {
            java.lang.reflect.Method method = AuthServiceImpl.class.getDeclaredMethod("containsNonAscii", String.class);
            method.setAccessible(true);
            boolean result = (boolean) method.invoke(authService, "тестuser");
            assertTrue(result);
        } catch (Exception e) {
            fail("Не удалось протестировать приватный метод containsNonAscii");
        }
    }
}
