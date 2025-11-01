/**
 * @file: SecurityConfig.java
 * @description: Конфигурация Spring Security: разрешаем доступ к /api/** без аутентификации,
 *               отключаем CSRF для REST, остальные ресурсы под базовой защитой.
 * @dependencies: org.springframework.security.config.annotation.web.builders.HttpSecurity
 * @created: 2025-10-05
 */
package src.brick_game.config;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.web.SecurityFilterChain;

@Configuration
@EnableWebSecurity
public class SecurityConfig {

    @Bean
    public SecurityFilterChain filterChain(HttpSecurity http) throws Exception {
        http
                .authorizeHttpRequests(authz -> authz
                        .requestMatchers("/api/**").permitAll()
                        .requestMatchers("/swagger-ui/**", "/api-docs/**").permitAll()
                        .requestMatchers("/", "/index.html", "/static/**").permitAll()
                        .anyRequest().authenticated())
                .csrf(csrf -> csrf.disable())
                .httpBasic(basic -> {
                })
                .formLogin(form -> form.disable());
        return http.build();
    }
}
