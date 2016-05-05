#ifndef WORKTILECORE_CORE_USER_USER_H_
#define WORKTILECORE_CORE_USER_USER_H_

#include <string>
#include <memory>
#include <vector>

#include "base/base.h"

NS_WTC_BEGIN

class LCC_DLL User : public CodingInterface {
public:

  enum class Status {
    NORMAL = 0,
    INVITED = 1,
    PENDING = 2,
  };

  // Creation and lifetime --------------------------------------------------------

  User();

  virtual ~User();

  void Init(const std::string& uid, const std::string& username, const std::string& display_name, User::Status status);

  std::unique_ptr<User> Clone() const;

  // Coding interface --------------------------------------------------------

  virtual bool InitWithJsonOrDie(const std::string& json) OVERRIDE;

  // Getter/Setter --------------------------------------------------------

  std::string uid() const { return uid_; }
  void set_uid(const std::string& uid) { uid_ = uid; }

  std::string username() const { return username_; }
  void set_username(const std::string& username) { username_ = username; }

  std::string display_name() const { return display_name_; }
  void set_display_name(const std::string& display_name) { display_name_ = display_name; }

  User::Status status() const { return status_; }
  void set_status(User::Status status) { status_ = status; }

private:

  // Variable --------------------------------------------------------

  std::string uid_;
  std::string username_;
  std::string display_name_;
  User::Status status_;


  DISALLOW_COPY_AND_ASSIGN(User);
};

NS_WTC_END

#endif /* defined(WORKTILECORE_CORE_USER_USER_H_) */

