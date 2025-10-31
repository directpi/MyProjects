package example.web.controller;

import com.fasterxml.jackson.databind.ObjectMapper;
import example.domain.service.AuthService;
import example.domain.service.UserService;
import example.web.model.JwtRequest;
import example.web.model.JwtResponse;
import example.web.model.RefreshJwtRequest;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.junit.jupiter.MockitoExtension;
import org.springframework.http.MediaType;
import org.springframework.security.core.AuthenticationException;
import org.springframework.test.web.servlet.MockMvc;

import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.*;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.*;

@ExtendWith(MockitoExtension.class)
class AuthControllerTest {

        private MockMvc mockMvc;

        @org.mockito.Mock
        private AuthService authService;

        @org.mockito.Mock
        private UserService userService;

        private ObjectMapper objectMapper = new ObjectMapper();

        private JwtRequest validJwtRequest;
        private JwtResponse jwtResponse;
        private RefreshJwtRequest refreshJwtRequest;

        @BeforeEach
        void setUp() {
                AuthController controller = new AuthController(authService, userService);
                mockMvc = org.springframework.test.web.servlet.setup.MockMvcBuilders.standaloneSetup(controller)
                                .build();
                validJwtRequest = new JwtRequest();
                validJwtRequest.setLogin("testuser");
                validJwtRequest.setPassword("password");

                jwtResponse = new JwtResponse("Bearer", "access-token", "refresh-token");

                refreshJwtRequest = new RefreshJwtRequest("refresh-token");
        }

        @Test
        void login_WithValidCredentials_ShouldReturnJwtResponse() throws Exception {
                // Given
                when(authService.authenticate(any(JwtRequest.class))).thenReturn(jwtResponse);

                // When & Then
                mockMvc.perform(post("/auth/login")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content(objectMapper.writeValueAsString(validJwtRequest)))
                                .andExpect(status().isOk())
                                .andExpect(content().contentType(MediaType.APPLICATION_JSON))
                                .andExpect(jsonPath("$.type").value("Bearer"))
                                .andExpect(jsonPath("$.accessToken").value("access-token"))
                                .andExpect(jsonPath("$.refreshToken").value("refresh-token"));

                verify(authService).authenticate(any(JwtRequest.class));
        }

        @Test
        void login_WithInvalidCredentials_ShouldReturnUnauthorized() throws Exception {
                // Given
                when(authService.authenticate(any(JwtRequest.class)))
                                .thenThrow(new AuthenticationException("Invalid credentials") {
                                });

                // When & Then
                mockMvc.perform(post("/auth/login")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content(objectMapper.writeValueAsString(validJwtRequest)))
                                .andExpect(status().isUnauthorized());

                verify(authService).authenticate(any(JwtRequest.class));
        }

        @Test
        void login_WithEmptyLogin_ShouldReturnBadRequest() throws Exception {
                // Given
                JwtRequest invalidRequest = new JwtRequest();
                invalidRequest.setLogin(""); // Пустой логин
                invalidRequest.setPassword("password");

                // When & Then
                mockMvc.perform(post("/auth/login")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content(objectMapper.writeValueAsString(invalidRequest)))
                                .andExpect(status().isBadRequest());

                verify(authService, never()).authenticate(any(JwtRequest.class));
        }

        @Test
        void login_WithEmptyPassword_ShouldReturnBadRequest() throws Exception {
                // Given
                JwtRequest invalidRequest = new JwtRequest();
                invalidRequest.setLogin("testuser");
                invalidRequest.setPassword(""); // Пустой пароль

                // When & Then
                mockMvc.perform(post("/auth/login")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content(objectMapper.writeValueAsString(invalidRequest)))
                                .andExpect(status().isBadRequest());

                verify(authService, never()).authenticate(any(JwtRequest.class));
        }

        @Test
        void login_WithNullFields_ShouldReturnBadRequest() throws Exception {
                // Given
                JwtRequest invalidRequest = new JwtRequest();
                // login и password остаются null

                // When & Then
                mockMvc.perform(post("/auth/login")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content(objectMapper.writeValueAsString(invalidRequest)))
                                .andExpect(status().isBadRequest());

                verify(authService, never()).authenticate(any(JwtRequest.class));
        }

        @Test
        void refreshAccessToken_WithValidToken_ShouldReturnNewAccessToken() throws Exception {
                // Given
                JwtResponse refreshResponse = new JwtResponse("Bearer", "new-access-token", "refresh-token");
                when(authService.refreshAccessToken(eq("refresh-token"))).thenReturn(refreshResponse);

                // When & Then
                mockMvc.perform(post("/auth/refresh")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content(objectMapper.writeValueAsString(refreshJwtRequest)))
                                .andExpect(status().isOk())
                                .andExpect(content().contentType(MediaType.APPLICATION_JSON))
                                .andExpect(jsonPath("$.type").value("Bearer"))
                                .andExpect(jsonPath("$.accessToken").value("new-access-token"))
                                .andExpect(jsonPath("$.refreshToken").value("refresh-token"));

                verify(authService).refreshAccessToken("refresh-token");
        }

        @Test
        void refreshAccessToken_WithInvalidToken_ShouldReturnUnauthorized() throws Exception {
                // Given
                when(authService.refreshAccessToken(eq("refresh-token")))
                                .thenThrow(new AuthenticationException("Invalid refresh token") {
                                });

                // When & Then
                mockMvc.perform(post("/auth/refresh")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content(objectMapper.writeValueAsString(refreshJwtRequest)))
                                .andExpect(status().isUnauthorized());

                verify(authService).refreshAccessToken("refresh-token");
        }

        @Test
        void refreshRefreshToken_WithValidToken_ShouldReturnNewTokens() throws Exception {
                // Given
                JwtResponse refreshResponse = new JwtResponse("Bearer", "new-access-token", "new-refresh-token");
                when(authService.refreshRefreshToken(eq("refresh-token"))).thenReturn(refreshResponse);

                // When & Then
                mockMvc.perform(post("/auth/refresh-token")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content(objectMapper.writeValueAsString(refreshJwtRequest)))
                                .andExpect(status().isOk())
                                .andExpect(content().contentType(MediaType.APPLICATION_JSON))
                                .andExpect(jsonPath("$.type").value("Bearer"))
                                .andExpect(jsonPath("$.accessToken").value("new-access-token"))
                                .andExpect(jsonPath("$.refreshToken").value("new-refresh-token"));

                verify(authService).refreshRefreshToken("refresh-token");
        }

        @Test
        void refreshRefreshToken_WithInvalidToken_ShouldReturnUnauthorized() throws Exception {
                // Given
                when(authService.refreshRefreshToken(eq("refresh-token")))
                                .thenThrow(new AuthenticationException("Invalid refresh token") {
                                });

                // When & Then
                mockMvc.perform(post("/auth/refresh-token")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content(objectMapper.writeValueAsString(refreshJwtRequest)))
                                .andExpect(status().isUnauthorized());

                verify(authService).refreshRefreshToken("refresh-token");
        }

        @Test
        void refreshAccessToken_WithNullToken_ShouldReturnBadRequest() throws Exception {
                // Given
                RefreshJwtRequest nullTokenRequest = new RefreshJwtRequest(null);

                // When & Then
                mockMvc.perform(post("/auth/refresh")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content(objectMapper.writeValueAsString(nullTokenRequest)))
                                .andExpect(status().isBadRequest());

                verify(authService, never()).refreshAccessToken(any());
        }

        @Test
        void refreshRefreshToken_WithEmptyToken_ShouldReturnBadRequest() throws Exception {
                // Given
                RefreshJwtRequest emptyTokenRequest = new RefreshJwtRequest("");

                // When & Then
                mockMvc.perform(post("/auth/refresh-token")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content(objectMapper.writeValueAsString(emptyTokenRequest)))
                                .andExpect(status().isBadRequest());

                verify(authService, never()).refreshRefreshToken(any());
        }

        @Test
        void login_WithMalformedJson_ShouldReturnBadRequest() throws Exception {
                // When & Then
                mockMvc.perform(post("/auth/login")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content("{invalid json}"))
                                .andExpect(status().isBadRequest());

                verify(authService, never()).authenticate(any(JwtRequest.class));
        }

        @Test
        void refreshEndpoints_WithMalformedJson_ShouldReturnBadRequest() throws Exception {
                // When & Then
                mockMvc.perform(post("/auth/refresh")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content("{invalid json}"))
                                .andExpect(status().isBadRequest());

                mockMvc.perform(post("/auth/refresh-token")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content("{invalid json}"))
                                .andExpect(status().isBadRequest());

                verify(authService, never()).refreshAccessToken(any());
                verify(authService, never()).refreshRefreshToken(any());
        }

        @Test
        void allEndpoints_WithoutContentType_ShouldReturnUnsupportedMediaType() throws Exception {
                // When & Then
                mockMvc.perform(post("/auth/login")
                                .content(objectMapper.writeValueAsString(validJwtRequest)))
                                .andExpect(status().isUnsupportedMediaType());

                mockMvc.perform(post("/auth/refresh")
                                .content(objectMapper.writeValueAsString(refreshJwtRequest)))
                                .andExpect(status().isUnsupportedMediaType());

                mockMvc.perform(post("/auth/refresh-token")
                                .content(objectMapper.writeValueAsString(refreshJwtRequest)))
                                .andExpect(status().isUnsupportedMediaType());

                verify(authService, never()).authenticate(any(JwtRequest.class));
                verify(authService, never()).refreshAccessToken(any());
                verify(authService, never()).refreshRefreshToken(any());
        }
}
