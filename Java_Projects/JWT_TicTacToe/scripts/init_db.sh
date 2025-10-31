#!/bin/bash

# Параметры подключения к PostgreSQL
DB_NAME="tictactoe_db"
DB_USER="java_user"
DB_PASSWORD="123456"
PG_HOST="localhost"  # Используем TCP/IP вместо локального сокета

PG_USER="postgres"
PG_PASSWORD="postgres"  # Пароль пользователя postgres
# Установка переменной окружения для пароля
export PGPASSWORD="$PG_PASSWORD"

# Проверка, установлен ли PostgreSQL
if ! command -v psql &> /dev/null; then
    echo "PostgreSQL не установлен. Установите PostgreSQL и повторите попытку."
    exit 1
fi

# Проверка, запущен ли сервер PostgreSQL
if ! pg_isready -q -h $PG_HOST; then
    echo "Сервер PostgreSQL не запущен. Запустите сервер и повторите попытку."
    exit 1
fi

# Функция для инициализации базы данных
init_db() {
    # Проверка существования базы данных
    if psql -h $PG_HOST -U postgres -tAc "SELECT 1 FROM pg_database WHERE datname = '$DB_NAME'" | grep -q 1; then
        echo "База данных $DB_NAME уже существует."
    else
        echo "Создание базы данных $DB_NAME..."
        psql -h $PG_HOST -U postgres -c "CREATE DATABASE $DB_NAME;" || {
            echo "Ошибка при создании базы данных $DB_NAME."
            exit 1
        }
    fi

    # Проверка существования пользователя
    if psql -h $PG_HOST -U postgres -tAc "SELECT 1 FROM pg_roles WHERE rolname = '$DB_USER'" | grep -q 1; then
        echo "Пользователь $DB_USER уже существует."
    else
        echo "Создание пользователя $DB_USER..."
        psql -h $PG_HOST -U postgres -c "CREATE USER $DB_USER WITH PASSWORD '$DB_PASSWORD';" || {
            echo "Ошибка при создании пользователя $DB_USER."
            exit 1
        }
    fi

    # Предоставление прав пользователю
    echo "Предоставление прав пользователю $DB_USER на базу данных $DB_NAME..."
    psql -h $PG_HOST -U postgres -d $DB_NAME -c "GRANT CONNECT, CREATE ON DATABASE $DB_NAME TO $DB_USER;" || {
        echo "Ошибка при предоставлении прав пользователю $DB_USER."
        exit 1
    }

    # Предоставление прав на схему public
    echo "Предоставление прав на схему public..."
    psql -h $PG_HOST -U postgres -d $DB_NAME -c "GRANT ALL PRIVILEGES ON SCHEMA public TO $DB_USER;" || {
        echo "Ошибка при предоставлении прав на схему public."
        exit 1
    }

    # Предоставление прав на все таблицы в схеме public
    echo "Предоставление прав на все таблицы в схеме public..."
    psql -h $PG_HOST -U postgres -d $DB_NAME -c "GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO $DB_USER;" || {
        echo "Ошибка при предоставлении прав на таблицы."
        exit 1
    }

    # Предоставление прав на все последовательности в схеме public
    echo "Предоставление прав на все последовательности в схеме public..."
    psql -h $PG_HOST -U postgres -d $DB_NAME -c "GRANT ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public TO $DB_USER;" || {
        echo "Ошибка при предоставлении прав на последовательности."
        exit 1
    }

    # Сделать пользователя владельцем схемы public
    echo "Назначение пользователя $DB_USER владельцем схемы public..."
    psql -h $PG_HOST -U postgres -d $DB_NAME -c "ALTER SCHEMA public OWNER TO $DB_USER;" || {
        echo "Ошибка при назначении владельца схемы public."
        exit 1
    }

    echo "База данных $DB_NAME и пользователь $DB_USER успешно созданы. Все права предоставлены."
}

# Функция для очистки базы данных и пользователя
clean_db() {
    echo "Удаление базы данных $DB_NAME..."
    psql -h $PG_HOST -U postgres -c "DROP DATABASE IF EXISTS $DB_NAME;" || {
        echo "Ошибка при удалении базы данных $DB_NAME."
        exit 1
    }

    echo "Удаление пользователя $DB_USER..."
    psql -h $PG_HOST -U postgres -c "DROP USER IF EXISTS $DB_USER;" || {
        echo "Ошибка при удалении пользователя $DB_USER."
        exit 1
    }

    echo "База данных $DB_NAME и пользователь $DB_USER успешно удалены."
}

# Обработка аргументов командной строки
case "$1" in
    init)
        init_db
        ;;
    clean)
        clean_db
        ;;
    *)
        echo "Использование: $0 {init|clean}"
        exit 1
        ;;
esac
