#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, LCCUserStatus) {
  LCCUserStatusNormal = 0,
  LCCUserStatusInvited = 1,
  LCCUserStatusPending = 2,
};


NS_ASSUME_NONNULL_BEGIN
@interface WTCUser : NSObject

@property (nonatomic, copy) NSString *uid;

@property (nonatomic, copy) NSString *username;

@property (nonatomic, copy) NSString *displayName;

@property (nonatomic) LCCUserStatus status;

@end
NS_ASSUME_NONNULL_END

