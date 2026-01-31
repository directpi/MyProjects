package example.datasource.model;

import lombok.Getter;
import lombok.Setter;
import java.util.UUID;

@Getter
@Setter
public class ImageDAO {
    private UUID id;
    private byte[] imageData;
}
