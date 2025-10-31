package s21.example.config;

import org.springframework.context.annotation.ComponentScan;

import s21.example.domain.service.GameService;
import s21.example.datasource.repository.GameRepository;
import s21.example.datasource.repository.UserRepository;

import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Bean;
import s21.example.domain.service.GameServiceImpl;

@Configuration
@ComponentScan(basePackages = "s21.example")
public class AppConfig {
}