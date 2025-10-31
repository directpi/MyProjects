package s21.example.api;

import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.AutoConfigureMockMvc;
import org.springframework.boot.test.autoconfigure.web.servlet.WebMvcTest;
import org.springframework.http.MediaType;
import org.springframework.test.web.servlet.MockMvc;
import s21.example.domain.service.AuthService;
import s21.example.web.controller.AuthController;
import s21.example.web.model.SignUpRequest;

import java.util.Base64;
import java.util.UUID;

import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.BDDMockito.given;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.*;
import org.springframework.boot.test.context.TestConfiguration;
import org.springframework.context.annotation.Bean;
import org.mockito.Mockito;

@WebMvcTest(controllers = AuthController.class)
@AutoConfigureMockMvc(addFilters = false)
class AuthControllerWebMvcTest {

    @Autowired
    private MockMvc mockMvc;

    @Autowired
    private AuthService authService;

    @TestConfiguration
    static class Config {
        @Bean
        AuthService authService() { return Mockito.mock(AuthService.class); }
    }

    @Test
    void register_NewUser_ReturnsOk() throws Exception {
        given(authService.register(any(SignUpRequest.class))).willReturn(true);

        String body = "{\n  \"login\": \"u\",\n  \"password\": \"p\"\n}";

        mockMvc.perform(post("/auth/register")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(body))
                .andExpect(status().isOk())
                .andExpect(content().string("Пользователь зарегистрирован"));
    }

    @Test
    void login_ValidBasic_ReturnsUserInfo() throws Exception {
        UUID uid = UUID.randomUUID();
        String basic = "Basic " + Base64.getEncoder().encodeToString("u:p".getBytes());
        given(authService.authenticate(eq(basic))).willReturn(uid);

        mockMvc.perform(post("/auth/login")
                        .header("Authorization", basic))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.authenticated").value(true))
                .andExpect(jsonPath("$.userId").value(uid.toString()))
                .andExpect(jsonPath("$.username").value("u"));
    }
}


