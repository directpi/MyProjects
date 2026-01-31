-- Подключаем расширение pgcrypto для генерации UUID
CREATE EXTENSION IF NOT EXISTS "pgcrypto";

-- Создаем таблицу addresses (базовая, на неё ссылаются все)
CREATE TABLE IF NOT EXISTS addresses(
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    country VARCHAR(100) NOT NULL,
    city VARCHAR(100) NOT NULL,
    street VARCHAR(100) NOT NULL
);
-- Создаем таблицу для категорий
CREATE TABLE IF NOT EXISTS categories(
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    code VARCHAR(50) NOT NULL UNIQUE,        -- 'REFRIGERATORS'
    display_name VARCHAR(100) NOT NULL      -- 'Холодильники'
    );

-- Создаем таблицу suppliers (ссылается на addresses)
CREATE TABLE IF NOT EXISTS suppliers (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    name VARCHAR(100) NOT NULL,
    address_id UUID NOT NULL,
    phone_number VARCHAR(25) NOT NULL
    CHECK (phone_number ~ '^\+?[0-9\s\-\(\)]+$'),
    -- Внешний ключ для связи с адресом
    FOREIGN KEY (address_id) REFERENCES addresses(id)
    ON DELETE RESTRICT   -- Запрещает удаление, если есть ссылки
    ON UPDATE CASCADE    -- При изменении id адреса обновляем у поставщика

    );

-- Создаем таблицу images
CREATE TABLE IF NOT EXISTS images(
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    image_data bytea NOT NULL
    );

-- Создаем таблицу clients (ссылается на addresses)
CREATE TABLE IF NOT EXISTS clients (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    client_name VARCHAR(100) NOT NULL,
    client_surname VARCHAR(100) NOT NULL,
    birthday DATE NOT NULL
    CHECK (birthday <= CURRENT_DATE AND birthday >= '1900-01-01'),
    gender VARCHAR(10) NOT NULL CHECK (gender IN ('MALE', 'FEMALE')),
    registration_date DATE NOT NULL DEFAULT CURRENT_DATE,
    address_id UUID NOT NULL,

    -- Внешний ключ для связи с адресом
    FOREIGN KEY (address_id) REFERENCES addresses(id)
        ON DELETE RESTRICT   -- Запрещает удаление, если есть ссылки
        ON UPDATE CASCADE    -- При изменении id адреса обновляем у клиента
);

-- Создаем таблицу products (ссылается на categories, suppliers, images)
CREATE TABLE IF NOT EXISTS products (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    name VARCHAR(100) NOT NULL,
    category_id UUID NOT NULL,
    price DECIMAL(10, 2) NOT NULL CHECK (price >= 0),
    available_stock INT NOT NULL  DEFAULT 0, -- число закупленных экземпляров товара
    last_update_date DATE NOT NULL DEFAULT CURRENT_DATE
        CHECK (last_update_date <= CURRENT_DATE AND last_update_date >= '2000-01-01'),-- число последней закупки
    supplier_id UUID NULL,
    image_id UUID NULL,

    -- Внешний ключ для связи с поставщиком
    FOREIGN KEY (supplier_id) REFERENCES suppliers(id)
        ON DELETE SET NULL   -- При удалении адреса ставим NULL у товара
        ON UPDATE CASCADE,

    -- Внешний ключ для связи с изображением
    FOREIGN KEY (image_id) REFERENCES images(id)
        ON DELETE SET NULL   -- При удалении картинки ставим NULL у товара
        ON UPDATE CASCADE,

    -- Внешний ключ для связи с категориями
    FOREIGN KEY (category_id) REFERENCES categories(id)
        ON DELETE RESTRICT   -- Запрещает удаление категории, если есть товары
        ON UPDATE CASCADE    -- При изменении id категории обновляем у товара
);

INSERT INTO categories (code, display_name) VALUES
                                      ('REFRIGERATORS', 'Холодильники'),
                                      ('OVENS', 'Плиты'),
                                      ('WASHING_MACHINES', 'Стиральные машины'),
                                      ('DISHWASHERS', 'Посудомоечные машины'),
                                      ('MICROWAVES', 'Микроволновые печи'),
                                      ('VACUUM_CLEANERS', 'Пылесосы'),
                                      ('IRONS', 'Утюги'),
                                      ('BLENDERS', 'Блендеры'),
                                      ('COFFEE_MAKERS', 'Кофеварки')
    ON CONFLICT (code) DO NOTHING;

-- Индексы для производительности
-- CREATE INDEX IF NOT EXISTS idx_clients_address ON clients(address_id);
-- CREATE INDEX IF NOT EXISTS idx_suppliers_address_id ON suppliers(address_id);

-- CREATE INDEX IF NOT EXISTS idx_products_supplier ON products(supplier_id);
-- CREATE INDEX IF NOT EXISTS idx_products_image ON products(image_id);
-- CREATE INDEX IF NOT EXISTS idx_products_category ON products(category_id);
-- CREATE INDEX IF NOT EXISTS idx_products_price ON products(price);
-- CREATE INDEX IF NOT EXISTS idx_products_name ON products(name);

-- CREATE INDEX IF NOT EXISTS idx_clients_name_surname ON clients(client_name, client_surname);
-- CREATE INDEX IF NOT EXISTS idx_suppliers_name ON suppliers(name);

-- CREATE INDEX IF NOT EXISTS idx_clients_registration_date ON clients(registration_date DESC);
-- CREATE INDEX IF NOT EXISTS idx_products_category ON products(category);
-- CREATE INDEX IF NOT EXISTS idx_products_price ON products(price);

-- Комментарии к таблицам
COMMENT ON TABLE addresses IS 'Таблица адресов';
COMMENT ON TABLE clients IS 'Таблица клиентов';
COMMENT ON TABLE suppliers IS 'Таблица поставщиков';
COMMENT ON TABLE categories IS 'Таблица категорий товаров';
COMMENT ON TABLE products IS 'Таблица товаров';
COMMENT ON TABLE images IS 'Таблица изображений товаров';
