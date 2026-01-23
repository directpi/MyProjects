package example.web.model.request;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.*;
import lombok.Getter;
import lombok.Setter;

import java.time.LocalDate;
import java.util.UUID;

@Getter
@Setter
public class CreateClientRequest {
    @Schema(description = "Имя клиента", example = "Иван")
    @NotBlank(message = "ввод имен обязателен")
    @Size(min = 2, max = 50, message = "Имя должно быть от 2 до 50 символов")
    private String firstName;

    @Schema(description = "Фамилия клиента", example = "Петров")
    @NotBlank(message = "Ввод фамилии обязателен")
    @Size(min = 2, max = 50, message = "Фамилия должна быть от 2 до 50 символов")
    private String lastName;

    @Schema(description = "Дата рождения", example = "1990-05-15")
    @NotNull(message = "Ввод дня рождения обязателен")
    @Past(message = "Birth date must be in the past")
    private LocalDate birthDate;

    @Schema(description = "Пол", example = "MALE")
    @NotBlank(message = "Ввод пола обязателен")
    @Pattern(regexp = "^(MALE|FEMALE)$", message = "Пол должен быть MALE или FEMALE")
    private String gender;

    @Schema(description = "ID адреса", example = "11111111-1111-1111-1111-111111111111")
    @NotNull(message = "Ввод адреса обязателен")
    private UUID addressId;
}
