package example.datasource.model;

import example.domain.model.AddressDTO;
import lombok.Getter;
import lombok.Setter;

import java.util.UUID;

@Getter
@Setter
public class AddressDAO {
    private UUID id;
    private String country;
    private String city;
    private String street;

    public AddressDAO(){}

    public AddressDAO(UUID id, String country, String city, String street){
        this.id = id;
        this.country = country;
        this.city = city;
        this.street = street;
    }

    //Other
    @Override
    public String toString(){
        return String.format("AddressDAO{id = %s, country = '%s', city = '%s', street = '%s'}", id, country, city, street);
    }
}


