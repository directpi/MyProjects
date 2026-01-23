package example.domain.model;

import java.time.LocalDate;
import java.util.UUID;
import lombok.Getter;
import lombok.Setter;

@Getter
@Setter

public class SupplierDTO {

    private UUID id;
    private String name;
    private UUID addressId;
    private String phoneNumber;

    // equals(), hashCode(), toString()
    @Override
    public String toString() {
        return String.format("SupplierDTO{id=%s, name='%s', phoneNumber='%s'}", id, name, phoneNumber);
    }

    public boolean hasAddress() {
        return addressId != null;
    }
}
