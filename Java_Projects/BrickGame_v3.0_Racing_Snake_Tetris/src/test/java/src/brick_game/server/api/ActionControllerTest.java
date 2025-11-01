/**
 * @file: ActionControllerTest.java
 * @description: Базовые WebMvc тесты для ActionController
 * @dependencies: org.springframework.boot.test.autoconfigure.web.servlet.WebMvcTest
 * @created: 2025-10-06
 */
package src.brick_game.server.api;

import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.AutoConfigureMockMvc;
import org.springframework.boot.test.autoconfigure.web.servlet.WebMvcTest;
import org.springframework.http.MediaType;
//import org.springframework.test.context.bean.override.mockito.MockBean;
import org.springframework.test.web.servlet.MockMvc;
import src.brick_game.server.service.GameService;

//@WebMvcTest(ActionController.class)
@AutoConfigureMockMvc(addFilters = false)
class ActionControllerTest {

    @Autowired
    private MockMvc mockMvc;

//    @MockBean
    private GameService gameService;

    @Test
    void performAction_returnsOk() throws Exception {
        String body = "{\"action_id\":1,\"hold\":false}";
        mockMvc.perform(post("/api/actions").contentType(MediaType.APPLICATION_JSON).content(body))
                .andExpect(status().isOk());
    }
}