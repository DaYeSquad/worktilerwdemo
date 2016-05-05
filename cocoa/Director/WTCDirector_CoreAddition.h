//
//  WTCDirector_CoreAddition.h
//  HelloWorktile
//
//  Created by Frank Lin on 5/5/16.
//  Copyright © 2016 Frank Lin. All rights reserved.
//

#import "WTCDirector.h"

#include "director.h"

@interface WTCDirector () {
 @package
  worktile::Director *_directorHandle;
}

@end
