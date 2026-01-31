package example.domain.mapper;

import example.datasource.model.AddressDAO;
import example.domain.model.AddressDTO;
import org.springframework.stereotype.Component;

@Component
public class AddressMapper {
    public AddressDTO toDTO(AddressDAO dao) {
        if (dao == null) return null;
        AddressDTO dto = new AddressDTO();
        dto.setId(dao.getId());
        dto.setCountry(dao.getCountry());
        dto.setCity(dao.getCity());
        dto.setStreet(dao.getStreet());

        return dto;
    }

    public AddressDAO toDAO(AddressDTO dto) {
        if (dto == null) return null;
        AddressDAO dao = new AddressDAO();
        dao.setId(dto.getId());
        dao.setCountry(dto.getCountry());
        dao.setCity(dto.getCity());
        dao.setStreet(dto.getStreet());

        return dao;    }
}