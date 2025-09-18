//package s21.example.config;
//
//import org.mockito.Mockito;
//import org.springframework.boot.test.context.TestConfiguration;
//import org.springframework.context.annotation.Bean;
//import s21.example.domain.service.AuthService;
//import s21.example.web.filter.AuthFilter;
//
//import static org.mockito.Mockito.mock;
//
//@TestConfiguration
//public class TestSecurityConfig {
//
//    @Bean
//    public AuthService authService() {
//        return Mockito.mock(AuthService.class);
//    }
//
//    @Bean
//    public AuthFilter authFilter(AuthService authService) {
//        return new AuthFilter(authService);
//    }
//}