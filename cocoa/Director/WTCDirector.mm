//
//  WTCDirector.m
//  HelloWorktile
//
//  Created by Frank Lin on 5/5/16.
//  Copyright © 2016 Frank Lin. All rights reserved.
//

#import "WTCDirector.h"
#import "WTCDirector_CoreAddition.h"

static WTCDirector *_sharedDirector;

@implementation WTCDirector

#pragma mark - Lifecycle

+ (instancetype)defaultDirector {
  return _sharedDirector;
}

+ (void)resetDefaultDirector:(WTCDirector *)director {
  _sharedDirector = nil;
  _sharedDirector = director;
  
  if (_sharedDirector) {
    worktile::Director::SetDefaultDirector(_sharedDirector->_directorHandle);
  } else {
    worktile::Director::SetDefaultDirector(nullptr);
  }
}

@end
