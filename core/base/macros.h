//
//  macros.h
//  HelloWorktile
//
//  Created by Frank Lin on 5/4/16.
//  Copyright © 2016 Frank Lin. All rights reserved.
//

#ifndef WORKTILECORE_CORE_BASE_MACROS_H_
#define WORKTILECORE_CORE_BASE_MACROS_H_

// DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
TypeName(const TypeName&); \
void operator=(const TypeName&)

// namespace Worktile {}
#ifdef __cplusplus
#define NS_WTC_BEGIN                     namespace worktile {
#define NS_WTC_END                       }
#else
#define NS_WTC_BEGIN
#define NS_WTC_END
#endif // NS_WTC_BEGIN, NS_WTC_END

// delete
#define WTC_SAFE_DELETE(p) do { if(p) { delete (p); (p) = nullptr; } } while(0)

// 在 Android 的平台上添加一些兼容方法
#if WTC_PLATFORM == WTC_PLATFORM_ANDROID
#include "jni/string_utils_android.h"
#endif

// WTC_DLL
#if WTC_PLATFORM == WTC_PLATFORM_WIN32
#if defined(WTC_STATIC)
#define WTC_DLL
#else
#if defined(_USRDLL)
#define WTC_DLL     __declspec(dllexport)
#else
#define WTC_DLL     __declspec(dllimport)
#endif
#endif
#endif

// break if
#define WTC_BREAK_IF(XXXX) if(XXXX)break;

// assert
#ifdef WTC_USING_ASSERT
#include <assert.h>
#define WTC_ASSERT(p) assert(p)
#else
#define WTC_ASSERT(p)
#endif

// lock main database
#define WTC_DB_LOCK_GUARD std::lock_guard<std::mutex> lg(Director::DefaultDirector()->main_db_mutex_)

#endif /* WORKTILECORE_CORE_BASE_MACROS_H_ */