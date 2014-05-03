/* Copyright (C) 2006 o2on project. All rights reserved.
 * http://o2on.net/
 */

/*
 * project		: o2on
 * filename		: O2LagQueryQueue.h
 * description	: 
 *
 */

#pragma once
#include "O2KeyDB.h"
#include "O2Profile.h"
#include "mutex.h"
#include "dataconv.h"


class O2LagQueryQueue
	: public Mutex
{
protected:
	typedef std::map<hashT,O2Key> O2KeyMap;
	typedef std::map<hashT,O2Key>::iterator O2KeyMapIt;

	bool		Active;

#ifdef _WIN32 /** EventObject class for win32 thread */
	EventObject	StopSignal;
#else /** For Boost threads processing */
	DosMocking::EventObject	StopSignal;
#endif

#ifdef _WIN32 /** HANDLE for win32 thread */
	HANDLE		ThreadHandle;
#else /** For POSIX thread processing */
	pthread_t	ThreadHandle;
	enum O2LagQueryQueueHandleEnum { THREAD = 0 };
	neosmart::neosmart_event_t handles[1];
#endif

	O2KeyMap	queries;
	O2Logger	*Logger;
	O2Profile	*Profile;
	O2KeyDB		*QueryDB;
	HWND		hwndBaloonCallback;
	UINT		msgBaloonCallback;

public:
	O2LagQueryQueue(O2Logger	*lgr
				  , O2Profile	*prof
				  , O2KeyDB		*qdb)
		: Active(false)
		, ThreadHandle(NULL)
		, Logger(lgr)
		, Profile(prof)
		, QueryDB(qdb)
#ifdef _WIN32   /** windows */
		, hwndBaloonCallback(NULL)
#endif
		, msgBaloonCallback(0)
	{
		start();
	};
	~O2LagQueryQueue() {
		stop();
	};
	void SetBaloonCallbackMsg(HWND hwnd, UINT msg)
	{
		hwndBaloonCallback = hwnd;
		msgBaloonCallback = msg;
	}

public:
	bool add(const O2DatPath &datpath, const uint64 size)
	{
		O2Key querykey;
		querykey.ip = 0;
		querykey.port = 0;
		querykey.size = size;
		datpath.gethash(querykey.hash);
		datpath.geturl(querykey.url);
		datpath.gettitle(querykey.title);
		querykey.enable = true;

		Lock();
		{
			O2KeyMapIt it = queries.find(querykey.hash);
			if (it == queries.end()) {
				queries.insert(O2KeyMap::value_type(querykey.hash, querykey));
			}
			else {
				it->second.date = time(NULL);
			}
		}
		Unlock();

		return true;
	}

	bool remove(const O2DatPath &datpath)
	{
		hashT hash;
		datpath.gethash(hash);

		Lock();
		{
			O2KeyMapIt it = queries.find(hash);
			if (it != queries.end())
				queries.erase(it);
		}
		Unlock();
		return true;
	}

private:
	void start(void)
	{
		if (!ThreadHandle) {
			Active = true;
			StopSignal.Off();
#ifdef _WIN32           /** windows */
			ThreadHandle = (HANDLE)_beginthreadex(NULL, 0, StaticThread, (void*)this, 0, NULL);
#else                   /** unix */
			pthread_attr_t attr1;
			if (pthread_attr_init(&attr1)) return;
			handles[0] = neosmart::CreateEvent();
			pthread_create(&ThreadHandle, &attr1, StaticThread, this);
#endif
		}
	}

	void stop(void)
	{
		if (ThreadHandle) {
			Active = false;
			//note:SignalObjectAndWait関数の方が処理をatomicに行えるため適切
			StopSignal.On();

			//Join

#ifdef _WIN32           /** win32 thread */
			WaitForSingleObject(ThreadHandle, INFINITE);
			CloseHandle(ThreadHandle);
			ThreadHandle = NULL;

#else                   /** Unix */
			neosmart::WaitForEvent(handles[THREAD], DosMocking::INFINITE);
			neosmart::DestroyEvent(handles[THREAD]);
#endif
		}
	}


#ifdef _WIN32 /** for win32 thread */
	static uint WINAPI StaticThread(void *data)
	{
		O2LagQueryQueue *me = (O2LagQueryQueue*)data;

		CoInitialize(NULL);
		me->Checker();
		CoUninitialize();

		return (0);
	};
#else /** for POSIX thread processing */
	static void* StaticThread(void *data)
	{
		O2LagQueryQueue *me = (O2LagQueryQueue*)data;
		me->Checker();
	};
#endif

	void Checker(void)
	{
		while (Active) 
		{
			Lock();
			for (O2KeyMapIt it = queries.begin(); it != queries.end(); ) 
			{
				O2Key &querykey = it->second;
				TRACE(querykey.title.c_str());
				TRACE(L"\n");

				if (time(NULL) - querykey.date < 5) 
				{
					it++;
					continue;
				}

				//キュー登録から5秒経過していたら登録
				querykey.date = 0;
				if (QueryDB->AddKey(querykey) == 1) 
				{
					if (Logger) 
					{
						Logger->AddLog(O2LT_INFO, L"LagQueryQueue", 0, 0,
							L"検索登録 %s", querykey.url.c_str());
					}

					if (hwndBaloonCallback && Profile->IsBaloon_Query()) 
					{
#ifdef _WIN32                                   /** windows */
						SendMessage(hwndBaloonCallback, msgBaloonCallback,
							(WPARAM)L"検索登録", (LPARAM)querykey.url.c_str());
#else                                           /** unix */
                                                #warning "TODO: implement wxWidgets event method here"
#endif
					}
					QueryDB->Save(Profile->GetQueryFilePath());
				}
				it = queries.erase(it);
			}
			Unlock();
			StopSignal.Wait(2500);
		}
	}
};
