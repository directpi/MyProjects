/**
 * @file: BrickGameApplication.java
 * @description: Главный класс Spring Boot приложения BrickGame v3.0
 * @dependencies: org.springframework.boot.SpringApplication,
 *                org.springframework.boot.autoconfigure.SpringBootApplication
 * @created: 2025-10-05
 */
package src.brick_game;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class BrickGameApplication {
    public static void main(String[] args) {
        SpringApplication.run(BrickGameApplication.class, args);
    }
}
