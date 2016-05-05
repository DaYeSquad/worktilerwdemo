#include "user.h"

#include "json11/json11.hpp"

using std::string;
using std::unique_ptr;
using std::vector;

NS_WTC_BEGIN

////////////////////////////////////////////////////////////////////////////////
// User, public:

// Creation and lifetime --------------------------------------------------------

User::User() {}

User::~User() {}

void User::Init(const std::string& uid, const std::string& username, const std::string& display_name, User::Status status) {
  uid_ = uid;
  username_ = username;
  display_name_ = display_name;
  status_ = status;
}

std::unique_ptr<User> User::Clone() const {
  std::unique_ptr<User> user(new User());
  user->Init(uid_, username_, display_name_, status_);
  return user;
}

// Coding interface --------------------------------------------------------

bool User::InitWithJsonOrDie(const std::string& json) {
  string error;
  json11::Json json_obj = json11::Json::parse(json, error);

  if (!error.empty()) {
    sakura::log_error("User InitWithJson died");
    return false;
  }

  uid_ = json_obj["_id"].string_value();
  username_ = json_obj["username"].string_value();
  display_name_ = json_obj["display_name"].string_value();
  status_ = static_cast<User::Status>(json_obj["status"].int_value());

  return true;
}

NS_WTC_END

