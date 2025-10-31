package s21.example.domain.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.security.core.AuthenticationException;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.stereotype.Service;
import s21.example.datasource.repository.UserRepository;
import s21.example.domain.model.User;
import s21.example.web.model.SignUpRequest;

import java.util.Base64;
import java.util.UUID;

@Service
public class AuthServiceImpl implements AuthService {
    private final UserRepository userRepository;
    private final PasswordEncoder passwordEncoder;

    @Autowired
    public AuthServiceImpl(UserRepository userRepository, PasswordEncoder passwordEncoder) {
        this.userRepository = userRepository;
        this.passwordEncoder = passwordEncoder;
    }

    @Value("${default.user.login}")
    private String defaultUserLogin;

    @Value("${default.user.password}")
    private String defaultUserPassword;

    @Override
    public boolean register(SignUpRequest request) {
        if (userRepository.findByLogin(request.getLogin()).isPresent()) {
            return false; // Пользователь уже существует
        }

        User user = new User();
        user.setLogin(request.getLogin());
        user.setPassword(passwordEncoder.encode(request.getPassword()));// Хешируем пароль
        userRepository.save(user);
        return true;
    }

    @Override
    public UUID authenticate(String authHeader) throws AuthenticationException {
        if (authHeader == null || !authHeader.startsWith("Basic ")) {
            throw new org.springframework.security.core.AuthenticationException("Invalid authorization header") {};
        }

        String base64Credentials = authHeader.substring("Basic ".length()).trim();
        String credentials = new String(Base64.getDecoder().decode(base64Credentials));
        String[] loginPassword = credentials.split(":", 2);

        if (loginPassword.length != 2) {
            throw new org.springframework.security.core.AuthenticationException("Invalid credentials format") {};
        }

        return userRepository.findByLogin(loginPassword[0])
                .filter(user -> passwordEncoder.matches(loginPassword[1], user.getPassword()))
                .map(User::getId)
                .orElseThrow(() -> new org.springframework.security.core.AuthenticationException("Invalid credentials") {});
    }

    @Override
    public boolean hasActiveSecondPlayer() {
        return userRepository.count() >= 2;
    }

    // Метод для инициализации пользователя по умолчанию
    @Override
    public void initializeDefaultUser() {
        System.out.println("Метод initializeDefaultUser() вызван");
        if (userRepository.findByLogin(defaultUserLogin).isEmpty()) {
            User user = new User();
            user.setLogin(defaultUserLogin);
            user.setPassword(passwordEncoder.encode(defaultUserPassword)); // Хешируем пароль
            userRepository.save(user);
            System.out.println("Пользователь " + defaultUserLogin + " добавлен в базу данных");
        } else {
            System.out.println("Пользователь " + defaultUserLogin + " уже существует");
        }
    }
}