package s21.example.api;

import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.AutoConfigureMockMvc;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.http.MediaType;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.transaction.annotation.Transactional;
import s21.example.datasource.repository.UserRepository;

import java.util.Base64;

import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.*;

@SpringBootTest(webEnvironment = SpringBootTest.WebEnvironment.RANDOM_PORT)
@AutoConfigureMockMvc
class AuthControllerTest {

    @Autowired
    private MockMvc mockMvc;

    @Autowired
    private UserRepository userRepository;

    @Test
    @Transactional
    void registerUser_Success() throws Exception {
        mockMvc.perform(post("/auth/register")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("""
                    {
                        "login": "testuser",
                        "password": "password123"
                    }
                """))
                .andExpect(status().isOk())
                .andExpect(content().string("Пользователь зарегистрирован"));
    }

    @Test
    void loginUser_InvalidCredentials_Fails() throws Exception {
        mockMvc.perform(post("/auth/login")
                        .header("Authorization", "Basic " + Base64.getEncoder().encodeToString("wrong:creds".getBytes())))
                .andExpect(status().isUnauthorized());
    }
}