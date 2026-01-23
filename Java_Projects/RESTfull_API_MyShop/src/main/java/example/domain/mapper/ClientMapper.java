package example.domain.mapper;

import example.datasource.model.ClientDAO;
import example.domain.model.ClientDTO;
import  org.springframework.stereotype.Component;

import java.time.LocalDate;
import java.time.Period;

@Component
public class ClientMapper {
    public ClientDTO toDTO (ClientDAO dao){
        if (dao == null) {
            return null;
        }

        ClientDTO dto = new ClientDTO();
        dto.setId(dao.getId());
        dto.setFirstName(dao.getClientName());
        dto.setLastName(dao.getClientSurname());
        dto.setBirthDate(dao.getBirthday());
        dto.setRegistrationDate(dao.getRegistrationDate());
        dto.setAddressId(dao.getAddressId());
        dto.setAddress(null);
        dto.setGender(dao.getGender());

        return dto;
    }

    public ClientDAO  toDAO(ClientDTO clientDTO) {
        if (clientDTO == null) {
            return null;
        }
        ClientDAO dao = new ClientDAO();
        dao.setId(clientDTO.getId());
        dao.setClientName(clientDTO.getFirstName());
        dao.setClientSurname(clientDTO.getLastName());
        dao.setBirthday(clientDTO.getBirthDate());
        dao.setRegistrationDate(clientDTO.getRegistrationDate());
        dao.setAddressId(clientDTO.getAddressId());
        dao.setGender(clientDTO.getGender());

        return dao;
    }

    private Integer calculateAge (LocalDate birthDate){
        return Period.between(birthDate, LocalDate.now()).getYears();
    }

}
