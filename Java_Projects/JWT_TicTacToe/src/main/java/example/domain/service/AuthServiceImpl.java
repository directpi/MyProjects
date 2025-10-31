package example.domain.service;

import example.datasource.repository.UserRepository;
import example.domain.model.Role;
import example.domain.model.User;
import example.web.model.JwtRequest;
import example.web.model.JwtResponse;
import example.web.model.SignUpRequest;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.security.core.AuthenticationException;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.stereotype.Service;

import java.util.Base64;
import java.util.Map;
import java.util.UUID;

@Service
public class AuthServiceImpl implements AuthService {
    private final UserRepository userRepository;
    private final PasswordEncoder passwordEncoder;
    private final JwtProvider jwtProvider;

    @Autowired
    public AuthServiceImpl(UserRepository userRepository, PasswordEncoder passwordEncoder, JwtProvider jwtProvider) {
        this.userRepository = userRepository;
        this.passwordEncoder = passwordEncoder;
        this.jwtProvider = jwtProvider;
    }

    private boolean containsNonAscii(String str) {
        if (str == null) return false;
        return str.chars().anyMatch(c -> c > 127); // true если есть не-ASCII символы
    }

    @Value("${default.user.login}")
    private String defaultUserLogin;

    @Value("${default.user.password}")
    private String defaultUserPassword;

    @Override
    public UUID register(SignUpRequest request) throws IllegalStateException {
        // Проверяем существует ли пользователь
        if (userRepository.findByLogin(request.getLogin()).isPresent()) {
            throw new IllegalStateException("Пользователь " + request.getLogin() + " уже существует");
        }

        User user = new User();
        user.setLogin(request.getLogin());
        user.setPassword(passwordEncoder.encode(request.getPassword()));// Хешируем пароль
        user.setRole(Role.USER);

        // Сохраняем и возвращаем ID
        User savedUser = userRepository.save(user);
        return savedUser.getId();
    }

    @Override
    public JwtResponse authenticate(JwtRequest request) throws AuthenticationException {
        // Валидация логина/пароля
        if (containsNonAscii(request.getLogin())) {
            throw new AuthenticationException("Логин должен содержать только ASCII символы") {};
        }

        // Поиск пользователя в БД
        User user = userRepository.findByLogin(request.getLogin())
                .orElseThrow(() -> new AuthenticationException("Пользователь не найден") {});

        // Проверка пароля
        if (!passwordEncoder.matches(request.getPassword(), user.getPassword())) {
            throw new AuthenticationException("Неверный пароль") {};
        }

        // Генерация JWT токенов
        String accessToken = jwtProvider.generateAccessToken(user);
        String refreshToken = jwtProvider.generateRefreshToken(user);

        return new JwtResponse("Bearer", accessToken, refreshToken);
    }

    // Basic Auth для обратной совместимости
    @Override
    public UUID authenticate(String authHeader) throws AuthenticationException {
        // Basic Auth логика для старых endpoint'ов
        if (authHeader == null || !authHeader.startsWith("Basic ")) {
            throw new AuthenticationException("Invalid authorization header") {};
        }

        String base64Credentials = authHeader.substring("Basic ".length()).trim();

        if (base64Credentials.isEmpty()) {
            throw new AuthenticationException("Empty credentials") {};
        }

        String credentials = new String(Base64.getDecoder().decode(base64Credentials));
        String[] loginPassword = credentials.split(":", 2);

        if (loginPassword.length != 2) {
            throw new AuthenticationException("Invalid credentials format") {};
        }

        String username = loginPassword[0].trim();
        String password = loginPassword[1].trim();

        // Валидация
        if (containsNonAscii(username)) {
            throw new AuthenticationException("Логин должен содержать только ASCII символы") {};
        }

        if (containsNonAscii(password)) {
            throw new AuthenticationException("Пароль должен содержать только ASCII символы") {};
        }

        if (username.isEmpty() || username.length() < 3 || username.length() > 20) {
            throw new AuthenticationException("Неверная длина логина") {};
        }

        if (password.isEmpty() || password.length() < 9) {
            throw new AuthenticationException("Неверная длина пароля") {};
        }

        // Поиск пользователя и проверка пароля
        User user = userRepository.findByLogin(username)
                .orElseThrow(() -> new AuthenticationException("Пользователь не найден") {});

        if (!passwordEncoder.matches(password, user.getPassword())) {
            throw new AuthenticationException("Неверный пароль") {};
        }
        return user.getId();
    }

    @Override
    public JwtResponse refreshAccessToken(String refreshToken) {
        if (!jwtProvider.validateRefreshToken(refreshToken)) {
            throw new AuthenticationException("Invalid refresh token") {};
        }

        Map<String, Object> claims = jwtProvider.getClaims(refreshToken);
        String userId = (String) claims.get("sub");
        User user = userRepository.findById(UUID.fromString(userId))
                .orElseThrow(() -> new AuthenticationException("User not found") {});

        String newAccessToken = jwtProvider.generateAccessToken(user);
        return new JwtResponse("Bearer", newAccessToken, refreshToken);
    }

    @Override
    public JwtResponse refreshRefreshToken(String refreshToken) {
        if (!jwtProvider.validateRefreshToken(refreshToken)) {
            throw new AuthenticationException("Invalid refresh token") {};
        }

        Map<String, Object> claims = jwtProvider.getClaims(refreshToken);
        String userId = (String) claims.get("sub");
        User user = userRepository.findById(UUID.fromString(userId))
                .orElseThrow(() -> new AuthenticationException("User not found") {});

        String newRefreshToken = jwtProvider.generateRefreshToken(user);
        String newAccessToken = jwtProvider.generateAccessToken(user);

        return new JwtResponse("Bearer", newAccessToken, newRefreshToken);
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