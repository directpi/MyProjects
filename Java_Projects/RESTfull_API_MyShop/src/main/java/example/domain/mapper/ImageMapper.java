package example.domain.mapper;

import example.datasource.model.ImageDAO;
import example.domain.model.ImageDTO;
import org.springframework.stereotype.Component;

@Component
public class ImageMapper {

    public ImageDTO toDTO (ImageDAO dao){
        if (dao == null) return null;
        ImageDTO dto = new ImageDTO();

        dto.setId(dao.getId());
        dto.setImageData(dao.getImageData());

        return dto;
    }

    public ImageDAO toDAO (ImageDTO dto){
        if (dto == null) return null;

        ImageDAO dao = new ImageDAO();
        dao.setId(dto.getId());
        dao.setImageData(dto.getImageData());

        return dao;
    }
}
