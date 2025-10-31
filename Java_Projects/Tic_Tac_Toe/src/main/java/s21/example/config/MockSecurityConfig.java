//package s21.example.config;
//
//import org.springframework.boot.test.context.TestConfiguration;
//import org.springframework.context.annotation.Bean;
//import org.springframework.context.annotation.Primary;
//import s21.example.domain.service.AuthService;
//
//import java.util.UUID;
//
//import static org.mockito.ArgumentMatchers.anyString;
//import static org.mockito.Mockito.mock;
//import static org.mockito.Mockito.when;
//
//@TestConfiguration
//public class MockSecurityConfig {
//
//    @Bean
//    @Primary
//    public AuthService authService() {
//        AuthService mock = mock(AuthService.class);
//        when(mock.authenticate(anyString()))
//                .thenReturn(UUID.randomUUID());
//        return mock;
//    }
//}