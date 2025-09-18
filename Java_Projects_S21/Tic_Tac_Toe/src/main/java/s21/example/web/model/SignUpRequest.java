package s21.example.web.model;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class SignUpRequest {
    private String login;
    private String password;
}