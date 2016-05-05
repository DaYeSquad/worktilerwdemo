//
//  web_api.hpp
//  WorktileCore
//
//  Created by Frank Lin on 10/29/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#ifndef WORKTILECORE_CORE_WEB_API_WEB_API_H_
#define WORKTILECORE_CORE_WEB_API_WEB_API_H_

#include <memory>
#include <vector>
#include <functional>

#include "base/base.h"

#include "web_api/web_api_configuration.h"

#include "user/user.h"

NS_WTC_BEGIN

static const std::string kJsonKeyState = "code";
static const std::string kJsonKeyData  = "data";

static const int kHttpRequestErrorCode = -1; // means failed during HTTP request, no response from API.

/// |WebApi| is for all REST api access to Worktile.
///
/// @since 1.0
/// @author Frank Lin
class WebApi {
public:

  // WebApi::Error describes code and description from HTTP or our server.
  struct Error {
    int code;
    std::string description;

    Error(int state_code, const std::string& desc) {
      code = state_code;
      description = desc;
    }

    virtual ~Error() {}

    static Error NoError() {
      return Error(200, "");
    }
  };

  // Creation and lifetime --------------------------------------------------------

  WebApi();
  virtual ~WebApi();
  void Init(const WebApiConfiguration& configuration);

  static WebApi* Api();
  static void ResetApi(WebApi* api);
  static void PurgeApi();

  // Getter --------------------------------------------------------

  const WebApiConfiguration* configuration() const { return configuration_.get(); }
  
  // User --------------------------------------------------------
  
  // /api/mobile/users
  void GetAllUsers(std::function<void(bool success, const std::string& error, std::vector<std::unique_ptr<User>> users)> callback);
  
  // Utils --------------------------------------------------------

  std::unique_ptr<sakura::HttpRequest> HttpRequestWithMethod(sakura::HttpRequest::Type method,
                                                             const std::string& api_path_wo_host) const;
  
  static std::string ErrorMessageFromStateCode(int state_code);
  
  std::string BaseUrlForCurrentTeam() const;
  
  std::unique_ptr<sakura::HttpRequest> GenBaseRequestForCurrentTeam(sakura::HttpRequest::Type type) const;
  
private:
  std::unique_ptr<WebApiConfiguration> configuration_;


  DISALLOW_COPY_AND_ASSIGN(WebApi);
};

NS_WTC_END

#endif /* WORKTILECORE_CORE_WEB_API_WEB_API_H_ */
