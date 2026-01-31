package example.datasource.mapper;

import example.datasource.model.CategoryDAO;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.stereotype.Component;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.UUID;

@Component
public class CategoryRowMapper implements RowMapper<CategoryDAO> {

    @Override
    public CategoryDAO mapRow(ResultSet rs, int rowNum) throws SQLException {
        CategoryDAO category = new CategoryDAO();
        category.setId(rs.getObject("id", UUID.class));
        category.setCode(rs.getString("code"));
        category.setDisplayName(rs.getString("display_name"));
        return category;
    }
}
