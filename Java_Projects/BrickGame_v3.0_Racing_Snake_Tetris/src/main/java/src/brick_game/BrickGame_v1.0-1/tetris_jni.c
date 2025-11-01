/**
 * @file: tetris_jni.c
 * @description: Минимальная JNI обёртка для Tetris C библиотеки (с глобальной структурой)
 * @dependencies: jni.h, s21_tetris_back.h
 * @created: 2025-10-03
 */
#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include "brick_game/tetris/s21_tetris_back.h"

/*
 * Class:     src_brick_game_tetris_jni_TetrisJniBridge
 * Method:    initGameNative
 * Signature: ()J
 * 
 * Инициализирует игру через updateCurrentState(Start)
 * Возвращает 1 (фиктивный указатель, т.к. используется глобальная структура)
 */
JNIEXPORT jlong JNICALL Java_src_brick_1game_tetris_jni_TetrisJniBridge_initGameNative
  (JNIEnv *env, jobject obj) {
    // Инициализируем игру через FSM
    GameInfo_t *game = updateCurrentState(Start);
    
    if (game == NULL || game->field == NULL) {
        return 0; // Ошибка инициализации
    }
    
    // Возвращаем 1 как фиктивный указатель (на самом деле используется глобальная структура)
    return 1;
}

/*
 * Class:     src_brick_game_tetris_jni_TetrisJniBridge
 * Method:    freeGameNative
 * Signature: (J)V
 * 
 * Освобождает ресурсы игры
 */
JNIEXPORT void JNICALL Java_src_brick_1game_tetris_jni_TetrisJniBridge_freeGameNative
  (JNIEnv *env, jobject obj, jlong gamePtr) {
    if (gamePtr == 0) return;
    
    // Вызываем Terminate для корректного завершения
    GameInfo_t *game = updateCurrentState(Terminate);
    
    // Освобождаем память и сбрасываем поля в NULL для следующей инициализации
    if (game != NULL) {
        memory_free(game);
        game->field = NULL;
        game->next = NULL;
    }
}

/*
 * Helper: копирование field из C в Java
 */
static void copyFieldToJava(JNIEnv *env, GameInfo_t *game, jobject gameInfoData) {
    if (game == NULL || game->field == NULL) return;
    
    jclass cls = (*env)->GetObjectClass(env, gameInfoData);
    jfieldID fieldId = (*env)->GetFieldID(env, cls, "field", "[[I");
    
    // Создаём Java массив 20x10
    jclass intArrayClass = (*env)->FindClass(env, "[I");
    jobjectArray field = (*env)->NewObjectArray(env, 20, intArrayClass, NULL);
    
    for (int y = 0; y < 20; y++) {
        jintArray row = (*env)->NewIntArray(env, 10);
        jint tempRow[10];
        
        for (int x = 0; x < 10; x++) {
            tempRow[x] = game->field[y][x];
        }
        
        (*env)->SetIntArrayRegion(env, row, 0, 10, tempRow);
        (*env)->SetObjectArrayElement(env, field, y, row);
        (*env)->DeleteLocalRef(env, row);
    }
    
    (*env)->SetObjectField(env, gameInfoData, fieldId, field);
    (*env)->DeleteLocalRef(env, field);
}

/*
 * Helper: копирование next из C в Java
 */
static void copyNextToJava(JNIEnv *env, GameInfo_t *game, jobject gameInfoData) {
    if (game == NULL || game->next == NULL) return;
    
    jclass cls = (*env)->GetObjectClass(env, gameInfoData);
    jfieldID nextId = (*env)->GetFieldID(env, cls, "next", "[[I");
    
    jclass intArrayClass = (*env)->FindClass(env, "[I");
    jobjectArray next = (*env)->NewObjectArray(env, 4, intArrayClass, NULL);
    
    for (int y = 0; y < 4; y++) {
        jintArray row = (*env)->NewIntArray(env, 4);
        jint tempRow[4];
        
        for (int x = 0; x < 4; x++) {
            tempRow[x] = game->next[y][x];
        }
        
        (*env)->SetIntArrayRegion(env, row, 0, 4, tempRow);
        (*env)->SetObjectArrayElement(env, next, y, row);
        (*env)->DeleteLocalRef(env, row);
    }
    
    (*env)->SetObjectField(env, gameInfoData, nextId, next);
    (*env)->DeleteLocalRef(env, next);
}

/*
 * Helper: заполнение GameInfoData из C структуры
 */
static void fillGameInfoData(JNIEnv *env, GameInfo_t *game, jobject gameInfoData) {
    if (game == NULL) return;
    
    jclass cls = (*env)->GetObjectClass(env, gameInfoData);
    
    // Копируем массивы
    copyFieldToJava(env, game, gameInfoData);
    copyNextToJava(env, game, gameInfoData);
    
    // Копируем скалярные поля
    jfieldID scoreId = (*env)->GetFieldID(env, cls, "score", "I");
    (*env)->SetIntField(env, gameInfoData, scoreId, game->score);
    
    jfieldID highScoreId = (*env)->GetFieldID(env, cls, "highScore", "I");
    (*env)->SetIntField(env, gameInfoData, highScoreId, game->high_score);
    
    jfieldID levelId = (*env)->GetFieldID(env, cls, "level", "I");
    (*env)->SetIntField(env, gameInfoData, levelId, game->level);
    
    jfieldID speedId = (*env)->GetFieldID(env, cls, "speed", "I");
    (*env)->SetIntField(env, gameInfoData, speedId, game->speed);
    
    jfieldID pauseId = (*env)->GetFieldID(env, cls, "pause", "I");
    (*env)->SetIntField(env, gameInfoData, pauseId, game->pause);
}

/*
 * Class:     src_brick_game_tetris_jni_TetrisJniBridge
 * Method:    updateStateNative
 * Signature: (JI)Lsrc/brick_game/tetris/jni/GameInfoData;
 * 
 * Обновляет состояние игры на основе пользовательского действия
 */
JNIEXPORT jobject JNICALL Java_src_brick_1game_tetris_jni_TetrisJniBridge_updateStateNative
  (JNIEnv *env, jobject obj, jlong gamePtr, jint action) {
    if (gamePtr == 0) return NULL;
    
    // Преобразуем action в UserAction_t
    UserAction_t userAction;
    switch (action) {
        case 0: userAction = Start; break;
        case 1: userAction = Pause; break;
        case 2: userAction = Terminate; break;
        case 3: userAction = Left; break;
        case 4: userAction = Right; break;
        case 5: userAction = Up; break;
        case 6: userAction = Down; break;
        case 7: userAction = Action; break;
        default: userAction = -1; break; // No action
    }
    
    // Вызываем C функцию обновления состояния (работает с глобальной структурой)
    GameInfo_t *game = updateCurrentState(userAction);
    
    if (game == NULL) return NULL;
    
    // Создаём объект GameInfoData
    jclass cls = (*env)->FindClass(env, "src/brick_game/tetris/jni/GameInfoData");
    if (cls == NULL) {
        fprintf(stderr, "[JNI] Error: Cannot find GameInfoData class\n");
        return NULL;
    }
    
    jmethodID constructor = (*env)->GetMethodID(env, cls, "<init>", "()V");
    if (constructor == NULL) {
        fprintf(stderr, "[JNI] Error: Cannot find GameInfoData constructor\n");
        return NULL;
    }
    
    jobject result = (*env)->NewObject(env, cls, constructor);
    if (result == NULL) {
        fprintf(stderr, "[JNI] Error: Cannot create GameInfoData object\n");
        return NULL;
    }
    
    // Заполняем данными
    fillGameInfoData(env, game, result);
    
    return result;
}

/*
 * Class:     src_brick_game_tetris_jni_TetrisJniBridge
 * Method:    getStateNative
 * Signature: (J)Lsrc/brick_game/tetris/jni/GameInfoData;
 * 
 * Получает текущее состояние без обновления
 */
JNIEXPORT jobject JNICALL Java_src_brick_1game_tetris_jni_TetrisJniBridge_getStateNative
  (JNIEnv *env, jobject obj, jlong gamePtr) {
    if (gamePtr == 0) return NULL;
    
    // Вызываем updateCurrentState с Start (но игра уже инициализирована, 
    // так что это просто вернет текущее состояние без изменений)
    // Используем действие Start, которое безопасно для уже запущенной игры
    GameInfo_t *game = updateCurrentState(Start);
    
    if (game == NULL) return NULL;
    
    // Создаём объект GameInfoData
    jclass cls = (*env)->FindClass(env, "src/brick_game/tetris/jni/GameInfoData");
    jmethodID constructor = (*env)->GetMethodID(env, cls, "<init>", "()V");
    jobject result = (*env)->NewObject(env, cls, constructor);
    
    // Заполняем данными
    fillGameInfoData(env, game, result);
    
    return result;
}
