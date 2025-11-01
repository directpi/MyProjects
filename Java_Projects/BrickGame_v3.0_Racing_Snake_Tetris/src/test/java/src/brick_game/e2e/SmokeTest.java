/**
 * @file: SmokeTest.java
 * @description: E2E smoke-тест REST API: /games, /games/{id}, /actions, /state
 * @dependencies: org.springframework.boot.test.context.SpringBootTest,
 *                org.springframework.test.web.reactive.server.WebTestClient
 * @created: 2025-10-06
 */
package src.brick_game.e2e;

import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.boot.test.web.client.TestRestTemplate;
import org.springframework.boot.test.web.server.LocalServerPort;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpMethod;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;

@SpringBootTest(webEnvironment = SpringBootTest.WebEnvironment.RANDOM_PORT)
class SmokeTest {

  @LocalServerPort
  private int port;
  @Autowired
  private TestRestTemplate restTemplate;

  @Test
  void fullFlow() {
    String base = "http://localhost:" + port + "/api";

    ResponseEntity<String> gamesResp = restTemplate.getForEntity(base + "/games", String.class);
    assert gamesResp.getStatusCode() == HttpStatus.OK;

    ResponseEntity<Void> startResp =
        restTemplate.postForEntity(base + "/games/1", null, Void.class);
    assert startResp.getStatusCode() == HttpStatus.OK;

    HttpHeaders headers = new HttpHeaders();
    headers.setContentType(MediaType.APPLICATION_JSON);
    HttpEntity<String> actionBody = new HttpEntity<>("{\"action_id\":1,\"hold\":false}", headers);
    ResponseEntity<Void> actionResp =
        restTemplate.exchange(base + "/actions", HttpMethod.POST, actionBody, Void.class);
    assert actionResp.getStatusCode() == HttpStatus.OK;

    ResponseEntity<String> stateResp = restTemplate.getForEntity(base + "/state", String.class);
    assert stateResp.getStatusCode() == HttpStatus.OK;
  }
}
