import org.springframework.boot.gradle.tasks.bundling.BootJar

plugins {
	java
//	jacoco
	id("org.springframework.boot") version "3.4.3"
	id("io.spring.dependency-management") version "1.1.7"
//    id("com.diffplug.spotless") version "6.25.0"
//    id("checkstyle")
}

group = "src.brick_game"
version = "1.0.1-SNAPSHOT"

java {
	toolchain {
		languageVersion = JavaLanguageVersion.of(21)
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
//    implementation("org.springframework.boot:spring-boot-starter-data-jpa")
    implementation("org.springframework.boot:spring-boot-starter-security")
	implementation("org.springframework.boot:spring-boot-starter-web")
	implementation("org.springframework.boot:spring-boot-starter-validation")

    // JSON
    implementation("com.fasterxml.jackson.core:jackson-databind:2.17.2")
    
    // Lombok
    implementation("org.projectlombok:lombok")
    annotationProcessor("org.projectlombok:lombok")

    // Database
//	implementation("org.flywaydb:flyway-core:10.15.0")
//    implementation("org.flywaydb:flyway-database-postgresql:10.15.0")
//    runtimeOnly("org.postgresql:postgresql")

    // JWT зависимости
//    implementation("io.jsonwebtoken:jjwt-api:0.11.5")
//    implementation("io.jsonwebtoken:jjwt-impl:0.11.5")
//    implementation("io.jsonwebtoken:jjwt-jackson:0.11.5")

    // ✅ Swagger / OpenAPI
    implementation("org.springdoc:springdoc-openapi-starter-webmvc-ui:2.7.0")

	// Логирование (версии управляются Spring Boot)
	implementation("org.slf4j:slf4j-api")
	implementation("ch.qos.logback:logback-classic")

    // Dev tools
	developmentOnly("org.springframework.boot:spring-boot-devtools")

	// Тестовые зависимости
	testImplementation("org.springframework.boot:spring-boot-starter-test") {
		exclude(group = "org.junit.vintage") // Исключаем JUnit 4
	}
	testImplementation("org.springframework.security:spring-security-test")
//	testImplementation("org.testcontainers:postgresql:1.19.7")
	testImplementation("org.testcontainers:junit-jupiter:1.19.7")
	testImplementation("io.rest-assured:rest-assured:5.4.0")
	testImplementation("org.awaitility:awaitility:4.2.1")
    // Для новых MockBean аннотаций в Spring Boot 3.4.0+
    testImplementation("org.springframework.boot:spring-boot-testcontainers")
}

tasks.withType<Test> {
	useJUnitPlatform()

    // Для лучшей производительности тестов
    maxParallelForks = Runtime.getRuntime().availableProcessors().div(2)

    testLogging {
        events("passed", "skipped", "failed")
        showExceptions = true
        exceptionFormat = org.gradle.api.tasks.testing.logging.TestExceptionFormat.FULL
        showCauses = true
        showStackTraces = true
    }
}

tasks.named<BootJar>("bootJar") {
	archiveBaseName.set("BrickGame_v3.0_Jv")
//	archiveVersion.set("")
    mainClass.set("src.brick_game.BrickGameApplication")
    // Оптимизация для production
//    isLaunchScript = true
}
// Конфигурация компиляции Java
tasks.withType<JavaCompile> {
    options.encoding = "UTF-8"
    options.compilerArgs.addAll(listOf("-parameters", "-Xlint:unchecked", "-Xlint:deprecation"))
}

// Задача для запуска приложения
tasks.register<JavaExec>("runApp") {
    classpath = sourceSets["main"].runtimeClasspath
    mainClass.set("src.brick_game.BrickGameApplication")
}

// Задача для запуска Swing клиента
tasks.register<JavaExec>("runSwingExample") {
    classpath = sourceSets["main"].runtimeClasspath
    mainClass.set("src.brick_game.client_lib.examples.SwingExample")
    standardInput = System.`in`
}

// Задача для запуска консольного клиента
tasks.register<JavaExec>("runConsoleExample") {
    classpath = sourceSets["main"].runtimeClasspath
    mainClass.set("src.brick_game.client_lib.examples.ConsoleExample")
    standardInput = System.`in`
}

// Задачи для запуска универсального CLI по играм
// =========================
// JNI build helpers (Tetris)
// =========================
val javaHome: String = System.getenv("JAVA_HOME") ?: "/usr/lib/jvm/java-21-openjdk-amd64"
val jniInclude: String = file("$javaHome/include").absolutePath
// По умолчанию Linux; для macOS можно переопределить через env JNI_OS_INCLUDE=darwin
val jniIncludeOs: String = System.getenv("JNI_OS_INCLUDE")?.let { "$javaHome/include/$it" }
    ?: "$javaHome/include/linux"

tasks.register<Exec>("buildTetrisJni") {
    group = "native"
    description = "Build libtetris_jni.so from C sources"
    workingDir = file("src/main/java/src/brick_game/BrickGame_v1.0-1")
    commandLine = listOf(
        "bash", "-lc",
        "gcc -fPIC -shared " +
            "-I\"$jniInclude\" -I\"$jniIncludeOs\" " +
            "-I\"./brick_game/tetris\" " +
            "-o libtetris_jni.so " +
            "tetris_jni.c brick_game/tetris/s21_tetris_back.c"
    )
}

tasks.register<Copy>("copyTetrisJniToResources") {
    group = "native"
    description = "Copy libtetris_jni.so to resources/native"
    dependsOn("buildTetrisJni")
    from("src/main/java/src/brick_game/BrickGame_v1.0-1/libtetris_jni.so")
    into("src/main/resources/native")
}

tasks.register("rebuildTetrisJni") {
    group = "native"
    description = "Build and copy libtetris_jni.so into resources"
    dependsOn("buildTetrisJni", "copyTetrisJniToResources")
}

// =========================
// JNI build helpers (Snake)
// =========================
tasks.register<Exec>("buildSnakeJni") {
    group = "native"
    description = "Build libsnake_jni.so from C++ sources"
    workingDir = file("src/main/java/src/brick_game/BrickGame_v2.0-1")
    commandLine = listOf(
        "bash", "-lc",
        "g++ -fPIC -shared -std=c++17 " +
            "-I\"$jniInclude\" -I\"$jniIncludeOs\" " +
            "-I\"./brick_game/common\" " +
            "-I\"./brick_game/snake/model\" " +
            "-I\"./brick_game/snake/controller\" " +
            "-o libsnake_jni.so " +
            "snake_jni.cpp " +
            "brick_game/snake/model/s21_model.cpp " +
            "brick_game/snake/controller/s21_controller.cpp " +
            "brick_game/tetris/s21_tetris_back.c"
    )
}

tasks.register<Copy>("copySnakeJniToResources") {
    group = "native"
    description = "Copy libsnake_jni.so to resources/native"
    dependsOn("buildSnakeJni")
    from("src/main/java/src/brick_game/BrickGame_v2.0-1/libsnake_jni.so")
    into("src/main/resources/native")
}

tasks.register("rebuildSnakeJni") {
    group = "native"
    description = "Build and copy libsnake_jni.so into resources"
    dependsOn("buildSnakeJni", "copySnakeJniToResources")
}

// =========================
// JNI build helpers (Racing)
// =========================
tasks.register<Exec>("buildRacingJni") {
    group = "native"
    description = "Build libracing_jni.so from C++ sources"
    workingDir = file("src/main/java/src/brick_game/BrickGame_v2.0-1")
    commandLine = listOf(
        "bash", "-lc",
        "g++ -fPIC -shared -std=c++17 " +
            "-I\"$jniInclude\" -I\"$jniIncludeOs\" " +
            "-I\"./brick_game/common\" " +
            "-o libracing_jni.so " +
            "racing_jni.cpp s21_racing.cpp"
    )
}

tasks.register<Copy>("copyRacingJniToResources") {
    group = "native"
    description = "Copy libracing_jni.so to resources/native"
    dependsOn("buildRacingJni")
    from("src/main/java/src/brick_game/BrickGame_v2.0-1/libracing_jni.so")
    into("src/main/resources/native")
}

tasks.register("rebuildRacingJni") {
    group = "native"
    description = "Build and copy libracing_jni.so into resources"
    dependsOn("buildRacingJni", "copyRacingJniToResources")
}

// Общая задача для всех JNI библиотек
tasks.register("rebuildAllJni") {
    group = "native"
    description = "Build and copy all JNI libraries (Tetris, Snake, Racing)"
    dependsOn("rebuildTetrisJni", "rebuildSnakeJni", "rebuildRacingJni")
}

// ============================================================================
// Нативные консольные (терминальные) версии игр на C/C++ с ncurses
// ============================================================================

// BrickGame Terminal (C++ + ncurses) - универсальная консольная версия с меню
// Включает: Tetris (C) + Snake (C++) в одной программе
tasks.register<Exec>("buildTerminal") {
    group = "terminal"
    description = "Build BrickGame CLI with menu (Tetris + Snake, C/C++ + ncurses)"
    workingDir = file("src/main/java/src/brick_game/BrickGame_v2.0-1")
    commandLine = listOf("make", "cli")
}

// Запуск BrickGame Terminal
// ВАЖНО: Gradle НЕ МОЖЕТ корректно запускать ncurses приложения
// Поэтому задача только собирает и выводит путь для прямого запуска
tasks.register("runTerminal") {
    group = "terminal"
    description = "Build and show command to run BrickGame (Gradle can't run ncurses apps properly)"
    dependsOn("buildTerminal")
    
    doLast {
        // Создаем скрипт автоматически
        val scriptFile = file("run-terminal.sh")
        scriptFile.writeText("""#!/bin/bash
# Запуск BrickGame Terminal (Tetris + Snake)
./src/main/java/src/brick_game/BrickGame_v2.0-1/build/brickgame
""")
        scriptFile.setExecutable(true)
        
        println("\n" + "=".repeat(70))
        println("  BrickGame Terminal собран успешно!")
        println("=".repeat(70))
        println("\n  Запустите игру командой:\n")
        println("  \u001B[1;32m./run-terminal.sh\u001B[0m")
        println("\n" + "=".repeat(70) + "\n")
    }
}

// =========================
// Desktop (Qt) build & run
// =========================
val desktopDir = file("src/main/java/src/brick_game/BrickGame_v2.0-1/gui/desktop")

tasks.register<Exec>("desktopBuild") {
    group = "desktop"
    description = "Build Qt desktop application (Tetris/Snake/Racing)"
    workingDir = desktopDir
    commandLine = listOf("make")
}

tasks.register<Exec>("runDesktop") {
    group = "desktop"
    description = "Run Qt desktop application using script"
    dependsOn("desktopBuild")
    workingDir = projectDir
    // Запускаем приложение через скрипт
    commandLine = listOf("./run_desktop.sh")
    isIgnoreExitValue = true
}

// Очистка всех исполняемых файлов и артефактов
tasks.register("cleanAll") {
    group = "build"
    description = "Clean all executable files and build artifacts"
    
    doLast {
        // Удаляем JNI библиотеки из resources
        val nativeDir = file("src/main/resources/native")
        if (nativeDir.exists()) {
            delete(nativeDir.listFiles()?.filter { it.name.endsWith(".so") })
            println("  ✅ Удалены JNI библиотеки из resources/native/")
        }
        
        // Удаляем исполняемые файлы из BrickGame_v1.0-1
        val v1Dir = file("src/main/java/src/brick_game/BrickGame_v1.0-1")
        if (v1Dir.exists()) {
            val v1Makefile = file("${v1Dir}/Makefile")
            if (v1Makefile.exists()) {
                // Вызываем make clean для полной очистки .o и .a файлов
                try {
                    val process = Runtime.getRuntime().exec(arrayOf("make", "clean"), null, v1Dir)
                    process.waitFor()
                    if (process.exitValue() == 0) {
                        println("  ✅ Выполнен make clean в BrickGame_v1.0-1/")
                    }
                } catch (e: Exception) {
                    println("  ⚠️  Не удалось выполнить make clean в BrickGame_v1.0-1/: ${e.message}")
                }
            }
            delete(v1Dir.listFiles()?.filter { it.name.endsWith(".so") })
            println("  Удалены JNI библиотеки из BrickGame_v1.0-1/")
        }
        
        // Удаляем исполняемые файлы из BrickGame_v2.0-1
        val v2Dir = file("src/main/java/src/brick_game/BrickGame_v2.0-1")
        if (v2Dir.exists()) {
            val v2Makefile = file("${v2Dir}/Makefile")
            if (v2Makefile.exists()) {
                // Вызываем make clean для полной очистки .o и .a файлов (включая поддиректории)
                try {
                    val process = Runtime.getRuntime().exec(arrayOf("make", "clean"), null, v2Dir)
                    process.waitFor()
                    if (process.exitValue() == 0) {
                        println("  ✅ Выполнен make clean в BrickGame_v2.0-1/")
                    }
                } catch (e: Exception) {
                    println("  ⚠️  Не удалось выполнить make clean в BrickGame_v2.0-1/: ${e.message}")
                }
            }
            // Дополнительная очистка на случай если make clean не сработал
            delete(v2Dir.listFiles()?.filter { it.name.endsWith(".so") })
            delete(v2Dir.listFiles()?.filter { it.name.endsWith(".a") })
            delete(v2Dir.listFiles()?.filter { it.name.endsWith(".o") })
            // Рекурсивная очистка .o и .a файлов в поддиректориях
            delete(fileTree(v2Dir).include("**/*.o"))
            delete(fileTree(v2Dir).include("**/*.a"))
            delete(file("${v2Dir}/build/brickgame"))
            println("  Удалены исполняемые файлы из BrickGame_v2.0-1/")
        }
        
        // НЕ удаляем run-terminal.sh - он создается автоматически задачей desktopRunScript
        
        // Удаляем игровые данные
        val dataDir = file("data")
        if (dataDir.exists()) {
            delete(dataDir)
            println("  Удалена папка data/ с игровыми данными")
        }
        
        // Очистка Qt desktop версии
        if (desktopDir.exists()) {
            val desktopMakefile = file("${desktopDir}/Makefile")
            if (desktopMakefile.exists()) {
                // Вызываем make clean для полной очистки .o файлов и других артефактов
                try {
                    val process = Runtime.getRuntime().exec(arrayOf("make", "clean"), null, desktopDir)
                    process.waitFor()
                    if (process.exitValue() == 0) {
                        println("  ✅ Выполнен make clean в desktop/")
                    }
                } catch (e: Exception) {
                    println("  ⚠️  Не удалось выполнить make clean в desktop/: ${e.message}")
                }
            }
            // Дополнительная очистка на случай если make clean не сработал
            delete(file("${desktopDir}/s21_brickgame_desktop"))
            // Удаляем объектные файлы рекурсивно
            val objDir = file("${desktopDir}/obj")
            if (objDir.exists()) {
                delete(fileTree(objDir).include("**/*.o"))
            }
            // Удаляем объектные файлы в корне desktopDir
            desktopDir.listFiles()?.filter { it.name.endsWith(".o") }?.forEach { it.delete() }
            // Удаляем MOC файлы (автогенерируемые)
            val mocDir = file("${desktopDir}/moc")
            if (mocDir.exists()) {
                delete(mocDir)
            }
            println("  Очищена Qt desktop версия")
        }
        
        // Очистка Java desktop версии (SwingExample)
        val javaDesktopDir = file("src/main/java/src/brick_game/client_lib/examples")
        if (javaDesktopDir.exists()) {
            // Удаляем скомпилированные .class файлы
            delete(javaDesktopDir.listFiles()?.filter { it.name.endsWith(".class") })
            // Удаляем JAR файлы если есть
            delete(javaDesktopDir.listFiles()?.filter { it.name.endsWith(".jar") })
            println("  Очищена Java desktop версия")
        }
        
        // Очистка log файлов
        val logFiles = listOf(
            "application.log",
            "server.log", 
            "game.log",
            "debug.log",
            "error.log"
        )
        logFiles.forEach { logFile ->
            val file = file(logFile)
            if (file.exists()) {
                delete(file)
            }
        }
        // Удаляем все .log файлы в корне проекта
        delete(fileTree(".").matching { include("*.log") })
        println("  Очищены log файлы")
        
        println("\n" + "=".repeat(50))
        println("  Очистка завершена!")
        println("=".repeat(50) + "\n")
    }
}

// Общая задача для полной сборки проекта с JNI
tasks.register("fullBuild") {
    group = "build"
    description = "Full build: compile all JNI libraries, copy to resources, build project"
    dependsOn("rebuildAllJni", "build")
    
    // Убеждаемся, что JNI библиотеки собираются ПЕРЕД основной сборкой
    tasks.findByName("build")?.mustRunAfter("rebuildAllJni")
}

// Автоматически копировать JNI библиотеки при processResources
// Gradle сам определит, нужна ли пересборка (up-to-date check)
tasks.named("processResources") {
    dependsOn("copyTetrisJniToResources", "copySnakeJniToResources")
}

// Конфигурация Spring Boot
springBoot {
    buildInfo()
}

// Задача для создания исходников JAR (опционально)
tasks.register<Jar>("sourcesJar") {
    archiveClassifier.set("sources")
    from(sourceSets.main.get().allJava)
}

// Конфигурация сборки
tasks.withType<BootJar> {
    duplicatesStrategy = DuplicatesStrategy.EXCLUDE
}

// Конфигурация developmentOnly зависимостей
configurations {
    developmentOnly {
        extendsFrom(configurations.annotationProcessor.get())
    }
}
// Spotless configuration (Google Java Style via Eclipse formatter config)
//spotless {
//    java {
//        target("src/**/*.java")
//        eclipse().configFile(rootProject.file("config/eclipse-java-google-style.xml"))
//    }
//}

// Checkstyle configuration
// Checkstyle временно отключён (A2). Вернуть после починки конфига.
//tasks.withType<Checkstyle> {
//    enabled = false
//}
//
//tasks.check {
//    dependsOn("spotlessCheck")
//}
