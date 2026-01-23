package example.datasource.model;

import lombok.Getter;
import lombok.Setter;

import java.time.LocalDate;
import java.util.UUID;

@Getter
@Setter
public class SupplierDAO {
    private UUID id;
    private String name;
    private UUID addressId;
    private String phoneNumber;
}
