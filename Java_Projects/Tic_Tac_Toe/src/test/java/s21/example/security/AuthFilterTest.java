package s21.example.security;

import jakarta.servlet.FilterChain;
import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;
import s21.example.domain.service.AuthService;
import s21.example.web.filter.AuthFilter;

import java.io.IOException;
import java.util.UUID;

import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.*;

@ExtendWith(MockitoExtension.class)
class AuthFilterTest {

    @Mock
    private AuthService authService;

    @Mock
    private HttpServletRequest request;

    @Mock
    private HttpServletResponse response;

    @Mock
    private FilterChain chain;

    @Test
    void doFilter_PassPublicPaths() throws IOException, ServletException {
        // Проверяем, что публичные пути проходят без аутентификации
        AuthFilter filter = new AuthFilter(authService);
        when(request.getRequestURI()).thenReturn("/swagger-ui/index.html");

        filter.doFilter(request, response, chain);
        verify(chain).doFilter(request, response);
        verifyNoInteractions(authService);
    }

    @Test
    void doFilter_MissingHeader_Returns401() throws IOException, ServletException {
        // Проверяем, что при отсутствии заголовка Authorization возвращается 401
        AuthFilter filter = new AuthFilter(authService);
        when(request.getRequestURI()).thenReturn("/game/available");
        when(request.getHeader("Authorization")).thenReturn(null);

        filter.doFilter(request, response, chain);
        verify(response).sendError(HttpServletResponse.SC_UNAUTHORIZED);
        verify(chain, never()).doFilter(any(), any());
    }

    @Test
    void doFilter_ValidHeader_AuthenticatesAndProceeds() throws IOException, ServletException {
        // Проверяем, что при валидном заголовке происходит аутентификация и запрос проходит дальше
        AuthFilter filter = new AuthFilter(authService);
        when(request.getRequestURI()).thenReturn("/game/available");
        when(request.getHeader("Authorization")).thenReturn("Basic dTpw");
        when(authService.authenticate(any())).thenReturn(UUID.randomUUID());

        filter.doFilter(request, response, chain);
        verify(chain).doFilter(request, response);
    }
}
