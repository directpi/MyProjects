package example.domain.exception;

public class EntityNotFoundException extends ServiceException {
    public EntityNotFoundException(String entityName, Object id) {
        super(entityName + " с id: " + id + " не найден", "NOT_FOUND");
    }

    public EntityNotFoundException(String entityName, String field, Object value) {
        super(entityName + " с " + field + ": " + value + " не найден", "NOT_FOUND");
    }
}