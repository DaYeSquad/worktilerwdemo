/*
 * jni_ref_cache.cc
 *
 *  Created on: May 21, 2015
 *      Author: FrankLin
 */

#include "jni_ref_cache.h"

USING_NS_LCC;

static JniReferenceCache* shared_cache_ = nullptr;

JniReferenceCache::JniReferenceCache()
    : callback_method_info_(nullptr) {
}

JniReferenceCache::~JniReferenceCache() {
  JNIEnv* env = JniHelper::GetJniEnv();
  env->DeleteGlobalRef(director_jclass_);
  env->DeleteGlobalRef(team_jclass_);
  env->DeleteGlobalRef(user_jclass_);
  env->DeleteGlobalRef(token_jclass_);
  env->DeleteGlobalRef(file_manager_jclass_);
  env->DeleteGlobalRef(file_jclass_);
}

JniReferenceCache* JniReferenceCache::SharedCache() {
  if (shared_cache_ == nullptr) {
    shared_cache_ = new JniReferenceCache();
  }
  return shared_cache_;
}

void JniReferenceCache::PurgeCache() {
  LCC_SAFE_DELETE(shared_cache_);
}

void JniReferenceCache::CacheHttpClientCallbackMethod() {
  JniMethodInfo* method_info = new JniMethodInfo();
  bool success = JniHelper::GetStaticMethodInfoOrDie(
      method_info, "com/lesschat/core/jni/HttpClientJniHelper",
      "HandleCallbackOnMainThread", "(J)V");
  method_info->class_id = reinterpret_cast<jclass>(method_info->env->NewGlobalRef(method_info->class_id));
  callback_method_info_ = method_info;
}

void JniReferenceCache::HandleCallbackOnMainThread(JNIEnv* env,
    HttpResponseCallback* callback) {
  jlong pointer = reinterpret_cast<jlong>(callback);
  env->CallStaticVoidMethod(
      callback_method_info_->class_id, callback_method_info_->method_id,
      pointer);
}

void JniReferenceCache::CacheJclasses(JNIEnv* env) {
  // director
  jclass local_director_jclass = env->FindClass("com/lesschat/core/director/Director");
  director_jclass_ = reinterpret_cast<jclass>(env->NewGlobalRef(local_director_jclass));

  // team
  jclass local_team_jclass = env->FindClass("com/lesschat/core/team/Team");
  team_jclass_ = reinterpret_cast<jclass>(env->NewGlobalRef(local_team_jclass));

  // token
  jclass local_token_jclass = env->FindClass("com/lesschat/core/api/Token");
  token_jclass_ = reinterpret_cast<jclass>(env->NewGlobalRef(local_token_jclass));

  // user
  jclass local_user_jclass = env->FindClass("com/lesschat/core/user/User");
  user_jclass_ = reinterpret_cast<jclass>(env->NewGlobalRef(local_user_jclass));

  //drive
  jclass local_file_manager_jclass = env->FindClass("com/lesschat/core/drive/FileManager");
  file_manager_jclass_ = reinterpret_cast<jclass>(env->NewGlobalRef(local_file_manager_jclass));

  jclass local_file_jclass = env->FindClass("com/lesschat/core/drive/File");
  file_jclass_ = reinterpret_cast<jclass>(env->NewGlobalRef(local_file_jclass));
}
