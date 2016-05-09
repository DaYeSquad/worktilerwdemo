/*
 * jni_helper.h
 *
 *  Created on: Jan 25, 2015
 *      Author: FrankLin
 */

#ifndef WORKTILECORE_CORE_UTILS_ANDROID_JNI_HELPER_H_
#define WORKTILECORE_CORE_UTILS_ANDROID_JNI_HELPER_H_

#include <jni.h>

#include <string>
#include <memory>
#include <vector>

#include "base/base.h"
#include "user/user.h"

NS_WTC_BEGIN

typedef struct JniMethodInfo_ {
  JNIEnv* env;
  jclass class_id;
  jmethodID method_id;
} JniMethodInfo;

//JNI interface helper.
class JniHelper {
 public:

  // JavaVM & JNIEnv --------------------------------------------------------

  static void SetJavaVm(JavaVM* jvm);
  static JavaVM* GetJavaVm();

  static JNIEnv* GetJniEnv();

  static bool GetStaticMethodInfoOrDie(JniMethodInfo* methodinfo,
                                       const char* class_name,
                                       const char* method_name,
                                       const char* param_code);

  static bool GetMethodInfoOrDie(JniMethodInfo* methodinfo,
                                 const char* class_name,
                                 const char* method_name,
                                 const char* param_code);

  // Utils --------------------------------------------------------

  static std::string StringFromJstring(jstring str);

  static jstring JstringFromString(const std::string& str);

  static std::vector<std::string> StringVectorFromJobjectArray(jobjectArray jobjAry);

  static jobjectArray JStringObjectArrayFromStringVector(const std::vector<std::string>& strs);

  template<class T>
  static jlongArray JlongArrayFromNativeArray(std::vector<std::unique_ptr<T>> native_array) {
    size_t num_team = native_array.size();
    jlong* team_buf = reinterpret_cast<jlong*>(malloc(
        static_cast<unsigned long>(num_team * sizeof(jlong))));

    if (num_team > 0) {
      std::unique_ptr<T>* raw_teams = &native_array[0];
      for (int i = 0; i < num_team; ++i) {
        team_buf[i] = reinterpret_cast<jlong>(raw_teams[i].release());
      }
    }

    JNIEnv* env = GetJniEnv();

    jlongArray ret = env->NewLongArray(num_team);
    env->SetLongArrayRegion(ret, 0, num_team, team_buf);

    free(team_buf);
    return ret;
  }

  // WorktileObjects --------------------------------------------------------

  static jobject GetJUserByCoreUser(const User& user);

  static jobjectArray GetJUsersArrayByCoreUsers(const std::vector<std::unique_ptr<User>>& users);

 private:
  static JNIEnv* CacheJniEnv(JavaVM* jvm);
};

NS_WTC_END

#endif /* WORKTILECORE_CORE_UTILS_ANDROID_JNI_HELPER_H_ */
