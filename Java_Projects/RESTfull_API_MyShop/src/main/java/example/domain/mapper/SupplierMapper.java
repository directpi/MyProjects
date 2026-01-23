package example.domain.mapper;

import example.datasource.model.SupplierDAO;
import example.domain.model.SupplierDTO;
import org.springframework.stereotype.Component;

@Component
public class SupplierMapper {
    public final SupplierDTO toDTO(SupplierDAO dao){
        if (dao == null) {
            return null;
        }
        SupplierDTO dto = new SupplierDTO();

        dto.setId(dao.getId());
        dto.setName(dao.getName());
        dto.setAddressId(dao.getAddressId());
        dto.setPhoneNumber(dao.getPhoneNumber());

        return dto;
    }

    public SupplierDAO toDAO(SupplierDTO dto){
        if (dto == null) return null;
        SupplierDAO dao = new SupplierDAO();

        dao.setId(dto.getId());
        dao.setName(dto.getName());
        dao.setAddressId(dto.getAddressId());
        dao.setPhoneNumber(dto.getPhoneNumber());

        return dao;
    }
}
