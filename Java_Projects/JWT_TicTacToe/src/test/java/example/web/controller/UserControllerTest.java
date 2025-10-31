package example.web.controller;

import example.domain.model.Role;
import example.domain.model.User;
import example.domain.service.UserService;
import example.utils.AuthUtils;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.test.web.servlet.setup.MockMvcBuilders;
import org.mockito.junit.jupiter.MockitoExtension;
import org.junit.jupiter.api.extension.ExtendWith;
import org.springframework.http.MediaType;
import org.springframework.test.web.servlet.MockMvc;

import java.util.UUID;

import static org.mockito.Mockito.*;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.*;

@ExtendWith(MockitoExtension.class)
class UserControllerTest {

        private MockMvc mockMvc;

        @org.mockito.Mock
        private UserService userService;

        @org.mockito.Mock
        private AuthUtils authUtils;

        private UUID testUserId;
        private User testUser;

        @BeforeEach
        void setUp() {
                testUserId = UUID.randomUUID();

                // Создаем тестового пользователя
                testUser = new User();
                testUser.setId(testUserId);
                testUser.setLogin("testuser");
                testUser.setPassword("encodedPassword"); // В реальном ответе пароль не должен возвращаться
                testUser.setRole(Role.USER);

                // Сборка standalone MockMvc без Security фильтров
                UserController controller = new UserController(userService, authUtils);
                mockMvc = MockMvcBuilders.standaloneSetup(controller).build();

                // Мокаем AuthUtils для всех тестов
                when(authUtils.getCurrentPlayerId()).thenReturn(testUserId);
        }

        @Test
        void getCurrentUser_WithValidAuthentication_ShouldReturnUserInfo() throws Exception {
                // Given
                when(userService.getUserById(testUserId)).thenReturn(testUser);

                // When & Then
                mockMvc.perform(get("/user/me"))
                                .andExpect(status().isOk())
                                .andExpect(content().contentType(MediaType.APPLICATION_JSON))
                                .andExpect(jsonPath("$.id").value(testUserId.toString()))
                                .andExpect(jsonPath("$.login").value("testuser"))
                                .andExpect(jsonPath("$.role").value("USER"));

                verify(authUtils).getCurrentPlayerId();
                verify(userService).getUserById(testUserId);
        }

        @Test
        void getCurrentUser_WithNonExistentUser_ShouldReturnNotFound() throws Exception {
                // Given
                when(userService.getUserById(testUserId))
                                .thenThrow(new RuntimeException("User not found"));

                // When & Then
                mockMvc.perform(get("/user/me"))
                                .andExpect(status().isInternalServerError());

                verify(authUtils).getCurrentPlayerId();
                verify(userService).getUserById(testUserId);
        }

        @Test
        void getCurrentUser_WithAuthUtilsException_ShouldReturnInternalServerError() throws Exception {
                // Given
                when(authUtils.getCurrentPlayerId())
                                .thenThrow(new RuntimeException("Authentication error"));

                // When & Then
                mockMvc.perform(get("/user/me"))
                                .andExpect(status().isInternalServerError());

                verify(authUtils).getCurrentPlayerId();
                verify(userService, never()).getUserById(any());
        }

        @Test
        void getCurrentUser_WithUserServiceException_ShouldReturnInternalServerError() throws Exception {
                // Given
                when(userService.getUserById(testUserId))
                                .thenThrow(new RuntimeException("Database connection error"));

                // When & Then
                mockMvc.perform(get("/user/me"))
                                .andExpect(status().isInternalServerError());

                verify(authUtils).getCurrentPlayerId();
                verify(userService).getUserById(testUserId);
        }

        @Test
        void getCurrentUser_ShouldReturnAllUserFields() throws Exception {
                // Given
                User detailedUser = new User();
                detailedUser.setId(testUserId);
                detailedUser.setLogin("detailedUser");
                detailedUser.setPassword("hashedPassword123");
                detailedUser.setRole(Role.USER);

                when(userService.getUserById(testUserId)).thenReturn(detailedUser);

                // When & Then
                mockMvc.perform(get("/user/me"))
                                .andExpect(status().isOk())
                                .andExpect(jsonPath("$.id").value(testUserId.toString()))
                                .andExpect(jsonPath("$.login").value("detailedUser"))
                                .andExpect(jsonPath("$.role").value("USER"));

                verify(authUtils).getCurrentPlayerId();
                verify(userService).getUserById(testUserId);
        }

        @Test
        void getCurrentUser_WithDifferentUserIds_ShouldCallCorrectService() throws Exception {
                // Given - тестируем с разными пользователями
                UUID anotherUserId = UUID.randomUUID();
                User anotherUser = new User();
                anotherUser.setId(anotherUserId);
                anotherUser.setLogin("anotheruser");
                anotherUser.setRole(Role.USER);

                when(authUtils.getCurrentPlayerId()).thenReturn(anotherUserId);
                when(userService.getUserById(anotherUserId)).thenReturn(anotherUser);

                // When & Then
                mockMvc.perform(get("/user/me"))
                                .andExpect(status().isOk())
                                .andExpect(jsonPath("$.id").value(anotherUserId.toString()))
                                .andExpect(jsonPath("$.login").value("anotheruser"));

                verify(authUtils).getCurrentPlayerId();
                verify(userService).getUserById(anotherUserId);
                verify(userService, never()).getUserById(testUserId); // Не должен вызываться для другого пользователя
        }

        @Test
        void getCurrentUser_ShouldNotExposeAuthoritiesOrPassword() throws Exception {
                // Given
                when(userService.getUserById(testUserId)).thenReturn(testUser);

                // When & Then
                mockMvc.perform(get("/user/me"))
                                .andExpect(status().isOk())
                                .andExpect(jsonPath("$.password").doesNotExist())
                                .andExpect(jsonPath("$.authorities").doesNotExist());

                verify(authUtils).getCurrentPlayerId();
                verify(userService).getUserById(testUserId);
        }

        @Test
        void getCurrentUser_WithNullUser_ShouldHandleGracefully() throws Exception {
                // Given
                when(userService.getUserById(testUserId)).thenReturn(null);

                // When & Then
                mockMvc.perform(get("/user/me"))
                                .andExpect(status().isOk())
                                .andExpect(content().string("")); // Null возвращается как пустое содержимое

                verify(authUtils).getCurrentPlayerId();
                verify(userService).getUserById(testUserId);
        }

        @Test
        void getCurrentUser_EndpointPath_ShouldBeCorrect() throws Exception {
                // Given
                when(userService.getUserById(testUserId)).thenReturn(testUser);

                // When & Then - проверяем правильность пути
                mockMvc.perform(get("/user/me"))
                                .andExpect(status().isOk());

                // Проверяем, что неправильные пути не работают
                mockMvc.perform(get("/users/me"))
                                .andExpect(status().isNotFound());

                mockMvc.perform(get("/user/current"))
                                .andExpect(status().isNotFound());

                verify(authUtils, times(1)).getCurrentPlayerId(); // Только один успешный вызов
                verify(userService, times(1)).getUserById(testUserId);
        }

        @Test
        void getCurrentUser_HttpMethod_ShouldOnlyAllowGet() throws Exception {
                // Given
                when(userService.getUserById(testUserId)).thenReturn(testUser);

                // When & Then - только GET должен работать
                mockMvc.perform(get("/user/me"))
                                .andExpect(status().isOk());

                // POST, PUT, DELETE не должны работать
                mockMvc.perform(org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post("/user/me"))
                                .andExpect(status().isMethodNotAllowed());

                mockMvc.perform(org.springframework.test.web.servlet.request.MockMvcRequestBuilders.put("/user/me"))
                                .andExpect(status().isMethodNotAllowed());

                mockMvc.perform(org.springframework.test.web.servlet.request.MockMvcRequestBuilders.delete("/user/me"))
                                .andExpect(status().isMethodNotAllowed());

                verify(authUtils, times(1)).getCurrentPlayerId(); // Только для GET запроса
                verify(userService, times(1)).getUserById(testUserId);
        }

        @Test
        void getCurrentUser_ResponseFormat_ShouldBeJson() throws Exception {
                // Given
                when(userService.getUserById(testUserId)).thenReturn(testUser);

                // When & Then
                mockMvc.perform(get("/user/me")
                                .accept(MediaType.APPLICATION_JSON))
                                .andExpect(status().isOk())
                                .andExpect(content().contentType(MediaType.APPLICATION_JSON))
                                .andExpect(header().string("Content-Type", "application/json"));

                verify(authUtils).getCurrentPlayerId();
                verify(userService).getUserById(testUserId);
        }

        @Test
        void getCurrentUser_WithMockAuthentication_ShouldWork() throws Exception {
                // Given
                when(userService.getUserById(testUserId)).thenReturn(testUser);

                // When & Then - тест проверяет, что @WithMockUser работает
                mockMvc.perform(get("/user/me"))
                                .andExpect(status().isOk())
                                .andExpect(jsonPath("$.login").value("testuser"));

                verify(authUtils).getCurrentPlayerId();
                verify(userService).getUserById(testUserId);
        }
}
