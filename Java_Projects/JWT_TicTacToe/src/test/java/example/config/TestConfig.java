package example.config;

import example.domain.service.AuthService;
import example.domain.service.GameService;
import example.domain.service.UserService;
import example.utils.AuthUtils;
import org.springframework.boot.test.context.TestConfiguration;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Primary;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;

import static org.mockito.Mockito.mock;

/**
 * Конфигурация для тестов контроллеров
 */
@TestConfiguration
public class TestConfig {

    @Bean
    @Primary
    public PasswordEncoder passwordEncoder() {
        return new BCryptPasswordEncoder();
    }

    @Bean
    @Primary
    public AuthUtils authUtils() {
        return mock(AuthUtils.class);
    }

    @Bean
    @Primary
    public AuthService authService() {
        return mock(AuthService.class);
    }

    @Bean
    @Primary
    public UserService userService() {
        return mock(UserService.class);
    }

    @Bean
    @Primary
    public GameService gameService() {
        return mock(GameService.class);
    }
}








