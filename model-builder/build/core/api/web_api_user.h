// User --------------------------------------------------------

// /api/mobile/users
void GetAllUsers(std::function<void(bool success, const std::string& error, std::vector<std::unique_ptr<User>> users)> callback);

