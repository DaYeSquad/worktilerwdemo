package com.worktilecore.core.user;

import android.support.annotation.Nullable;


import com.worktilecore.core.api.WebApiWithListResponse;
import com.worktilecore.core.base.WorktileObject;
import com.worktilecore.core.jni.CoreObject;
import com.worktilecore.core.director.Director;

public class UserManager extends CoreObject {

    private @Nullable WebApiWithListResponse mGetAllUsersResponse;

    public static UserManager getInstance() { return Director.getInstance().getUserManager(); }

    public void setNativeHandler(long handler) {
        mNativeHandler = handler;
    }

    @Override
    public void dispose() { }

    @Nullable
    public User fetchUserFromCacheByUid(String uid){
        return nativeFetchUserFromCacheByUid(mNativeHandler, uid);
    }

    public User[] fetchUsersFromCache() {
        return nativeFetchUsersFromCache(mNativeHandler);
    }

    public void getAllUsers(@Nullable WebApiWithListResponse response){
        mGetAllUsersResponse = response;
        nativeGetAllUsers(mNativeHandler);
    }

    public void onGetAllUsers(boolean success, String error, Object[] users){
        if (mGetAllUsersResponse == null){
            return;
        }

        if (success) {
            mGetAllUsersResponse.onSuccess((WorktileObject[])users);
        } else {
            mGetAllUsersResponse.onFailure(error);
        }
    }

    private native @Nullable User nativeFetchUserFromCacheByUid(long handler, String uid);

    private native User[] nativeFetchUsersFromCache(long handler);

    private native void nativeGetAllUsers(long handler);
}