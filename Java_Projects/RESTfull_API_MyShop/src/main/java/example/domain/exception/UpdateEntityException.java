package example.domain.exception;

public class UpdateEntityException extends ServiceException {
    public UpdateEntityException(String entityName, Object id) {
        super("Не удалось обновить " + entityName + " с id: " + id, "UPDATE_ERROR");
    }
}