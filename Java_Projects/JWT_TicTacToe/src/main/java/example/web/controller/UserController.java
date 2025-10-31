package example.web.controller;

// import example.datasource.repository.UserRepository;
import example.domain.model.User;
import example.domain.service.UserService;
import example.utils.AuthUtils;
import example.web.model.UserDTO;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.UUID;

@RestController
@RequestMapping("/user")
public class UserController {
    private final UserService userService;
    private final AuthUtils authUtils;

    @Autowired
    public UserController(UserService userService, AuthUtils authUtils) {
        this.userService = userService;
        this.authUtils = authUtils;
    }

    // @Autowired
    // private UserRepository userRepository;

    // @GetMapping("/{id}")
    // public ResponseEntity<User> getUser(@PathVariable UUID id) {
    // User user = userRepository.findById(id)
    // .orElseThrow(() -> new RuntimeException("Пользователь с ID " + id + " не
    // найден"));
    // return ResponseEntity.ok(user);
    // }
    @GetMapping("/me")
    public ResponseEntity<UserDTO> getCurrentUser() {
        try {
            // Получение UUID текущего пользователя из SecurityContext (JWT токена)
            UUID userId = authUtils.getCurrentPlayerId();

            // Поиск пользователя в базе данных через UserService
            User user = userService.getUserById(userId);

            if (user == null) {
                return ResponseEntity.ok().build();
            }

            UserDTO dto = new UserDTO(user.getId(), user.getLogin(), user.getRole().name());
            return ResponseEntity.ok(dto);
        } catch (Exception ex) {
            return ResponseEntity.internalServerError().build();
        }
    }
}