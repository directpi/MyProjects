package example.web.model.request;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import lombok.Getter;
import lombok.Setter;

import java.util.UUID;

@Getter
@Setter
public class UpdateAddressRequest {
    @Schema(description = "ID адреса", example = "11111111-1111-1111-1111-111111111111")
    @NotNull(message = "Id адреса обязателен")
    private UUID id; // По ТЗ: адрес приходит с ID

    @Schema(description = "Страна", example = "Россия")
    @NotBlank(message = "Ввод страны обязателен")
    private String country;

    @Schema(description = "Город", example = "Москва")
    @NotBlank(message = "Ввод города обязателен")
    private String city;

    @Schema(description = "Улица", example = "ул. Тверская, д. 1")
    @NotBlank(message = "Ввод улицы обязателен")
    private String street;
}
