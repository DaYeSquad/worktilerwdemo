//
//  director.cpp
//  WorktileCore
//
//  Created by Frank Lin on 1/20/15.
//  Copyright (c) 2015 Frank Lin. All rights reserved.
//
#include <assert.h>

#include "director.h"

#include "user/user_manager.h"
#include "web_api/web_api.h"

using std::unique_ptr;
using std::string;
using sakura::log_event;
using sakura::log_error;
using sakura::FileUtils;

NS_WTC_BEGIN

static Director* shared_instance_ = nullptr;


////////////////////////////////////////////////////////////////////////////////
// Director, public:

// Creation and lifetime --------------------------------------------------------

Director::Director() {
  
}

Director::~Director() {
  
}

Director* Director::DefaultDirector() {
  if (shared_instance_ == nullptr) {
    shared_instance_ = new Director();
  }
  
  return shared_instance_;
}

void Director::PurgeDirector() {
  FileUtils::PurgeFileUtils();
  WTC_SAFE_DELETE(shared_instance_);
}

void Director::SetDefaultDirector(Director* director) {
  shared_instance_ = director;
}

void Director::InitAsStranger(const WebApiConfiguration& config) {
  InitAsStranger(sakura::FileUtils::SharedFileUtils()->GetWritablePath(), config);
}

void Director::InitAsStranger(const std::string& base_directory, const WebApiConfiguration& config) {
  base_directory_ = base_directory;
  unique_ptr<WebApi> web_api(new WebApi());
  web_api->Init(config);
  WebApi::ResetApi(web_api.release());
}

bool Director::InitOrDie(const User& me, const WebApiConfiguration* config) {
  return InitOrDie(sakura::FileUtils::SharedFileUtils()->GetWritablePath(), me, config);
}

bool Director::InitOrDie(const std::string& base_directory, const User& me, const WebApiConfiguration* config) {
  base_directory_ = base_directory;
  me_ = me.Clone();
  
  //Create folder if not exists
  string path = base_directory;
  string folder_path = path + "/" + me.uid() + "/";
  string main_db_path = folder_path + "Worktile.db";
  
  sakura::log_event("database path is %s", main_db_path.c_str());
  
  if (!FileUtils::SharedFileUtils()->IsDirectoryExist(folder_path)) {
    FileUtils::SharedFileUtils()->CreateDirectory(folder_path);
  }
  
  bool success = true;
  
  do {
    //Open SQLite
    main_db_ = unique_ptr<sql::Database>(new sql::Database());
    try {
      success = main_db_->open(main_db_path);
    } catch(sql::Exception e) {
      log_error("Director cannot open database %s", e.msg().c_str());
      return false;
    }
    
#if WTC_PLATFORM == WTC_PLATFORM_ANDROID
    char *errorMsg = nullptr;
    int result = sqlite3_exec(main_db_->getHandle(),
                              "PRAGMA temp_store = MEMORY",
                              NULL,
                              NULL, &errorMsg);
    WTC_ASSERT(result == SQLITE_OK);
#endif
    
    WTC_BREAK_IF(!success);
    
    //Init tables
    user_manager_ = unique_ptr<UserManager>(new UserManager(this));
    
    //User manager
    success = user_manager_->InitOrDie();
    WTC_BREAK_IF(!success);
    
    // init WebApi
    if (config != nullptr) {
      unique_ptr<WebApi> web_api(new WebApi());
      web_api->Init(*config);
      WebApi::ResetApi(web_api.release());
    }
    
  } while (0);
  
  return success;
}

// SQL Utils --------------------------------------------------------

void Director::LockMainDatabase() {
  main_db_mutex_.lock();
}

void Director::UnlockMainDatabase() {
  main_db_mutex_.unlock();
}

void Director::BeginTransaction() {
  main_db_->transactionBegin();
}

void Director::CommitTransaction() {
  main_db_->transactionCommit();
}

std::unique_ptr<sql::Table> Director::SqlTable(const std::string& table_name,
                                               sql::Field* definition) {
  return std::unique_ptr<sql::Table>(new sql::Table(main_db_->getHandle(), table_name, definition));
}

NS_WTC_END
