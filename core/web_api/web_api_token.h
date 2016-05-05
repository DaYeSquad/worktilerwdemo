//
//  token.hpp
//  WorktileCore
//
//  Created by Frank Lin on 10/29/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#ifndef WORKTILECORE_CORE_WEB_API_WEB_API_TOKEN_H_
#define WORKTILECORE_CORE_WEB_API_WEB_API_TOKEN_H_

#include <memory>

#include "base/base.h"

NS_WTC_BEGIN

/// Token for access Web REST api. Usually get after signed in successed.
/// App developer can cache the token in their client side.
///
/// @since 1.0
/// @author Frank Lin
class WebApiToken {
public:
  
  // Creation and lifetime --------------------------------------------------------
  
  WebApiToken() {}
  virtual ~WebApiToken() {}
  
  void Init(const std::string& access_token, const std::string acccess_secret);
  
  std::unique_ptr<WebApiToken> Clone() const;
  
  // Getters/Setters --------------------------------------------------------
  
  std::string access_token() const { return access_token_; }
  
  std::string access_secret() const { return access_secret_; }
  
private:
  std::string access_token_;
  std::string access_secret_;
  
  
  DISALLOW_COPY_AND_ASSIGN(WebApiToken);
};

NS_WTC_END

#endif /* WORKTILECORE_CORE_WEB_API_WEB_API_TOKEN_H_ */
