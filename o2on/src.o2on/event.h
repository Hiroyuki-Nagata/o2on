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

#ifdef _WIN32 /** Windows EventObject */

#include <windows.h>

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

#else /** Unix side EventObject equivalent implement */

#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>

class EventObject;
typedef EventObject* EventHandle;
static const unsigned k_INFINITE = 0xFFFFFFFF;

class EventObject
{

protected:
	EventHandle evt;

public:
	EventObject(void) : m_bool(false)
	{
		CreateEvent();
	};
	~EventObject(void)
	{
		CloseHandle(evt);
	};
	EventHandle CreateEvent( void )
	{
		return new EventObject;
	};
	void CloseHandle( EventHandle evt )
	{
		delete evt;
	};
	EventHandle GetHandle(void)
	{
		return evt;
	};
	void On(void)
	{
		SetEvent(evt);
	};
	void Off(void)
	{
		ResetEvent(evt);
	};
	void Wait(DWORD timeout_ms = k_INFINITE)
	{
		WaitForSingleObject(evt, timeout_ms);
	};

	bool m_bool;
	boost::mutex m_mutex;
	boost::condition m_cond;

private:
	EventObject(const EventObject& rhs);
	EventObject& operator=(const EventObject& rhs);

	void SetEvent(EventHandle evt)
	{
		// シグナル状態へ
		evt->m_bool = true;
		evt->m_cond.notify_all();
	};
	void ResetEvent(EventHandle evt)
	{
		// 非シグナル状態へ
		evt->m_bool = false;
	};
	void WaitForSingleObject(EventHandle evt, DWORD timeout_ms)
	{
		boost::mutex::scoped_lock lock( evt->m_mutex );
		if( timeout_ms == k_INFINITE )
		{
			while( !evt->m_bool )
			{
				evt->m_cond.wait( lock );
			}
		}
		else
		{
   			//slightly more complex code for timeouts
		}
	};
};
#endif
