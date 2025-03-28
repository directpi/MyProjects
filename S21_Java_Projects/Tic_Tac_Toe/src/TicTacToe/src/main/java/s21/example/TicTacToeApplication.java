package s21.example;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication(scanBasePackages = "s21.example")
public class TicTacToeApplication {

    public static void main(String[] args) {

        SpringApplication.run(TicTacToeApplication.class, args);

    }
}
