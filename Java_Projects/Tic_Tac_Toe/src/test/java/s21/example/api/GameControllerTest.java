package s21.example.api;

import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.AutoConfigureMockMvc;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.security.test.context.support.WithMockUser;
import org.springframework.test.web.servlet.MockMvc;
import s21.example.datasource.repository.GameRepository;

import java.util.UUID;

import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.*;

@SpringBootTest
@AutoConfigureMockMvc
class GameControllerTest {

    @Autowired
    private MockMvc mockMvc;

    @Autowired
    private GameRepository gameRepository;

    @Test
    @WithMockUser
    void createGame_ValidRequest_ReturnsGame() throws Exception {
        UUID playerId = UUID.randomUUID();

        mockMvc.perform(post("/game/create?vsComputer=true")
                        .header("X-Player-Id", playerId))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.player1Id").value(playerId.toString()))
                .andExpect(jsonPath("$.vsComputer").value(true));
    }
}