/* Copyright (C) 2006 o2on project. All rights reserved.
 * http://o2on.net/
 */

/*
 * project		: o2on
 * filename		: O2Scheduler.h
 * description		: o2on internal scheduler
 *
 */

#pragma once
#include "mutex.h"
#include "O2Job.h"

#ifndef _WIN32
   #include "event.h"
#endif


class O2Scheduler : public Mutex
{

protected:
	typedef std::vector<O2Job*> Jobs;

	Jobs	jobs;
#ifdef _WIN32
	HANDLE	ThreadHandle;
#else
	pthread_t* ThreadHandle;
#endif
	bool	Active;

public:
	O2Scheduler(void)
		: ThreadHandle(NULL)
		, Active(false)
	{
	};
	~O2Scheduler()
	{
		if (Active)
			Stop();
	};
	void Start(void)
	{
		if (!ThreadHandle) 
		{
			Lock();
			for (uint i = 0; i < jobs.size(); i++) 
			{
				jobs[i]->SetActive(true);
				if (jobs[i]->IsRunStartup())
					jobs[i]->SetLastTime(0);
				else
					jobs[i]->SetLastTime(time(NULL));
			}
			Unlock();

			Active = true;
#ifdef _WIN32
			ThreadHandle = (HANDLE)_beginthreadex(
				NULL, 0, StaticSchedulerThread, (void*)this, 0, NULL);
#else
			pthread_create(ThreadHandle,
					NULL,
					StaticSchedulerThread,
					NULL);
#endif
		}
	};
	void Stop(void)
	{
		if (ThreadHandle) {
			Active = false;
#ifdef _WIN32
			WaitForSingleObject(ThreadHandle, INFINITE);
			CloseHandle(ThreadHandle);
			ThreadHandle = NULL;
#else                   /** TODO: ここは多分後で改修する  */
			DosMocking::EventObject evt;
			evt.WaitForSingleObject( &evt, DosMocking::INFINITE );
			evt.CloseHandle( &evt );
#endif
		}
		for (uint i = 0; i < jobs.size(); i++) {
			jobs[i]->SetActive(false);
		}
		for (uint i = 0; i < jobs.size(); i++) {
			while (jobs[i]->IsWorking()) {
				TRACEW(jobs[i]->GetName());
				TRACEW(L"\n");
				Sleep(THREAD_ALIVE_WAIT_MS);
			}
			jobs[i]->ResetCounter();
		}
	};
	void Add(O2Job *job)
	{
		job->SetLastTime(time(NULL));

		Lock();
		jobs.push_back(job);
		Unlock();
	};

protected:

#ifdef _WIN32
	static uint WINAPI StaticSchedulerThread(void *data)
	{
		O2Scheduler *me = (O2Scheduler*)data;

		CoInitialize(NULL);
		me->Scheduler();
		CoUninitialize();

		return (0);
	};
#else
	static void* StaticSchedulerThread(void *data)
	{
		// 多分pthreadだとCoInitializeはいらない気がする
		O2Scheduler *me = (O2Scheduler*)data;
		me->Scheduler();
	};
#endif
	void Scheduler(void)
	{
		while (Active) {
			Sleep(1000);

			Lock();
			for (uint i = 0; i < jobs.size(); i++) {
				if (jobs[i]->IsActive() && !jobs[i]->IsWorking()) {
					if (time(NULL) - jobs[i]->GetLastTime() > jobs[i]->GetInterval()) {
						jobs[i]->SetLastTime(time(NULL)); //セット
						jobs[i]->SetWorking(true);

#ifdef _WIN32
						HANDLE handle = (HANDLE)_beginthreadex(
							NULL, 0, StaticJobStartThread, (void*)jobs[i], 0, NULL);
						CloseHandle(handle);
#else
			 			pthread_create(ThreadHandle,
			 					NULL,
			 					StaticJobStartThread,
							       (void*)jobs[i]);
#endif
					}
				}
			}
			Unlock();
		}
	}

#ifdef _WIN32
	static uint WINAPI StaticJobStartThread(void *data)
	{
		O2Job *job = (O2Job*)data;

		CoInitialize(NULL);
		job->JobThreadFunc();
		job->SetWorking(false);
		job->SetLastTime(time(NULL)); //再度セット
		CoUninitialize();

		//_endthreadex(0);
		return (0);
	};
#else
	static void* StaticJobStartThread(void *data)
	{
		O2Job *job = (O2Job*)data;

		// 多分pthreadだとCoInitializeはいらない気がする
		job->JobThreadFunc();
		job->SetWorking(false);
		job->SetLastTime(time(NULL)); //再度セット
	};
#endif
};
