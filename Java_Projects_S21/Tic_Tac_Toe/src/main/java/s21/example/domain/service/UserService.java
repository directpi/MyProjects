package s21.example.domain.service;

import s21.example.domain.model.User;
import java.util.UUID;

public interface UserService {
    User getUserById(UUID userId);
}