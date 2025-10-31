package example.domain.service;

import example.domain.model.User;

import java.util.UUID;

public interface UserService {
    User getUserById(UUID userId);
}