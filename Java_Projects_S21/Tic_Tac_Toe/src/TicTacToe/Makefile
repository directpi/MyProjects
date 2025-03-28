GRADLE=./gradlew
BUILD_TASK=shadowJar
CLEAN_TASK=clean
BUILD_DIR=build/libs/
ARTIFACT_NAME=TicTacToe-1.0.jar

#defauld: run
all: check_java check_gradle check_spring build run
clean:
	$(GRADLE) $(CLEAN_TASK)
	rm -rf temp
run: build
	java -jar $(BUILD_DIR)$(ARTIFACT_NAME)
build: clean
	$(GRADLE) build
#	./gradlew build
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

check_gradle:
	@if ! command -v $(GRADLE) > /dev/null 2>&1; then \
		echo "Gradle не найден. Убедитесь, что Gradle Wrapper (gradlew) доступен."; \
		exit 1; \
	else \
		echo "Gradle найден."; \
	fi

clean_cache:
	$(GRADLE) clean
	rm -rf ~/.gradle/caches/

check_spring:
	@if ! $(GRADLE) dependencies | grep -q 'org.springframework.boot'; then \
		echo "Spring Boot не найден в зависимостях. Убедитесь, что он добавлен в build.gradle."; \
		exit 1; \
	else \
		echo "Spring Boot найден в зависимостях."; \
	fi

check_port:
	@if lsof -i :8443 > /dev/null 2>&1; then \
		echo "Порт 8443 занят. Освободите порт и попробуйте снова."; \
		exit 1; \
	else \
		echo "Порт 8443 свободен."; \
	fi