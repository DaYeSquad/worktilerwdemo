//
//  WTCUtils.m
//  HelloWorktile
//
//  Created by Frank Lin on 5/5/16.
//  Copyright © 2016 Frank Lin. All rights reserved.
//

#import "WTCUtils.h"

NSError *WTCErrorWithCoreError(NSString *errorString) {
  NSMutableDictionary *errorDict = [NSMutableDictionary dictionary];
  [errorDict setObject:errorString forKey:NSLocalizedDescriptionKey];
  return [[NSError alloc] initWithDomain:@"WorktileCore" code:0 userInfo:errorDict];
}
