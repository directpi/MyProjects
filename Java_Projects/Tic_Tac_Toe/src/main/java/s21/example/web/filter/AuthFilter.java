package s21.example.web.filter;

import jakarta.servlet.FilterChain;
import jakarta.servlet.ServletException;
import jakarta.servlet.ServletRequest;
import jakarta.servlet.ServletResponse;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.authentication.BadCredentialsException;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.AuthenticationException;
import org.springframework.security.core.authority.SimpleGrantedAuthority;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.web.filter.GenericFilterBean;
import s21.example.domain.service.AuthService;

import java.io.IOException;
import java.util.Collections;
import java.util.UUID;

public class AuthFilter extends GenericFilterBean {
    private final AuthService authService;

    @Autowired
    public AuthFilter(AuthService authService) {
        this.authService = authService;
    }

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain)
            throws IOException, ServletException {

        HttpServletRequest httpRequest = (HttpServletRequest) request;

        // Пропускаем запросы к эндпоинтам авторизации и публичным ресурсам (Swagger/статикa)
        String uri = httpRequest.getRequestURI();

        if (
                uri.equals("/auth/logout") ||
                uri.startsWith("/auth") ||
                uri.startsWith("/swagger-ui") ||
                uri.equals("/swagger-ui.html") ||
                uri.startsWith("/v3/api-docs") ||
                uri.startsWith("/api-docs") ||
                uri.startsWith("/static/") ||
                uri.startsWith("/webjars/") ||
                uri.startsWith("/favicon")
        ) {
            chain.doFilter(request, response);
            return;
        }

        try {
            String authHeader = httpRequest.getHeader("Authorization");
            if (authHeader == null) {
                throw new BadCredentialsException("Missing auth header");
            }

            // Главное исправление - сохраняем результат authenticate()
            UUID userId = authService.authenticate(authHeader);

            // Создаем аутентифицированный контекст
            UsernamePasswordAuthenticationToken auth = new UsernamePasswordAuthenticationToken(
                    userId,
                    null,
                    Collections.singletonList(new SimpleGrantedAuthority("USER"))
            );

            SecurityContextHolder.getContext().setAuthentication(auth);

            chain.doFilter(request, response);

        } catch (AuthenticationException ex) {
            ((HttpServletResponse)response).sendError(HttpServletResponse.SC_UNAUTHORIZED);
        }
    }
}