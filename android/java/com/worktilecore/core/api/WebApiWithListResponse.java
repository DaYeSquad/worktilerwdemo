package com.worktilecore.core.api;


import android.support.annotation.WorkerThread;

import com.worktilecore.core.base.WorktileObject;

public abstract class WebApiWithListResponse extends WebApiResponse {

    @WorkerThread
    public abstract void onSuccess(WorktileObject[] response);

    @Override
    public void onSuccess() {}
}
