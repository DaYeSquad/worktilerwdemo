//
//  web_api_configuration.hpp
//  WorktileCore
//
//  Created by Frank Lin on 10/29/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#ifndef WORKTILECORE_CORE_WEB_API_WEB_API_CONFIGURATION_H_
#define WORKTILECORE_CORE_WEB_API_WEB_API_CONFIGURATION_H_

#include <map>

#include "base/base.h"
#include "web_api/web_api_token.h"

NS_WTC_BEGIN

// |WebApiConfiguration| defines some required property to init |WebApi| and provides some algorithem
//  for |WebApi| when request to Worktile server.
class WebApiConfiguration {
public:
  
  static const time_t kNow = 0;
  
  // Creation and lifetime --------------------------------------------------------
  
  WebApiConfiguration();
  virtual ~WebApiConfiguration();
  
  // Init with required property, if token is not nullptr, you can use all of |WebApi| methods, but if
  //  token is nullptr, you should sign in to request the token.
  // It is handy for app developer to cache the token in client side.
  void Init(const std::string& app_key,
            const std::string& app_secret,
            const std::string& client_version,
            const std::string& app_channel_name,
            const std::string& api_version,
            const std::string& device_id,
            const WebApiToken* token=nullptr);
  
  std::unique_ptr<WebApiConfiguration> Clone() const;
  
  // Json coding --------------------------------------------------------
  
  void InitWithJson(const std::string& json);
  std::string ToJson() const;
  
  // Getters/Setters --------------------------------------------------------
  
  std::string app_key() const { return app_key_; }
  std::string app_secret() const { return app_secret_; }
  std::string client_version() const { return client_version_; }
  std::string app_channel_name() const { return app_channel_name_; }
  std::string api_version() const { return api_version_; }
  std::string device_id() const { return device_id_; }
  
  const WebApiToken* token() const { return token_.get(); }
  void set_token(const WebApiToken& token) { token_ = token.Clone(); }
  
  // Header --------------------------------------------------------
  
  std::map<std::string, std::string> HttpHeaderForSignIn(time_t ts=kNow) const;
  
  std::map<std::string, std::string> HttpHeaderForHttpMethod(sakura::HttpRequest::Type http_method, time_t ts=kNow) const;
  
private:
  std::string app_key_;
  std::string app_secret_;
  std::string client_version_;
  std::string app_channel_name_;
  std::string api_version_;
  std::string device_id_;
  
  std::unique_ptr<WebApiToken> token_;
  
  // Signature --------------------------------------------------------
  
  std::string SignatureForHttpMethod(sakura::HttpRequest::Type http_method, time_t ts=kNow) const;
  
  std::string SignatureForSignIn(time_t ts=kNow) const;
  
  
  DISALLOW_COPY_AND_ASSIGN(WebApiConfiguration);
};

NS_WTC_END

#endif /* WORKTILECORE_CORE_WEB_API_WEB_API_CONFIGURATION_H_ */
