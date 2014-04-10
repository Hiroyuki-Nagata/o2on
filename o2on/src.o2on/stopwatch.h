#include <string>
#include "debug.h"
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

unsigned long long timespecto64bitval(struct timespec *tp)
{
	if(tp == NULL) return 0;
	return (unsigned long long)((long long)(tp->tv_sec) * ONESECONNANOSEC + (long long)(tp->tv_nsec));
}

unsigned long long getdifftimespec(struct timespec *tpd,struct timespec *tpb)
{
	time_t tds; long tdn;
	if(tpd == NULL || tpb == NULL) return 0;
	tds = tpd->tv_sec - tpb->tv_sec; tdn = tpd->tv_nsec - tpb->tv_nsec;
	return (unsigned long long)((long long)tds * ONESECONNANOSEC + (long long)tdn);
}
#endif /** end */

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

#ifndef _WIN32 /** for Unix */
	struct timespec t1,t2;
	unsigned long long tb,td;
#endif

public:
	stopwatch(const char *t, DWORD l = 1)
		: title(t)
		, ln(l)
		, start(0)
		, d(0)
	{
#ifdef _WIN32
		timeBeginPeriod(1);
		start = timeGetTime();
#else
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
#ifdef _WIN32
			d = timeGetTime() - start;
#else
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
