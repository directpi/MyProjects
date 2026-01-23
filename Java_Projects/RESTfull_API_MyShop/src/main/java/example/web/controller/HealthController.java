package example.web.controller;

import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.tags.Tag;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.actuate.health.HealthComponent;
import org.springframework.boot.actuate.health.HealthEndpoint;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.time.LocalDateTime;
import java.util.HashMap;
import java.util.Map;

@Slf4j
@RestController
@RequestMapping("/api/v1")
@Tag(name = "Health", description = "Эндпоинты проверки состояния API")
public class HealthController {

    private final HealthEndpoint healthEndpoint;

    @Value("${spring.application.name:api-service}")
    private String applicationName;

    @Value("${app.version:1.0.0}")
    private String version;

    public HealthController(HealthEndpoint healthEndpoint) {
        this.healthEndpoint = healthEndpoint;
    }

    @GetMapping("/health")
    @Operation(summary = "Проверка состояния сервиса", description = "Возвращает базовый статус здоровья приложения", responses = {
            @ApiResponse(responseCode = "200", description = "Сервис работает корректно", content = @Content(schema = @Schema(implementation = Map.class))),
            @ApiResponse(responseCode = "503", description = "Сервис недоступен")
    })
    public ResponseEntity<Map<String, Object>> health() {
        log.debug("Health check requested");

        HealthComponent health = healthEndpoint.health();
        Map<String, Object> response = new HashMap<>();
        response.put("status", health.getStatus().getCode());
        response.put("timestamp", LocalDateTime.now());
        response.put("service", applicationName);
        response.put("version", version);

        if (health.getStatus().getCode().equals("UP")) {
            return ResponseEntity.ok(response);
        } else {
            return ResponseEntity.status(HttpStatus.SERVICE_UNAVAILABLE).body(response);
        }
    }

    @GetMapping("/health/detailed")
    @Operation(summary = "Детальная проверка состояния", description = "Возвращает расширенную информацию о здоровье сервиса и зависимостях")
    public ResponseEntity<HealthComponent> detailedHealth() {
        log.debug("Detailed health check requested");
        return ResponseEntity.ok(healthEndpoint.health());
    }

    @GetMapping("/version")
    @Operation(summary = "Версия приложения", description = "Возвращает текущую версию и информацию о сборке")
    public ResponseEntity<Map<String, Object>> version() {
        Map<String, Object> versionInfo = new HashMap<>();
        versionInfo.put("application", applicationName);
        versionInfo.put("version", version);
        versionInfo.put("timestamp", LocalDateTime.now());
        versionInfo.put("status", "operational");
        versionInfo.put("environment", System.getProperty("spring.profiles.active", "default"));

        return ResponseEntity.ok(versionInfo);
    }

    @GetMapping("/info")
    @Operation(summary = "Информация о приложении", description = "Возвращает расширенную информацию о приложении")
    public ResponseEntity<Map<String, Object>> info() {
        Map<String, Object> info = new HashMap<>();
        info.put("name", applicationName);
        info.put("version", version);
        info.put("description", "E-Commerce API Service");
        info.put("timestamp", LocalDateTime.now());
        info.put("java.version", System.getProperty("java.version"));
        info.put("java.vendor", System.getProperty("java.vendor"));
        info.put("os.name", System.getProperty("os.name"));
        info.put("os.version", System.getProperty("os.version"));

        // Добавляем информацию о памяти
        Runtime runtime = Runtime.getRuntime();
        info.put("memory.free", runtime.freeMemory());
        info.put("memory.total", runtime.totalMemory());
        info.put("memory.max", runtime.maxMemory());
        info.put("available.processors", runtime.availableProcessors());

        return ResponseEntity.ok(info);
    }
}