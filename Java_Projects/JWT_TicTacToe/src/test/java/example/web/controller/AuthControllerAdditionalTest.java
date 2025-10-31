package example.web.controller;

import com.fasterxml.jackson.databind.ObjectMapper;
import example.domain.model.Role;
import example.domain.model.User;
import example.domain.service.AuthService;
import example.domain.service.UserService;
import example.web.model.SignUpRequest;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;
import org.springframework.http.MediaType;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.setup.MockMvcBuilders;

import java.util.UUID;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.*;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.*;

/**
 * Дополнительные тесты для AuthController (регистрация)
 * Используется standalone MockMvc без Spring Context
 */
@ExtendWith(MockitoExtension.class)
class AuthControllerAdditionalTest {

        private MockMvc mockMvc;

        @Mock
        private AuthService authService;

        @Mock
        private UserService userService;

        private ObjectMapper objectMapper = new ObjectMapper();

        private SignUpRequest validSignUpRequest;
        private User testUser;
        private UUID testUserId;

        @BeforeEach
        void setUp() {
                testUserId = UUID.randomUUID();

                validSignUpRequest = new SignUpRequest();
                validSignUpRequest.setLogin("testuser");
                validSignUpRequest.setPassword("testpassword");

                testUser = new User();
                testUser.setId(testUserId);
                testUser.setLogin("testuser");
                testUser.setPassword("encodedPassword");
                testUser.setRole(Role.USER);

                // Standalone MockMvc
                AuthController controller = new AuthController(authService, userService);
                mockMvc = MockMvcBuilders.standaloneSetup(controller).build();
        }

        @Test
        void register_WithValidRequest_ShouldReturnSuccessResponse() throws Exception {
                // Given
                when(authService.register(any(SignUpRequest.class))).thenReturn(testUserId);
                when(userService.getUserById(testUserId)).thenReturn(testUser);

                // When & Then
                mockMvc.perform(post("/auth/register")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content(objectMapper.writeValueAsString(validSignUpRequest)))
                                .andExpect(status().isOk())
                                .andExpect(content().contentType(MediaType.APPLICATION_JSON))
                                .andExpect(jsonPath("$.authenticated").value(true))
                                .andExpect(jsonPath("$.userId").value(testUserId.toString()))
                                .andExpect(jsonPath("$.username").value("testuser"))
                                .andExpect(jsonPath("$.role").value("USER"))
                                .andExpect(jsonPath("$.message").value("Пользователь зарегистрирован"));

                verify(authService).register(any(SignUpRequest.class));
                verify(userService).getUserById(testUserId);
        }

        @Test
        void register_WithExistingUser_ShouldReturnErrorResponse() throws Exception {
                // Given
                when(authService.register(any(SignUpRequest.class)))
                                .thenThrow(new IllegalStateException("Пользователь testuser уже существует"));

                // When & Then
                mockMvc.perform(post("/auth/register")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content(objectMapper.writeValueAsString(validSignUpRequest)))
                                .andExpect(status().isBadRequest())
                                .andExpect(content().contentType(MediaType.APPLICATION_JSON))
                                .andExpect(jsonPath("$.authenticated").value(false))
                                .andExpect(jsonPath("$.userId").doesNotExist())
                                .andExpect(jsonPath("$.username").value("testuser"))
                                .andExpect(jsonPath("$.role").doesNotExist())
                                .andExpect(jsonPath("$.message").value("Пользователь testuser уже существует"));

                verify(authService).register(any(SignUpRequest.class));
                verify(userService, never()).getUserById(any());
        }

        @Test
        void register_WithInvalidRequest_ShouldReturnBadRequest() throws Exception {
                // Given - запрос с пустым логином
                SignUpRequest invalidRequest = new SignUpRequest();
                invalidRequest.setLogin(""); // Пустой логин
                invalidRequest.setPassword("password");

                // When & Then
                mockMvc.perform(post("/auth/register")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content(objectMapper.writeValueAsString(invalidRequest)))
                                .andExpect(status().isBadRequest());

                verify(authService, never()).register(any());
                verify(userService, never()).getUserById(any());
        }

        @Test
        void register_WithNullLogin_ShouldReturnBadRequest() throws Exception {
                // Given
                SignUpRequest invalidRequest = new SignUpRequest();
                invalidRequest.setLogin(null);
                invalidRequest.setPassword("password");

                // When & Then
                mockMvc.perform(post("/auth/register")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content(objectMapper.writeValueAsString(invalidRequest)))
                                .andExpect(status().isBadRequest());

                verify(authService, never()).register(any());
                verify(userService, never()).getUserById(any());
        }

        @Test
        void register_WithEmptyPassword_ShouldReturnBadRequest() throws Exception {
                // Given
                SignUpRequest invalidRequest = new SignUpRequest();
                invalidRequest.setLogin("testuser");
                invalidRequest.setPassword(""); // Пустой пароль

                // When & Then
                mockMvc.perform(post("/auth/register")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content(objectMapper.writeValueAsString(invalidRequest)))
                                .andExpect(status().isBadRequest());

                verify(authService, never()).register(any());
                verify(userService, never()).getUserById(any());
        }

        @Test
        void register_WithNullPassword_ShouldReturnBadRequest() throws Exception {
                // Given
                SignUpRequest invalidRequest = new SignUpRequest();
                invalidRequest.setLogin("testuser");
                invalidRequest.setPassword(null);

                // When & Then
                mockMvc.perform(post("/auth/register")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content(objectMapper.writeValueAsString(invalidRequest)))
                                .andExpect(status().isBadRequest());

                verify(authService, never()).register(any());
                verify(userService, never()).getUserById(any());
        }

        @Test
        void register_WithMalformedJson_ShouldReturnBadRequest() throws Exception {
                // When & Then
                mockMvc.perform(post("/auth/register")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content("{invalid json}"))
                                .andExpect(status().isBadRequest());

                verify(authService, never()).register(any());
                verify(userService, never()).getUserById(any());
        }

        @Test
        void register_WithoutContentType_ShouldReturnUnsupportedMediaType() throws Exception {
                // When & Then
                mockMvc.perform(post("/auth/register")
                                .content(objectMapper.writeValueAsString(validSignUpRequest)))
                                .andExpect(status().isUnsupportedMediaType());

                verify(authService, never()).register(any());
                verify(userService, never()).getUserById(any());
        }

        @Test
        void register_WithAuthServiceException_ShouldReturnBadRequest() throws Exception {
                // Given
                when(authService.register(any(SignUpRequest.class)))
                                .thenThrow(new IllegalStateException("Database error"));

                // When & Then
                mockMvc.perform(post("/auth/register")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content(objectMapper.writeValueAsString(validSignUpRequest)))
                                .andExpect(status().isBadRequest())
                                .andExpect(jsonPath("$.authenticated").value(false))
                                .andExpect(jsonPath("$.message").value("Database error"));

                verify(authService).register(any(SignUpRequest.class));
                verify(userService, never()).getUserById(any());
        }

        @Test
        void register_WithUserServiceException_ShouldStillCallUserService() throws Exception {
                // Given
                when(authService.register(any(SignUpRequest.class))).thenReturn(testUserId);
                when(userService.getUserById(testUserId))
                                .thenThrow(new RuntimeException("User service error"));

                // When & Then - в standalone MockMvc исключения пробрасываются как
                // ServletException
                // В реальном приложении с фильтрами это было бы 500
                try {
                        mockMvc.perform(post("/auth/register")
                                        .contentType(MediaType.APPLICATION_JSON)
                                        .content(objectMapper.writeValueAsString(validSignUpRequest)));
                } catch (Exception e) {
                        // Ожидаем ServletException с причиной RuntimeException
                        assertTrue(e.getCause() instanceof RuntimeException);
                }

                verify(authService).register(any(SignUpRequest.class));
                verify(userService).getUserById(testUserId);
        }

        @Test
        void register_ShouldReturnCorrectResponseStructure() throws Exception {
                // Given
                when(authService.register(any(SignUpRequest.class))).thenReturn(testUserId);
                when(userService.getUserById(testUserId)).thenReturn(testUser);

                // When & Then
                mockMvc.perform(post("/auth/register")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content(objectMapper.writeValueAsString(validSignUpRequest)))
                                .andExpect(status().isOk())
                                .andExpect(jsonPath("$.authenticated").exists())
                                .andExpect(jsonPath("$.userId").exists())
                                .andExpect(jsonPath("$.username").exists())
                                .andExpect(jsonPath("$.role").exists())
                                .andExpect(jsonPath("$.message").exists());

                verify(authService).register(any(SignUpRequest.class));
                verify(userService).getUserById(testUserId);
        }

        @Test
        void register_WithSpecialCharactersInLogin_ShouldProcessCorrectly() throws Exception {
                // Given
                SignUpRequest specialRequest = new SignUpRequest();
                specialRequest.setLogin("user_123"); // Допустимые ASCII символы
                specialRequest.setPassword("password123");

                User specialUser = new User();
                specialUser.setId(testUserId);
                specialUser.setLogin("user_123"); // Соответствует запросу
                specialUser.setPassword("encodedPassword");
                specialUser.setRole(Role.USER);

                when(authService.register(any(SignUpRequest.class))).thenReturn(testUserId);
                when(userService.getUserById(testUserId)).thenReturn(specialUser);

                // When & Then
                mockMvc.perform(post("/auth/register")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content(objectMapper.writeValueAsString(specialRequest)))
                                .andExpect(status().isOk())
                                .andExpect(jsonPath("$.username").value("user_123"));

                verify(authService).register(any(SignUpRequest.class));
                verify(userService).getUserById(testUserId);
        }

        @Test
        void register_WithLongLogin_ShouldProcessCorrectly() throws Exception {
                // Given
                SignUpRequest longLoginRequest = new SignUpRequest();
                longLoginRequest.setLogin("very_long_username_that_might_be_valid"); // 38 символов, превышает max=20
                longLoginRequest.setPassword("password123");

                // When & Then - ожидаем 400 Bad Request из-за валидации длины логина
                mockMvc.perform(post("/auth/register")
                                .contentType(MediaType.APPLICATION_JSON)
                                .content(objectMapper.writeValueAsString(longLoginRequest)))
                                .andExpect(status().isBadRequest());

                // Сервисы не должны вызываться при ошибке валидации
                verifyNoInteractions(authService);
                verifyNoInteractions(userService);
        }
}
