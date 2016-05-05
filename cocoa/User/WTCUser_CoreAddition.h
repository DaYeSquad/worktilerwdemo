#include "user.h"

@interface WTCUser () {
 @package
  std::unique_ptr<worktile::User> _coreHandle;
}

+ (instancetype)userWithCoreUser:(const worktile::User&)coreUser;

@end