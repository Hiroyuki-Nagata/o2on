/* Copyright (C) 2006 o2on project. All rights reserved.
 * http://o2on.net/
 */

/*
 * project		: 
 * filename		: thread.h
 * description	: 
 *
 */

// ����g���ĂȂ�
// ������Win32/POSIX�X���b�h��Wrapper�������\��

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
