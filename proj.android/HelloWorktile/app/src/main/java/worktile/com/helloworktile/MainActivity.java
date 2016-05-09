package worktile.com.helloworktile;

import android.os.Handler;
import android.support.annotation.UiThread;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.worktilecore.core.api.WebApiWithListResponse;
import com.worktilecore.core.base.WorktileObject;
import com.worktilecore.core.director.Director;
import com.worktilecore.core.user.User;
import com.worktilecore.core.user.UserManager;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("worktilecore");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // 在第一次登录前构建 Stranger 模式的的 Director
        Director director = new Director();
        director.initAsStranger();
        director.signIn(); // 此处仅仅模拟登录，在登录成功之后，可以操作所有的 ObjectManagers

        // 从缓存中取出需要的 users 并显示在列表中
        User[] users = UserManager.getInstance().fetchUsersFromCache();
        displayAllUsers(users);

        // 此时屏幕上已经显示出来了缓存数据，可以使用 HTTP 请求请求数据
        // 在请求成功后，在主线程中显示数据
        UserManager.getInstance().getAllUsers(new WebApiWithListResponse() {
            @Override
            public void onSuccess(WorktileObject[] response) {
                final User[] users = (User[])response;
                Handler mainHandler = new Handler(getMainLooper());
                mainHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        displayAllUsers(users);
                    }
                });
            }
        });
    }

    @UiThread
    void displayAllUsers(User[] users) {
        // display users data as list view.
    }
}
