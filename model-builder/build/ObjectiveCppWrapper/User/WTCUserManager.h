#import <Foundation/Foundation.h>

@class WTCUser;

NS_ASSUME_NONNULL_BEGIN
@interface WTCUserManager : NSObject

///-----------------------------------------
/// @name Lifecycle
///-----------------------------------------

+ (instancetype)defaultManager;

///-----------------------------------------
/// @name HTTP
///-----------------------------------------

- (void)getAllUsersSuccess:(void (^)(NSArray<WTCUser *> *users))successBlock failure:(void (^)(NSError *error))failureBlock;

///-----------------------------------------
/// @name Persistent store
///-----------------------------------------

- (nullable WTCUser *)fetchUserFromCacheByUid:(NSString *)uid;

@end
NS_ASSUME_NONNULL_END
