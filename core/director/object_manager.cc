//
//  object_manager.cpp
//  LessChatCore
//
//  Created by Frank Lin on 11/23/14.
//
//

#include "object_manager.h"

#include <stdlib.h>

#include "director/director.h"

NS_WTC_BEGIN

using std::string;

////////////////////////////////////////////////////////////////////////////////
// ObjectManager, public:

// Creation and lifetime --------------------------------------------------------

ObjectManager::ObjectManager()
:director_(nullptr) {
    
}

ObjectManager::ObjectManager(Director* director)
:director_(director) {
  
}

ObjectManager::~ObjectManager() {
  director_ = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
// ObjectManager, protected:

// SQLite --------------------------------------------------------

sqlite3* ObjectManager::MainDatabaseHandler() const {
  WTC_ASSERT(director_->main_db_->isOpen());
  return director_->main_db_->getHandle();
}

void ObjectManager::LockMainDatabase() const {
  director_->main_db_mutex_.lock();
}

void ObjectManager::UnlockMainDatabase() const {
  director_->main_db_mutex_.unlock();
}

void ObjectManager::BeginTransaction() const {
  director_->main_db_->transactionBegin();
}

void ObjectManager::CommitTransaction() const {
  director_->main_db_->transactionCommit();
}

NS_WTC_END
