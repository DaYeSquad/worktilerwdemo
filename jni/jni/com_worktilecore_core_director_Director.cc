#include <jni.h>

#include "director/director.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jlong JNICALL
Java_com_worktilecore_core_director_Director_nativeCreateDirector(JNIEnv *env, jobject instance) {
  worktile::Director* director = new worktile::Director();
  return reinterpret_cast<jlong>(director);
}

JNIEXPORT void JNICALL
Java_com_worktilecore_core_director_Director_nativeDisposeDirector(JNIEnv *env,
                                                                   jobject instance,
                                                                   jlong handler) {
  worktile::Director* director = reinterpret_cast<worktile::Director*>(handler);
  WTC_SAFE_DELETE(director);
}

JNIEXPORT void JNICALL
Java_com_worktilecore_core_director_Director_nativeInit(JNIEnv *env,
                                                        jobject instance,
                                                        jlong handler) {
  // 此处仅是事例，并未构建 WebApiConfiguration，如果构建，在此处应当传入 WebApiConfiguration 构建的必要参数
  // 可以是多个 String 的形式也可以直接传入 JSON
  worktile::WebApiConfiguration* config = new worktile::WebApiConfiguration();
  worktile::Director* director = reinterpret_cast<worktile::Director*>(handler);
  director->InitAsStranger(*config);
}

JNIEXPORT void JNICALL
Java_com_worktilecore_core_director_Director_nativeResetDefaultInstance(JNIEnv *env,
                                                                        jclass type,
                                                                        jlong handler) {
  worktile::Director* director = reinterpret_cast<worktile::Director*>(handler);
  worktile::Director::SetDefaultDirector(director);
}

JNIEXPORT jlong JNICALL
Java_com_worktilecore_core_director_Director_nativeGetUserManagerHandler(JNIEnv *env,
                                                                         jobject instance,
                                                                         jlong handler) {

  worktile::Director* director = reinterpret_cast<worktile::Director*>(handler);
  return reinterpret_cast<jlong>(director->user_manager());
}

#ifdef __cplusplus
}
#endif

