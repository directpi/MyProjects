package example;

import example.domain.service.AuthService;
import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.Bean;

@SpringBootApplication(scanBasePackages = "example")

public class JwtTicTacToeApplication {

    public static void main(String[] args) {

        SpringApplication.run(JwtTicTacToeApplication.class, args);

    }
    @Bean
    public CommandLineRunner initializeDefaultUser(AuthService authService) {
        return args -> authService.initializeDefaultUser();
    }
}
