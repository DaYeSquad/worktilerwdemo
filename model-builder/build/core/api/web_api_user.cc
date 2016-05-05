#include "web_api.h"

#include "json11/json11.hpp"

#include "utils/string_utils.h"
#include "utils/json11_utils.h"

using std::string;
using std::vector;
using std::unique_ptr;

using sakura::HttpRequest;
using sakura::HttpClient;
using sakura::HttpResponse;

NS_LCC_BEGIN

////////////////////////////////////////////////////////////////////////////////
// WebApi, public:

// User --------------------------------------------------------

void WebApi::GetAllUsers(std::function<void(bool success, const std::string& error, std::vector<std::unique_ptr<User>> users)> callback) {
  string api_path = "/api/mobile/users";
  string url = BaseUrlForCurrentTeam() + api_path;

  unique_ptr<HttpRequest> request = GenBaseRequestForCurrentTeam(HttpRequest::Type::GET);
  request->set_url(url);

  HttpClient::SharedClient()->
  Send(std::move(request), [callback](unique_ptr<HttpResponse> response) {
    if (response->is_succeed()) {
      string error;
      json11::Json json_obj = json11::Json::parse(response->response_data_as_string(), error);
      int state_code = json_obj[kJsonKeyState].int_value();
      if (state_code == 200) {
        vector<unique_ptr<User>> users;
        vector<json11::Json> users_jsons = json_obj["data"].array_items();
        for (auto json : users_jsons) {
          unique_ptr<User> obj(new User());
          obj->InitWithJsonOrDie(json.dump());
          users.push_back(std::move(obj));
        }

        callback(true, "", std::move(users));
      } else {
        string ret_error = ErrorMessageFromStateCode(state_code);
        callback(false, ret_error, {});
      }
    } else {
      string ret_error = response->error_buffer();
      callback(false, ret_error, {});
    }
  });
}

NS_LCC_END