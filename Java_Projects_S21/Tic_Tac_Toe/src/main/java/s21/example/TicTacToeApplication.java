package s21.example;

import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.Bean;
import s21.example.domain.service.AuthService;
//import org.springframework.boot.autoconfigure.EnableAutoConfiguration;
//import org.springframework.boot.autoconfigure.jdbc.DataSourceAutoConfiguration;
//import org.springframework.boot.autoconfigure.websocket.servlet.WebSocketServletAutoConfiguration;
//import org.springframework.boot.autoconfigure.websocket.reactive.WebSocketReactiveAutoConfiguration;

@SpringBootApplication(scanBasePackages = "s21.example")
//@EnableAutoConfiguration(exclude = {
  //      DataSourceAutoConfiguration.class, // Пример отключения автоконфигурации для DataSource
    //    WebSocketServletAutoConfiguration.class, // Отключение WebSocket
      //  WebSocketReactiveAutoConfiguration.class // Отключение реактивного WebSocket
//})
public class TicTacToeApplication {

    public static void main(String[] args) {

        SpringApplication.run(TicTacToeApplication.class, args);

    }
    @Bean
    public CommandLineRunner initializeDefaultUser(AuthService authService) {
        return args -> authService.initializeDefaultUser();
    }
}
