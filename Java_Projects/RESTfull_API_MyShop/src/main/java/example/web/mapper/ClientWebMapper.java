package example.web.mapper;

import example.domain.mapper.AddressMapper;
import example.domain.model.AddressDTO;
import example.domain.model.ClientDTO;
import example.web.model.request.CreateClientRequest;
import example.web.model.request.UpdateAddressRequest;
import example.web.model.response.AddressResponse;
import example.web.model.response.ClientResponse;
import jakarta.validation.Valid;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class ClientWebMapper {

    private final AddressMapper addressMapper;

    @Autowired
    public ClientWebMapper(AddressMapper addressMapper){
        this.addressMapper = addressMapper;
    }

    // Request → Domain DTO
    public ClientDTO toDTO(@Valid CreateClientRequest request) {
        if (request == null) return null;

        ClientDTO dto = new ClientDTO();
        dto.setFirstName(request.getFirstName());
        dto.setLastName(request.getLastName());
        dto.setBirthDate(request.getBirthDate());
        dto.setGender(request.getGender());  // "MALE" или "FEMALE"
        dto.setAddressId(request.getAddressId());

        return dto;
    }

    // Domain DTO → Response
    public ClientResponse toResponse(ClientDTO dto) {
        if (dto == null) return null;

        ClientResponse response = new ClientResponse();
        response.setId(dto.getId());
        response.setFirstName(dto.getFirstName());
        response.setLastName(dto.getLastName());
        response.setBirthDate(dto.getBirthDate());
        response.setGender(dto.getGender());
        response.setRegistrationDate(dto.getRegistrationDate());
        response.setAddressId(dto.getAddressId());  // Только ID

//        // Если в DTO есть полный адрес - маппим его
//        if (dto.getAddress() != null) {
//            AddressResponse addressResponse = new AddressResponse();
//            addressResponse.setId(dto.getAddress().getId());
//            addressResponse.setCountry(dto.getAddress().getCountry());
//            addressResponse.setCity(dto.getAddress().getCity());
//            addressResponse.setStreet(dto.getAddress().getStreet());
//            response.setAddress(addressResponse);
//        }

        return response;
    }

    public AddressDTO toAddressDTO(@Valid UpdateAddressRequest request) {
        if (request == null) return null;

        AddressDTO dto = new AddressDTO();
        dto.setId(request.getId());  // По ТЗ: адрес приходит с ID
        dto.setCountry(request.getCountry());
        dto.setCity(request.getCity());
        dto.setStreet(request.getStreet());

        return dto;
    }
}
