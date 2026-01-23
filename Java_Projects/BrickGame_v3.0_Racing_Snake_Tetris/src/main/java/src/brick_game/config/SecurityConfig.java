/**
 * @file: SecurityConfig.java
 * @description: Конфигурация Spring Security: разрешаем доступ к /api/** без аутентификации,
 *               отключаем CSRF для REST, остальные ресурсы под базовой защитой.
 * @dependencies: org.springframework.security.config.annotation.web.builders.HttpSecurity
 * @created: 2025-10-05
 */
package src.brick_game.config;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.web.SecurityFilterChain;

@Configuration
@EnableWebSecurity
public class SecurityConfig {

    /**
     * Dev-friendly default: public API enabled.
     * Production: set brickgame.security.public-api=false and enable auth properly.
     */
    @Value("${brickgame.security.public-api:true}")
    private boolean publicApi;

    /**
     * Keep disabled by default (avoid prompting browsers with Basic Auth).
     * Enable only when actually configured/needed.
     */
    @Value("${brickgame.security.http-basic:false}")
    private boolean httpBasicEnabled;

    @Bean
    public SecurityFilterChain filterChain(HttpSecurity http) throws Exception {
        http.csrf(csrf -> csrf.disable());

        http.authorizeHttpRequests(authz -> {
            authz.requestMatchers("/swagger-ui/**", "/api-docs/**").permitAll();
            authz.requestMatchers("/", "/index.html", "/static/**").permitAll();
            if (publicApi) {
                authz.requestMatchers("/api/**").permitAll();
            } else {
                authz.requestMatchers("/api/**").authenticated();
            }
            authz.anyRequest().authenticated();
        });

        if (httpBasicEnabled) {
            http.httpBasic(basic -> {});
        } else {
            http.httpBasic(basic -> basic.disable());
        }

        http.formLogin(form -> form.disable());
        return http.build();
    }
}
