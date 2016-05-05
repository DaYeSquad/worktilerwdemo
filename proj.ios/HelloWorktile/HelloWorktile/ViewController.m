//
//  ViewController.m
//  HelloWorktile
//
//  Created by Frank Lin on 3/23/16.
//  Copyright © 2016 Frank Lin. All rights reserved.
//

#import "ViewController.h"

#import "WorktileCore.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
  [super viewDidLoad];
  
  // 初始化 Director 以完成所有的 ObjectManager 的初始化
  WTCDirector *director = [[WTCDirector alloc] init];
  [WTCDirector resetDefaultDirector:director];
  // [[WTCDirector defaultDirector] signIn...]
  
  // 此时可以开始使用 ObjectManager，此处用 UserManager 来获取所有的 users
  WTCUser *user = [[WTCUserManager defaultManager] fetchUserFromCacheByUid:@"ooxx"];
  if (user != nil) {
    // 将缓存的 user 先显示在界面上，之后再在网络请求中获取新的 users
  }
  
  // 此时可以开始使用 ObjectManager，此处用 UserManager 来获取所有的 users
  [[WTCUserManager defaultManager] getAllUsersSuccess:^(NSArray<WTCUser *> *users) {
    // 获取了所有 users 后刷新界面上的 users
  } failure:^(NSError *error) {
    // 错误的响应事件
  }];
}

@end
