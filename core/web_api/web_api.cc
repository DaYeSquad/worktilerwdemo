//
//  web_api.cpp
//  WorktileCore
//
//  Created by Frank Lin on 10/29/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#include "web_api.h"

NS_WTC_BEGIN

using std::unique_ptr;
using std::string;

////////////////////////////////////////////////////////////////////////////////
// WebApi, public:

static WebApi* shared_instance_ = nullptr;

// Creation and lifetime --------------------------------------------------------

WebApi::WebApi() {

}

WebApi::~WebApi() {
  
}

void WebApi::Init(const WebApiConfiguration& configuration) {
  configuration_ = configuration.Clone();
}

WebApi* WebApi::Api() {
  return shared_instance_;
}

void WebApi::ResetApi(WebApi* api) {
  WTC_SAFE_DELETE(shared_instance_);
  shared_instance_ = api;
}

void WebApi::PurgeApi() {
  WTC_SAFE_DELETE(shared_instance_);
}

////////////////////////////////////////////////////////////////////////////////
// WebApi, private:

// Http request for Worktile --------------------------------------------------------

std::unique_ptr<sakura::HttpRequest> WebApi::HttpRequestWithMethod(sakura::HttpRequest::Type method,
                                                                   const std::string& api_path_wo_host) const {
  WTC_ASSERT(configuration_ != nullptr);
  
  unique_ptr<sakura::HttpRequest> http_request(new sakura::HttpRequest());
  http_request->set_request_type(method);
  http_request->set_url(api_path_wo_host);
  http_request->set_request_header(configuration_->HttpHeaderForHttpMethod(method));
  return http_request;
}

std::string WebApi::BaseUrlForCurrentTeam() const {
  return "pro.worktile.com";
}

std::string WebApi::ErrorMessageFromStateCode(int state_code) {
  switch (state_code) {
    case 200:
      return "OK";
    case 401:
      return "INVALID_INPUT";
    case 404:
      return "NOT_FOUND";
    case 1005:
      return "PERMISSION_DENIED";
    case 1000:
      return "USER_NOT_EXISTS";
    case 1001:
      return "USERNAME_PWD_ERROR";
    case 1002:
      return "INVALID_DISPLAY_NAME";
    case 1003:
      return "UPDATE_USER_ERROR";
    case 1004:
      return "SIGNATURE_ERROR";
    case 1012:
      return "INSUFFICIENT_BALANCE";
    case 1013:
      return "MAX_NUMBER_OF_MEMBERS";
    case 2000:
      return "SECURITY_GET_ERROR";
    default:
      return "UNKNOWN ERROR " + std::to_string(state_code);
      break;
  }
}

std::unique_ptr<sakura::HttpRequest> WebApi::GenBaseRequestForCurrentTeam(sakura::HttpRequest::Type type) const {
  time_t current_ts;
  time(&current_ts);
  
  std::unique_ptr<sakura::HttpRequest> request(new sakura::HttpRequest());
  request->AddValueToHttpHeaderField("timestamp", std::to_string(current_ts));
  request->AddValueToHttpHeaderField("client-version", configuration_->client_version());
  request->AddValueToHttpHeaderField("device-id", configuration_->device_id());
  request->AddValueToHttpHeaderField("Content-type", "application/json");
  
  request->set_request_type(type);
  
  return request;
}

NS_WTC_END
