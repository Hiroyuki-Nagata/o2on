/* Copyright (C) 2006 o2on project. All rights reserved.
 * http://o2on.net/
 */

/*
 * project		: o2on
 * filename		: O2Client.h
 * description	: o2on agent class
 *
 */

#pragma once
#include "O2Logger.hpp"
#include "O2SocketSession.hpp"

#ifndef _WIN32
   #include <pevents.h>
   #include <netinet/ip.h>
#endif

#ifndef _MSC_VER /** VC++以外はwxWidgetsを使用 */
   #include <wx/version.h>
#endif

/*
 *	O2Client
 */
class O2Client
{
protected:
	O2Logger	*Logger;

	wstring		ClientName;
	uint64		SessionLimit;
	uint64		SessionPeak;
	uint64		RecvSizeLimit;
	uint64		TotalSessionCount;
	uint64		TotalConnectError;
	uint64		RecvByte;
	uint64		SendByte;
	bool		Active;

#ifdef _WIN32 /** HANDLE for win32 thread */
	HANDLE		LaunchThreadHandle;
	HANDLE		NetIOThreadHandle;
#else /** For POSIX thread processing */
	pthread_t	 LaunchThreadHandle;
	pthread_t	 NetIOThreadHandle;
	neosmart::neosmart_event_t handles[2];
#endif
	HWND		hwndSetIconCallback;
	UINT		msgSetIconCallback;

	O2SocketSessionPList	queue;
	O2SocketSessionPSet	connectss;
	O2SocketSessionPList	sss;

	Mutex		QueueLock;
	Mutex		ConnectSessionLock;
	Mutex		SessionListLock;

#ifdef _WIN32 /** EventObject class for win32 thread */
	EventObject	QueueExistSignal;
	EventObject	SessionExistSignal;
#else /** For Boost threads processing */
	DosMocking::EventObject	QueueExistSignal;
	DosMocking::EventObject	SessionExistSignal;
#endif

	struct ConnectThreadParam {
		O2Client *client;
		O2SocketSession *ss;
	};

public:
	O2Client(const wchar_t *name, O2Logger *lgr);
	~O2Client();

	bool	Start(void);
	bool	Stop(void);
	bool	Restart(void);
	bool	IsActive(void);

	uint64	GetSessionLimit(void);
	bool	SetSessionLimit(uint64 limit);
	uint64	GetRecvSizeLimit(void);
	void	SetRecvSizeLimit(uint64 limit);
	uint64	GetSessionPeak(void);
	uint64	GetTotalSessionCount(void);
	uint64	GetRecvByte(void);
	uint64	GetSendByte(void);
	void	ResetCounter(void);
	size_t	GetSessionList(O2SocketSessionPList &out);

	void SetIconCallbackMsg(HWND hwnd, UINT msg);
	void AddRequest(O2SocketSession *ss, bool high_priority = false);

protected:
	virtual void OnClientStart(void) = 0;
	virtual void OnClientStop(void) = 0;
	virtual void OnConnect(O2SocketSession *ss) = 0;
	virtual void OnRecv(O2SocketSession *ss) = 0;
	virtual void OnSend(O2SocketSession *ss) = 0;
	virtual void OnClose(O2SocketSession *ss) = 0;

private:


#ifdef _WIN32 /** for win32 thread */
	static uint WINAPI StaticLaunchThread(void *data);
	static uint WINAPI StaticConnectionThread(void *data);
	static uint WINAPI StaticNetIOThread(void *data);
#else /** for POSIX thread */
	static void* StaticLaunchThread(void *data);
	static void* StaticConnectionThread(void *data);
	static void* StaticNetIOThread(void *data);
#endif
	void LaunchThread(void);
	void ConnectionThread(O2SocketSession *ss);
	void NetIOThread(void);
	int connect2(SOCKET s, const struct sockaddr *name, int namelen, int timeout);
};
