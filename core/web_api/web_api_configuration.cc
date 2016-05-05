//
//  web_api_configuration.cpp
//  WorktileCore
//
//  Created by Frank Lin on 10/29/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#include "web_api_configuration.h"

#include "json11/json11.hpp"

NS_WTC_BEGIN

using std::string;
using std::map;
using std::unique_ptr;

const std::string kJosnConfigurationJsonVersion    = "1";

const std::string kJosnKeyConfigurationJsonVersion = "version";
const std::string kJsonKeyAppKey                   = "app_key";
const std::string kJsonKeyAppSecret                = "app_secret";
const std::string kJsonKeyClientVersion            = "client_version";
const std::string kJsonKeyAppChannelName           = "app_channel_name";
const std::string kJsonKeyApiVersion               = "api_version";
const std::string kJsonKeyDeviceId                 = "device_id";

const std::string kJsonKeyAccessToken              = "access_token";
const std::string kJsonKeyAccessSecret             = "access_secret";

////////////////////////////////////////////////////////////////////////////////
// WebApiConfiguration, public:

// Creation and lifetime --------------------------------------------------------

WebApiConfiguration::WebApiConfiguration() {
  app_key_.clear();
  app_secret_.clear();
  client_version_.clear();
  app_channel_name_.clear();
  api_version_.clear();
  device_id_.clear();
}

WebApiConfiguration::~WebApiConfiguration() {
  
}

void WebApiConfiguration::Init(const std::string& app_key,
                               const std::string& app_secret,
                               const std::string& client_version,
                               const std::string& app_channel_name,
                               const std::string& api_version,
                               const std::string& device_id,
                               const WebApiToken* token) {
  app_key_ = app_key;
  app_secret_ = app_secret;
  client_version_ = client_version;
  app_channel_name_ = app_channel_name;
  api_version_ = api_version;
  device_id_ = device_id;
 
  if (token) {
    token_ = token->Clone();
  }
}

std::unique_ptr<WebApiConfiguration> WebApiConfiguration::Clone() const {
  unique_ptr<WebApiConfiguration> rtn_instance(new WebApiConfiguration());
  rtn_instance->Init(app_key_, app_secret_, client_version_, app_channel_name_, api_version_, device_id_);
  if (token_ != nullptr) {
    rtn_instance->token_ = token_->Clone();
  }
  return rtn_instance;
}

// Json coding --------------------------------------------------------

void WebApiConfiguration::InitWithJson(const std::string& json) {
  std::string error;
  json11::Json json_obj = json11::Json::parse(json, error);
  
  if (!error.empty()) {
    return ;
  }
  
  app_key_ = json_obj[kJsonKeyAppKey].string_value();
  app_secret_ = json_obj[kJsonKeyAppSecret].string_value();
  client_version_ = json_obj[kJsonKeyClientVersion].string_value();
  app_channel_name_ = json_obj[kJsonKeyAppChannelName].string_value();
  api_version_ = json_obj[kJsonKeyApiVersion].string_value();
  device_id_ = json_obj[kJsonKeyDeviceId].string_value();
  
  string access_token = json_obj[kJsonKeyAccessToken].string_value();
  string access_secret = json_obj[kJsonKeyAccessSecret].string_value();
  token_ = unique_ptr<WebApiToken>(new WebApiToken());
  token_->Init(access_token, access_secret);
}

std::string WebApiConfiguration::ToJson() const {
  string access_token = "";
  string access_secret = "";
  
  if (token_) {
    access_token = token_->access_token();
    access_secret = token_->access_secret();
  }
  
  json11::Json config_json = json11::Json::object {
    { kJsonKeyAppKey, app_key_ },
    { kJsonKeyAppSecret, app_secret_ },
    { kJsonKeyClientVersion, client_version_ },
    { kJsonKeyAppChannelName, app_channel_name_ },
    { kJsonKeyApiVersion, api_version_ },
    { kJsonKeyDeviceId, device_id_ },
    { kJsonKeyAccessToken, access_token },
    { kJsonKeyAccessSecret, access_secret },
  };
  
  return config_json.dump();
}

// Header --------------------------------------------------------

std::map<std::string, std::string> WebApiConfiguration::HttpHeaderForSignIn(time_t ts) const {
  time_t current_ts;
  if (ts == kNow) {
    time(&current_ts);
  } else {
    current_ts = ts;
  }
  
  map<string, string> header;
  header.insert(std::make_pair("app_key", app_key_));
  header.insert(std::make_pair("timestamp", std::to_string(current_ts)));
  header.insert(std::make_pair("client_version", client_version_));
  header.insert(std::make_pair("device_id", device_id_));
  header.insert(std::make_pair("store_from", app_channel_name_));
  header.insert(std::make_pair("version", api_version_));
  header.insert(std::make_pair("signature", SignatureForSignIn(ts)));
  header.insert(std::make_pair("Content-type", "application/json"));
  
  return header;
}

std::map<std::string, std::string> WebApiConfiguration::HttpHeaderForHttpMethod(sakura::HttpRequest::Type http_method, time_t ts) const {
  WTC_ASSERT(token_ != nullptr);
  
  time_t current_ts;
  if (ts == kNow) {
    time(&current_ts);
  } else {
    current_ts = ts;
  }
  
//  sakura::log_event("app_key : %s", app_key_.c_str());
//  sakura::log_event("timestamp : %s", std::to_string(current_ts).c_str());
//  sakura::log_event("client_version : %s", client_version_.c_str());
//  sakura::log_event("device_id : %s", device_id_.c_str());
//  sakura::log_event("store_from : %s", app_channel_name_.c_str());
//  sakura::log_event("version : %s", api_version_.c_str());
//  sakura::log_event("signature : %s", SignatureForHttpMethod(http_method, ts).c_str());
//  sakura::log_event("access_token : %s", token_->access_token().c_str());
  
  map<string, string> header;
  header.insert(std::make_pair("app_key", app_key_));
  header.insert(std::make_pair("timestamp", std::to_string(current_ts)));
  header.insert(std::make_pair("client_version", client_version_));
  header.insert(std::make_pair("device_id", device_id_));
  header.insert(std::make_pair("store_from", app_channel_name_));
  header.insert(std::make_pair("version", api_version_));
  header.insert(std::make_pair("signature", SignatureForHttpMethod(http_method, ts)));
  header.insert(std::make_pair("Content-type", "application/json"));
  header.insert(std::make_pair("access_token", token_->access_token()));
  
  return header;
}

////////////////////////////////////////////////////////////////////////////////
// WebApiConfiguration, private:

// Signature --------------------------------------------------------

std::string WebApiConfiguration::SignatureForHttpMethod(sakura::HttpRequest::Type http_method, time_t ts) const {
  std::stringstream ss;
  
  time_t current_ts;
  if (ts == kNow) {
    time(&current_ts);
  } else {
    current_ts = ts;
  }
  
  ss << sakura::HttpRequest::StringFromHttpRequestType(http_method) << "&"
  << "access_token=" << token_->access_token() << "&"
  << "app_key=" << app_key_ << "&"
  << "client_version=" << client_version_ << "&"
  << "device_id=" << device_id_ << "&"
  << "store_from=" << app_channel_name_ << "&"
  << "timestamp=" <<  std::to_string(current_ts) << "&"
  << "version=" << api_version_ << "&"
  << app_secret_ << "&"
  << token_->access_secret();
  
  string original_string = ss.str();
  
  return original_string;
}

std::string WebApiConfiguration::SignatureForSignIn(time_t ts) const {
  std::stringstream ss;
  
  time_t current_ts;
  if (ts == kNow) {
    time(&current_ts);
  } else {
    current_ts = ts;
  }
  
  ss << sakura::HttpRequest::StringFromHttpRequestType(sakura::HttpRequest::Type::POST) << "&"
  << "app_key=" << app_key_ << "&"
  << "client_version=" << client_version_ << "&"
  << "device_id=" << device_id_ << "&"
  << "store_from=" << app_channel_name_ << "&"
  << "timestamp=" << std::to_string(current_ts) << "&"
  << "version=" << api_version_ << "&"
  << app_secret_ ;
  
  string original_string = ss.str();
  
  return original_string;
}

NS_WTC_END
