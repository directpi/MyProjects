import org.springframework.boot.gradle.tasks.bundling.BootJar

plugins {
	java
	id("org.springframework.boot") version "3.4.3"
	id("io.spring.dependency-management") version "1.1.7"
}

group = "s21.example"
version = "0.0.1-SNAPSHOT"

java {
	toolchain {
		languageVersion = JavaLanguageVersion.of(18)
	}
}

configurations {
	compileOnly {
		extendsFrom(configurations.annotationProcessor.get())
	}
}

repositories {
	mavenCentral()
}

dependencies {
	// Основные зависимости
	implementation("org.springframework.boot:spring-boot-starter-data-jpa")
	implementation("org.springframework.boot:spring-boot-starter-security")
	implementation("org.springframework.boot:spring-boot-starter-web")
	implementation("org.springframework.boot:spring-boot-starter-validation")
	implementation("com.google.code.gson:gson:2.10.1")
	implementation("org.flywaydb:flyway-core:10.15.0")
    implementation("org.flywaydb:flyway-database-postgresql:10.15.0")
    implementation("org.springdoc:springdoc-openapi-starter-webmvc-ui:2.5.0")
    implementation("org.springdoc:springdoc-openapi-ui:1.6.15")

	// Логирование (версии управляются Spring Boot)
	implementation("org.slf4j:slf4j-api")
	implementation("ch.qos.logback:logback-classic")

	compileOnly("org.projectlombok:lombok")
	annotationProcessor("org.projectlombok:lombok")
	developmentOnly("org.springframework.boot:spring-boot-devtools")
	runtimeOnly("org.postgresql:postgresql")

	// Тестовые зависимости
	testImplementation("org.springframework.boot:spring-boot-starter-test") {
		exclude(group = "org.junit.vintage") // Исключаем JUnit 4
	}
	testImplementation("org.springframework.security:spring-security-test")
	testImplementation("org.testcontainers:postgresql:1.19.7")
	testImplementation("org.testcontainers:junit-jupiter:1.19.7")
	testImplementation("io.rest-assured:rest-assured:5.4.0")
	testImplementation("org.awaitility:awaitility:4.2.1")
}

tasks.withType<Test> {
	useJUnitPlatform()
}

tasks.named<BootJar>("bootJar") {
	archiveBaseName.set("TicTacToe")
	archiveVersion.set("1.1")
}