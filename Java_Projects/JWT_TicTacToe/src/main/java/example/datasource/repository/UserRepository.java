package example.datasource.repository;

import example.domain.model.User;
import org.springframework.data.repository.CrudRepository;
import org.springframework.lang.NonNull;

import java.util.Optional;
import java.util.UUID;

public interface UserRepository extends CrudRepository<User, UUID> {
    Optional<User> findByLogin(@NonNull String login);
    boolean existsById(@NonNull UUID id);
}