package example.domain.model;

import java.time.LocalDate;
import java.util.UUID;
import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class ClientDTO {
    private UUID id;
    private String firstName;
    private String lastName;
    private LocalDate birthDate;
    private String gender;
    private LocalDate registrationDate;
    private AddressDTO address;      // Полный объект адреса (может быть null)
    private UUID addressId;          // ID адреса (всегда есть если адрес привязан)

    public ClientDTO(){}

    // equals(), hashCode(), toString()
    @Override
    public String toString() {
        return String.format("%s %s", firstName, lastName);
    }

    public boolean hasAddress() {
        return addressId != null;
    }

}
