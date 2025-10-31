package s21.example.datasource.repository;

import org.springframework.data.repository.CrudRepository;
import org.springframework.lang.NonNull;
import s21.example.domain.model.User;

import java.util.Optional;
import java.util.UUID;

public interface UserRepository extends CrudRepository<User, UUID> {
    Optional<User> findByLogin(@NonNull String login);
    boolean existsById(@NonNull UUID id);
}