#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

#import "WTCUser.h"
#import "WTCUser_CoreAddition.h"

#import "WTCObjcAdapter.h"

@implementation WTCUser

#pragma mark - Property

-(NSString *)uid {
  return [NSString stringWithUTF8String:_coreHandle->uid().c_str()];
}

-(void)setUid:(NSString *)uid {
  _coreHandle->set_uid([uid UTF8String]);
}

-(NSString *)username {
  return [NSString stringWithUTF8String:_coreHandle->username().c_str()];
}

-(void)setUsername:(NSString *)username {
  _coreHandle->set_username([username UTF8String]);
}

-(NSString *)displayName {
  return [NSString stringWithUTF8String:_coreHandle->display_name().c_str()];
}

-(void)setDisplayName:(NSString *)displayName {
  _coreHandle->set_display_name([displayName UTF8String]);
}

-(WTCUserStatus)status {
  return (WTCUserStatus)_coreHandle->status();
}

-(void)setStatus:(WTCUserStatus)status {
  _coreHandle->set_status((worktile::User::Status)status);
}

#pragma mark - Core Addition

+ (instancetype)userWithCoreUser:(const worktile::User&)coreUser {
  WTCUser *user = [[WTCUser alloc] init];
  user->_coreHandle = coreUser.Clone();
  return user;
}

@end
