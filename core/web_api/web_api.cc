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

NS_WTC_END
