package example.web.mapper;

import example.domain.mapper.AddressMapper;
import example.domain.model.AddressDTO;
import example.domain.model.SupplierDTO;
import example.web.model.request.CreateSupplierRequest;
import example.web.model.request.UpdateAddressRequest;
import example.web.model.response.AddressResponse;
import example.web.model.response.SupplierResponse;
import jakarta.validation.Valid;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class SupplierWebMapper {

    private final AddressMapper addressMapper;

    @Autowired
    public SupplierWebMapper(AddressMapper addressMapper) {
        this.addressMapper = addressMapper;
    }


    // Request → Domain DTO
    public SupplierDTO toDTO(@Valid CreateSupplierRequest request) {
        if (request == null) return null;

        SupplierDTO dto = new SupplierDTO();
        dto.setName(request.getName());
        dto.setAddressId(request.getAddressId());
        dto.setPhoneNumber(request.getPhoneNumber());

        return dto;
    }

    // Domain DTO → Response
    public SupplierResponse toResponse(SupplierDTO dto) {
        if (dto == null) return null;

        SupplierResponse response = new SupplierResponse();
        response.setId(dto.getId());
        response.setName(dto.getName());
        response.setAddressId(dto.getAddressId());  // Только ID
        response.setPhoneNumber(dto.getPhoneNumber());

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
