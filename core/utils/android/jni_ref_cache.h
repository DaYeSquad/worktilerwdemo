/*
 * jni_ref_cache.h
 *
 *  Created on: May 21, 2015
 *      Author: FrankLin
 */

#ifndef LESSCORE_CORE_UTILS_ANDROID_JNI_REF_CACHE_H_
#define LESSCORE_CORE_UTILS_ANDROID_JNI_REF_CACHE_H_

#include "base/base.h"

#include "utils/android/jni_helper.h"
#include "network/android/http_client_android.h"

NS_LCC_BEGIN

class JniReferenceCache {
 public:
  JniReferenceCache();
  virtual ~JniReferenceCache();

  static JniReferenceCache* SharedCache();
  static void PurgeCache();

  void CacheHttpClientCallbackMethod();
  void HandleCallbackOnMainThread(JNIEnv* env, HttpResponseCallback* callback);

  void CacheJclasses(JNIEnv* env);

  jclass director_jclass() const { return director_jclass_; }
  jclass team_jclass() const { return team_jclass_; }
  jclass token_jclass() const { return token_jclass_; }
  jclass user_jclass() const { return user_jclass_; }
  jclass file_manager_jclass() const { return file_manager_jclass_; }
  jclass file_jclass() const { return file_jclass_; }

 private:
  JniMethodInfo* callback_method_info_;

  jclass director_jclass_;
  jclass team_jclass_;
  jclass token_jclass_;
  jclass user_jclass_;
  jclass file_manager_jclass_;
  jclass file_jclass_;


  DISALLOW_COPY_AND_ASSIGN(JniReferenceCache);
};

NS_LCC_END

#endif /* LESSCORE_CORE_UTILS_ANDROID_JNI_REF_CACHE_H_ */
