import org.gradle.api.GradleException
import org.gradle.api.tasks.Exec
import java.net.Socket
import java.net.InetSocketAddress
import java.io.IOException

plugins {
    java
    id("org.springframework.boot") version "3.4.3"
    id("io.spring.dependency-management") version "1.1.7"
}

group = "example"
version = "1.0.0"
java.sourceCompatibility = JavaVersion.VERSION_21

repositories {
    mavenCentral()
}

dependencies {
    // Основные зависимости

    // Spring Boot starters
    implementation ("org.springframework.boot:spring-boot-starter-data-jdbc")
    implementation("org.springframework.boot:spring-boot-starter-security")
    implementation("org.springframework.boot:spring-boot-starter-web")
    implementation("org.springframework.boot:spring-boot-starter-validation")
    implementation ("org.springframework.boot:spring-boot-starter-actuator")

    // JSON
    implementation ("com.fasterxml.jackson.datatype:jackson-datatype-jsr310")

    // Database
    runtimeOnly("org.postgresql:postgresql")

    // ✅ Swagger / OpenAPI
    implementation("org.springdoc:springdoc-openapi-starter-webmvc-ui:2.7.0")

    // Logging (versions managed by Spring Boot)
    implementation("org.slf4j:slf4j-api")
    implementation("ch.qos.logback:logback-classic")

    // AOP для логирования
    implementation("org.springframework.boot:spring-boot-starter-aop")
    // Lombok
    compileOnly("org.projectlombok:lombok")
    annotationProcessor("org.projectlombok:lombok")

    // Dev tools
    // developmentOnly("org.springframework.boot:spring-boot-devtools")

    // Тестовые зависимости
    testImplementation("org.springframework.boot:spring-boot-starter-test") {
        exclude(group = "org.junit.vintage") // Исключаем JUnit 4
    }
    testImplementation("org.springframework.security:spring-security-test")
    testImplementation("org.testcontainers:postgresql:1.19.7")
    testImplementation("org.testcontainers:junit-jupiter:1.19.7")
    testImplementation("io.rest-assured:rest-assured:5.4.0")
    testImplementation("org.awaitility:awaitility:4.2.1")

    testImplementation(platform("org.junit:junit-bom:5.10.0"))
    testImplementation("org.junit.jupiter:junit-jupiter")
    testRuntimeOnly("org.junit.platform:junit-platform-launcher")
}

// Задача для проверки Java версии
tasks.register("checkJava") {
    group = "verification"
    description = "Проверка версии Java"

    doFirst {
        val requiredVersion = JavaVersion.VERSION_17
        val currentVersion = JavaVersion.current()

        println("🔍 Проверка Java: требуется $requiredVersion, текущая $currentVersion")

        if (currentVersion < requiredVersion) {
            throw GradleException("❌ Требуется Java $requiredVersion или выше. Установите: https://adoptium.net/")
        }
        println("✅ Java версия подходит")
    }
}

tasks.register("checkPort") {
    group = "verification"
    description = "Проверка занятости порта"

    doFirst {
        val port = 8080
        try {
            Socket().use { socket ->
                socket.connect(InetSocketAddress("localhost", port), 1000)
                throw GradleException("❌ Порт $port занят! Освободите порт или измените server.port")
            }
        } catch (e: IOException) {
            println("✅ Порт $port свободен")
        }
    }
}

tasks.register<Exec>("initDb") {
    group = "database"
    description = "Инициализация базы данных"

    commandLine("bash", "scripts/init_db.sh", "init")
    dependsOn("checkJava")

    doFirst {
        println("🗄️  Инициализация базы данных...")
    }
}

tasks.register<Exec>("cleanDb") {
    group = "database"
    description = "Очистка базы данных"

    commandLine("bash", "scripts/init_db.sh", "clean")
}

tasks.register<Exec>("checkDb") {
    group = "database"
    description = "Проверка подключения к БД"

    commandLine("bash", "scripts/init_db.sh", "check")
}

// Настройка зависимостей
tasks.named("bootRun") {
    dependsOn("checkJava", "checkPort")
    doFirst {
        println("🚀 Запуск приложения на порту 8080...")
        println("📖 Документация: http://localhost:8080/api/swagger-ui.html")
        println("🏥 Health check: http://localhost:8080/api/health")
    }
}

tasks.named("build") {
    dependsOn("checkJava")
}
