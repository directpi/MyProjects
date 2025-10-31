#GRADLE=./gradlew
BUILD_DIR=build/libs/
SCRIPTS_DIR=scripts
PORT=8443 # Порт по умолчанию (можно изменить на 8080, если SSL не подключен)
URL=http://localhost:$(PORT)/game # URL для открытия в браузере
ARTIFACT_NAME=TicTacToe-1.1.jar

# Определяем, какой файл gradlew использовать
ifneq (,$(wildcard ./gradlew))
    GRADLE=./gradlew
else ifneq (,$(wildcard ./gradlew.bat))
    GRADLE=./gradlew.bat
else
    $(error Gradle wrapper not found)
endif

#defauld: run
all: check_java check_gradle check_spring init_db build run

# Запуск приложения и открытие браузера
run: build
	@echo "Запуск приложения..."
	@java -jar $(BUILD_DIR)$(ARTIFACT_NAME)

# Очистка проекта
clean:
	$(GRADLE) clean
	rm -rf temp

#run: build
#	java -jar $(BUILD_DIR)$(ARTIFACT_NAME)

# Сборка проекта
build: clean
	$(GRADLE) build

# Проверка Java
check_java:
	@if ! java -version 2>&1 | grep -q 'openjdk version "18'; then \
		echo "Java 18 не найден. Устанавливаю Amazon Corretto 18..."; \
		wget -q https://corretto.aws/downloads/latest/amazon-corretto-18-x64-linux-jdk.tar.gz; \
		sudo mkdir -p /usr/lib/jvm; \
		sudo tar -xzf amazon-corretto-18-x64-linux-jdk.tar.gz -C /usr/lib/jvm; \
		sudo mv /usr/lib/jvm/amazon-corretto-18* $(JAVA_PATH); \
		sudo update-alternatives --install /usr/bin/java java $(JAVA_PATH)/bin/java 1; \
		sudo update-alternatives --install /usr/bin/javac javac $(JAVA_PATH)/bin/javac 1; \
		sudo update-alternatives --set java $(JAVA_PATH)/bin/java; \
		sudo update-alternatives --set javac $(JAVA_PATH)/bin/javac; \
		rm amazon-corretto-18-x64-linux-jdk.tar.gz; \
		echo "Amazon Corretto 18 установлен."; \
	else \
		echo "Java 18 найден."; \
	fi

# Проверка Gradle
check_gradle:
	@if ! command -v $(GRADLE) > /dev/null 2>&1; then \
		echo "Gradle не найден. Убедитесь, что Gradle Wrapper (gradlew) доступен."; \
		exit 1; \
	else \
		echo "Gradle найден."; \
	fi

# Очистка кэша
clean_cache:
	$(GRADLE) clean
	rm -rf ~/.gradle/caches/

# Проверка Spring Boot
check_spring:
	@if ! $(GRADLE) dependencies | grep -q 'org.springframework.boot'; then \
		echo "Spring Boot не найден в зависимостях. Убедитесь, что он добавлен в build.gradle."; \
		exit 1; \
	else \
		echo "Spring Boot найден в зависимостях."; \
	fi

# Проверка порта
check_port:
	@if lsof -i :8443 > /dev/null 2>&1; then \
		echo "Порт 8443 занят. Освободите порт и попробуйте снова."; \
		exit 1; \
	else \
		echo "Порт 8443 свободен."; \
	fi

# Очистка базы данных и пользователя
clean_db:
	@echo "Удаление базы данных и пользователя..."
	@if [ -f "$(SCRIPTS_DIR)/init_db.sh" ]; then \
		chmod +x $(SCRIPTS_DIR)/init_db.sh; \
		$(SCRIPTS_DIR)/init_db.sh clean; \
	else \
		echo "Файл $(SCRIPTS_DIR)/init_db.sh не найден."; \
		exit 1; \
	fi

# Инициализация базы данных
init_db: check_postgres_version edit_pg_hba run_init_script

# Проверка версии PostgreSQL на порту 5432
check_postgres_version:
	@echo "Проверка версии PostgreSQL на порту 5432..."
	@if sudo netstat -tuln | grep -q ':5432'; then \
		echo "PostgreSQL использует порт 5432."; \
		POSTGRES_VERSION=$$(pg_lsclusters | awk '{if ($$3 == "5432") print $$1}'); \
		echo "Активная версия PostgreSQL: $$POSTGRES_VERSION"; \
	else \
		echo "PostgreSQL не использует порт 5432. Убедитесь, что PostgreSQL запущен."; \
		exit 1; \
	fi

# Редактирование pg_hba.conf для активной версии PostgreSQL
edit_pg_hba:
	@echo "Редактирование pg_hba.conf..."
	@POSTGRES_VERSION=$$(pg_lsclusters | awk '{if ($$3 == "5432") print $$1}'); \
	if [ -z "$$POSTGRES_VERSION" ]; then \
		echo "Не удалось определить версию PostgreSQL."; \
		exit 1; \
	fi; \
	PG_HBA_CONF="/etc/postgresql/$$POSTGRES_VERSION/main/pg_hba.conf"; \
	if [ ! -f "$$PG_HBA_CONF" ]; then \
		echo "Файл $$PG_HBA_CONF не найден."; \
		exit 1; \
	fi; \
	sudo sed -i 's/local   all             all                                     peer/local   all             all                                     md5/' $$PG_HBA_CONF; \
	echo "Метод аутентификации изменен на md5 в $$PG_HBA_CONF."; \
	sudo systemctl restart postgresql@$$POSTGRES_VERSION-main; \
	echo "PostgreSQL перезапущен."

# Запуск скрипта init_db.sh
run_init_script:
	@echo "Запуск скрипта init_db.sh..."
	@if [ -f "$(SCRIPTS_DIR)/init_db.sh" ]; then \
		chmod +x $(SCRIPTS_DIR)/init_db.sh; \
		$(SCRIPTS_DIR)/init_db.sh init; \
	else \
		echo "Файл $(SCRIPTS_DIR)/init_db.sh не найден."; \
		exit 1; \
	fi

# Остановка приложения
stop:
	@echo "Завершение процессов на порту $(PORT)..."
	@-for pid in $$(lsof -ti :$(PORT)); do \
		echo "Завершение процесса $$pid..."; \
		kill -9 $$pid; \
	done
	@echo "Процессы завершены."