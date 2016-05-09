package com.worktilecore.core.api;

/**
 * 该类是调用web api的回调
 * <p>继承该类的子类都应该实现onFailure方法，并且在onFailure方法中调用super.onFailure()</p>*/
public abstract class WebApiResponse {

    /**注意：该方法是在子线程中调用的(2015-12-29以后的代码，使用了Looper.getMainLooper, 所以在主线程中)*/
    public abstract void onSuccess();

    /**
     * 返回:事件是否被消耗  true:被消耗，不再继续  false:继续
     * <p>注意:该方法是在子线程中调用的(2015-12-29以后的代码，使用了Looper.getMainLooper, 所以在主线程中)</p>
     * <p>如果返回的错误码是SIGNATURE_ERROR,说明登录凭证失效，app跳转到登录页面，重新登录</p>
     */
    public boolean onFailure(final String error) { return true; }
}
