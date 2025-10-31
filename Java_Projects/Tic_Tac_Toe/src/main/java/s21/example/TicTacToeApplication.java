package s21.example;

import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.Bean;
import s21.example.domain.service.AuthService;
@SpringBootApplication(scanBasePackages = "s21.example")

public class TicTacToeApplication {

    public static void main(String[] args) {

        SpringApplication.run(TicTacToeApplication.class, args);

    }
    @Bean
    public CommandLineRunner initializeDefaultUser(AuthService authService) {
        return args -> authService.initializeDefaultUser();
    }
}
