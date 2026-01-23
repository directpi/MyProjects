package example.web.controller;

import example.domain.model.ClientDTO;
import example.domain.model.AddressDTO;
import example.domain.service.ClientService;
import example.web.mapper.ClientWebMapper;
import example.web.model.request.CreateClientRequest;
import example.web.model.request.UpdateAddressRequest;
import example.web.model.response.ClientResponse;
import example.web.model.response.ErrorResponse;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import lombok.extern.slf4j.Slf4j;
import java.util.List;
import java.util.UUID;
import java.util.stream.Collectors;

@Slf4j
@RestController
@RequestMapping("/api/v1/clients")
@Tag(name = "Clients", description = "API для управления клиентами")
public class ClientController {
    private final ClientService clientService;
    private final ClientWebMapper clientWebMapper;

    @Autowired
    public ClientController(ClientService clientService, ClientWebMapper clientWebMapper) {
        this.clientService = clientService;
        this.clientWebMapper = clientWebMapper;
    }

    // Создать клиента
    @PostMapping
    @Operation(summary = "Создание клиента", description = "Создает нового клиента и возвращает его данные", responses = {
            @ApiResponse(responseCode = "201", description = "Клиент создан", content = @Content(schema = @Schema(implementation = ClientResponse.class))),
            @ApiResponse(responseCode = "400", description = "Неверные данные", content = @Content(schema = @Schema(implementation = ErrorResponse.class)))
    })
    public ResponseEntity<ClientResponse> createClient(@Valid @RequestBody CreateClientRequest request) {

        log.info("Creating new client: {} {}", request.getFirstName(), request.getLastName());

        // Преобразуем Request → DTO
        ClientDTO clientDTO = clientWebMapper.toDTO(request);

        // Вызываем Service
        ClientDTO createdClient = clientService.createClient(clientDTO);

        // Преобразуем DTO → Response
        ClientResponse response = clientWebMapper.toResponse(createdClient);

        return ResponseEntity.status(HttpStatus.CREATED).body(response);
    }

    // Получить клиента по ID
    @GetMapping("/{id}")
    @Operation(summary = "Получение клиента по ID", responses = {
            @ApiResponse(responseCode = "200", description = "Клиент найден", content = @Content(schema = @Schema(implementation = ClientResponse.class))),
            @ApiResponse(responseCode = "404", description = "Клиент не найден", content = @Content(schema = @Schema(implementation = ErrorResponse.class)))
    })
    public ResponseEntity<ClientResponse> getClientById(@PathVariable UUID id) {

        log.debug("Getting client by id: {}", id);

        ClientDTO clientDTO = clientService.getById(id);
        ClientResponse response = clientWebMapper.toResponse(clientDTO);
        return ResponseEntity.ok(response);
    }

    // 3. Удалить клиента по ID
    @DeleteMapping("/{id}")
    @Operation(summary = "Удаление клиента по ID", responses = {
            @ApiResponse(responseCode = "204", description = "Клиент удален"),
            @ApiResponse(responseCode = "404", description = "Клиент не найден", content = @Content(schema = @Schema(implementation = ErrorResponse.class)))
    })
    public ResponseEntity<Void> deleteClient(@PathVariable UUID id) {

        log.info("Deleting client with id: {}", id);
        clientService.deleteById(id);
        log.info("Client deleted successfully: {}", id);
        return ResponseEntity.noContent().build(); // 204 No Content
    }

    // 4. Поиск клиентов по имени и фамилии
    @GetMapping("/search")
    @Operation(summary = "Поиск клиентов по имени и фамилии", responses = {
            @ApiResponse(responseCode = "200", description = "Список клиентов", content = @Content(schema = @Schema(implementation = ClientResponse.class))),
            @ApiResponse(responseCode = "400", description = "Неверные параметры", content = @Content(schema = @Schema(implementation = ErrorResponse.class)))
    })
    public ResponseEntity<List<ClientResponse>> searchClients(
            @RequestParam String firstName,
            @RequestParam String lastName) {

        List<ClientDTO> clients = clientService.findByNameAndSurname(firstName, lastName);

        List<ClientResponse> responses = clients.stream()
                .map(clientWebMapper::toResponse)
                .collect(Collectors.toList());

        return ResponseEntity.ok(responses);
    }

    // 5. Получить всех клиентов (опциональная пагинация: limit/offset)
    @GetMapping
    @Operation(summary = "Получение списка клиентов", description = "Возвращает всех клиентов. При наличии limit/offset возвращает пагинированный список.", responses = {
            @ApiResponse(responseCode = "200", description = "Список клиентов", content = @Content(schema = @Schema(implementation = ClientResponse.class))),
            @ApiResponse(responseCode = "400", description = "Неверные параметры", content = @Content(schema = @Schema(implementation = ErrorResponse.class)))
    })
    public ResponseEntity<List<ClientResponse>> getAllClients(
            @Parameter(description = "Максимальное количество записей", example = "10") @RequestParam(required = false) Integer limit,
            @Parameter(description = "Смещение от начала списка", example = "0") @RequestParam(required = false) Integer offset) {

        List<ClientDTO> clients;

        if (limit != null && offset != null) {
            clients = clientService.findAllPaginated(limit, offset);
        } else {
            clients = clientService.findAll();
        }

        List<ClientResponse> responses = clients.stream()
                .map(clientWebMapper::toResponse)
                .collect(Collectors.toList());

        return ResponseEntity.ok(responses);
    }

    // 6. Изменить адрес клиента
    @PatchMapping("/{id}/address")
    @Operation(summary = "Изменение адреса клиента", responses = {
            @ApiResponse(responseCode = "204", description = "Адрес обновлен"),
            @ApiResponse(responseCode = "400", description = "Неверные данные", content = @Content(schema = @Schema(implementation = ErrorResponse.class))),
            @ApiResponse(responseCode = "404", description = "Клиент не найден", content = @Content(schema = @Schema(implementation = ErrorResponse.class)))
    })
    public ResponseEntity<Void> updateClientAddress(
            @PathVariable("id") UUID clientId,
            @Valid @RequestBody UpdateAddressRequest request) {

        log.info("Updating address for client id: {}, new address: {} {} {}",
                clientId, request.getCountry(), request.getCity(), request.getStreet());

        // Преобразуем Request → DTO
        AddressDTO addressDTO = clientWebMapper.toAddressDTO(request);

        // Вызываем Service
        clientService.updateClientAddress(clientId, addressDTO);

        log.info("Address updated successfully for client: {}", clientId);

        return ResponseEntity.noContent().build(); // 204 No Content
    }
}
