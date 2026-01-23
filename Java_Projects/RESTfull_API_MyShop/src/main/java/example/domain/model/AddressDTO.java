package example.domain.model;

import lombok.Getter;
import lombok.Setter;

import  java.util.UUID;

@Getter
@Setter
public class AddressDTO {
    private UUID id; // Может быть null для нового адреса!
    private String country;
    private String city;
    private String street;

    public AddressDTO(){}
    public AddressDTO(UUID id, String country, String city, String street){
        this.id = id;
        this.country = country;
        this.city = city;
        this.street = street;
    }

    // equals(), hashCode(), toString()
    @Override
    public String toString(){
        return String.format("%s, %s, %s", country, city, street);
    }
}
