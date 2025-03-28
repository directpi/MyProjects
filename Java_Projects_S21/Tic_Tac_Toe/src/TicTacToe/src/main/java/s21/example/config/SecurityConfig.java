package s21.example.config;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.config.Customizer;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.config.annotation.web.configurers.AbstractHttpConfigurer;
import org.springframework.security.core.userdetails.User;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.security.provisioning.InMemoryUserDetailsManager;
import org.springframework.security.web.SecurityFilterChain;

@Configuration
@EnableWebSecurity
public class SecurityConfig {

    @Bean
    public SecurityFilterChain securityFilterChain(HttpSecurity http) throws Exception {
        http
                .authorizeHttpRequests(auth -> auth
                        .requestMatchers("/game/start").authenticated() // Требуем аутентификации для /game/start
                        .anyRequest().permitAll() // Остальные эндпоинты доступны без аутентификации
                )
                .httpBasic(Customizer.withDefaults()) // Включаем базовую аутентификацию
                .csrf(AbstractHttpConfigurer::disable); // Отключаем CSRF для упрощения тестирования
        return http.build();
    }

    @Bean
    public UserDetailsService userDetailsService() {
        // Создаем пользователя для тестирования
        UserDetails user = User.builder()
                .username("myserver") // Логин
                .password(passwordEncoder().encode("123456")) // Пароль
                .roles("USER") // Роль
                .build();

        return new InMemoryUserDetailsManager(user); // Храним пользователя в памяти
    }

    @Bean
    public PasswordEncoder passwordEncoder() {
        return new BCryptPasswordEncoder(); // Используем BCrypt для хеширования паролей
    }
}