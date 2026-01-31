package example.config;

import io.swagger.v3.oas.models.Components;
import io.swagger.v3.oas.models.OpenAPI;
import io.swagger.v3.oas.models.info.Contact;
import io.swagger.v3.oas.models.info.Info;
import io.swagger.v3.oas.models.info.License;
import io.swagger.v3.oas.models.security.SecurityRequirement;
import io.swagger.v3.oas.models.security.SecurityScheme;
import io.swagger.v3.oas.models.servers.Server;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import java.util.List;

@Configuration
public class OpenApiConfig {

        @Value("${spring.application.name:API Service}")
        private String applicationName;

        @Value("${app.version:1.0.0}")
        private String version;

        @Bean
        public OpenAPI customOpenApi() {
                final String securitySchemeName = "basicAuth";

                return new OpenAPI()
                                .info(new Info()
                                                .title(applicationName)
                                                .version(version)
                                                .description("""
                                                                ## E-Commerce API Documentation

                                                                ### Описание
                                                                REST API для управления клиентами и товарами в электронной коммерции.

                                                                ### Функциональность
                                                                - Управление клиентами (CRUD операции)
                                                                - Управление товарами (CRUD операции)
                                                                - Управление запасами товаров
                                                                - Управление адресами

                                                                ### Аутентификация
                                                                Используется HTTP Basic аутентификация.
                                                                """)
                                                .termsOfService("http://example.com/terms")
                                                .contact(new Contact()
                                                                .name("Directpi")
                                                                .email("directpi@student.s21-school.ru")
                                                                .url("http://example.com"))
                                                .license(new License()
                                                                .name("Apache 2.0")
                                                                .url("http://www.apache.org/licenses/LICENSE-2.0.html")))
                                .servers(List.of(
                                                new Server()
                                                                .url("http://localhost:8080")
                                                                .description("Локальный сервер разработки"),
                                                new Server()
                                                                .url("https://api.example.com")
                                                                .description("Продуктивный сервер")))
                                .addSecurityItem(new SecurityRequirement()
                                                .addList(securitySchemeName))
                                .components(new Components()
                                                .addSecuritySchemes(securitySchemeName,
                                                                new SecurityScheme()
                                                                                .name(securitySchemeName)
                                                                                .type(SecurityScheme.Type.HTTP)
                                                                                .scheme("basic")
                                                                                .description("HTTP Basic аутентификация. "
                                                                                                +
                                                                                                "Используйте username/password для доступа к API"))
                                                .addSecuritySchemes("bearerAuth",
                                                                new SecurityScheme()
                                                                                .type(SecurityScheme.Type.HTTP)
                                                                                .scheme("bearer")
                                                                                .bearerFormat("JWT")
                                                                                .description("JWT токен аутентификации (запасной вариант)")))
                                .externalDocs(new io.swagger.v3.oas.models.ExternalDocumentation()
                                                .description("Дополнительная документация")
                                                .url("http://example.com/docs"));
        }
}