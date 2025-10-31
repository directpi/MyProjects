plugins {
    id("com.gradleup.shadow") version "8.3.2"
    id("java")
    id("application")
}

group = "org.example"

version = "1.0-SNAPSHOT"
description "Roglike"

repositories {
    mavenCentral()
}

dependencies {
    implementation("com.google.code.gson:gson:2.10.1")
    implementation ("com.fasterxml.jackson.core:jackson-databind:2.13.3")
    compileOnly("org.projectlombok:lombok:1.18.34")
    annotationProcessor("org.projectlombok:lombok:1.18.34")
    testImplementation(platform("org.junit:junit-bom:5.10.0"))
    testImplementation("org.junit.jupiter:junit-jupiter")
    implementation("com.baulsupp.kolja:jcurses:0.9.5.3")
//    implementation(files("libs/native/jcurses.jar"))
}
java {
    toolchain {
        languageVersion.set(JavaLanguageVersion.of(21)) // Убедитесь, что версия соответствует установленной версии JDK
    }
    sourceCompatibility = JavaVersion.VERSION_21
    targetCompatibility = JavaVersion.VERSION_21
}

tasks.test {

   useJUnitPlatform()
}
tasks.shadowJar {
    archiveBaseName.set("Rogue")
    archiveClassifier.set("")
    archiveVersion.set("1.0")
}

application {
    // Укажите главный класс вашего приложения
    mainClass.set("org.example.Main")
}