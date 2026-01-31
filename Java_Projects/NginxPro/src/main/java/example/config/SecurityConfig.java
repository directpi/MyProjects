package example.config;

import lombok.extern.slf4j.Slf4j;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.config.Customizer;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.config.annotation.web.configurers.AbstractHttpConfigurer;
import org.springframework.security.config.http.SessionCreationPolicy;
import org.springframework.security.core.userdetails.User;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.security.provisioning.InMemoryUserDetailsManager;
import org.springframework.security.web.SecurityFilterChain;
import org.springframework.web.cors.CorsConfiguration;
import org.springframework.web.cors.CorsConfigurationSource;
import org.springframework.web.cors.UrlBasedCorsConfigurationSource;

import java.util.Arrays;

@Slf4j
@Configuration
@EnableWebSecurity
public class SecurityConfig {

        @Bean
        public SecurityFilterChain filterChain(HttpSecurity http) throws Exception {
                log.info("Configuring security filter chain");

                http
                                .cors(cors -> cors.configurationSource(corsConfigurationSource()))
                                .csrf(AbstractHttpConfigurer::disable) // Отключаем CSRF для REST API
                                .sessionManagement(session -> session
                                                .sessionCreationPolicy(SessionCreationPolicy.STATELESS)) // Stateless
                                                                                                         // сессии
                                .authorizeHttpRequests(authz -> authz
                                                // 1. Swagger UI и документация
                                                .requestMatchers(
                                                                "/swagger/**",
                                                                "/api-docs/**",
                                                                "/webjars/**")
                                                .permitAll()

                                                // 2. Публичные эндпоинты
                                                .requestMatchers(
                                                                "/api/v1/health",
                                                                "/api/v1/health/**",
                                                                "/api/v1/version",
                                                                "/api/v1/info")
                                                .permitAll()

                                                // 3. Spring Boot Actuator (если нужно)
                                                .requestMatchers(
                                                                "/actuator/health",
                                                                "/actuator/info")
                                                .permitAll()

                                                // 4. Защищенные endpoint'ы
                                                .requestMatchers("/api/v1/clients/**").hasAnyRole("USER", "ADMIN")
                                                .requestMatchers("/api/v1/products/**").hasAnyRole("USER", "ADMIN")
                                                .requestMatchers("/api/admin/**").hasRole("ADMIN")

                                                // 5. Все остальные запросы требуют аутентификации
                                                .anyRequest().authenticated())
                                .httpBasic(Customizer.withDefaults()) // Базовая HTTP аутентификация
                                .formLogin(AbstractHttpConfigurer::disable) // Отключаем форму логина для REST API
                                .logout(AbstractHttpConfigurer::disable); // Отключаем стандартный logout

                return http.build();
        }

        @Bean
        public CorsConfigurationSource corsConfigurationSource() {
                CorsConfiguration configuration = new CorsConfiguration();
                configuration.setAllowedOrigins(Arrays.asList(
                                "http://localhost:3000",
                                "http://localhost:8080"));
                configuration.setAllowedMethods(Arrays.asList(
                                "GET", "POST", "PUT", "PATCH", "DELETE", "OPTIONS", "HEAD"));
                configuration.setAllowedHeaders(Arrays.asList(
                                "Authorization",
                                "Content-Type",
                                "Accept",
                                "X-Requested-With",
                                "Cache-Control"));
                configuration.setExposedHeaders(Arrays.asList(
                                "Location",
                                "Content-Disposition"));
                configuration.setAllowCredentials(true);
                configuration.setMaxAge(3600L);

                UrlBasedCorsConfigurationSource source = new UrlBasedCorsConfigurationSource();
                source.registerCorsConfiguration("/**", configuration);
                return source;
        }

        @Bean
        public PasswordEncoder passwordEncoder() {
                return new BCryptPasswordEncoder();
        }

        // InMemory пользователи для демонстрации (в реальном проекте использовать базу
        // данных)
        @Bean
        public UserDetailsService userDetailsService(PasswordEncoder passwordEncoder) {
                log.debug("Creating in-memory users for demonstration");

                UserDetails user = User.builder()
                                .username("user")
                                .password(passwordEncoder.encode("user123"))
                                .roles("USER")
                                .build();

                UserDetails admin = User.builder()
                                .username("admin")
                                .password(passwordEncoder.encode("admin123"))
                                .roles("ADMIN", "USER")
                                .build();

                return new InMemoryUserDetailsManager(user, admin);
        }
}