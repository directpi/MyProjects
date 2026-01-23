package example.domain.exception;

public class CreateEntityException extends ServiceException {
    public CreateEntityException(String entityName) {
        super("Не удалось создать " + entityName, "CREATE_ERROR");
    }

    public CreateEntityException(String entityName, Throwable cause) {
        super("Не удалось создать " + entityName + ": " + cause.getMessage(),
                "CREATE_ERROR", cause);
    }
}