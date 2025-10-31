package example.web.filter;

import example.domain.service.JwtProvider;
import example.utils.JwtUtil;
import jakarta.servlet.FilterChain;
import jakarta.servlet.ServletException;
import jakarta.servlet.ServletRequest;
import jakarta.servlet.ServletResponse;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.authentication.BadCredentialsException;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.stereotype.Component;
import org.springframework.web.filter.GenericFilterBean;

import java.io.IOException;
import java.util.Map;

@Component
public class AuthFilter extends GenericFilterBean {
    private final JwtProvider jwtProvider;
    private final JwtUtil jwtUtil;

    @Autowired
    public AuthFilter(JwtProvider jwtProvider,  
                      JwtUtil jwtUtil) {
        this.jwtProvider = jwtProvider;
        this.jwtUtil = jwtUtil;
    }

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain)
            throws IOException, ServletException {

        HttpServletRequest httpRequest = (HttpServletRequest) request;
        String uri = httpRequest.getRequestURI();

        // Пропускаем публичные эндпоинты
        if (isPublicEndpoint(uri)) {
            chain.doFilter(request, response);
            return;
        }

        try {
            String authHeader = httpRequest.getHeader("Authorization");
            if (authHeader == null || !authHeader.startsWith("Bearer ")) {
                throw new BadCredentialsException("Missing or invalid auth header");
            }

            // JWT логика вместо Basic Auth
            String token = authHeader.substring("Bearer ".length()).trim();

            if (jwtProvider.validateAccessToken(token)) {
                Map<String, Object> claims = jwtProvider.getClaims(token);
                JwtAuthentication auth = jwtUtil.generate(claims);
                SecurityContextHolder.getContext().setAuthentication(auth);
            } else {
                throw new BadCredentialsException("Invalid JWT token");
            }

        } catch (Exception ex) {
            ((HttpServletResponse)response).sendError(HttpServletResponse.SC_UNAUTHORIZED);
            return;
        }
        chain.doFilter(request, response);
    }
    private boolean isPublicEndpoint(String uri) {
        return uri.startsWith("/auth/register") ||
                uri.startsWith("/auth/login") ||
                uri.startsWith("/auth/refresh") ||
                uri.startsWith("/auth/refresh-token") ||
                uri.startsWith("/swagger-ui") ||
                uri.startsWith("/v3/api-docs");
    }
}