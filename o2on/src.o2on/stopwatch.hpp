#include <string>
#include "debug.hpp"
#pragma once

#ifdef _WIN32
   #include <windows.h>
   #include <mmsystem.h>
   #pragma comment(lib, "winmm.lib")
#else /** Unix用の時間計算関数 */

//
// ref: http://d.hatena.ne.jp/shiku_otomiya/20130324/p1
//
#define ONESECONNANOSEC		1000000000ll

unsigned long long timespecto64bitval(timespec* tp)
{
	if(tp == NULL) return 0;
	return (unsigned long long)((long long)(tp->tv_sec) * ONESECONNANOSEC + (long long)(tp->tv_nsec));
}

unsigned long long getdifftimespec(timespec* tpd,timespec* tpb)
{
	time_t tds; long tdn;
	if(tpd == NULL || tpb == NULL) return 0;
	tds = tpd->tv_sec - tpb->tv_sec; tdn = tpd->tv_nsec - tpb->tv_nsec;
	return (unsigned long long)((long long)tds * ONESECONNANOSEC + (long long)tdn);
}

#endif /** Unix(Linux) end */

#ifdef __MACH__ /** Mac OS */
   #include <mach/clock.h>
   #include <mach/mach.h>

unsigned long long getdifftimespec(mach_timespec_t* mtpd,mach_timespec_t* mtpb)
{
	struct timespec tpd,tpb;

	tpd.tv_sec  = mtpd->tv_sec;
	tpd.tv_nsec = mtpd->tv_nsec;
	tpb.tv_sec  = mtpb->tv_sec;
	tpb.tv_nsec = mtpb->tv_nsec;

	return getdifftimespec(&tpd, &tpb);
}

#endif

/**
 * 時間計測用クラス
 */
class stopwatch
{
private:
	std::string title;
	DWORD ln;
	DWORD start;
	DWORD d;

#if !defined(_WIN32) && !defined(__MACH__) /** for Linux */
	struct timespec t1,t2;
	unsigned long long td;
#elif !defined(_WIN32) && defined(__MACH__) /** for Mac OS */
	mach_timespec_t t1,t2;
	unsigned long long td;
	clock_serv_t cclock;
#endif

public:
	stopwatch(const char *t, DWORD l = 1)
		: title(t)
		, ln(l)
		, start(0)
		, d(0)
	{
#ifdef _WIN32 /** Windows */
		timeBeginPeriod(1);
		start = timeGetTime();
#elif __MACH__ /** Mac OS */
		host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
		clock_get_time(cclock, &t1);
		mach_port_deallocate(mach_task_self(), cclock);
		td = ONESECONNANOSEC;
#else /** Linux or other */
	      	td = ONESECONNANOSEC;
		clock_gettime(CLOCK_REALTIME,&t1);
#endif

		OutputDebugStringA(title.c_str());
		OutputDebugStringA(" ==>\n");
	}

	~stopwatch()
	{
		end();
	}

	void end(void)
	{
		if (start) {
#ifdef _WIN32 /** Windows */
			d = timeGetTime() - start;
#elif __MACH__ /** Mac OS */
			host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
			clock_get_time(cclock, &t2);
			mach_port_deallocate(mach_task_self(), cclock);
			d = getdifftimespec(&t2,&t1);
#else /** Linux or other */
			clock_gettime(CLOCK_REALTIME,&t2);
			d = getdifftimespec(&t2,&t1);
#endif
			if (d >= ln) {
				OutputDebugStringA(title.c_str());
				char tmp[32];
				sprintf_s(tmp, 32, ":%d\n", d);
				OutputDebugStringA(tmp);
			}
		}
#ifdef _WIN32
		timeEndPeriod(1);
#endif
	}
};
