#ifndef WORKTILECORE_CORE_USER_USER_MANAGER_H_
#define WORKTILECORE_CORE_USER_USER_MANAGER_H_

#include <string>
#include <memory>
#include <vector>
#include <functional>

#include "easySQLite/easySQLite.h"

#include "base/base.h"
#include "director/object_manager.h"
#include "api/web_api.h"

#include "user/user.h"

NS_WTC_BEGIN

class LCC_DLL UserManager : public ObjectManager {
public:

  // Creation and lifetime --------------------------------------------------------

  explicit UserManager(Director* director);

  virtual ~UserManager();

  bool InitOrDie();

  static const UserManager* DefaultManager();

  // HTTP --------------------------------------------------------

  void GetAllUsers(std::function<void(bool success, const std::string& error, std::vector<std::unique_ptr<User>> users)> callback) const;

  // Persisent store --------------------------------------------------------

  void SaveUsersToCache(const std::vector<std::unique_ptr<User>>& users) const;

  std::unique_ptr<User> FetchUserFromCacheByUid(const std::string& uid) const;

  std::vector<std::unique_ptr<User>> FetchUsersFromCache() const;

private:
  std::unique_ptr<sql::Table> users_tb_;

  // Utils --------------------------------------------------------

  sql::Record RecordByUser(const User& user) const;

  void UnsafeSaveUserToCache(const User& user) const;

  std::unique_ptr<User> UserFromRecord(sql::Record* record) const;


  DISALLOW_COPY_AND_ASSIGN(UserManager);
};

NS_WTC_END

#endif /* defined(WORKTILECORE_CORE_USER_USER_MANAGER_H_) */

