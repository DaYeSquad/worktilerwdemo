package com.worktilecore.core.user;

import com.worktilecore.core.api.*;
import com.worktilecore.core.user.User.*;
import com.worktilecore.core.jni.CoreObject;
import com.worktilecore.core.director.Director;
import com.worktilecore.core.jni.JniHelper;

import java.util.ArrayList;
import java.util.List;

public class UserManager extends CoreObject {

    private @Nullable WebApiWithListResponse mGetAllUsersResponse;

    public static UserManager getInstance() { return Director.getInstance().getUserManager(); }

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
        if (success){
            mGetAllUsersResponse.onSuccess(users);
        } else {
            mGetAllUsersResponse.onFailure(error);
        }
    }

    private native @Nullable User nativeFetchUserFromCacheByUid(long handler, String uid);

    private native User[] nativeFetchUsersFromCache(long handler);

    private native void nativeGetAllUsers(long handler);

}