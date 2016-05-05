#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

#import "WTCUserManager.h"
#import "WTCUserManager_CoreAddition.h"

#import "WTCDirector.h"

#import "WTCUser.h"
#import "WTCUser_CoreAddition.h"

#import "WTCUtils.h"

@implementation WTCUserManager

#pragma mark - Lifecycle

- (instancetype)init {
  if (self = [super init]) {
    _coreManagerHandler = worktile::UserManager::DefaultManager();
  }
  return self;
}

+ (instancetype)defaultManager {
  return [WTCDirector defaultDirector].userManager;
}

#pragma mark - HTTP

- (void)getAllUsersSuccess:(void (^)(NSArray<WTCUser *> *users))successBlock failure:(void (^)(NSError *error))failureBlock {
  _coreManagerHandler->
  GetAllUsers([successBlock, failureBlock](bool success,
                                           const std::string& errorUTF8String,
                                           std::vector<std::unique_ptr<worktile::User>> coreUsers) {
    if (success) {
      NSMutableArray *users = [NSMutableArray array];
      for (auto it = coreUsers.begin(); it != coreUsers.end(); ++it) {
        [users addObject:[WTCUser userWithCoreUser:**it]];
      }

      successBlock(users);
    } else {
      NSString *error = [NSString stringWithUTF8String:errorUTF8String.c_str()];
      failureBlock(WTCErrorWithCoreError(error));
    }
  });
}

#pragma mark - Persistent store

- (nullable WTCUser *)fetchUserFromCacheByUid:(NSString *)uid {
  std::unique_ptr<worktile::User> coreUser = _coreManagerHandler->FetchUserFromCacheByUid([uid UTF8String]);
  if (coreUser) {
    return [WTCUser userWithCoreUser:*coreUser];
  }
  return nil;
}

@end
