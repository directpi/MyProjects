/**
 * @file: snake_jni.cpp
 * @description: JNI bridge for Snake C++ game logic
 * @dependencies: SnakeController (C++)
 * @created: 2025-10-06
 */

#include <jni.h>
#include "brick_game/snake/controller/s21_controller.hpp"
#include "brick_game/common/common_file.h"
#include "brick_game/common/defines.h"

using namespace s21;

extern "C" {

/*
 * Class:     src_brick_game_snake_jni_SnakeJniBridge
 * Method:    initGameNative
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_src_brick_1game_snake_jni_SnakeJniBridge_initGameNative
  (JNIEnv *env, jobject obj) {
    // Snake использует singleton, просто получаем ссылку
    Game &game = Game::getInstance();
    
    // Если field был освобожден в freeGameNative, нужно переинициализировать
    if (game.getField() == nullptr) {
        game.initField();
    }
    
    // Устанавливаем начальное состояние
    game.setCurrentState(START_STATE);
    game.setUserAction(Start);
    
    // Вызываем updateCurrentState для инициализации игры (spawn)
    updateCurrentState();
    
    // Возвращаем фиктивный указатель (1), т.к. используется singleton
    return 1;
}

/*
 * Class:     src_brick_game_snake_jni_SnakeJniBridge
 * Method:    freeGameNative
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_src_brick_1game_snake_jni_SnakeJniBridge_freeGameNative
  (JNIEnv *env, jobject obj, jlong gamePtr) {
    if (gamePtr == 0) return;
    
    // Для singleton НЕ освобождаем память (он живёт всё время)
    // Просто очищаем игровые данные для следующей игры
    Game &game = Game::getInstance();
    game.setUserAction(Terminate);
    game.setCurrentState(EXIT_STATE);
    // НЕ вызываем game.reset() - это освобождает field!
    
    // Очищаем только игровые объекты
    Snake &snake = Snake::getInstance();
    snake.snakeBody.clear();
    snake.lastTurn = Down;
    snake.snake_direction = {-1, 0};
    snake.justAteApple = false;
    
    // Сбрасываем score и level, но НЕ трогаем field
    game.getGameInfo()->score = 0;
    game.getGameInfo()->level = 1;
    game.getGameInfo()->speed = GAME_SPEED;
}

/*
 * Class:     src_brick_game_snake_jni_SnakeJniBridge
 * Method:    updateStateNative
 * Signature: (JIZ)Lsrc/brick_game/snake/jni/GameInfoData;
 */
JNIEXPORT jobject JNICALL Java_src_brick_1game_snake_jni_SnakeJniBridge_updateStateNative
  (JNIEnv *env, jobject obj, jlong gamePtr, jint actionId, jboolean hold) {
    if (gamePtr == 0) return NULL;
    
    // Преобразуем actionId в UserAction_t
    UserAction_t action = static_cast<UserAction_t>(actionId);
    
    // Отправляем действие в контроллер с параметром hold
    SnakeController::userInput(action, hold == JNI_TRUE);
    
    // Обновляем состояние FSM
    updateCurrentState();
    
    // Получаем текущее состояние игры
    const GameInfo_t *gameInfo = SnakeController::getGameInfo();
    
    // Создаем Java объект GameInfoData
    jclass gameInfoClass = env->FindClass("src/brick_game/snake/jni/GameInfoData");
    if (gameInfoClass == NULL) {
        return NULL;
    }
    
    jmethodID constructor = env->GetMethodID(gameInfoClass, "<init>", "()V");
    if (constructor == NULL) {
        return NULL;
    }
    
    jobject gameInfoObj = env->NewObject(gameInfoClass, constructor);
    if (gameInfoObj == NULL) {
        return NULL;
    }
    
    // Копируем поле field (int[][] в Java)
    jclass intArrayClass = env->FindClass("[I");
    jobjectArray fieldArray = env->NewObjectArray(20, intArrayClass, NULL);
    for (int i = 0; i < 20; i++) {
        jintArray row = env->NewIntArray(10);
        jint *rowData = new jint[10];
        for (int j = 0; j < 10; j++) {
            rowData[j] = gameInfo->field[i][j];
        }
        env->SetIntArrayRegion(row, 0, 10, rowData);
        env->SetObjectArrayElement(fieldArray, i, row);
        env->DeleteLocalRef(row);
        delete[] rowData;
    }
    
    // Копируем поле next (int[][] в Java) - для Snake не используется, заполняем нулями
    jobjectArray nextArray = env->NewObjectArray(4, intArrayClass, NULL);
    for (int i = 0; i < 4; i++) {
        jintArray row = env->NewIntArray(4);
        jint *rowData = new jint[4]{0, 0, 0, 0};
        env->SetIntArrayRegion(row, 0, 4, rowData);
        env->SetObjectArrayElement(nextArray, i, row);
        env->DeleteLocalRef(row);
        delete[] rowData;
    }
    
    // Устанавливаем поля объекта
    jfieldID fieldField = env->GetFieldID(gameInfoClass, "field", "[[I");
    jfieldID nextField = env->GetFieldID(gameInfoClass, "next", "[[I");
    jfieldID scoreField = env->GetFieldID(gameInfoClass, "score", "I");
    jfieldID highScoreField = env->GetFieldID(gameInfoClass, "highScore", "I");
    jfieldID levelField = env->GetFieldID(gameInfoClass, "level", "I");
    jfieldID speedField = env->GetFieldID(gameInfoClass, "speed", "I");
    jfieldID pauseField = env->GetFieldID(gameInfoClass, "pause", "I");
    
    env->SetObjectField(gameInfoObj, fieldField, fieldArray);
    env->SetObjectField(gameInfoObj, nextField, nextArray);
    env->SetIntField(gameInfoObj, scoreField, gameInfo->score);
    env->SetIntField(gameInfoObj, highScoreField, gameInfo->high_score);
    env->SetIntField(gameInfoObj, levelField, gameInfo->level);
    env->SetIntField(gameInfoObj, speedField, gameInfo->speed);
    env->SetIntField(gameInfoObj, pauseField, gameInfo->pause);
    
    return gameInfoObj;
}

} // extern "C"

