package example.datasource.model;

import lombok.Getter;
import lombok.Setter;

import java.time.LocalDate;
import java.util.UUID;

@Getter
@Setter

public class ClientDAO {
    private UUID id;
    private String clientName; // = client_name в БД
    private String clientSurname; // = client_surname
    private LocalDate birthday;
    private String  gender;
    private LocalDate registrationDate;
    private UUID addressId;
}
