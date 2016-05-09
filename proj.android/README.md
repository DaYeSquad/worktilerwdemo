# Android Demo (2)
# 用于演示完整的 Worktile Android app 网络请求以及缓存的过程

# 环境
见 Gradle

# C++ 部分
相对于上一节，本节我们添加了核心类 Director 用于表示 "我"，用其可以 signIn 等。我们还使用了 [Model Builder](https://github.com/DaYeSquad/cpp-core-model-builder)来构建出 User, UserManager，WebApi 并将其复制到 core 之下。其中，WebApi 用于表示所有的网络请求，UserManager 用于调用网络请求并完成对应的缓存操作。

# Java/JNI 部分
相对于上一节，本节我们添加了 Director，意义同 C++ 的 Director，同样，Java/JNI 的代码也是由 Model Builder 构建出来的，不过在应用层调用的时候，我们通常是在一个 activity 的 onStart 中先用 UserManager.fetchUsersFromCache 取出我们在该界面上需要的缓存，然后调用 UserManager.getAllUsers 获取最新的用户数据(同时在 C++ 层 UserManager 会更新对应的缓存)，以此来达到界面上始终有可以向用户展示的缓存数据并且第一时间获取最新数据的结果。

