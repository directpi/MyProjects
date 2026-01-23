package example.web.controller;

import example.domain.model.SupplierDTO;
import example.domain.model.AddressDTO;
import example.domain.service.SupplierService;
import example.web.mapper.SupplierWebMapper;
import example.web.model.request.CreateSupplierRequest;
import example.web.model.request.UpdateAddressRequest;
import example.web.model.response.ErrorResponse;
import example.web.model.response.SupplierResponse;
import io.swagger.v3.oas.annotations.Operation;
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
@RequestMapping("/api/v1/suppliers")
@Tag(name = "Suppliers", description = "API для управления поставщиками")
public class SupplierController {
    private final SupplierService supplierService;
    private final SupplierWebMapper supplierWebMapper;

    @Autowired
    public SupplierController(SupplierService supplierService, SupplierWebMapper supplierWebMapper) {
        this.supplierService = supplierService;
        this.supplierWebMapper = supplierWebMapper;
    }

    // 1. Создать поставщика
    @PostMapping
    @Operation(summary = "Создание поставщика", responses = {
            @ApiResponse(responseCode = "201", description = "Поставщик создан", content = @Content(schema = @Schema(implementation = SupplierResponse.class))),
            @ApiResponse(responseCode = "400", description = "Неверные данные", content = @Content(schema = @Schema(implementation = ErrorResponse.class)))
    })
    public ResponseEntity<SupplierResponse> createSupplier(@Valid @RequestBody CreateSupplierRequest request) {

        log.info("Creating new supplier: {} {}", request.getName(), request.getPhoneNumber());

        // Преобразуем Request → DTO
        SupplierDTO supplierDTO = supplierWebMapper.toDTO(request);

        // Вызываем Service
        SupplierDTO createdSupplier = supplierService.createSupplier(supplierDTO);

        // Преобразуем DTO → Response
        SupplierResponse response = supplierWebMapper.toResponse(createdSupplier);

        return ResponseEntity.status(HttpStatus.CREATED).body(response);
    }

    // 2. Получить поставщика по ID
    @GetMapping("/{id}")
    @Operation(summary = "Получение поставщика по ID", responses = {
            @ApiResponse(responseCode = "200", description = "Поставщик найден", content = @Content(schema = @Schema(implementation = SupplierResponse.class))),
            @ApiResponse(responseCode = "404", description = "Поставщик не найден", content = @Content(schema = @Schema(implementation = ErrorResponse.class)))
    })
    public ResponseEntity<SupplierResponse> getSupplierById(@PathVariable UUID id) {

        log.debug("Getting supplier by id: {}", id);

        SupplierDTO supplierDTO = supplierService.getById(id);
        SupplierResponse response = supplierWebMapper.toResponse(supplierDTO);
        return ResponseEntity.ok(response);
    }

    // 3. Удалить поставщика по ID
    @DeleteMapping("/{id}")
    @Operation(summary = "Удаление поставщика", responses = {
            @ApiResponse(responseCode = "204", description = "Поставщик удален"),
            @ApiResponse(responseCode = "404", description = "Поставщик не найден", content = @Content(schema = @Schema(implementation = ErrorResponse.class)))
    })
    public ResponseEntity<Void> deleteSupplier(@PathVariable UUID id) {

        log.info("Deleting supplier with id: {}", id);
        supplierService.deleteById(id);
        log.info("Supplier deleted successfully: {}", id);
        return ResponseEntity.noContent().build(); // 204 No Content
    }

    // 4. Получить всех поставщиков (без пагинации)
    @GetMapping
    @Operation(summary = "Получение списка поставщиков", responses = {
            @ApiResponse(responseCode = "200", description = "Список поставщиков", content = @Content(schema = @Schema(implementation = SupplierResponse.class)))
    })
    public ResponseEntity<List<SupplierResponse>> getAllSuppliers() {
        List<SupplierDTO> suppliers = supplierService.findAll();

        List<SupplierResponse> responses = suppliers.stream()
                .map(supplierWebMapper::toResponse)
                .collect(Collectors.toList());

        return ResponseEntity.ok(responses);
    }

    // 5. Изменить адрес поставщика
    @PatchMapping("/{id}/address")
    @Operation(summary = "Изменение адреса поставщика", responses = {
            @ApiResponse(responseCode = "204", description = "Адрес обновлен"),
            @ApiResponse(responseCode = "400", description = "Неверные данные", content = @Content(schema = @Schema(implementation = ErrorResponse.class))),
            @ApiResponse(responseCode = "404", description = "Поставщик не найден", content = @Content(schema = @Schema(implementation = ErrorResponse.class)))
    })
    public ResponseEntity<Void> updateSupplierAddress(
            @PathVariable("id") UUID supplierId,
            @Valid @RequestBody UpdateAddressRequest request) {

        log.info("Updating address for supplier id: {}, new address: {} {} {}",
                supplierId, request.getCountry(), request.getCity(), request.getStreet());

        // Преобразуем Request → DTO
        AddressDTO addressDTO = supplierWebMapper.toAddressDTO(request);

        // Вызываем Service
        supplierService.updateSupplierAddress(supplierId, addressDTO);

        log.info("Address updated successfully for supplier: {}", supplierId);

        return ResponseEntity.noContent().build(); // 204 No Content
    }
}
