#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, WTCUserStatus) {
  WTCUserStatusNormal = 0,
  WTCUserStatusInvited = 1,
  WTCUserStatusPending = 2,
};


NS_ASSUME_NONNULL_BEGIN
@interface WTCUser : NSObject

@property (nonatomic, copy) NSString *uid;

@property (nonatomic, copy) NSString *username;

@property (nonatomic, copy) NSString *displayName;

@property (nonatomic) WTCUserStatus status;

@end
NS_ASSUME_NONNULL_END

