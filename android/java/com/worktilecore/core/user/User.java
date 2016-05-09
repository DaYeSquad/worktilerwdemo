package com.worktilecore.core.user;

import android.support.annotation.IntDef;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

import com.worktilecore.core.base.WorktileObject;

/**
 * 该部分代码由 Model Builder 直接生成
 */
public final class User extends WorktileObject {

    @Retention(RetentionPolicy.SOURCE)
    @IntDef({STATUS_NORMAL, STATUS_INVITED, STATUS_PENDING})
    public @interface Status {}
    public static final int STATUS_NORMAL = 0;
    public static final int STATUS_INVITED = 1;
    public static final int STATUS_PENDING = 2;

    private final String mUid;
    private final String mUsername;
    private final String mDisplayName;
    private final @User.Status int mStatus;

    /*package*/ User(String uid,
                     String username,
                     String displayName,
                     @User.Status int status) {
        mUid = uid;
        mUsername = username;
        mDisplayName = displayName;
        mStatus = status;
    }

    public String getUid() {
        return mUid;
    }

    public String getUsername() {
        return mUsername;
    }

    public String getDisplayName() {
        return mDisplayName;
    }

    @Status
    public int getStatus() {
        return mStatus;
    }

}
