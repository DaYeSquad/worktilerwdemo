// Copyright 2015 Frank Lin (lin.xiaoe.f@gmail.com). All rights reserved.
// Use of this source code is governed by a license that can be
// found in the LICENSE file.

#ifndef WORKTILECORE_CORE_DIRECTOR_DIRECTOR_H_
#define WORKTILECORE_CORE_DIRECTOR_DIRECTOR_H_

#include <memory>
#include <mutex>
#include <stdint.h>

#include "easySQLite/easySQLite.h"

#include "base/base.h"

#include "user/user.h"

#include "web_api/web_api.h"

NS_WTC_BEGIN

class UserManager;
class ObjectManager;

// Director is the first citizen of WorktileCore, it describes the run time environment.
// Make sure you call |InitOrDie| at very beginning of the app's lifecycle and call |PurgeDirector| at
//   the end of the lifecycle.
// |Director| use |base_directory| as SQLite cache directory.
//
// When you first lanuch the app, use |InitAsStranger| to init then use sign in/up, then you can cache the config locally, so
//  that the next time user lanuches the app, you can user director directly with |WebApiConfiguration|.
//
// @since 1.0
// @author Frank Lin
class Director {
public:

  // Creation and lifetime --------------------------------------------------------

  Director();
  virtual ~Director();

  static Director* DefaultDirector();
  static void PurgeDirector();
  static void SetDefaultDirector(Director* director);

  // init before sign in
  // developer can use this method to init and then sign in.
  // use |FileUtils::WritablePath| as base directory
  // iOS developer may prefer to use this method rather than the method with base_directory.
  void InitAsStranger(const WebApiConfiguration& config);
  
  // init before sign in
  // developer can use this method to init and then sign in.
  void InitAsStranger(const std::string& base_directory, const WebApiConfiguration& config);
  
  // use |FileUtils::WritablePath| as base directory
  // iOS developer may prefer to use this method rather than the method with base_directory.
  bool InitOrDie(const User& me, const WebApiConfiguration* config=nullptr);
  
  bool InitOrDie(const std::string& base_directory, const User& me, const WebApiConfiguration* config=nullptr);
  
  // Getter/Setter --------------------------------------------------------

  const User* me() { return me_.get(); }
  void set_my_display_name(const std::string& display_name) { me_->set_display_name(display_name); }
  
  const WebApiConfiguration* web_api_configuration() const { return WebApi::Api()->configuration(); }
  
  std::string base_directory() const { return base_directory_; }
  
  const UserManager* user_manager() const { return user_manager_.get(); }
  
  // SQL Utils --------------------------------------------------------
  
  void LockMainDatabase();
  void UnlockMainDatabase();
  
  void BeginTransaction();
  void CommitTransaction();
  
  std::unique_ptr<sql::Table> SqlTable(const std::string& table_name,
                                       sql::Field* definition);

private:
  friend class UserManager;
  friend class ObjectManager;
  
  std::unique_ptr<User> me_;
  std::string base_directory_;

  // Cache --------------------------------------------------------

  std::unique_ptr<sql::Database> main_db_;
  std::mutex main_db_mutex_;

  // Manager --------------------------------------------------------

  std::unique_ptr<UserManager> user_manager_;


  DISALLOW_COPY_AND_ASSIGN(Director);
};

NS_WTC_END

#endif /* defined(WORKTILECORE_CORE_DIRECTOR_DIRECTOR_H_) */
