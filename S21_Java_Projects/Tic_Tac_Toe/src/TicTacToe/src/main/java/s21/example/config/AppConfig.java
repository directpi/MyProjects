package s21.example.config;

import org.springframework.context.annotation.ComponentScan;
import s21.example.datasource.repository.ArchivedGameRepository;
import s21.example.datasource.repository.GameRepository;
import s21.example.domain.service.GameService;
import org.springframework.context.annotation.Scope;

import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Bean;
import s21.example.domain.service.GameServiceImpl;

@Configuration
@ComponentScan(basePackages = "s21.example")
public class AppConfig {
    @Bean
    @Scope("singleton")
    public GameRepository gameRepository() {
        return new GameRepository();
    }

    @Bean
    @Scope("singleton")
    public ArchivedGameRepository archivedGameRepository() {
        return new ArchivedGameRepository();
    }

    @Bean
    public GameService gameService(GameRepository gameRepository, ArchivedGameRepository archivedGameRepository) {
        return new GameServiceImpl(gameRepository, archivedGameRepository);
    }
}