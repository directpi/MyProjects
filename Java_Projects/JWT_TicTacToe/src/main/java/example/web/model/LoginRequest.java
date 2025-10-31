package example.web.model;

import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class LoginRequest {

    @NotBlank(message = "Логин не может быть пустым")
    @Size(min = 3, max = 20, message = "Логин должен быть от 3 до 20 символов")
    private String login;

    @NotBlank(message = "Пароль не может быть пустым")
    @Size(min = 9, message = "Пароль должен быть не менее 9 символов")
    private String password;

    public LoginRequest() {}

    public LoginRequest(String login, String password) {
        this.login = login;
        this.password = password;
    }
}