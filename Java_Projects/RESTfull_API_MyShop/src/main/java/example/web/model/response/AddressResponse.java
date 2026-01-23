package example.web.model.response;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;

import java.util.UUID;

@Getter
@Setter
public class AddressResponse {
    @Schema(description = "ID адреса", example = "11111111-1111-1111-1111-111111111111")
    private UUID id;
    @Schema(description = "Страна", example = "Россия")
    private String country;
    @Schema(description = "Город", example = "Москва")
    private String city;
    @Schema(description = "Улица", example = "ул. Тверская, д. 1")
    private String street;

    public AddressResponse() {
    }

    public AddressResponse(UUID id, String country, String city, String street) {
        this.id = id;
        this.country = country;
        this.city = city;
        this.street = street;
    }

    @Override
    public String toString() {
        return String.format("%s, %s, %s", country, city, street);
    }
}
