package example.datasource.repository;

import example.datasource.model.SupplierDAO;
import example.datasource.model.SupplierDAO;

import java.util.List;
import java.util.Optional;
import java.util.UUID;

public interface SupplierRepository {
    // Добавить поставщика
    SupplierDAO save(SupplierDAO supplier);

    // Обновить адрес клиента
    boolean updateSupplierAddress(UUID supplierId, UUID newAddressId);

    // Удалить поставщика по id
    boolean deleteById(UUID id);

    //Получить всех
    List<SupplierDAO> findAll();

    // Найти по id
    Optional<SupplierDAO> findById(UUID id);
}
