package example.domain.service;

import example.datasource.model.AddressDAO;
import example.datasource.model.SupplierDAO;
import example.datasource.repository.AddressRepository;
import example.domain.exception.EntityNotFoundException;
import example.domain.mapper.AddressMapper;
import example.domain.model.AddressDTO;
import example.domain.model.SupplierDTO;
import example.domain.mapper.SupplierMapper;
import example.datasource.repository.SupplierRepository;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import example.domain.exception.ValidationException;

import java.util.List;
import java.util.Optional;
import java.util.UUID;
import java.util.stream.Collectors;

@Slf4j
@Service
public class SupplierService {
    private final SupplierRepository supplierRepository;
    private final SupplierMapper supplierMapper;
    private final AddressRepository addressRepository;
    private final AddressMapper addressMapper;

    @Autowired
    public SupplierService(SupplierRepository supplierRepository, SupplierMapper supplierMapper, AddressRepository addressRepository, AddressMapper addressMapper) {
        this.supplierRepository = supplierRepository;
        this.supplierMapper = supplierMapper;
        this.addressRepository = addressRepository;
        this.addressMapper = addressMapper;
    }


    // ============ PUBLIC METHODS ============
    // 1. Создать поставщика
    @Transactional
    public SupplierDTO createSupplier(SupplierDTO supplierDTO){
        try {
            log.debug("Попытка создать поставщика: {}",
                    supplierDTO.getName());
            // Валидация
            validateSupplier(supplierDTO);
            validatePhoneNumber(supplierDTO.getPhoneNumber());

            // Проверка существования адреса (по ТЗ адрес должен существовать)
            validateAddressExists(supplierDTO.getAddressId());

            // Маппинг DTO → DAO
            SupplierDAO supplierDAO = supplierMapper.toDAO(supplierDTO);

            // Сохранение в БД
            SupplierDAO  savedDAO = supplierRepository.save(supplierDAO);

            // Маппинг обратно DAO → DTO
            SupplierDTO created = supplierMapper.toDTO(savedDAO);
            log.info("Поставщик создан успешно: {} {} {} with id: {}",
                    created.getName(), created.getAddressId(), created.getPhoneNumber(), created.getId());
            return created;
        } catch (Exception e) {
            log.error("Не удалось создать поставщика: {}", e.getMessage(), e);
            throw new RuntimeException("Не удалось создать поставщика: " + e.getMessage(), e);
        }
    }

    // 2. Поиск по Id
    public Optional<SupplierDTO> findById (UUID id){
        if (id == null) {
            throw new ValidationException("ID поставщика не должен быть null");
        }
        return supplierRepository.findById(id).map(supplierMapper::toDTO);
    }

    public SupplierDTO getById(UUID id){

        return findById(id).orElseThrow(() -> new EntityNotFoundException("Поставщик", id));
    }

    // 3. Получить всех
    public List<SupplierDTO> findAll (){

        return supplierRepository.findAll()
                .stream()
                .map(supplierMapper::toDTO)
                .collect(Collectors.toList());
    }

    // 4. Удаление поставщика по идентификатору
    @Transactional
    public void deleteById (UUID id){
        log.debug("Попытка удалить поставщика: {}", id);
        if (id == null) {
            throw new ValidationException("ID поставщика не должен быть null");
        }

        getById(id); // Проверяет существование

        // Удаляем поставщика (адрес остается в БД)
        supplierRepository.deleteById(id);
        log.info("Поставщик удален: {}", id);
    }

    // Изменение адреса поставщика
    @Transactional
    public void updateSupplierAddress(UUID supplierId, AddressDTO newAddressDTO){

        log.debug("Попытка обновить поставщика: {}", supplierId);
        // Валидация
        if (supplierId == null){
            throw new ValidationException("ID поставщика не должен быть null");
        }
        if (newAddressDTO == null) {
            throw new ValidationException("Адрес поставщика не должен быть null");
        }

        //Проверяем, что клиент существует
        SupplierDTO supplier = getById(supplierId); // Бросит EntityNotFoundException если нет

        // Валидируем данные нового адреса
        validateAddressData(newAddressDTO);

        // Получаем текущий адрес поставщика для логирования
        UUID oldAddressId = supplier.getAddressId();
        String oldAddressInfo = getAddressInfo(oldAddressId);

        // ВСЕГДА создаем новый адрес (игнорируем переданный ID)
        AddressDAO newAddress = new AddressDAO();
        newAddress.setCountry(newAddressDTO.getCountry());
        newAddress.setCity(newAddressDTO.getCity());
        newAddress.setStreet(newAddressDTO.getStreet());
        // ID явно устанавливаем в null, чтобы создать новую запись
        newAddress.setId(null);

        AddressDAO savedAddress = addressRepository.save(newAddress);
        UUID addressIdToSet = savedAddress.getId();

        // Меняем адрес у поставщика
        boolean updated = supplierRepository.updateSupplierAddress(supplierId, addressIdToSet);

        if (!updated) {
            // Это маловероятно, так как getById() уже проверил существование
            throw new EntityNotFoundException("Поставщик", supplierId);
        }
        // Логируем изменения
        log.info("Адрес обновлен. Поставщик: {}, ID: {}",
                supplier.getName(), supplierId);
        log.info("Старый адрес ID: {} - {}", oldAddressId, oldAddressInfo);
        log.info("Новый адрес ID: {} - {}, {}, {}",
                addressIdToSet,
                newAddressDTO.getCountry(),
                newAddressDTO.getCity(),
                newAddressDTO.getStreet());
    }

    // ============ PRIVATE HELPER METHODS ============

    /**
     * Проверяет существование адреса по ID
     * Нужен только для createSupplier
     */
    private void validateAddressExists(UUID addressId) {
        if (!addressRepository.existsById(addressId)) {
            throw new EntityNotFoundException("Адрес", addressId);
        }
    }

    private void validateSupplier(SupplierDTO supplierDTO) {

        // Проверка на null
        if (supplierDTO == null) {
            throw new ValidationException("Данные поставщика не должны быть null");
        }

        // Валидация имени
        validateName(supplierDTO.getName());

        // Адрес обязателен
        if (supplierDTO.getAddressId() == null) {
            throw new ValidationException("ID адресе не должен быть null");
        }
    }

    private void validateName(String name){
        if (name == null || name.trim().isEmpty()){
            throw new ValidationException("Имя не должно быть пустым");
        }
        if (name.length() < 2) {
            throw  new ValidationException("Имя должно состоять как минимум из 2 символов");
        }
        if (name.length() > 100) {
            throw  new ValidationException("Имя должно состоять не более чем из 100 символов");
        }
    }

    private void validatePhoneNumber(String phoneNumber) {
        if (phoneNumber == null || phoneNumber.trim().isEmpty()) {
            throw new ValidationException("Номер телефона не должен быть пустым");
        }

        // Реалистичные лимиты
        if (phoneNumber.length() < 7) {
            throw new ValidationException("Номер телефона слишком короткий (min 7 знаков)");
        }

        if (phoneNumber.length() > 25) {
            throw new ValidationException("Номер телефона слишком длинный (max 25 знаков)");
        }


        String phoneRegex = "^\\+?[0-9\\s\\-\\(\\)]+$";
        if (!phoneNumber.matches(phoneRegex)) {
            throw new ValidationException(
                    "Неправильный формат номер. Пример: +7(999)123-45-67, 89991234567, (999) 123-45-67"
            );
        }

        // Дополнительная проверка - должен содержать хотя бы 5 цифр
        int digitCount = phoneNumber.replaceAll("[^0-9]", "").length();
        if (digitCount < 5) {
            throw new ValidationException("Номер телефона должен состоять как минимум из 5 цифр");
        }
    }

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
}
