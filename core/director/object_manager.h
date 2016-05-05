// Copyright 2015 Frank Lin (lin.xiaoe.f@gmail.com). All rights reserved.
// Use of this source code is governed by a license that can be
// found in the LICENSE file.

#ifndef LESSCHAT_CORE_DIRECTOR_OBJECT_MANAGER_H_
#define LESSCHAT_CORE_DIRECTOR_OBJECT_MANAGER_H_

#include <time.h>

#include "easySQLite/easySQLite.h"

#include "base/base.h"

NS_WTC_BEGIN

class Director;

// There are many objects in LessChat, like |User|, |Service|, |Channel| and so forth, LessChatCore
// would like to give object modifiers by providing an abstract class |ObjectManager|, you can use
// subclass of this class to make http(s) requests or sqlite fetch requests on those objects.
class ObjectManager {
public:
  
  // Creation and lifetime --------------------------------------------------------
  
  ObjectManager();
  explicit ObjectManager(Director* director);
  virtual ~ObjectManager();
  
  // Database Action --------------------------------------------------------
  
  void LockMainDatabase() const;
  void UnlockMainDatabase() const;
  
  void BeginTransaction() const;
  void CommitTransaction() const;
  
protected:
  sqlite3* MainDatabaseHandler() const;
  
  bool UpdateTimestampForKey(time_t timestamp, const std::string& key) const;
  time_t LastTimestampForKey(const std::string& key) const;
  
  /*Weak Reference*/ Director* director_;
  
private:
  DISALLOW_COPY_AND_ASSIGN(ObjectManager);
};

NS_WTC_END

#endif /* defined(LESSCHAT_CORE_DIRECTOR_OBJECT_MANAGER_H_) */
