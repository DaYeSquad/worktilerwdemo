// Copy belows to core/utils/android/jni_helper.h


static jobject GetJUserByCoreUser(const User& user);

static jobjectArray GetJUsersArrayByCoreUsers(const std::vector<std::unique_ptr<User>>& users);



// Copy belows to core/utils/android/jni_helper.cc


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
