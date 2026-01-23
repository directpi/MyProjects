package example.datasource.repository;

import example.datasource.model.AddressDAO;

import java.util.Optional;
import java.util.UUID;

public interface AddressRepository {

    // CRUD операции
    AddressDAO save(AddressDAO address);
    Optional<AddressDAO> findById(UUID id);
    void deleteById(UUID id);

    // проверка существования
    boolean existsById(UUID id);
}
