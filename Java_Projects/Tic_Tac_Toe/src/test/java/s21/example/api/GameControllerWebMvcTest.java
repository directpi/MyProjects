package s21.example.api;

import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.AutoConfigureMockMvc;
import org.springframework.boot.test.autoconfigure.web.servlet.WebMvcTest;
import org.springframework.test.web.servlet.MockMvc;
import s21.example.domain.model.Game;
import s21.example.domain.model.GameState;
import s21.example.domain.service.GameService;
import s21.example.web.controller.GameController;

import java.util.List;
import java.util.UUID;

import static org.mockito.ArgumentMatchers.*;
import static org.mockito.BDDMockito.given;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.*;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.*;
import org.springframework.boot.test.context.TestConfiguration;
import org.springframework.context.annotation.Bean;
import org.mockito.Mockito;

@WebMvcTest(controllers = GameController.class)
@AutoConfigureMockMvc(addFilters = false)
class GameControllerWebMvcTest {

    @Autowired
    private MockMvc mockMvc;

    @Autowired
    private GameService gameService;

    @TestConfiguration
    static class Config {
        @Bean
        GameService gameService() { return Mockito.mock(GameService.class); }
    }

    @Test
    void getGame_ReturnsGame() throws Exception {
        UUID id = UUID.randomUUID();
        UUID player = UUID.randomUUID();
        Game g = new Game();
        g.setId(id);
        g.setState(GameState.WAITING_FOR_PLAYERS);

        given(gameService.getGameById(id)).willReturn(g);

        mockMvc.perform(get("/game/{id}", id)
                        .header("X-Player-Id", player))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.id").value(id.toString()))
                .andExpect(jsonPath("$.state").value("WAITING_FOR_PLAYERS"));
    }

    @Test
    void availableGames_ReturnsList() throws Exception {
        UUID player = UUID.randomUUID();
        Game g = new Game();
        g.setId(UUID.randomUUID());
        g.setState(GameState.WAITING_FOR_PLAYERS);
        given(gameService.getAvailableGames()).willReturn(List.of(g));

        mockMvc.perform(get("/game/available")
                        .header("X-Player-Id", player))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$[0].id").isNotEmpty());
    }

    @Test
    void createGame_ReturnsCreated() throws Exception {
        UUID player = UUID.randomUUID();
        Game g = new Game();
        g.setId(UUID.randomUUID());
        g.setState(GameState.WAITING_FOR_PLAYERS);
        given(gameService.createGame(eq(player), eq(false))).willReturn(g);

        mockMvc.perform(post("/game/create")
                        .param("vsComputer", "false")
                        .header("X-Player-Id", player))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.id").value(g.getId().toString()));
    }
}


