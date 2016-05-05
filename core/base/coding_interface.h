// Copyright 2015 Frank Lin (lin.xiaoe.f@gmail.com). All rights reserved.
// Use of this source code is governed by a license that can be
// found in the LICENSE file.

#ifndef WORKTILECORE_CORE_BASE_CODING_INTERFACE_H_
#define WORKTILECORE_CORE_BASE_CODING_INTERFACE_H_

#include <string>

#include "base/macros.h"

NS_WTC_BEGIN

/// Coding interface declares a method for providing functional copies of an object by JSON.
///
/// @since 1.0
/// @author Frank Lin
class CodingInterface {
public:
  virtual bool InitWithJsonOrDie(const std::string& json) = 0;
  virtual ~CodingInterface() {}
};

NS_WTC_END

#endif //end WORKTILECORE_CORE_BASE_CODING_INTERFACE_H_
