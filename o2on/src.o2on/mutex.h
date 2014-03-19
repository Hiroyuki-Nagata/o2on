/* Copyright (C) 2006 o2on project. All rights reserved.
 * http://o2on.net/
 */

/*
 * project		: o2on
 * filename		: mutex.h
 * description		: class for multi thread
 */

#pragma once

#ifdef _WIN32
   #include <windows.h>
   #include "stopwatch.h"
#else
   #include "typedef.h"
   #include <pthread.h>
#endif

class Mutex
{
protected:

#ifdef _WIN32
	HANDLE mutex_handle;
#else
	pthread_mutex_t mutex_handle;
#endif

public:

#ifdef _WIN32
	Mutex(void)
	  : mutex_handle(CreateMutex(NULL, FALSE, NULL)){};

	~Mutex(void)
	{
		Lock();
		Unlock();
		CloseHandle(mutex_handle);
	};
	void Lock(void)
	{
		WaitForSingleObject(mutex_handle, INFINITE);
	};
	bool LockTest(void)
	{
		DWORD r = WaitForSingleObject(mutex_handle, 0);
		if (r == WAIT_TIMEOUT)
			return false;
		return true;
	};

	void Unlock(void)
	{
		ReleaseMutex(mutex_handle);
	};

#else

	Mutex(void)
	{
	  pthread_mutex_init(&mutex_handle, NULL);
	};

	~Mutex(void)
	{
		Lock();
		Unlock();
		pthread_mutex_destroy(&mutex_handle);
	};

	void Lock(void)
	{
	  	pthread_mutex_lock(&mutex_handle);
	};

	bool LockTest(void)
	{
	  //DWORD r = WaitForSingleObject(mutex_handle, 0);
	  //	if (r == WAIT_TIMEOUT)
	  //		return false;
		return true;
	};

	void Unlock(void)
	{
		pthread_mutex_unlock(&mutex_handle);
	};

#endif

private:
	Mutex(const Mutex& rhs);
	Mutex& operator=(const Mutex& rhs);
};
