package example.web.controller;

import example.domain.model.ProductDTO;
import example.domain.service.ProductService;
import example.web.mapper.ProductWebMapper;
import example.web.model.request.CreateProductRequest;
import example.web.model.request.UpdateStockRequest;
import example.web.model.response.ErrorResponse;
import example.web.model.response.ProductResponse;
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

import java.util.List;
import java.util.UUID;
import java.util.stream.Collectors;
import lombok.extern.slf4j.Slf4j;

@Slf4j
@RestController
@RequestMapping("/api/v1/products")
@Tag(name = "Products", description = "API для управления товарами")
public class ProductController {
    private final ProductService productService;
    private final ProductWebMapper productWebMapper;

    @Autowired
    public ProductController(ProductService productService, ProductWebMapper productWebMapper) {
        this.productService = productService;
        this.productWebMapper = productWebMapper;
    }

    // 1. Добавление товара
    @PostMapping
    @Operation(summary = "Создание товара", responses = {
            @ApiResponse(responseCode = "201", description = "Товар создан", content = @Content(schema = @Schema(implementation = ProductResponse.class))),
            @ApiResponse(responseCode = "400", description = "Неверные данные", content = @Content(schema = @Schema(implementation = ErrorResponse.class)))
    })
    public ResponseEntity<ProductResponse> createProduct(@Valid @RequestBody CreateProductRequest request) {

        log.info("Creating new product: {} {} {}", request.getName(), request.getCategory(),
                request.getAvailableStock());

        ProductDTO productDTO = productWebMapper.toDTO(request);
        ProductDTO createdProduct = productService.createProduct(productDTO);
        ProductResponse response = productWebMapper.toResponse(createdProduct);
        return ResponseEntity.status(HttpStatus.CREATED).body(response);
    }

    // 2. Уменьшение количества товара
    @PatchMapping("/{id}/stock/decrease")
    @Operation(summary = "Уменьшение количества товара", responses = {
            @ApiResponse(responseCode = "204", description = "Количество уменьшено"),
            @ApiResponse(responseCode = "400", description = "Неверные данные", content = @Content(schema = @Schema(implementation = ErrorResponse.class))),
            @ApiResponse(responseCode = "404", description = "Товар не найден", content = @Content(schema = @Schema(implementation = ErrorResponse.class)))
    })
    public ResponseEntity<Void> decreaseProductStock(
            @PathVariable UUID id,
            @Valid @RequestBody UpdateStockRequest request) {

        log.info("Decreasing stock for product id: {}, amount: {}", id, request.getAmount());
        productService.decreaseStock(id, request.getAmount());
        log.info("Stock decreased successfully for product: {}, amount: {}", id, request.getAmount());

        return ResponseEntity.noContent().build();
    }

    // 3. Получение товара по ID
    @GetMapping("/{id}")
    @Operation(summary = "Получение товара по ID", responses = {
            @ApiResponse(responseCode = "200", description = "Товар найден", content = @Content(schema = @Schema(implementation = ProductResponse.class))),
            @ApiResponse(responseCode = "404", description = "Товар не найден", content = @Content(schema = @Schema(implementation = ErrorResponse.class)))
    })
    public ResponseEntity<ProductResponse> getProductById(@PathVariable UUID id) {

        log.debug("Getting product by id: {}", id);

        ProductDTO productDTO = productService.getById(id);
        ProductResponse response = productWebMapper.toResponse(productDTO);
        return ResponseEntity.ok(response);
    }

    // 4. Получение всех доступных товаров
    @GetMapping("/available")
    @Operation(summary = "Получение доступных товаров", responses = {
            @ApiResponse(responseCode = "200", description = "Список товаров", content = @Content(schema = @Schema(implementation = ProductResponse.class)))
    })
    public ResponseEntity<List<ProductResponse>> getAvailableProducts() {
        List<ProductDTO> products = productService.getAllAvailableProducts();
        List<ProductResponse> responses = products.stream()
                .map(productWebMapper::toResponse)
                .collect(Collectors.toList());
        return ResponseEntity.ok(responses);
    }

    // 5. Удаление товара по ID
    @DeleteMapping("/{id}")
    @Operation(summary = "Удаление товара", responses = {
            @ApiResponse(responseCode = "204", description = "Товар удален"),
            @ApiResponse(responseCode = "404", description = "Товар не найден", content = @Content(schema = @Schema(implementation = ErrorResponse.class)))
    })
    public ResponseEntity<Void> deleteProduct(@PathVariable UUID id) {

        log.info("Deleting product with id: {}", id);
        productService.deleteById(id);
        log.info("Product deleted successfully: {}", id);
        return ResponseEntity.noContent().build();
    }

    // 6. Увеличение количества товара (дополнительно)
    @PatchMapping("/{id}/stock/increase")
    @Operation(summary = "Увеличение количества товара", responses = {
            @ApiResponse(responseCode = "204", description = "Количество увеличено"),
            @ApiResponse(responseCode = "400", description = "Неверные данные", content = @Content(schema = @Schema(implementation = ErrorResponse.class))),
            @ApiResponse(responseCode = "404", description = "Товар не найден", content = @Content(schema = @Schema(implementation = ErrorResponse.class)))
    })
    public ResponseEntity<Void> increaseProductStock(
            @PathVariable UUID id,
            @Valid @RequestBody UpdateStockRequest request) {

        log.info("Increasing stock for product id: {}, amount: {}", id, request.getAmount());
        productService.increaseStock(id, request.getAmount());
        log.info("Stock increased successfully for product: {}, amount: {}", id, request.getAmount());

        return ResponseEntity.noContent().build();
    }

    // 7. Получение текущего количества товара (опционально)
    @GetMapping("/{id}/stock")
    @Operation(summary = "Получение количества товара", responses = {
            @ApiResponse(responseCode = "200", description = "Количество товара"),
            @ApiResponse(responseCode = "404", description = "Товар не найден", content = @Content(schema = @Schema(implementation = ErrorResponse.class)))
    })
    public ResponseEntity<Integer> getProductStock(@PathVariable UUID id) {
        int stock = productService.getCurrentStock(id);
        return ResponseEntity.ok(stock);
    }
}