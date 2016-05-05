#include "user_manager.h"
#include "director/director.h"

using std::string;
using std::unique_ptr;
using std::vector;

using sakura::FileUtils;


NS_WTC_BEGIN

// SQLite schema --------------------------------------------------------

static std::string const kUid = "uid";
static std::string const kUsername = "username";
static std::string const kDisplayName = "display_name";
static std::string const kStatus = "status";

static std::string const kSqlAnd = " AND ";

static sql::Field definition_users[] = {
  sql::Field(kUid, sql::type_text, sql::flag_primary_key),
  sql::Field(kUsername, sql::type_text, sql::flag_not_null),
  sql::Field(kDisplayName, sql::type_text, sql::flag_not_null),
  sql::Field(kStatus, sql::type_int, sql::flag_not_null),
  sql::Field(sql::DEFINITION_END),
};

////////////////////////////////////////////////////////////////////////////////
// UserManager, public:

// Creation and lifetime --------------------------------------------------------

UserManager::UserManager(Director* director)
:ObjectManager(director) {
}

UserManager::~UserManager() {
}

bool UserManager::InitOrDie() {
  bool success = true;
  
  do {
    users_tb_ = unique_ptr<sql::Table>(new sql::Table(MainDatabaseHandler(), "users", definition_users));
    
    if (!users_tb_->exists()) {
      success = users_tb_->create();
      WTC_ASSERT(success);
    }
    
  } while(0);
  
  return success;
}

const UserManager* UserManager::DefaultManager() {
  return Director::DefaultDirector()->user_manager();
}

// HTTP --------------------------------------------------------

void UserManager::GetAllUsers(std::function<void(bool success, const std::string& error, std::vector<std::unique_ptr<User>> users)> callback) const {
  WebApi::Api()->GetAllUsers([this, callback](bool success, const std::string &error, std::vector<std::unique_ptr<User>> users) {
    if (success) {
      this->SaveUsersToCache(users);
    }
    callback(success, error, std::move(users));
  });
}

// Persisent store --------------------------------------------------------

void UserManager::SaveUsersToCache(const std::vector<std::unique_ptr<User>>& users) const {
  LockMainDatabase();
  
  BeginTransaction();
  
  for (auto it = users.begin(); it != users.end(); ++it) {
    UnsafeSaveUserToCache(**it);
  }
  
  CommitTransaction();
  
  UnlockMainDatabase();
}

std::unique_ptr<User> UserManager::FetchUserFromCacheByUid(const std::string& uid) const {
  string where_condition = kUid + "='" + uid + "'";
  
  WTC_DB_LOCK_GUARD;
  
  users_tb_->open(where_condition);
  if (users_tb_->recordCount() != 0) {
    sql::Record* record = users_tb_->getRecord(0);
    unique_ptr<User> rtn(UserFromRecord(record));
    return rtn;
  }
  
  return nullptr;
}

std::vector<std::unique_ptr<User>> UserManager::FetchUsersFromCache() const {
  
  vector<unique_ptr<User>> users;
  
  string where_condition = "";
  
  LockMainDatabase();
  
  users_tb_->open(where_condition);
  
  for (int i = 0; i < users_tb_->recordCount(); ++i) {
    sql::Record* record = users_tb_->getRecord(i);
    users.push_back(UserFromRecord(record));
  }
  
  UnlockMainDatabase();
  
  return users;
}

////////////////////////////////////////////////////////////////////////////////
// UserManager, private:

// Utils --------------------------------------------------------

void UserManager::UnsafeSaveUserToCache(const User& user) const {
  sql::Record record = RecordByUser(user);
  users_tb_->addOrReplaceRecord(&record);
}

sql::Record UserManager::RecordByUser(const User& user) const {
  sql::Record record(users_tb_->fields());
  
  record.setString(kUid, user.uid());
  record.setString(kUsername, user.username());
  record.setString(kDisplayName, user.display_name());
  record.setInteger(kStatus, static_cast<int>(user.status()));
  
  return record;
}

std::unique_ptr<User> UserManager::UserFromRecord(sql::Record* record) const {
  std::string uid = record->getValue(kUid)->asString();
  std::string username = record->getValue(kUsername)->asString();
  std::string display_name = record->getValue(kDisplayName)->asString();
  User::Status status = static_cast<User::Status>(record->getValue(kStatus)->asInteger());
  
  unique_ptr<User> user(new User());
  user->Init(uid, username, display_name, status);
  return user;
}

NS_WTC_END

