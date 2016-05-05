//
//  WTCDirector.h
//  HelloWorktile
//
//  Created by Frank Lin on 5/5/16.
//  Copyright © 2016 Frank Lin. All rights reserved.
//

#import <Foundation/Foundation.h>

@class WTCUserManager;

/**
 *  这个文件是手写的，我们将 Director 当作是操作这台设备的用户，所有与其相关的操作如 SignIn, Logout 等都在该类中，
 *  该类采用单例模式设计，在初始化该类后，各个 ObjectManager 都会被初始化成功并且被 Director 所拥有 -- 在 Director
 *  销毁的时候销毁所有的 Manager。
 */
NS_ASSUME_NONNULL_BEGIN
@interface WTCDirector : NSObject

@property(nonatomic, strong, readonly) WTCUserManager *userManager;

+ (instancetype)defaultDirector;

+ (void)resetDefaultDirector:(WTCDirector *)director;

@end
NS_ASSUME_NONNULL_END
