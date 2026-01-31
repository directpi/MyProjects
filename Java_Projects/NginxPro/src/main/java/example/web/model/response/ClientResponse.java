package example.web.model.response;

import com.fasterxml.jackson.annotation.JsonFormat;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;

import java.time.LocalDate;
import java.util.UUID;

@Getter
@Setter
public class ClientResponse {
    @Schema(description = "ID клиента", example = "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaa1")
    private UUID id;
    @Schema(description = "Имя клиента", example = "Иван")
    private String firstName;
    @Schema(description = "Фамилия клиента", example = "Петров")
    private String lastName;

    @JsonFormat(pattern = "yyyy-MM-dd")
    @Schema(description = "Дата рождения", example = "1990-05-15")
    private LocalDate birthDate;

    @Schema(description = "Пол", example = "MALE")
    private String gender;
    @Schema(description = "Отображаемое значение пола", example = "Мужской")
    private String genderDisplay; // "Мужской" вместо "MALE" ...

    @JsonFormat(pattern = "yyyy-MM-dd")
    @Schema(description = "Дата регистрации", example = "2024-01-10")
    private LocalDate registrationDate;

    @Schema(description = "ID адреса", example = "11111111-1111-1111-1111-111111111111")
    private UUID addressId;
    // private AddressResponse address;

    public void setGender(String gender) {
        this.gender = gender;
        this.genderDisplay = "MALE".equals(gender) ? "Мужской" : "FEMALE".equals(gender) ? "Женский" : null;
    }
}
