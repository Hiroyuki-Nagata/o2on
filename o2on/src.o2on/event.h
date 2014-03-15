/* Copyright (C) 2006 o2on project. All rights reserved.
 * http://o2on.net/
 */

/*
 * project		: 
 * filename		: event.h
 * description	: 
 *
 */

#pragma once

#ifdef _WIN32
   #include <windows.h>
#endif

class EventObject
{
protected:
	HANDLE event_handle;

public:
	EventObject(void)
		: event_handle(CreateEvent(NULL, TRUE, FALSE, NULL))
	{
		// Manual reset
		// Default off
	}
	~EventObject(void)
	{
		CloseHandle(event_handle);
	}

	HANDLE GetHandle(void)
	{
		return (event_handle);
	}

	void On(void)
	{
		SetEvent(event_handle);
	}

	void Off(void)
	{
		ResetEvent(event_handle);
	}

	void Wait(DWORD timeout_ms = INFINITE)
	{
		WaitForSingleObject(event_handle, timeout_ms);
	}

private:
	EventObject(const EventObject& rhs);
	EventObject& operator=(const EventObject& rhs);
};
