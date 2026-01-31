package example.web.model.response;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;

import java.util.UUID;

@Getter
@Setter
public class SupplierResponse {

    @Schema(description = "ID поставщика", example = "cccccccc-cccc-cccc-cccc-ccccccccccc1")
    private UUID id;
    @Schema(description = "Название поставщика", example = "ООО \"БытовойТехника\"")
    private String name;
    @Schema(description = "ID адреса", example = "66666666-6666-6666-6666-666666666661")
    private UUID addressId;
    @Schema(description = "Телефон поставщика", example = "+7-495-123-45-67 или +7(495)1234567")
    private String phoneNumber;

}
