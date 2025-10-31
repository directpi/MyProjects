# ============================================
# JWT TicTacToe - Makefile
# ============================================

# Переменные конфигурации
GRADLEW=./gradlew
WRAPPER_DIR=gradle/wrapper
WRAPPER_JAR=$(WRAPPER_DIR)/gradle-wrapper.jar
BUILD_DIR=build/libs/
SCRIPTS_DIR=scripts
PORT?=8443
URL=https://localhost:$(PORT)
ARTIFACT_NAME=JwtTicTacToe-1.0.1-SNAPSHOT.jar

# Цвета для вывода (опционально)
COLOR_RESET=\033[0m
COLOR_INFO=\033[36m
COLOR_SUCCESS=\033[32m
COLOR_WARNING=\033[33m
COLOR_ERROR=\033[31m

# Цель по умолчанию
# .DEFAULT_GOAL := help

# PHONY targets (не файлы)
.PHONY: all all-with-tests build run stop clean clean-all clean_cache clean_gradle_all \
        test test-class test-coverage test-unit test-integration test-watch test-quick \
        check_java install_java check_gradle init-wrapper check_spring check_port check-all \
        init_db clean_db reset_db check_postgres_version run_init_script \
        help status logs dev docs open-reports run-bg restart swagger \
        deps-update deps-tree deps-outdated

# ============================================
# ОСНОВНЫЕ КОМАНДЫ
# ============================================

# Полная сборка и запуск приложения
all: check_java init-wrapper init_db build run

# Полная сборка с тестами
all-with-tests: check_java init-wrapper init_db build test run

# Быстрая разработка (без БД)
dev: clean build run

# Сборка проекта
build: init-wrapper
	@echo "$(COLOR_INFO)🔨 Сборка проекта...$(COLOR_RESET)"
	$(GRADLEW) build -x test --no-daemon
	@echo "$(COLOR_SUCCESS)✅ Сборка завершена$(COLOR_RESET)"

# Сборка с тестами
build-with-tests: init-wrapper
	@echo "$(COLOR_INFO)🔨 Сборка проекта с тестами...$(COLOR_RESET)"
	$(GRADLEW) build --no-daemon
	@echo "$(COLOR_SUCCESS)✅ Сборка с тестами завершена$(COLOR_RESET)"

# Запуск приложения
run: build
	@echo "$(COLOR_INFO)🚀 Запуск приложения на порту $(PORT)...$(COLOR_RESET)"
	@JAR_FILE=$$(ls $(BUILD_DIR) 2>/dev/null | grep JwtTicTacToe | grep -v plain | head -1); \
	if [ -z "$$JAR_FILE" ]; then \
		echo "$(COLOR_ERROR)❌ JAR-файл не найден!$(COLOR_RESET)"; \
		echo "Доступные файлы:"; \
		ls -la $(BUILD_DIR) 2>/dev/null || echo "Директория $(BUILD_DIR) не существует"; \
		exit 1; \
	fi; \
	echo "$(COLOR_SUCCESS)▶️  Запуск: $$JAR_FILE$(COLOR_RESET)"; \
	java -jar $(BUILD_DIR)$$JAR_FILE

# Запуск в фоновом режиме
run-bg: build
	@echo "$(COLOR_INFO)🚀 Запуск приложения в фоновом режиме...$(COLOR_RESET)"
	@JAR_FILE=$$(ls $(BUILD_DIR) | grep JwtTicTacToe | grep -v plain | head -1); \
	if [ -z "$$JAR_FILE" ]; then \
		echo "$(COLOR_ERROR)❌ JAR-файл не найден!$(COLOR_RESET)"; \
		exit 1; \
	fi; \
	nohup java -jar $(BUILD_DIR)$$JAR_FILE > app.log 2>&1 & \
	echo $$! > app.pid; \
	echo "$(COLOR_SUCCESS)✅ Приложение запущено (PID: $$(cat app.pid))$(COLOR_RESET)"; \
	echo "$(COLOR_INFO)📋 Просмотр логов: make logs  или  tail -f app.log$(COLOR_RESET)"

# Остановка приложения
stop:
	@echo "$(COLOR_WARNING)🛑 Остановка приложения...$(COLOR_RESET)"
	@if [ -f app.pid ]; then \
		PID=$$(cat app.pid); \
		if ps -p $$PID > /dev/null 2>&1; then \
			kill $$PID && echo "$(COLOR_SUCCESS)✅ Процесс $$PID остановлен$(COLOR_RESET)"; \
			rm -f app.pid; \
		else \
			echo "$(COLOR_WARNING)⚠️  Процесс $$PID уже не запущен$(COLOR_RESET)"; \
			rm -f app.pid; \
		fi; \
	fi
	@for pid in $$(lsof -ti :$(PORT) 2>/dev/null); do \
		echo "$(COLOR_INFO)Завершение процесса $$pid на порту $(PORT)...$(COLOR_RESET)"; \
		kill -9 $$pid 2>/dev/null || true; \
	done
	@echo "$(COLOR_SUCCESS)✅ Все процессы остановлены$(COLOR_RESET)"

# Перезапуск приложения
restart: stop run-bg
	@echo "$(COLOR_SUCCESS)✅ Приложение перезапущено$(COLOR_RESET)"

# Просмотр логов
logs:
	@if [ -f app.log ]; then \
		tail -f app.log; \
	else \
		echo "$(COLOR_ERROR)❌ Файл логов не найден$(COLOR_RESET)"; \
	fi

# Статус приложения
status:
	@echo "$(COLOR_INFO)📊 Статус приложения:$(COLOR_RESET)"
	@if [ -f app.pid ]; then \
		PID=$$(cat app.pid); \
		if ps -p $$PID > /dev/null 2>&1; then \
			echo "$(COLOR_SUCCESS)✅ Запущен (PID: $$PID)$(COLOR_RESET)"; \
			echo "Порт: $(PORT)"; \
			echo "URL: $(URL)"; \
		else \
			echo "$(COLOR_ERROR)❌ Не запущен (PID файл устарел)$(COLOR_RESET)"; \
			rm -f app.pid; \
		fi; \
	else \
		echo "$(COLOR_WARNING)⚠️  Приложение не запущено$(COLOR_RESET)"; \
	fi

# ============================================
# ОЧИСТКА
# ============================================

# Очистка проекта
clean:
	@echo "$(COLOR_INFO)🧹 Очистка проекта...$(COLOR_RESET)"
	@if [ -f "$(GRADLEW)" ]; then \
		$(GRADLEW) clean --no-daemon; \
	else \
		echo "Gradle Wrapper не найден, удаляем build вручную..."; \
		rm -rf build; \
	fi
	@rm -rf temp app.log app.pid
	@echo "$(COLOR_SUCCESS)✅ Проект очищен$(COLOR_RESET)"

# Полная очистка (включая кэш)
clean-all: clean clean_cache
	@echo "$(COLOR_SUCCESS)✅ Полная очистка завершена$(COLOR_RESET)"

# Очистка кэша Gradle (только зависимости, без Gradle Wrapper)
clean_cache:
	@echo "$(COLOR_INFO)🧹 Очистка кэша зависимостей Gradle...$(COLOR_RESET)"
	@if [ -f "$(GRADLEW)" ]; then \
		$(GRADLEW) clean --no-daemon; \
	fi
	@rm -rf ~/.gradle/caches/
	@echo "$(COLOR_SUCCESS)✅ Кэш зависимостей очищен$(COLOR_RESET)"
	@echo "$(COLOR_INFO)ℹ️  Gradle Wrapper сохранён$(COLOR_RESET)"

# Полная очистка Gradle (включая Wrapper - будет пересчачан при следующем запуске)
clean_gradle_all:
	@echo "$(COLOR_WARNING)⚠️  Полная очистка Gradle (включая Wrapper)...$(COLOR_RESET)"
	@if [ -f "$(GRADLEW)" ]; then \
		$(GRADLEW) clean --no-daemon; \
	fi
	@rm -rf ~/.gradle/caches/ ~/.gradle/wrapper/
	@echo "$(COLOR_SUCCESS)✅ Gradle полностью очищен$(COLOR_RESET)"
	@echo "$(COLOR_WARNING)⚠️  При следующем запуске Gradle будет скачан заново$(COLOR_RESET)"

# ============================================
# ТЕСТИРОВАНИЕ
# ============================================

# Запуск всех тестов
test: init-wrapper
	@echo "$(COLOR_INFO)🧪 Запуск всех тестов...$(COLOR_RESET)"
	$(GRADLEW) test --no-daemon
	@echo "$(COLOR_SUCCESS)✅ Тесты завершены$(COLOR_RESET)"
	@echo "$(COLOR_INFO)📊 Отчет: build/reports/tests/test/index.html$(COLOR_RESET)"

# Быстрые тесты (параллельно)
test-quick: init-wrapper
	@echo "$(COLOR_INFO)⚡ Быстрый запуск тестов (параллельно)...$(COLOR_RESET)"
	$(GRADLEW) test --parallel --max-workers=4 --no-daemon

# Запуск конкретного тестового класса
test-class: init-wrapper
	@if [ -z "$(CLASS)" ]; then \
		echo "$(COLOR_ERROR)❌ Укажите CLASS=<имя_класса>$(COLOR_RESET)"; \
		echo "$(COLOR_INFO)Пример: make test-class CLASS=example.domain.service.JwtProviderTest$(COLOR_RESET)"; \
		echo ""; \
		echo "Доступные тестовые классы:"; \
		find src/test/java -name "*Test.java" | sed 's|src/test/java/||' | sed 's|/|.|g' | sed 's|\.java||' | sort; \
		exit 1; \
	fi
	@echo "$(COLOR_INFO)🧪 Запуск тестов: $(CLASS)$(COLOR_RESET)"
	$(GRADLEW) test --tests "$(CLASS)" --no-daemon

# Тесты с покрытием кода
test-coverage: init-wrapper
	@echo "$(COLOR_INFO)📊 Запуск тестов с анализом покрытия...$(COLOR_RESET)"
	$(GRADLEW) test jacocoTestReport --no-daemon
	@echo "$(COLOR_SUCCESS)✅ Анализ покрытия завершен$(COLOR_RESET)"
	@echo "$(COLOR_INFO)📊 Отчет: build/reports/jacoco/test/html/index.html$(COLOR_RESET)"

# Только unit тесты
test-unit: init-wrapper
	@echo "$(COLOR_INFO)🧪 Запуск unit тестов...$(COLOR_RESET)"
	$(GRADLEW) test --tests "example.domain.*" --tests "example.utils.*" --no-daemon

# Только интеграционные тесты
test-integration: init-wrapper
	@echo "$(COLOR_INFO)🧪 Запуск интеграционных тестов...$(COLOR_RESET)"
	$(GRADLEW) test --tests "example.web.controller.*" --tests "example.web.filter.*" --no-daemon

# Непрерывный запуск тестов
test-watch: init-wrapper
	@echo "$(COLOR_INFO)👀 Непрерывный запуск тестов (Ctrl+C для остановки)...$(COLOR_RESET)"
	$(GRADLEW) test --continuous --no-daemon

# Открыть отчет о тестах
open-reports:
	@echo "$(COLOR_INFO)📊 Открытие отчетов...$(COLOR_RESET)"
	@if [ -f "build/reports/tests/test/index.html" ]; then \
		xdg-open build/reports/tests/test/index.html 2>/dev/null || \
		open build/reports/tests/test/index.html 2>/dev/null || \
		echo "$(COLOR_INFO)Отчет: build/reports/tests/test/index.html$(COLOR_RESET)"; \
	else \
		echo "$(COLOR_ERROR)❌ Отчет не найден. Запустите: make test$(COLOR_RESET)"; \
	fi
	@if [ -f "build/reports/jacoco/test/html/index.html" ]; then \
		xdg-open build/reports/jacoco/test/html/index.html 2>/dev/null || \
		open build/reports/jacoco/test/html/index.html 2>/dev/null || \
		echo "$(COLOR_INFO)Покрытие: build/reports/jacoco/test/html/index.html$(COLOR_RESET)"; \
	else \
		echo "$(COLOR_WARNING)⚠️  Отчет о покрытии не найден. Запустите: make test-coverage$(COLOR_RESET)"; \
	fi

# ============================================
# GRADLE И ЗАВИСИМОСТИ
# ============================================

# Инициализация Gradle Wrapper
init-wrapper:
	@if [ ! -f "$(GRADLEW)" ] || [ ! -f "$(WRAPPER_JAR)" ] || [ ! -f "gradle/wrapper/gradle-wrapper.properties" ]; then \
		echo "$(COLOR_INFO)📦 Создание Gradle Wrapper...$(COLOR_RESET)"; \
		if command -v gradle >/dev/null 2>&1; then \
			gradle wrapper --gradle-version 8.10; \
			echo "$(COLOR_SUCCESS)✅ Gradle Wrapper создан$(COLOR_RESET)"; \
		else \
			echo "$(COLOR_ERROR)❌ Gradle не установлен. Установите: sudo apt install gradle$(COLOR_RESET)"; \
			exit 1; \
		fi; \
	fi

# Обновление зависимостей
deps-update: init-wrapper
	@echo "$(COLOR_INFO)📦 Обновление зависимостей...$(COLOR_RESET)"
	$(GRADLEW) dependencies --refresh-dependencies --no-daemon

# Просмотр дерева зависимостей
deps-tree: init-wrapper
	@echo "$(COLOR_INFO)📦 Дерево зависимостей:$(COLOR_RESET)"
	$(GRADLEW) dependencies --no-daemon

# Поиск устаревших зависимостей
deps-outdated: init-wrapper
	@echo "$(COLOR_INFO)📦 Проверка устаревших зависимостей...$(COLOR_RESET)"
	$(GRADLEW) dependencyUpdates --no-daemon

# ============================================
# ПРОВЕРКИ ОКРУЖЕНИЯ
# ============================================

# Проверка Java
check_java:
	@echo "$(COLOR_INFO)☕ Проверка Java...$(COLOR_RESET)"
	@if ! java -version 2>&1 | grep -q 'version "18'; then \
		echo "$(COLOR_ERROR)❌ Java 18 не найден$(COLOR_RESET)"; \
		echo "Установка: make install_java"; \
		exit 1; \
	else \
		echo "$(COLOR_SUCCESS)✅ Java 18 найден$(COLOR_RESET)"; \
	fi

# Проверка Gradle
check_gradle:
	@echo "$(COLOR_INFO)📦 Проверка Gradle...$(COLOR_RESET)"
	@if ! command -v gradle >/dev/null 2>&1; then \
		echo "$(COLOR_WARNING)⚠️  System Gradle не установлен$(COLOR_RESET)"; \
		echo "Установка: sudo apt install gradle"; \
	else \
		echo "$(COLOR_SUCCESS)✅ Gradle найден: $$(gradle --version | head -1)$(COLOR_RESET)"; \
	fi

# Проверка Spring Boot
check_spring: init-wrapper
	@echo "$(COLOR_INFO)🍃 Проверка Spring Boot...$(COLOR_RESET)"
	@if $(GRADLEW) dependencies | grep -q 'org.springframework.boot'; then \
		echo "$(COLOR_SUCCESS)✅ Spring Boot найден$(COLOR_RESET)"; \
	else \
		echo "$(COLOR_ERROR)❌ Spring Boot не найден в зависимостях$(COLOR_RESET)"; \
		exit 1; \
	fi

# Проверка порта
check_port:
	@echo "$(COLOR_INFO)🔌 Проверка порта $(PORT)...$(COLOR_RESET)"
	@if lsof -i :$(PORT) > /dev/null 2>&1; then \
		echo "$(COLOR_ERROR)❌ Порт $(PORT) занят:$(COLOR_RESET)"; \
		lsof -i :$(PORT); \
		exit 1; \
	else \
		echo "$(COLOR_SUCCESS)✅ Порт $(PORT) свободен$(COLOR_RESET)"; \
	fi

# Проверка PostgreSQL
check_postgres_version:
	@echo "$(COLOR_INFO)🐘 Проверка PostgreSQL...$(COLOR_RESET)"
	@if ss -tuln | grep -q ':5432'; then \
		echo "$(COLOR_SUCCESS)✅ PostgreSQL запущен на порту 5432$(COLOR_RESET)"; \
		POSTGRES_VERSION=$$(pg_lsclusters 2>/dev/null | awk '{if ($$3 == "5432") print $$1}'); \
		if [ -n "$$POSTGRES_VERSION" ]; then \
			echo "$(COLOR_INFO)Версия: $$POSTGRES_VERSION$(COLOR_RESET)"; \
		fi; \
	else \
		echo "$(COLOR_ERROR)❌ PostgreSQL не запущен на порту 5432$(COLOR_RESET)"; \
		exit 1; \
	fi

# Полная проверка окружения
check-all: check_java check_gradle check_spring check_postgres_version check_port
	@echo "$(COLOR_SUCCESS)✅ Все проверки пройдены$(COLOR_RESET)"

# ============================================
# БАЗА ДАННЫХ
# ============================================

# Инициализация БД
init_db: check_postgres_version run_init_script

# Запуск скрипта инициализации
run_init_script:
	@echo "$(COLOR_INFO)🗄️  Инициализация базы данных...$(COLOR_RESET)"
	@if [ -f "$(SCRIPTS_DIR)/init_db.sh" ]; then \
		chmod +x $(SCRIPTS_DIR)/init_db.sh; \
		$(SCRIPTS_DIR)/init_db.sh init; \
		echo "$(COLOR_SUCCESS)✅ База данных инициализирована$(COLOR_RESET)"; \
	else \
		echo "$(COLOR_ERROR)❌ Скрипт не найден: $(SCRIPTS_DIR)/init_db.sh$(COLOR_RESET)"; \
		exit 1; \
	fi

# Очистка БД
clean_db:
	@echo "$(COLOR_WARNING)🗑️  Удаление базы данных...$(COLOR_RESET)"
	@if [ -f "$(SCRIPTS_DIR)/init_db.sh" ]; then \
		chmod +x $(SCRIPTS_DIR)/init_db.sh; \
		$(SCRIPTS_DIR)/init_db.sh clean; \
		echo "$(COLOR_SUCCESS)✅ База данных удалена$(COLOR_RESET)"; \
	else \
		echo "$(COLOR_ERROR)❌ Скрипт не найден: $(SCRIPTS_DIR)/init_db.sh$(COLOR_RESET)"; \
		exit 1; \
	fi

# Пересоздание БД
reset_db: clean_db init_db
	@echo "$(COLOR_SUCCESS)✅ База данных пересоздана$(COLOR_RESET)"

# ============================================
# УСТАНОВКА
# ============================================

# Установка Java 18 (Amazon Corretto)
install_java:
	@echo "$(COLOR_INFO)☕ Установка Amazon Corretto 18...$(COLOR_RESET)"
	@wget -q https://corretto.aws/downloads/latest/amazon-corretto-18-x64-linux-jdk.tar.gz
	@sudo mkdir -p /usr/lib/jvm
	@sudo tar -xzf amazon-corretto-18-x64-linux-jdk.tar.gz -C /usr/lib/jvm
	@sudo mv /usr/lib/jvm/amazon-corretto-18* /usr/lib/jvm/java-18-corretto
	@sudo update-alternatives --install /usr/bin/java java /usr/lib/jvm/java-18-corretto/bin/java 1
	@sudo update-alternatives --install /usr/bin/javac javac /usr/lib/jvm/java-18-corretto/bin/javac 1
	@sudo update-alternatives --set java /usr/lib/jvm/java-18-corretto/bin/java
	@sudo update-alternatives --set javac /usr/lib/jvm/java-18-corretto/bin/javac
	@rm amazon-corretto-18-x64-linux-jdk.tar.gz
	@echo "$(COLOR_SUCCESS)✅ Amazon Corretto 18 установлен$(COLOR_RESET)"

# ============================================
# ДОКУМЕНТАЦИЯ
# ============================================

# Генерация Javadoc
docs: init-wrapper
	@echo "$(COLOR_INFO)📚 Генерация документации...$(COLOR_RESET)"
	$(GRADLEW) javadoc --no-daemon
	@echo "$(COLOR_SUCCESS)✅ Документация: build/docs/javadoc/index.html$(COLOR_RESET)"

# Открыть Swagger UI
swagger:
	@echo "$(COLOR_INFO)📖 Открытие Swagger UI...$(COLOR_RESET)"
	@xdg-open "$(URL)/swagger-ui/index.html" 2>/dev/null || \
	open "$(URL)/swagger-ui/index.html" 2>/dev/null || \
	echo "$(COLOR_INFO)URL: $(URL)/swagger-ui/index.html$(COLOR_RESET)"

# ============================================
# СПРАВКА
# ============================================

help:
	@echo "$(COLOR_INFO)╔════════════════════════════════════════════════════════════════╗$(COLOR_RESET)"
	@echo "$(COLOR_INFO)║        🎮 JWT TicTacToe - Makefile Commands                   ║$(COLOR_RESET)"
	@echo "$(COLOR_INFO)╚════════════════════════════════════════════════════════════════╝$(COLOR_RESET)"
	@echo ""
	@echo "$(COLOR_SUCCESS)📦 ОСНОВНЫЕ КОМАНДЫ:$(COLOR_RESET)"
	@echo "  make                   - Показать эту справку"
	@echo "  make all               - Полная сборка и запуск"
	@echo "  make dev               - Быстрая разработка (clean + build + run)"
	@echo "  make build             - Сборка без тестов"
	@echo "  make run               - Запуск приложения"
	@echo "  make run-bg            - Запуск в фоновом режиме"
	@echo "  make stop              - Остановка приложения"
	@echo "  make restart           - Перезапуск приложения"
	@echo "  make status            - Статус приложения"
	@echo "  make logs              - Просмотр логов"
	@echo "  make clean             - Очистка проекта"
	@echo "  make clean-all         - Полная очистка (с кэшем зависимостей)"
	@echo "  make clean_gradle_all  - Полное удаление Gradle (будет скачан заново)"
	@echo ""
	@echo "$(COLOR_SUCCESS)🧪 ТЕСТИРОВАНИЕ:$(COLOR_RESET)"
	@echo "  make test              - Запуск всех тестов (185 шт.)"
	@echo "  make test-quick        - Быстрые тесты (параллельно)"
	@echo "  make test-unit         - Только unit тесты"
	@echo "  make test-integration  - Интеграционные тесты"
	@echo "  make test-coverage     - Тесты + покрытие кода (JaCoCo)"
	@echo "  make test-watch        - Непрерывный запуск тестов"
	@echo "  make test-class CLASS= - Запуск конкретного класса"
	@echo "  make open-reports      - Открыть отчеты о тестах"
	@echo ""
	@echo "$(COLOR_SUCCESS)🗄️  БАЗА ДАННЫХ:$(COLOR_RESET)"
	@echo "  make init_db           - Инициализация PostgreSQL БД"
	@echo "  make clean_db          - Удаление БД"
	@echo "  make reset_db          - Пересоздание БД"
	@echo ""
	@echo "$(COLOR_SUCCESS)🔧 УТИЛИТЫ:$(COLOR_RESET)"
	@echo "  make check-all         - Проверка всего окружения"
	@echo "  make check_java        - Проверка Java 18"
	@echo "  make install_java      - Установка Amazon Corretto 18"
	@echo "  make deps-update       - Обновление зависимостей"
	@echo "  make deps-tree         - Дерево зависимостей"
	@echo "  make docs              - Генерация Javadoc"
	@echo "  make swagger           - Открыть Swagger UI"
	@echo ""
	@echo "$(COLOR_SUCCESS)📊 ПРИМЕРЫ:$(COLOR_RESET)"
	@echo "  make test-class CLASS=example.domain.service.JwtProviderTest"
	@echo "  make PORT=8080 run     # Изменить порт на 8080"
	@echo "  make dev               # Быстрая разработка"
	@echo ""
	@echo "$(COLOR_SUCCESS)📁 ОТЧЕТЫ:$(COLOR_RESET)"
	@echo "  Тесты:     $(COLOR_INFO)build/reports/tests/test/index.html$(COLOR_RESET)"
	@echo "  Покрытие:  $(COLOR_INFO)build/reports/jacoco/test/html/index.html$(COLOR_RESET)"
	@echo "  Swagger:   $(COLOR_INFO)$(URL)/swagger-ui/index.html$(COLOR_RESET)"
	@echo ""
	@echo "$(COLOR_SUCCESS)🌐 URL ПРИЛОЖЕНИЯ:$(COLOR_RESET)"
	@echo "  $(COLOR_INFO)$(URL)$(COLOR_RESET)"
	@echo ""
	@echo "$(COLOR_INFO)ℹ️  Документация: make help$(COLOR_RESET)"
	@echo ""