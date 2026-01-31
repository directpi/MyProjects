-- Тестовые данные для базы данных myShop_db
-- Запускается автоматически Spring Boot после schema.sql

-- ============================================================================
-- 1. АДРЕСА (addresses)
-- ============================================================================

-- Очистка существующих данных (если нужно)
-- DELETE FROM addresses;

-- Вставка тестовых адресов
INSERT INTO addresses (id, country, city, street) VALUES
                                                      -- Москва
                                                      ('11111111-1111-1111-1111-111111111111', 'Россия', 'Москва', 'ул. Тверская, д. 1'),
                                                      ('11111111-1111-1111-1111-111111111112', 'Россия', 'Москва', 'ул. Арбат, д. 25'),
                                                      ('11111111-1111-1111-1111-111111111113', 'Россия', 'Москва', 'пр. Мира, д. 15'),

                                                      -- Санкт-Петербург
                                                      ('22222222-2222-2222-2222-222222222221', 'Россия', 'Санкт-Петербург', 'Невский пр., д. 28'),
                                                      ('22222222-2222-2222-2222-222222222222', 'Россия', 'Санкт-Петербург', 'ул. Садовая, д. 12'),

                                                      -- Казань
                                                      ('33333333-3333-3333-3333-333333333331', 'Россия', 'Казань', 'ул. Баумана, д. 10'),
                                                      ('33333333-3333-3333-3333-333333333332', 'Россия', 'Казань', 'ул. Кремлевская, д. 5'),

                                                      -- Екатеринбург
                                                      ('44444444-4444-4444-4444-444444444441', 'Россия', 'Екатеринбург', 'ул. Ленина, д. 33'),

                                                      -- Новосибирск
                                                      ('55555555-5555-5555-5555-555555555551', 'Россия', 'Novosibirsk', 'ул. Красный проспект, д. 20'),

                                                      -- Поставщики
                                                      ('66666666-6666-6666-6666-666666666661', 'Россия', 'Москва', 'ул. Промышленная, д. 45'),
                                                      ('66666666-6666-6666-6666-666666666662', 'Китай', 'Шэньчжэнь', 'Electronics Street, 88'),
                                                      ('66666666-6666-6666-6666-666666666663', 'Германия', 'Берлин', 'Industriestr. 10')
    ON CONFLICT (id) DO NOTHING;

-- ============================================================================
-- 2. КЛИЕНТЫ (clients)
-- ============================================================================

-- DELETE FROM clients;

INSERT INTO clients (id, client_name, client_surname, birthday, gender, address_id) VALUES
                                                                                        -- Мужчины
                                                                                        ('aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaa1', 'Иван', 'Иванов', '1990-05-15', 'MALE', '11111111-1111-1111-1111-111111111111'),
                                                                                        ('aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaa2', 'Петр', 'Петров', '1985-08-22', 'MALE', '11111111-1111-1111-1111-111111111112'),
                                                                                        ('aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaa3', 'Алексей', 'Сидоров', '1995-11-30', 'MALE', '22222222-2222-2222-2222-222222222221'),
                                                                                        ('aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaa4', 'Дмитрий', 'Кузнецов', '1988-03-10', 'MALE', '33333333-3333-3333-3333-333333333331'),
                                                                                        ('aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaa5', 'Михаил', 'Попов', '1992-07-18', 'MALE', '44444444-4444-4444-4444-444444444441'),

                                                                                        -- Женщины
                                                                                        ('bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbb1', 'Анна', 'Иванова', '1993-02-14', 'FEMALE', '11111111-1111-1111-1111-111111111113'),
                                                                                        ('bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbb2', 'Елена', 'Петрова', '1987-09-25', 'FEMALE', '22222222-2222-2222-2222-222222222222'),
                                                                                        ('bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbb3', 'Ольга', 'Сидорова', '1991-12-05', 'FEMALE', '33333333-3333-3333-3333-333333333332'),
                                                                                        ('bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbb4', 'Мария', 'Кузнецова', '1994-06-20', 'FEMALE', '55555555-5555-5555-5555-555555555551'),
                                                                                        ('bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbb5', 'Наталья', 'Попова', '1989-04-12', 'FEMALE', '11111111-1111-1111-1111-111111111111')
    ON CONFLICT (id) DO NOTHING;

-- ============================================================================
-- 3. ПОСТАВЩИКИ (suppliers)
-- ============================================================================

-- Проверяем существует ли таблица suppliers перед вставкой
DO $$
BEGIN
    IF EXISTS (SELECT FROM information_schema.tables WHERE table_name = 'suppliers') THEN
        INSERT INTO suppliers (id, name, address_id, phone_number) VALUES
            ('cccccccc-cccc-cccc-cccc-ccccccccccc1', 'ООО "БытовойТехника"', '66666666-6666-6666-6666-666666666661', '+7-495-123-45-67'),
            ('cccccccc-cccc-cccc-cccc-ccccccccccc2', 'TechImport China', '66666666-6666-6666-6666-666666666662', '+86-755-8888-9999'),
            ('cccccccc-cccc-cccc-cccc-ccccccccccc3', 'German Electronics GmbH', '66666666-6666-6666-6666-666666666663', '+49-30-12345678')
        ON CONFLICT (id) DO NOTHING;
END IF;
END $$;

-- ============================================================================
-- 4. КАТЕГОРИИ (categories) - дополнительные если нужно
-- ============================================================================

-- Категории уже должны быть вставлены из schema.sql, добавляем если нет
INSERT INTO categories (id, code, display_name) VALUES
                                                    ('550e8400-e29b-41d4-a716-446655440006', 'TVS', 'Телевизоры'),
                                                    ('550e8400-e29b-41d4-a716-446655440007', 'AUDIO', 'Аудиотехника'),
                                                    ('550e8400-e29b-41d4-a716-446655440008', 'COMPUTERS', 'Компьютеры'),
                                                    ('550e8400-e29b-41d4-a716-446655440009', 'SMARTPHONES', 'Смартфоны'),
                                                    ('550e8400-e29b-41d4-a716-446655440010', 'TABLETS', 'Планшеты')
    ON CONFLICT (code) DO NOTHING;

-- ============================================================================
-- 5. ТОВАРЫ (products)
-- ============================================================================

-- DELETE FROM products;

INSERT INTO products (id, name, category_id, price, available_stock, last_update_date) VALUES
                                                                                           ('dddddddd-dddd-dddd-dddd-ddddddddddd1', 'Холодильник Samsung RB33J',
                                                                                            (SELECT id FROM categories WHERE code = 'REFRIGERATORS'),
                                                                                            45999.99, 15, '2024-01-15'),
                                                                                           ('dddddddd-dddd-dddd-dddd-ddddddddddd2', 'Холодильник LG GA-B459',
                                                                                            (SELECT id FROM categories WHERE code = 'REFRIGERATORS'),
                                                                                            52999.50, 8, '2024-01-10'),
                                                                                           ('dddddddd-dddd-dddd-dddd-ddddddddddd3', 'Холодильник Bosch KGN36',
                                                                                            (SELECT id FROM categories WHERE code = 'REFRIGERATORS'),
                                                                                            68999.00, 5, '2024-01-05'),
                                                                                           ('dddddddd-dddd-dddd-dddd-ddddddddddd4', 'Холодильник Indesit ITS',
                                                                                            (SELECT id FROM categories WHERE code = 'REFRIGERATORS'),
                                                                                            32999.99, 20, '2024-01-18'),

                                                                                           ('dddddddd-dddd-dddd-dddd-ddddddddddd5', 'Плита газовая Gorenje',
                                                                                            (SELECT id FROM categories WHERE code = 'OVENS'),
                                                                                            18999.50, 12, '2024-01-12'),
                                                                                           ('dddddddd-dddd-dddd-dddd-ddddddddddd6', 'Плита электрическая Bosch',
                                                                                            (SELECT id FROM categories WHERE code = 'OVENS'),
                                                                                            27999.99, 7, '2024-01-08'),

                                                                                           ('dddddddd-dddd-dddd-dddd-ddddddddddd7', 'Стиральная машина LG F2J3',
                                                                                            (SELECT id FROM categories WHERE code = 'WASHING_MACHINES'),
                                                                                            34999.99, 10, '2024-01-14'),
                                                                                           ('dddddddd-dddd-dddd-dddd-ddddddddddd8', 'Стиральная машина Samsung WW80',
                                                                                            (SELECT id FROM categories WHERE code = 'WASHING_MACHINES'),
                                                                                            41999.50, 6, '2024-01-09'),
                                                                                           ('dddddddd-dddd-dddd-dddd-ddddddddddd9', 'Стиральная машина Indesit IWSC',
                                                                                            (SELECT id FROM categories WHERE code = 'WASHING_MACHINES'),
                                                                                            22999.99, 18, '2024-01-16'),

                                                                                           ('dddddddd-dddd-dddd-dddd-dddddddddd10', 'Посудомоечная машина Bosch SMS',
                                                                                            (SELECT id FROM categories WHERE code = 'DISHWASHERS'),
                                                                                            38999.99, 9, '2024-01-11'),

                                                                                           ('dddddddd-dddd-dddd-dddd-dddddddddd11', 'Микроволновка Samsung ME83',
                                                                                            (SELECT id FROM categories WHERE code = 'MICROWAVES'),
                                                                                            8999.99, 25, '2024-01-13'),
                                                                                           ('dddddddd-dddd-dddd-dddd-dddddddddd12', 'Микроволновка LG MS2042',
                                                                                            (SELECT id FROM categories WHERE code = 'MICROWAVES'),
                                                                                            10999.50, 15, '2024-01-07'),

                                                                                           ('dddddddd-dddd-dddd-dddd-dddddddddd13', 'Телевизор Samsung QLED 55"',
                                                                                            (SELECT id FROM categories WHERE code = 'TVS'),
                                                                                            89999.99, 4, '2024-01-06'),

                                                                                           ('dddddddd-dddd-dddd-dddd-dddddddddd14', 'Ноутбук ASUS VivoBook',
                                                                                            (SELECT id FROM categories WHERE code = 'COMPUTERS'),
                                                                                            54999.99, 8, '2024-01-04'),

                                                                                           ('dddddddd-dddd-dddd-dddd-dddddddddd15', 'Смартфон iPhone 15',
                                                                                            (SELECT id FROM categories WHERE code = 'SMARTPHONES'),
                                                                                            99999.99, 3, '2024-01-03')
    ON CONFLICT (id) DO NOTHING;

-- ============================================================================
-- 6. ИЗОБРАЖЕНИЯ (images) - РЕАЛЬНЫЕ ДАННЫЕ ДЛЯ ТЕСТИРОВАНИЯ
-- ============================================================================

DO $$
BEGIN
    IF EXISTS (SELECT FROM information_schema.tables WHERE table_name = 'images') THEN
        -- Создаем 6 разных тестовых изображений (симулируем реальные файлы)
        -- Каждое изображение - это минимальный валидный PNG (1x1 пиксель, черный)
        -- В реальном приложении здесь будут реальные байты из файлов

        INSERT INTO images (id, image_data) VALUES
            -- Холодильники
            ('eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee1',
             decode('89504E470D0A1A0A0000000D4948445200000001000000010100000000376EF9240000000A4944415478DA636400000000020000010005E5E5B80000000049454E44AE426082', 'hex')),
            ('eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee2',
             decode('89504E470D0A1A0A0000000D49484452000000010000000108060000001F15C4890000000A4944415478DA636400000000020000010005E5E5B80000000049454E44AE426082', 'hex')),

            -- Плиты
            ('eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee3',
             decode('89504E470D0A1A0A0000000D4948445200000001000000010802000000907753DE0000000A4944415478DA636400000000020000010005E5E5B80000000049454E44AE426082', 'hex')),

            -- Стиральные машины
            ('eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee4',
             decode('89504E470D0A1A0A0000000D4948445200000001000000010804000000B5FA37EA0000000A4944415478DA636400000000020000010005E5E5B80000000049454E44AE426082', 'hex')),
            ('eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee5',
             decode('89504E470D0A1A0A0000000D49484452000000010000000108060000001F15C4890000000A4944415478DA636400000000020000010005E5E5B80000000049454E44AE426082', 'hex')),

            -- Микроволновки
            ('eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee6',
             decode('89504E470D0A1A0A0000000D4948445200000001000000010802000000907753DE0000000A4944415478DA636400000000020000010005E5E5B80000000049454E44AE426082', 'hex')),

            -- Телевизоры
            ('eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee7',
             decode('89504E470D0A1A0A0000000D4948445200000001000000010804000000B5FA37EA0000000A4944415478DA636400000000020000010005E5E5B80000000049454E44AE426082', 'hex')),

            -- Ноутбуки
            ('eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee8',
             decode('89504E470D0A1A0A0000000D49484452000000010000000108060000001F15C4890000000A4944415478DA636400000000020000010005E5E5B80000000049454E44AE426082', 'hex')),

            -- Смартфоны
            ('eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee9',
             decode('89504E470D0A1A0A0000000D4948445200000001000000010802000000907753DE0000000A4944415478DA636400000000020000010005E5E5B80000000049454E44AE426082', 'hex')),

            -- Пылесосы (если есть такие товары)
            ('eeeeeeee-eeee-eeee-eeee-eeeeeeeeee10',
             decode('89504E470D0A1A0A0000000D4948445200000001000000010804000000B5FA37EA0000000A4944415478DA636400000000020000010005E5E5B80000000049454E44AE426082', 'hex'))
        ON CONFLICT (id) DO NOTHING;

        RAISE NOTICE '✅ Создано 10 тестовых изображений';
END IF;
END $$;

-- ============================================================================
-- 7. СВЯЗИ ТОВАРОВ С ИЗОБРАЖЕНИЯМИ И ПОСТАВЩИКАМИ
-- ============================================================================

DO $$
BEGIN
    -- Привязываем изображения к товарам
    IF EXISTS (SELECT FROM information_schema.tables WHERE table_name = 'images') THEN
        -- Холодильники получают изображения
UPDATE products SET image_id = 'eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee1' WHERE id = 'dddddddd-dddd-dddd-dddd-ddddddddddd1';
UPDATE products SET image_id = 'eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee2' WHERE id = 'dddddddd-dddd-dddd-dddd-ddddddddddd2';
UPDATE products SET image_id = 'eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee1' WHERE id = 'dddddddd-dddd-dddd-dddd-ddddddddddd3';
UPDATE products SET image_id = 'eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee2' WHERE id = 'dddddddd-dddd-dddd-dddd-ddddddddddd4';

-- Плиты
UPDATE products SET image_id = 'eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee3' WHERE id = 'dddddddd-dddd-dddd-dddd-ddddddddddd5';
UPDATE products SET image_id = 'eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee3' WHERE id = 'dddddddd-dddd-dddd-dddd-ddddddddddd6';

-- Стиральные машины
UPDATE products SET image_id = 'eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee4' WHERE id = 'dddddddd-dddd-dddd-dddd-ddddddddddd7';
UPDATE products SET image_id = 'eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee5' WHERE id = 'dddddddd-dddd-dddd-dddd-ddddddddddd8';
UPDATE products SET image_id = 'eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee4' WHERE id = 'dddddddd-dddd-dddd-dddd-ddddddddddd9';

-- Посудомоечные машины
UPDATE products SET image_id = 'eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee5' WHERE id = 'dddddddd-dddd-dddd-dddd-dddddddddd10';

-- Микроволновки
UPDATE products SET image_id = 'eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee6' WHERE id = 'dddddddd-dddd-dddd-dddd-dddddddddd11';
UPDATE products SET image_id = 'eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee6' WHERE id = 'dddddddd-dddd-dddd-dddd-dddddddddd12';

-- Телевизоры
UPDATE products SET image_id = 'eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee7' WHERE id = 'dddddddd-dddd-dddd-dddd-dddddddddd13';

-- Ноутбуки
UPDATE products SET image_id = 'eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee8' WHERE id = 'dddddddd-dddd-dddd-dddd-dddddddddd14';

-- Смартфоны
UPDATE products SET image_id = 'eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee9' WHERE id = 'dddddddd-dddd-dddd-dddd-dddddddddd15';

RAISE NOTICE '✅ Изображения привязаны к товарам';
END IF;

    -- Привязываем поставщиков к товарам
    IF EXISTS (SELECT FROM information_schema.tables WHERE table_name = 'suppliers') THEN
        -- Российский поставщик
UPDATE products SET supplier_id = 'cccccccc-cccc-cccc-cccc-ccccccccccc1'
WHERE id IN (
             'dddddddd-dddd-dddd-dddd-ddddddddddd1',  -- Холодильник Samsung
             'dddddddd-dddd-dddd-dddd-ddddddddddd4',  -- Холодильник Indesit
             'dddddddd-dddd-dddd-dddd-ddddddddddd5',  -- Плита газовая
             'dddddddd-dddd-dddd-dddd-ddddddddddd9'   -- Стиральная машина Indesit
    );

-- Китайский поставщик
UPDATE products SET supplier_id = 'cccccccc-cccc-cccc-cccc-ccccccccccc2'
WHERE id IN (
             'dddddddd-dddd-dddd-dddd-ddddddddddd2',  -- Холодильник LG
             'dddddddd-dddd-dddd-dddd-ddddddddddd7',  -- Стиральная машина LG
             'dddddddd-dddd-dddd-dddd-dddddddddd12',  -- Микроволновка LG
             'dddddddd-dddd-dddd-dddd-dddddddddd14',  -- Ноутбук ASUS
             'dddddddd-dddd-dddd-dddd-dddddddddd15'   -- iPhone
    );

-- Немецкий поставщик
UPDATE products SET supplier_id = 'cccccccc-cccc-cccc-cccc-ccccccccccc3'
WHERE id IN (
             'dddddddd-dddd-dddd-dddd-ddddddddddd3',  -- Холодильник Bosch
             'dddddddd-dddd-dddd-dddd-ddddddddddd6',  -- Плита электрическая Bosch
             'dddddddd-dddd-dddd-dddd-dddddddddd10',  -- Посудомоечная машина Bosch
             'dddddddd-dddd-dddd-dddd-dddddddddd13'   -- Телевизор Samsung
    );

-- Для остальных товаров (если есть) ставим поставщика по умолчанию
UPDATE products SET supplier_id = 'cccccccc-cccc-cccc-cccc-ccccccccccc1'
WHERE supplier_id IS NULL;

RAISE NOTICE '✅ Поставщики привязаны к товарам';
END IF;
END $$;

-- ============================================================================
-- 8. ИСПРАВЛЕНИЕ ОШИБКИ В ТОВАРЕ №13 (неправильная категория)
-- ============================================================================

-- В исходном коде товар №13 был привязан к категории VACUUM_CLEANERS вместо TVS
-- Исправляем это
UPDATE products
SET category_id = (SELECT id FROM categories WHERE code = 'TVS')
WHERE id = 'dddddddd-dddd-dddd-dddd-dddddddddd13'
  AND category_id = (SELECT id FROM categories WHERE code = 'VACUUM_CLEANERS');

-- ============================================================================
-- 9. ВЫВОД СТАТИСТИКИ
-- ============================================================================

DO $$
DECLARE
addr_count INTEGER;
    client_count INTEGER;
    product_count INTEGER;
    category_count INTEGER;
    image_count INTEGER;
    products_with_images INTEGER;
    products_with_suppliers INTEGER;
BEGIN
SELECT COUNT(*) INTO addr_count FROM addresses;
SELECT COUNT(*) INTO client_count FROM clients;
SELECT COUNT(*) INTO product_count FROM products;
SELECT COUNT(*) INTO category_count FROM categories;
SELECT COUNT(*) INTO image_count FROM images;
SELECT COUNT(*) INTO products_with_images FROM products WHERE image_id IS NOT NULL;
SELECT COUNT(*) INTO products_with_suppliers FROM products WHERE supplier_id IS NOT NULL;

RAISE NOTICE '✅ Тестовые данные загружены:';
    RAISE NOTICE '   Адреса: %', addr_count;
    RAISE NOTICE '   Клиенты: %', client_count;
    RAISE NOTICE '   Товары: %', product_count;
    RAISE NOTICE '   Категории: %', category_count;
    RAISE NOTICE '   Изображения: %', image_count;
    RAISE NOTICE '   Товаров с изображениями: %', products_with_images;
    RAISE NOTICE '   Товаров с поставщиками: %', products_with_suppliers;

    -- Дополнительная проверка
    IF products_with_images = 0 THEN
        RAISE WARNING '⚠️  ВНИМАНИЕ: Ни один товар не имеет изображения!';
    ELSIF products_with_images < product_count THEN
        RAISE NOTICE 'ℹ️   Некоторые товары (% из %) не имеют изображений',
                     product_count - products_with_images, product_count;
END IF;

    IF products_with_suppliers = 0 THEN
        RAISE WARNING '⚠️  ВНИМАНИЕ: Ни один товар не имеет поставщика!';
END IF;
END $$;