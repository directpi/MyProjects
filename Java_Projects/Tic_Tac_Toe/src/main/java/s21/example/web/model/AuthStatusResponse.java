package s21.example.web.model;

import java.util.UUID;

public record AuthStatusResponse(
        boolean authenticated,
        UUID userId,
        String username,
        String message
) {}