package s21.example.api;

import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.AutoConfigureMockMvc;
import org.springframework.boot.test.autoconfigure.web.servlet.WebMvcTest;
import org.springframework.test.web.servlet.MockMvc;
import s21.example.datasource.repository.UserRepository;
import s21.example.domain.model.User;
import s21.example.web.controller.UserController;
import org.springframework.boot.test.context.TestConfiguration;
import org.springframework.context.annotation.Bean;
import org.mockito.Mockito;

import java.util.Optional;
import java.util.UUID;

import static org.mockito.BDDMockito.given;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.*;

@WebMvcTest(controllers = UserController.class)
@AutoConfigureMockMvc(addFilters = false)
class UserControllerWebMvcTest {

    @Autowired
    private MockMvc mockMvc;

    @Autowired
    private UserRepository userRepository;

    @TestConfiguration
    static class Config {
        @Bean
        UserRepository userRepository() { return Mockito.mock(UserRepository.class); }
    }

    @Test
    void getUser_ReturnsUser() throws Exception {
        UUID id = UUID.randomUUID();
        User u = new User();
        u.setId(id);
        u.setLogin("user");
        u.setPassword("hash");
        given(userRepository.findById(id)).willReturn(Optional.of(u));

        mockMvc.perform(get("/user/{id}", id))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.id").value(id.toString()))
                .andExpect(jsonPath("$.login").value("user"));
    }
}


