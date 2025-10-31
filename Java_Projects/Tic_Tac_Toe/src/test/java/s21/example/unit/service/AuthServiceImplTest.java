package s21.example.unit.service;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;
import org.springframework.security.core.AuthenticationException;
import org.springframework.security.crypto.password.PasswordEncoder;
import s21.example.datasource.repository.UserRepository;
import s21.example.domain.model.User;
import s21.example.domain.service.AuthServiceImpl;
import s21.example.web.model.SignUpRequest;

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

    @InjectMocks
    private AuthServiceImpl authService;

    @Test
    void register_NewUser_ReturnsTrue() {
        SignUpRequest req = new SignUpRequest();
        req.setLogin("newuser");
        req.setPassword("pass");

        when(userRepository.findByLogin("newuser")).thenReturn(Optional.empty());
        when(passwordEncoder.encode("pass")).thenReturn("hashed");

        boolean created = authService.register(req);
        assertTrue(created);
        verify(userRepository).save(any(User.class));
    }

    @Test
    void register_ExistingUser_ReturnsFalse() {
        SignUpRequest req = new SignUpRequest();
        req.setLogin("exists");
        req.setPassword("pass");

        when(userRepository.findByLogin("exists")).thenReturn(Optional.of(new User()));

        boolean created = authService.register(req);
        assertFalse(created);
        verify(userRepository, never()).save(any());
    }

    @Test
    void authenticate_ValidBasicHeader_ReturnsUserId() {
        String raw = "user:pass";
        String header = "Basic " + java.util.Base64.getEncoder().encodeToString(raw.getBytes());

        User u = new User();
        UUID id = UUID.randomUUID();
        u.setId(id);
        u.setLogin("user");
        u.setPassword("hash");

        when(userRepository.findByLogin("user")).thenReturn(Optional.of(u));
        when(passwordEncoder.matches("pass", "hash")).thenReturn(true);

        UUID result = authService.authenticate(header);
        assertEquals(id, result);
    }

    @Test
    void authenticate_InvalidHeader_Throws() {
        assertThrows(AuthenticationException.class, () -> authService.authenticate(null));
        assertThrows(AuthenticationException.class, () -> authService.authenticate("Bearer abc"));
        String bad = "Basic " + java.util.Base64.getEncoder().encodeToString("badformat".getBytes());
        assertThrows(AuthenticationException.class, () -> authService.authenticate(bad));
    }
}







