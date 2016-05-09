#include <jni.h>

#include "user/user_manager.h"
#include "utils/android/jni_helper.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jobject JNICALL Java_com_worktilecore_core_user_UserManager_nativeFetchUserFromCacheByUid
  (JNIEnv *env, jobject thiz, jlong handler, jstring uid){
  const worktile::UserManager* coreManager = reinterpret_cast<worktile::UserManager*>(handler);

  std::string cppuid = worktile::JniHelper::StringFromJstring(uid);

  std::unique_ptr<worktile::User> coreObject = coreManager->FetchUserFromCacheByUid(cppuid);

  if(coreObject == nullptr){
    return NULL;
  }
  return worktile::JniHelper::GetJUserByCoreUser(*coreObject);
}

JNIEXPORT jobjectArray JNICALL Java_com_worktilecore_core_user_UserManager_nativeFetchUsersFromCache
  (JNIEnv *env, jobject thiz, jlong handler){
  const worktile::UserManager* coreManager = reinterpret_cast<worktile::UserManager*>(handler);

  std::vector<std::unique_ptr<worktile::User>> coreObjects = coreManager->FetchUsersFromCache();

  return worktile::JniHelper::GetJUsersArrayByCoreUsers(coreObjects);
}

JNIEXPORT void JNICALL Java_com_worktilecore_core_user_UserManager_nativeGetAllUsers
  (JNIEnv* env, jobject thiz, jlong handler) {
  const worktile::UserManager* core_manager = reinterpret_cast<worktile::UserManager*>(handler);

  jobject global_thiz = env->NewGlobalRef(thiz);
  jobject global_null = env->NewGlobalRef(NULL);


  core_manager->GetAllUsers([env, global_thiz, global_null](bool success,
                                                            worktile::WebApi::Error error,
                                                            std::vector<std::unique_ptr<worktile::User>> users){
    JNIEnv *jni_env = worktile::JniHelper::GetJniEnv();
    jclass jclazz = jni_env->GetObjectClass(global_thiz);

    if(jclazz == NULL) {
      return;
    }
    jmethodID method_id = jni_env->GetMethodID(jclazz, "onGetAllUsers", "(ZLjava/lang/String;[Ljava/lang/Object;)V");
    if(method_id == NULL) {
      return;
    }
    jstring jerror = jni_env->NewStringUTF(error.description.c_str());

    if (success){
      jni_env->CallVoidMethod(global_thiz, method_id, success, jerror,
                              worktile::JniHelper::GetJUsersArrayByCoreUsers(users));
    } else {
      jni_env->CallVoidMethod(global_thiz, method_id, success, jerror, global_null);
    }

    jni_env->DeleteGlobalRef(global_thiz);
    jni_env->DeleteGlobalRef(global_null);
  });
}

#ifdef __cplusplus
}
#endif
