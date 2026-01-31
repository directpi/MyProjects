package example.domain.exception;

public class DeleteEntityException extends ServiceException {
    public DeleteEntityException(String entityName, Object id) {
        super("Не удалось удалить " + entityName + " с id: " + id, "DELETE_ERROR");
    }
}