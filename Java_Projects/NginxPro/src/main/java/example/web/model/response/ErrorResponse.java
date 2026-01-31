// src/APIFromSpec/src/main/java/example/web/model/response/ErrorResponse.java
package example.web.model.response;

import com.fasterxml.jackson.annotation.JsonFormat;
import com.fasterxml.jackson.annotation.JsonInclude;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.time.LocalDateTime;
import java.util.Map;

@Getter
@Setter
@NoArgsConstructor
@AllArgsConstructor
@JsonInclude(JsonInclude.Include.NON_NULL)
@Schema(description = "Единый формат ответа об ошибке")
public class ErrorResponse {
    @JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss")
    @Schema(description = "Время возникновения ошибки", example = "2026-01-22 12:34:56")
    private LocalDateTime timestamp;
    @Schema(description = "HTTP статус", example = "400")
    private int status;
    @Schema(description = "Короткое описание ошибки", example = "Validation Error")
    private String error;
    @Schema(description = "Сообщение об ошибке", example = "Поле 'firstName' не может быть пустым")
    private String message;
    @Schema(description = "Путь запроса, если доступен", example = "/api/v1/clients")
    private String path;
    @Schema(description = "Детали ошибок валидации по полям")
    private Map<String, String> details;

    // Builder методы для удобства
    public static ErrorResponse of(int status, String error, String message) {
        return new ErrorResponse(
                LocalDateTime.now(),
                status,
                error,
                message,
                null,
                null);
    }

    public static ErrorResponse withDetails(int status, String error,
            String message, Map<String, String> details) {
        return new ErrorResponse(
                LocalDateTime.now(),
                status,
                error,
                message,
                null,
                details);
    }
}