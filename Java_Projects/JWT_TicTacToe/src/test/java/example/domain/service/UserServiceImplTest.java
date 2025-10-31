package example.domain.service;

import example.datasource.repository.UserRepository;
import example.domain.model.Role;
import example.domain.model.User;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;

import java.util.Optional;
import java.util.UUID;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.*;

@ExtendWith(MockitoExtension.class)
class UserServiceImplTest {

    @Mock
    private UserRepository userRepository;

    @InjectMocks
    private UserServiceImpl userService;

    private UUID testUserId;
    private User testUser;

    @BeforeEach
    void setUp() {
        testUserId = UUID.randomUUID();

        testUser = new User();
        testUser.setId(testUserId);
        testUser.setLogin("testuser");
        testUser.setPassword("encodedPassword");
        testUser.setRole(Role.USER);
    }

    @Test
    void getUserById_WithExistingUser_ShouldReturnUser() {
        // Given
        when(userRepository.findById(testUserId)).thenReturn(Optional.of(testUser));

        // When
        User result = userService.getUserById(testUserId);

        // Then
        assertNotNull(result);
        assertEquals(testUserId, result.getId());
        assertEquals("testuser", result.getLogin());
        assertEquals("encodedPassword", result.getPassword());
        assertEquals(Role.USER, result.getRole());

        verify(userRepository).findById(testUserId);
    }

    @Test
    void getUserById_WithNonExistentUser_ShouldThrowException() {
        // Given
        UUID nonExistentId = UUID.randomUUID();
        when(userRepository.findById(nonExistentId)).thenReturn(Optional.empty());

        // When & Then
        RuntimeException exception = assertThrows(RuntimeException.class,
                () -> userService.getUserById(nonExistentId));

        assertTrue(exception.getMessage().contains("не найден"));
        assertTrue(exception.getMessage().contains(nonExistentId.toString()));

        verify(userRepository).findById(nonExistentId);
    }

    @Test
    void getUserById_WithNullId_ShouldCallRepository() {
        // Given - тестируем поведение с null, хотя это не рекомендуется
        UUID nullId = null;
        when(userRepository.findById(any())).thenReturn(Optional.empty());

        // When & Then
        RuntimeException exception = assertThrows(RuntimeException.class,
                () -> userService.getUserById(nullId));

        // Проверяем, что исключение содержит информацию о null
        assertTrue(exception.getMessage().contains("null"));
        verify(userRepository).findById(any());
    }

    @Test
    void getUserById_WithRepositoryException_ShouldPropagateException() {
        // Given
        when(userRepository.findById(testUserId))
                .thenThrow(new RuntimeException("Database connection error"));

        // When & Then
        RuntimeException exception = assertThrows(RuntimeException.class,
                () -> userService.getUserById(testUserId));

        assertEquals("Database connection error", exception.getMessage());
        verify(userRepository).findById(testUserId);
    }

    @Test
    void getUserById_ShouldReturnUserWithAllFields() {
        // Given
        User detailedUser = new User();
        detailedUser.setId(testUserId);
        detailedUser.setLogin("detaileduser");
        detailedUser.setPassword("hashedPassword123");
        detailedUser.setRole(Role.USER);

        when(userRepository.findById(testUserId)).thenReturn(Optional.of(detailedUser));

        // When
        User result = userService.getUserById(testUserId);

        // Then
        assertEquals(testUserId, result.getId());
        assertEquals("detaileduser", result.getLogin());
        assertEquals("hashedPassword123", result.getPassword());
        assertEquals(Role.USER, result.getRole());

        // Проверяем UserDetails методы
        assertEquals("detaileduser", result.getUsername());
        assertTrue(result.isAccountNonExpired());
        assertTrue(result.isAccountNonLocked());
        assertTrue(result.isCredentialsNonExpired());
        assertTrue(result.isEnabled());
        assertNotNull(result.getAuthorities());
        assertFalse(result.getAuthorities().isEmpty());

        verify(userRepository).findById(testUserId);
    }

    @Test
    void getUserById_WithDifferentUserIds_ShouldCallCorrectRepository() {
        // Given
        UUID userId1 = UUID.randomUUID();
        UUID userId2 = UUID.randomUUID();

        User user1 = new User();
        user1.setId(userId1);
        user1.setLogin("user1");

        User user2 = new User();
        user2.setId(userId2);
        user2.setLogin("user2");

        when(userRepository.findById(userId1)).thenReturn(Optional.of(user1));
        when(userRepository.findById(userId2)).thenReturn(Optional.of(user2));

        // When
        User result1 = userService.getUserById(userId1);
        User result2 = userService.getUserById(userId2);

        // Then
        assertEquals(userId1, result1.getId());
        assertEquals("user1", result1.getLogin());

        assertEquals(userId2, result2.getId());
        assertEquals("user2", result2.getLogin());

        verify(userRepository).findById(userId1);
        verify(userRepository).findById(userId2);
    }

    @Test
    void getUserById_ExceptionMessage_ShouldContainUserId() {
        // Given
        UUID specificId = UUID.fromString("123e4567-e89b-12d3-a456-426614174000");
        when(userRepository.findById(specificId)).thenReturn(Optional.empty());

        // When & Then
        RuntimeException exception = assertThrows(RuntimeException.class,
                () -> userService.getUserById(specificId));

        String expectedMessage = "Пользователь с ID " + specificId + " не найден";
        assertEquals(expectedMessage, exception.getMessage());

        verify(userRepository).findById(specificId);
    }

    @Test
    void getUserById_MultipleCallsSameId_ShouldCallRepositoryEachTime() {
        // Given
        when(userRepository.findById(testUserId)).thenReturn(Optional.of(testUser));

        // When
        userService.getUserById(testUserId);
        userService.getUserById(testUserId);
        userService.getUserById(testUserId);

        // Then
        verify(userRepository, times(3)).findById(testUserId);
    }
}
