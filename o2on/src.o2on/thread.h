/* Copyright (C) 2006 o2on project. All rights reserved.
 * http://o2on.net/
 */

/*
 * project		: 
 * filename		: thread.h
 * description	: 
 *
 */

// これ使ってない
// ここにWin32/POSIXスレッドのWrapperを書く予定

#pragma once

#ifdef _WIN32
   #include <windows.h>
   #include <process.h>
#else
   #include <cstdlib> 
#endif

inline HANDLE GetRealThreadHandle(void)
{
	HANDLE handle;

	DuplicateHandle(
		GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(),
		&handle, 0, FALSE, DUPLICATE_SAME_ACCESS);

	return (handle);
}
