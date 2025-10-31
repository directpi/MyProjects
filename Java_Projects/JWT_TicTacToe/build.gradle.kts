import org.springframework.boot.gradle.tasks.bundling.BootJar

plugins {
	java
	jacoco
	id("org.springframework.boot") version "3.4.3"
	id("io.spring.dependency-management") version "1.1.7"
}

group = "example"
version = "1.0.1-SNAPSHOT"

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

    // Spring Boot starters
    implementation("org.springframework.boot:spring-boot-starter-data-jpa")
	implementation("org.springframework.boot:spring-boot-starter-security")
	implementation("org.springframework.boot:spring-boot-starter-web")
	implementation("org.springframework.boot:spring-boot-starter-validation")

    // JSON
	implementation("com.google.code.gson:gson:2.10.1")

    // Database
	implementation("org.flywaydb:flyway-core:10.15.0")
    implementation("org.flywaydb:flyway-database-postgresql:10.15.0")
    runtimeOnly("org.postgresql:postgresql")

    // JWT зависимости
    implementation("io.jsonwebtoken:jjwt-api:0.11.5")
    implementation("io.jsonwebtoken:jjwt-impl:0.11.5")
    implementation("io.jsonwebtoken:jjwt-jackson:0.11.5")

    // ✅ Swagger / OpenAPI
    implementation("org.springdoc:springdoc-openapi-starter-webmvc-ui:2.7.0")

	// Логирование (версии управляются Spring Boot)
	implementation("org.slf4j:slf4j-api")
	implementation("ch.qos.logback:logback-classic")

    // Lombok
	compileOnly("org.projectlombok:lombok")
	annotationProcessor("org.projectlombok:lombok")

    // Dev tools
	developmentOnly("org.springframework.boot:spring-boot-devtools")

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
	archiveBaseName.set("JwtTicTacToe")
//	archiveVersion.set("")
    mainClass.set("example.JwtTicTacToeApplication")
}

// JaCoCo configuration for test coverage
tasks.test {
	finalizedBy(tasks.jacocoTestReport) // report is always generated after tests run
}

tasks.jacocoTestReport {
	dependsOn(tasks.test) // tests are required to run before generating the report
	reports {
		xml.required = true
		html.required = true
		csv.required = false
	}
}

tasks.jacocoTestCoverageVerification {
	violationRules {
		rule {
			limit {
				minimum = "0.80".toBigDecimal()
			}
		}
	}
}