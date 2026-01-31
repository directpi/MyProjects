package example.domain.service;

import example.datasource.model.AddressDAO;
import example.datasource.model.ClientDAO;
import example.datasource.repository.AddressRepository;
import example.domain.exception.*;
import example.domain.mapper.AddressMapper;
import example.domain.model.AddressDTO;
import example.domain.model.ClientDTO;
import example.domain.mapper.ClientMapper;
import example.datasource.repository.ClientRepository;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDate;
import java.util.List;
import java.util.Optional;
import java.util.UUID;
import java.util.stream.Collectors;

@Slf4j
@Service
public class ClientService {
    private final ClientRepository clientRepository;
    private final ClientMapper clientMapper;
    private final AddressRepository addressRepository;
    private final AddressMapper addressMapper;

    @Autowired
    public ClientService(ClientRepository clientRepository, ClientMapper clientMapper, AddressRepository addressRepository, AddressMapper addressMapper){
        this.clientRepository = clientRepository;
        this.clientMapper = clientMapper;
        this.addressRepository = addressRepository;
        this.addressMapper = addressMapper;
    }

    // ============ PUBLIC METHODS ============
    // 1. Создать клиента
    @Transactional
    public ClientDTO createClient(ClientDTO clientDTO){
        try {
            log.debug("Попытка создать клиента: {} {}",
                    clientDTO.getFirstName(), clientDTO.getLastName());
            // Валидация
            validateClient(clientDTO);

            // Проверка существования адреса (по ТЗ адрес должен существовать)
            validateAddressExists(clientDTO.getAddressId());

            // Маппинг DTO → DAO
            ClientDAO  clientDAO = clientMapper.toDAO(clientDTO);

            // Сохранение в БД
            ClientDAO  savedDAO = clientRepository.save(clientDAO);

            // Маппинг обратно DAO → DTO
            ClientDTO created = clientMapper.toDTO(savedDAO);
            log.info("Клиент создан успешно: {} {} с id: {}",
                    created.getFirstName(), created.getLastName(), created.getId());
            return created;
        } catch (Exception e) {
            log.error("Не удалось создать клиента: {}", e.getMessage(), e);
            throw new CreateEntityException("клиента:", e);
        }
    }

    // 2. Поиск по Id
    public Optional<ClientDTO> findById (UUID id){
        if (id == null) {
            throw new ValidationException("ID клиента не может быть null");
        }
        return clientRepository.findById(id).map(clientMapper::toDTO);
    }

    public ClientDTO getById(UUID id){

        return findById(id).orElseThrow(() -> new EntityNotFoundException("Клиент", id));
    }

    // 3. Поиск по имени и фамилии
    public List<ClientDTO> findByNameAndSurname (String name, String surname){
        if (name == null || name.trim().isEmpty()){
            throw new ValidationException("Поле Имя не может быть пустым");
        }
        if (surname == null || surname.trim().isEmpty()) {
            throw new ValidationException("Поле Фамилия не может быть пустым");
        }
        return clientRepository.findByNameAndSurname(name, surname).stream().map(clientMapper::toDTO).collect(Collectors.toList());
    }

    // 4. Получить всех
    public List<ClientDTO> findAll (){

        return clientRepository.findAll()
                .stream()
                .map(clientMapper::toDTO)
                .collect(Collectors.toList());
    }
    // 5. Получить всех с пагинацией
    public List<ClientDTO> findAllPaginated(int limit, int offset){
        if (limit <= 0) {
            throw new ValidationException("Лимит должен быть положительным числом");
        }
        if (offset < 0) {
            throw new ValidationException("Смещение не может быть отрицательным");
        }

        log.debug("Получение клиентов с пагинацией: limit={}, offset={}", limit, offset);

        return clientRepository.findAll(limit, offset)
                .stream()
                .map(clientMapper::toDTO)
                .collect(Collectors.toList());
    }

    // 6. Удаление клиента по идентификатору
    @Transactional
    public void deleteById (UUID id){
        log.debug("Попытка удалить клиента: {}", id);
        if (id == null) {
            throw new ValidationException("ID клиента не может быть null");
        }

        getById(id); // Проверяет существование

        // Удаляем клиента (адрес остается в БД)
        clientRepository.deleteById(id);
        log.info("Клиент удален: {}", id);
    }

    // 7. Изменение адреса клиента
    @Transactional
    public void updateClientAddress(UUID clientId, AddressDTO newAddressDTO){

        log.debug("Попытка обновить адрес для клиента: {}", clientId);

        // Валидация
        if (clientId == null){
            throw new ValidationException("ID клиента не может быть null");
        }
        if (newAddressDTO == null) {
            throw new ValidationException("Адрес не может быть null");
        }

        //Проверяем, что клиент существует
        ClientDTO client = getById(clientId); // Бросит EntityNotFoundException если нет

        // Валидируем данные нового адреса
        validateAddressData(newAddressDTO);

        // Получаем текущий адрес клиента для логирования
        UUID oldAddressId = client.getAddressId();
        String oldAddressInfo = getAddressInfo(oldAddressId);

        // ВСЕГДА создаем новый адрес (игнорируем переданный ID)
        AddressDAO newAddress = new AddressDAO();
        newAddress.setCountry(newAddressDTO.getCountry());
        newAddress.setCity(newAddressDTO.getCity());
        newAddress.setStreet(newAddressDTO.getStreet());

        AddressDAO savedAddress = addressRepository.save(newAddress);
        UUID addressIdToSet = savedAddress.getId();

        // Меняем адрес у клиента
        boolean updated = clientRepository.updateClientAddress(clientId, addressIdToSet);

        if (!updated) {
            // Это маловероятно, так как getById() уже проверил существование
            throw new EntityNotFoundException("Клиент", clientId);
        }

        // Логируем изменения
        log.info("Адрес обновлен. Клиент: {} {}, ID: {}",
                client.getFirstName(), client.getLastName(), clientId);
        log.info("Старый адрес ID: {} - {}", oldAddressId, oldAddressInfo);
        log.info("Новый адрес ID: {} - {}, {}, {}",
                addressIdToSet,
                newAddressDTO.getCountry(),
                newAddressDTO.getCity(),
                newAddressDTO.getStreet());
    }


    // ============ PRIVATE HELPER METHODS ============
    // Вспомогательный метод для получения информации об адресе
    private String getAddressInfo(UUID addressId) {
        if (addressId == null) {
            return "нет адреса";
        }
        return addressRepository.findById(addressId)
                .map(addr -> String.format("%s, %s, %s",
                        addr.getCountry(), addr.getCity(), addr.getStreet()))
                .orElse("адрес не найден");
    }
    // Валидация данных адреса
    private void validateAddressData(AddressDTO addressDTO) {
        if (addressDTO.getCountry() == null || addressDTO.getCountry().trim().isEmpty()) {
            throw new ValidationException("Страна не может быть пустой");
        }
        if (addressDTO.getCity() == null || addressDTO.getCity().trim().isEmpty()) {
            throw new ValidationException("Город не может быть пустым");
        }
        if (addressDTO.getStreet() == null || addressDTO.getStreet().trim().isEmpty()) {
            throw new ValidationException("Улица не может быть пустой");
        }

        // Ограничения по длине
        if (addressDTO.getCountry().length() > 100) {
            throw new ValidationException("Название страны слишком длинное (max 100 символов)");
        }
        if (addressDTO.getCity().length() > 100) {
            throw new ValidationException("Название города слишком длинное (max 100 символов)");
        }
        if (addressDTO.getStreet().length() > 100) {
            throw new ValidationException("Название улицы слишком длинное (max 100 символов)");
        }
    }


    /**
     * Проверяет существование адреса по ID
     * Нужен только для createClient
     */
    private void validateAddressExists(UUID addressId) {
        if (!addressRepository.existsById(addressId)) {
            throw new EntityNotFoundException("Адрес",addressId);
        }
    }

    private void validateClient(ClientDTO clientDTO) {

        // Проверка на null
        if (clientDTO == null) {
            throw new ValidationException("Данные клиента не могут быть null");
        }

        // Валидация имени
        validateName(clientDTO.getFirstName(), "Имя");
        validateName(clientDTO.getLastName(), "Фамилия");

        // Валидация даты рождения
        validateBirthDate(clientDTO.getBirthDate());

        // Адрес обязателен
        if (clientDTO.getAddressId() == null) {
            throw new ValidationException("ID адреса не может быть null");
        }

        // Валидация gender
        validateGender(clientDTO.getGender());
    }

    private void validateName(String name, String fieldName){
        if (name == null || name.trim().isEmpty()){
            throw new ValidationException(fieldName + " не может быть пустым");
        }
        if (name.length() < 2) {
            throw  new ValidationException(fieldName +  " должно состоять минимум из 2-х символов");
        }
        if (name.length() > 100) {
            throw  new ValidationException(fieldName +  " должно состоять не более чем из 100 символов");
        }
    }

    private void validateBirthDate (LocalDate birthDate){
        if (birthDate == null) {
            throw new ValidationException("Дата рождения не может быть null");
        }
        if (birthDate.isAfter(LocalDate.now())){
            throw new ValidationException("Дата рождения не может быть в будущем");
        }
        if (birthDate.isBefore(LocalDate.of(1900, 1, 1))){
            throw new ValidationException("Дата рождения должна быть позднее даты 1900-01-01");
        }
    }

    private void validateGender(String gender) {
        if (gender == null || gender.trim().isEmpty()) {
            throw new ValidationException("Пол не может быть пустым");
        }
        if (!"MALE".equalsIgnoreCase(gender) && !"FEMALE".equalsIgnoreCase(gender)) {
            throw new ValidationException("Пол может быть только MALE или FEMALE");
        }
    }
}
