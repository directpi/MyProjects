package example.datasource.repository;

import example.datasource.model.ClientDAO;

import java.util.List;
import java.util.Optional;
import java.util.UUID;

public interface ClientRepository {

    // Добавить клиента
    ClientDAO save(ClientDAO client);

    // Удалить клиента по id
    boolean deleteById(UUID id);

    // Найти по имени и фамилии
    List<ClientDAO> findByNameAndSurname(String name, String surname);

    // Найти по id
    Optional<ClientDAO> findById(UUID id);

    //Получить всех (с пагинацией)
    List<ClientDAO> findAll();
    List<ClientDAO> findAll(int limit, int offset);

    // Обновить адрес клиента
    boolean updateClientAddress(UUID clientId, UUID newAddressId);
}
