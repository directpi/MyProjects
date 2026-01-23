package example.web.model.request;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.*;
import lombok.Getter;
import lombok.Setter;

import java.util.UUID;

@Getter
@Setter
public class CreateSupplierRequest {
    @Schema(description = "Название поставщика", example = "ООО \"БытовойТехника\"")
    @NotBlank(message = "ввод названия поставщика обязателен")
    @Size(min = 2, max = 50, message = "Имя должно быть от 2 до 50 символов")
    private String name;

    @Schema(description = "ID адреса", example = "66666666-6666-6666-6666-666666666661")
    @NotNull(message = "Ввод адреса обязателен")
    private UUID addressId;

    @Schema(description = "Телефон поставщика", example = "+7-495-123-45-67 или +7(495)1234567")
    @NotBlank(message = "Ввод номера телефона обязателен")
    @Pattern(regexp = "^\\+?[0-9\\s\\-\\(\\)]+$", message = "Неверный формат номера телефона. Допустимы цифры, пробелы, скобки, дефисы и знак + в начале")
    @Size(min = 7, max = 25, message = "Номер телефона должен быть от 7 до 25 символов")
    private String phoneNumber;
}
