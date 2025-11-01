/**
 * @file: GameControllerTest.java
 * @description: Базовые WebMvc тесты для GameController
 * @dependencies: org.springframework.boot.test.autoconfigure.web.servlet.WebMvcTest
 * @created: 2025-09-30
 */
package src.brick_game.server.api;

import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.AutoConfigureMockMvc;
import org.springframework.boot.test.autoconfigure.web.servlet.WebMvcTest;
import org.springframework.boot.test.mock.mockito.MockBean;
import org.springframework.test.web.servlet.MockMvc;
import src.brick_game.server.service.GameService;

@WebMvcTest(GameController.class)
@AutoConfigureMockMvc(addFilters = false)
class GameControllerTest {

  @Autowired
  private MockMvc mockMvc;

  @MockBean
  private GameService gameService;

  @Test
  void getGames_returnsOk() throws Exception {
    mockMvc.perform(get("/api/games")).andExpect(status().isOk());
  }
}
