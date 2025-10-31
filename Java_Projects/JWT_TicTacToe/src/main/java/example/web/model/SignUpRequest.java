package example.web.model;

import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Pattern;
import jakarta.validation.constraints.Size;
import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class SignUpRequest {
    @NotBlank(message = "Логин не может быть пустым")
    @Size(min = 3, max = 20, message = "Логин должен быть от 3 до 20 символов")
    @Pattern(regexp = "^[\\x00-\\x7F]+$", message = "Логин должен содержать только US-ASCII символы")
    private String login;

    @NotBlank(message = "Пароль не может быть пустым")
    @Size(min = 9, message = "Пароль должен быть не менее 9 символов")
    private String password;
}