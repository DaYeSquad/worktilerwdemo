/*
 * jni_helper.cc
 *
 *  Created on: Jan 25, 2015
 *      Author: FrankLin
 */

#include "jni_helper.h"

#include "jni_ref_cache.h"


NS_WTC_BEGIN;

////////////////////////////////////////////////////////////////////////////////
// JniHelper, C:

static pthread_key_t k_jvm;

void detach_current_thread(void* parameter) {
  JniHelper::GetJavaVm()->DetachCurrentThread();
}

////////////////////////////////////////////////////////////////////////////////
// JniHelper, public:

// JavaVM & JNIEnv --------------------------------------------------------

static JavaVM* shared_java_vm_ = nullptr;


std::string JniHelper::StringFromJstring(jstring jstr) {
  if (jstr == nullptr) {
    return "";
  }

  JNIEnv* env = GetJniEnv();
  if (!env) {
    return nullptr;
  }

  const char* chars = env->GetStringUTFChars(jstr, nullptr);
  std::string ret(chars);
  env->ReleaseStringUTFChars(jstr, chars);

  return ret;
}

jstring JniHelper::JstringFromString(const std::string& str) {
  JNIEnv* env = GetJniEnv();
  if (!env) {
    return nullptr;
  }
  return env->NewStringUTF(str.c_str());
}

std::vector<std::string> JniHelper::StringVectorFromJobjectArray(
    jobjectArray jobjAry) {

  std::vector<std::string> string_vector;

  if (jobjAry == nullptr) {
    return string_vector;
  }

  JNIEnv* env = GetJniEnv();
  if (!env) {
    return string_vector;
  }

  jsize size = env->GetArrayLength(jobjAry);

  for (int i = 0; i < size; i++) {
    jstring jstr = (jstring) env->GetObjectArrayElement(jobjAry, i);
    std::string str = StringFromJstring(jstr);
    string_vector.push_back(str);
    env->DeleteLocalRef(jstr);
  }
  return string_vector;
}

jobjectArray JniHelper::JStringObjectArrayFromStringVector(const std::vector<std::string>& strs) {
  JNIEnv* env = GetJniEnv();
  if (!env) {
    return nullptr;
  }
  jobjectArray jstrs = env->NewObjectArray(strs.size(), env->FindClass("java/lang/String"), env->NewStringUTF(""));
  for (int i = 0; i < strs.size(); ++i) {
    jstring jStr = env->NewStringUTF(strs[i].c_str());
    env->SetObjectArrayElement(jstrs, i, jStr);
    env->DeleteLocalRef(jStr);
  }
  return jstrs;
}

void JniHelper::SetJavaVm(JavaVM* jvm) {
  shared_java_vm_ = jvm;

  pthread_key_create(&k_jvm, detach_current_thread);
}

JavaVM* JniHelper::GetJavaVm() {
  return shared_java_vm_;
}

JNIEnv* JniHelper::GetJniEnv() {
  JNIEnv* env = reinterpret_cast<JNIEnv*>(pthread_getspecific(k_jvm));
  if (env == nullptr) {
    env = CacheJniEnv(shared_java_vm_);
  }
  return env;
}

bool JniHelper::GetStaticMethodInfoOrDie(JniMethodInfo* methodinfo,
                                         const char* class_name,
                                         const char* method_name,
                                         const char* param_code) {
  if ((class_name == nullptr) || (method_name == nullptr)
      || (param_code == nullptr)) {
    sakura::log_error("you nullptr");
    return false;
  }

  JNIEnv* env = GetJniEnv();
  if (!env) {
    sakura::log_error("Failed to get JNIEnv");
    return false;
  }

  jclass class_id = env->FindClass(class_name);
  if (!class_id) {
    sakura::log_error("Failed to find class %s", class_name);
    return false;
  }

  jmethodID method_id = env->GetStaticMethodID(class_id, method_name,
                                               param_code);
  if (!method_id) {
    sakura::log_error("Failed to find static method %s", method_name);
    return false;
  }

  if (methodinfo) {
    methodinfo->class_id = class_id;
    methodinfo->env = env;
    methodinfo->method_id = method_id;
  }

  return true;
}

bool JniHelper::GetMethodInfoOrDie(JniMethodInfo* methodinfo,
                                   const char* class_name,
                                   const char* method_name,
                                   const char* param_code) {
  if ((class_name == nullptr) || (method_name == nullptr)
      || (param_code == nullptr)) {
    return false;
  }

  JNIEnv* env = GetJniEnv();
  if (!env) {
    sakura::log_error("Failed to get JNIEnv");
    return false;
  }

  jclass class_id = env->FindClass(class_name);
  if (!class_id) {
    sakura::log_error("Failed to find class %s", class_name);
    return false;
  }

  jmethodID method_id = env->GetMethodID(class_id, method_name, param_code);
  if (!method_id) {
    sakura::log_error("Failed to find method %s", method_name);
    return false;
  }

  if (methodinfo) {
    methodinfo->class_id = class_id;
    methodinfo->env = env;
    methodinfo->method_id = method_id;
  }

  return true;
}

// WorktileObjects --------------------------------------------------------

jobject JniHelper::GetJUserByCoreUser(const User& user) {
  JNIEnv* env = GetJniEnv();
  if (!env) {
    sakura::log_error("Failed to get JNIEnv");
    return nullptr;
  }

  jclass userJclass = JniReferenceCache::SharedCache()->user_jclass();
  jmethodID userConstructorMethodID = env->GetMethodID(userJclass, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V");

  jstring jUid = env->NewStringUTF(user.uid().c_str());
  jstring jUsername = env->NewStringUTF(user.username().c_str());
  jstring jDisplayName = env->NewStringUTF(user.display_name().c_str());
  jint jStatus = static_cast<jint>(user.status());

  jobject jUserObject = env->NewObject(userJclass,
                                       userConstructorMethodID,
                                       jUid,
                                       jUsername,
                                       jDisplayName,
                                       jStatus);
  env->DeleteLocalRef(jUid);
  env->DeleteLocalRef(jUsername);
  env->DeleteLocalRef(jDisplayName);

  return jUserObject;
}

jobjectArray JniHelper::GetJUsersArrayByCoreUsers(const std::vector<std::unique_ptr<User>>& users) {
  jclass userJclass = JniReferenceCache::SharedCache()->user_jclass();
  JNIEnv* env = GetJniEnv();
  if (!env) {
    return env->NewObjectArray(0, userJclass, NULL);
  }

  jobjectArray jobjs = env->NewObjectArray(users.size(), userJclass, NULL);

  jsize i = 0;
  for (auto it = users.begin(); it != users.end(); ++it) {
    jobject jUser = GetJUserByCoreUser(**it);
    env->SetObjectArrayElement(jobjs, i, jUser);
    env->DeleteLocalRef(jUser);
    ++i;
  }
  return jobjs;
}

////////////////////////////////////////////////////////////////////////////////
// JniHelper, private:

JNIEnv* JniHelper::CacheJniEnv(JavaVM* jvm) {
  JNIEnv* env = nullptr;
  jint ret = jvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);

  switch (ret) {
    case JNI_OK:
      pthread_setspecific(k_jvm, env);
      return env;

    case JNI_EDETACHED: {
      //Thread not attached
      if (jvm->AttachCurrentThread(&env, nullptr) != JNI_OK) {
        sakura::log_error("Failed to get the environment using AttachCurrentThread()");
        return nullptr;
      } else {
        //success to attach and obtain JNIEnv
        pthread_setspecific(k_jvm, env);
        return env;
      }
    }

    case JNI_EVERSION:
      sakura::log_error("JNI version 1.6 not supported");
      return nullptr;

    default:
      sakura::log_error("Failed to get the environment using GetEnv()");
      return nullptr;
  }
}

NS_WTC_END
