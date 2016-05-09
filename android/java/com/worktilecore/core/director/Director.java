
package com.worktilecore.core.director;

import android.os.Handler;
import android.os.Looper;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import com.worktilecore.core.jni.CoreObject;
import com.worktilecore.core.jni.JniHelper;
import com.worktilecore.core.user.UserManager;

/**
 * Director 代表 App 的控制器，其内拥有 UserManger
 */
public class Director extends CoreObject {

    private static Director sInstance;

    private UserManager mUserManager;

    public Director() {
        mNativeHandler = nativeCreateDirector();
    }

    @Override
    public void dispose() {
        nativeDisposeDirector(mNativeHandler);
    }

    /**
     * 返回全局的 instance
     * @return 全局单例 director
     */
    @Nullable
    public static Director getInstance() {
        return sInstance;
    }

    /**
     * 重置全局 instance, 如果为 null, 则删除全局的 instance
     * @param director instance of director, 可以为空
     */
    public static void resetInstance(@Nullable Director director) {
        if (director == null) {
            nativeResetDefaultInstance(JniHelper.NULLPTR);
            sInstance = null;
        } else {
            sInstance = director;
            nativeResetDefaultInstance(sInstance.mNativeHandler);
        }
    }

    /**
     * 初始化 director 为登录前的状态
     */
    public void initAsStranger(){
        nativeInit(mNativeHandler);
    }

    public void signIn() {
        // 在登录成功之后初始化 manager
        initObjectManager();
    }

    private void initObjectManager(){
        mUserManager = new UserManager();
        mUserManager.setNativeHandler(nativeGetUserManagerHandler(mNativeHandler));
    }

    /**
     * 获取用户信息相关的 manager
     * @return UserManager
     */
    public @NonNull UserManager getUserManager(){
        return mUserManager;
    }


    private native long nativeCreateDirector();

    private native void nativeDisposeDirector(long handler);

    private native void nativeInit(long handler);

    private static native void nativeResetDefaultInstance(long handler);

    private native long nativeGetUserManagerHandler(long handler);
}
