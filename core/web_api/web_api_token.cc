//
//  token.cpp
//  WorktileCore
//
//  Created by Frank Lin on 10/29/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#include "json11/json11.hpp"

#include "web_api_token.h"
#include "web_api.h"


NS_WTC_BEGIN

using std::unique_ptr;
using std::string;

////////////////////////////////////////////////////////////////////////////////
// WebApiToken, public:

// Creation and lifetime --------------------------------------------------------

std::unique_ptr<WebApiToken> WebApiToken::Clone() const {
  unique_ptr<WebApiToken> rtn_token(new WebApiToken());
  rtn_token->Init(access_token_, access_secret_);
  return rtn_token;
}

void WebApiToken::Init(const std::string& access_token, const std::string acccess_secret) {
  access_token_ = access_token;
  access_secret_ = acccess_secret;
}

NS_WTC_END
