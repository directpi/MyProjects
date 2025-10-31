package example.web.filter;

import example.domain.service.AuthService;
import example.domain.service.JwtProvider;
import example.utils.JwtUtil;
import jakarta.servlet.FilterChain;
import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;
import org.springframework.security.core.context.SecurityContextHolder;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.*;

@ExtendWith(MockitoExtension.class)
class AuthFilterTest {

    @Mock
    private AuthService authService;

    @Mock
    private JwtProvider jwtProvider;

    @Mock
    private JwtUtil jwtUtil;

    @Mock
    private HttpServletRequest request;

    @Mock
    private HttpServletResponse response;

    @Mock
    private FilterChain filterChain;

    @InjectMocks
    private AuthFilter authFilter;

    @BeforeEach
    void setUp() {
        SecurityContextHolder.clearContext();
    }

    @Test
    void doFilter_WithPublicEndpoint_ShouldSkipAuthentication() throws IOException, ServletException {
        // Given
        when(request.getRequestURI()).thenReturn("/auth/login");

        // When
        authFilter.doFilter(request, response, filterChain);

        // Then
        verify(filterChain).doFilter(request, response);
        verify(request, never()).getHeader("Authorization");
        verify(jwtProvider, never()).validateAccessToken(any());
        assertNull(SecurityContextHolder.getContext().getAuthentication());
    }

    @Test
    void doFilter_WithRegisterEndpoint_ShouldSkipAuthentication() throws IOException, ServletException {
        // Given
        when(request.getRequestURI()).thenReturn("/auth/register");

        // When
        authFilter.doFilter(request, response, filterChain);

        // Then
        verify(filterChain).doFilter(request, response);
        verify(request, never()).getHeader("Authorization");
        verify(jwtProvider, never()).validateAccessToken(any());
    }

    @Test
    void doFilter_WithRefreshEndpoints_ShouldSkipAuthentication() throws IOException, ServletException {
        // Given - тестируем оба refresh endpoint'а
        when(request.getRequestURI()).thenReturn("/auth/refresh");
        authFilter.doFilter(request, response, filterChain);

        when(request.getRequestURI()).thenReturn("/auth/refresh-token");
        authFilter.doFilter(request, response, filterChain);

        // Then
        verify(filterChain, times(2)).doFilter(request, response);
        verify(request, never()).getHeader("Authorization");
    }

    @Test
    void doFilter_WithSwaggerEndpoints_ShouldSkipAuthentication() throws IOException, ServletException {
        // Given
        when(request.getRequestURI()).thenReturn("/swagger-ui/index.html");

        // When
        authFilter.doFilter(request, response, filterChain);

        // Then
        verify(filterChain).doFilter(request, response);
        verify(request, never()).getHeader("Authorization");
    }

    @Test
    void doFilter_WithValidJWT_ShouldSetAuthentication() throws IOException, ServletException {
        // Given
        String token = "valid-jwt-token";
        String authHeader = "Bearer " + token;
        UUID userId = UUID.randomUUID();

        Map<String, Object> claims = new HashMap<>();
        claims.put("sub", userId.toString());
        claims.put("role", "USER");

        JwtAuthentication jwtAuth = mock(JwtAuthentication.class);

        when(request.getRequestURI()).thenReturn("/game/create");
        when(request.getHeader("Authorization")).thenReturn(authHeader);
        when(jwtProvider.validateAccessToken(token)).thenReturn(true);
        when(jwtProvider.getClaims(token)).thenReturn(claims);
        when(jwtUtil.generate(claims)).thenReturn(jwtAuth);

        // When
        authFilter.doFilter(request, response, filterChain);

        // Then
        verify(filterChain).doFilter(request, response);
        verify(jwtProvider).validateAccessToken(token);
        verify(jwtProvider).getClaims(token);
        verify(jwtUtil).generate(claims);
        assertEquals(jwtAuth, SecurityContextHolder.getContext().getAuthentication());
    }

    @Test
    void doFilter_WithoutAuthHeader_ShouldReturnUnauthorized() throws IOException, ServletException {
        // Given
        when(request.getRequestURI()).thenReturn("/game/create");
        when(request.getHeader("Authorization")).thenReturn(null);

        // When
        authFilter.doFilter(request, response, filterChain);

        // Then
        verify(response).sendError(HttpServletResponse.SC_UNAUTHORIZED);
        verify(filterChain, never()).doFilter(request, response);
        assertNull(SecurityContextHolder.getContext().getAuthentication());
    }

    @Test
    void doFilter_WithInvalidAuthHeader_ShouldReturnUnauthorized() throws IOException, ServletException {
        // Given
        when(request.getRequestURI()).thenReturn("/game/create");
        when(request.getHeader("Authorization")).thenReturn("Basic invalid-header");

        // When
        authFilter.doFilter(request, response, filterChain);

        // Then
        verify(response).sendError(HttpServletResponse.SC_UNAUTHORIZED);
        verify(filterChain, never()).doFilter(request, response);
        verify(jwtProvider, never()).validateAccessToken(any());
    }

    @Test
    void doFilter_WithInvalidJWT_ShouldReturnUnauthorized() throws IOException, ServletException {
        // Given
        String token = "invalid-jwt-token";
        String authHeader = "Bearer " + token;

        when(request.getRequestURI()).thenReturn("/game/create");
        when(request.getHeader("Authorization")).thenReturn(authHeader);
        when(jwtProvider.validateAccessToken(token)).thenReturn(false);

        // When
        authFilter.doFilter(request, response, filterChain);

        // Then
        verify(response).sendError(HttpServletResponse.SC_UNAUTHORIZED);
        verify(filterChain, never()).doFilter(request, response);
        verify(jwtProvider).validateAccessToken(token);
        verify(jwtProvider, never()).getClaims(any());
        assertNull(SecurityContextHolder.getContext().getAuthentication());
    }

    @Test
    void doFilter_WithJWTValidationException_ShouldReturnUnauthorized() throws IOException, ServletException {
        // Given
        String token = "jwt-token-causing-exception";
        String authHeader = "Bearer " + token;

        when(request.getRequestURI()).thenReturn("/game/create");
        when(request.getHeader("Authorization")).thenReturn(authHeader);
        when(jwtProvider.validateAccessToken(token)).thenThrow(new RuntimeException("JWT parsing error"));

        // When
        authFilter.doFilter(request, response, filterChain);

        // Then
        verify(response).sendError(HttpServletResponse.SC_UNAUTHORIZED);
        verify(filterChain, never()).doFilter(request, response);
        assertNull(SecurityContextHolder.getContext().getAuthentication());
    }

    @Test
    void doFilter_WithClaimsException_ShouldReturnUnauthorized() throws IOException, ServletException {
        // Given
        String token = "valid-token-but-claims-fail";
        String authHeader = "Bearer " + token;

        when(request.getRequestURI()).thenReturn("/game/create");
        when(request.getHeader("Authorization")).thenReturn(authHeader);
        when(jwtProvider.validateAccessToken(token)).thenReturn(true);
        when(jwtProvider.getClaims(token)).thenThrow(new RuntimeException("Claims extraction error"));

        // When
        authFilter.doFilter(request, response, filterChain);

        // Then
        verify(response).sendError(HttpServletResponse.SC_UNAUTHORIZED);
        verify(filterChain, never()).doFilter(request, response);
        verify(jwtProvider).validateAccessToken(token);
        verify(jwtProvider).getClaims(token);
        assertNull(SecurityContextHolder.getContext().getAuthentication());
    }

    @Test
    void doFilter_WithEmptyBearerToken_ShouldReturnUnauthorized() throws IOException, ServletException {
        // Given
        when(request.getRequestURI()).thenReturn("/game/create");
        when(request.getHeader("Authorization")).thenReturn("Bearer ");

        // When
        authFilter.doFilter(request, response, filterChain);

        // Then
        verify(response).sendError(HttpServletResponse.SC_UNAUTHORIZED);
        verify(filterChain, never()).doFilter(request, response);
        // Не проверяем validateAccessToken так как может быть вызван с пустой строкой
    }

    @Test
    void doFilter_WithBearerTokenWithSpaces_ShouldTrimAndValidate() throws IOException, ServletException {
        // Given
        String token = "valid-jwt-token";
        String authHeader = "Bearer   " + token + "   "; // Пробелы до и после токена
        UUID userId = UUID.randomUUID();

        Map<String, Object> claims = new HashMap<>();
        claims.put("sub", userId.toString());

        JwtAuthentication jwtAuth = mock(JwtAuthentication.class);

        when(request.getRequestURI()).thenReturn("/game/create");
        when(request.getHeader("Authorization")).thenReturn(authHeader);
        when(jwtProvider.validateAccessToken(token)).thenReturn(true);
        when(jwtProvider.getClaims(token)).thenReturn(claims);
        when(jwtUtil.generate(claims)).thenReturn(jwtAuth);

        // When
        authFilter.doFilter(request, response, filterChain);

        // Then
        verify(filterChain).doFilter(request, response);
        verify(jwtProvider).validateAccessToken(token); // Токен должен быть обрезан
        assertEquals(jwtAuth, SecurityContextHolder.getContext().getAuthentication());
    }

    @Test
    void doFilter_WithProtectedEndpoint_ShouldRequireAuthentication() throws IOException, ServletException {
        // Given - тестируем один защищенный endpoint
        when(request.getRequestURI()).thenReturn("/game/create");
        when(request.getHeader("Authorization")).thenReturn(null);

        // When
        authFilter.doFilter(request, response, filterChain);

        // Then
        verify(response).sendError(HttpServletResponse.SC_UNAUTHORIZED);
        verify(filterChain, never()).doFilter(request, response);
    }

    @Test
    void doFilter_ShouldClearSecurityContextOnException() throws IOException, ServletException {
        // Given
        String token = "token-causing-util-exception";
        String authHeader = "Bearer " + token;
        Map<String, Object> claims = Map.of("sub", UUID.randomUUID().toString());

        when(request.getRequestURI()).thenReturn("/game/create");
        when(request.getHeader("Authorization")).thenReturn(authHeader);
        when(jwtProvider.validateAccessToken(token)).thenReturn(true);
        when(jwtProvider.getClaims(token)).thenReturn(claims);
        when(jwtUtil.generate(claims)).thenThrow(new RuntimeException("JwtUtil error"));

        // When
        authFilter.doFilter(request, response, filterChain);

        // Then
        verify(response).sendError(HttpServletResponse.SC_UNAUTHORIZED);
        verify(filterChain, never()).doFilter(request, response);
        assertNull(SecurityContextHolder.getContext().getAuthentication());
    }
}
