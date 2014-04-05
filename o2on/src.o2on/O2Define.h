/* Copyright (C) 2006 o2on project. All rights reserved.
 * http://o2on.net/
 */

/*
 * project		: o2on
 * filename		: define.h
 * description	: 
 *
 */

#pragma once
#pragma warning(disable : 4786 4503 4819)
#include "typedef.h"
#include "debug.h"

// o2on cpp string define
#define MODULE			L"Boards"
#define DEFAULT_BBSMENU_URL	"http://menu.2ch.net/bbsmenu.html"
#define DOMAIN_2CH		"2ch.net"
#define DOMAIN_BBSPINK		"bbspink.com"
#define DOMAIN_MACHI		"machi.to"
#define FAKE_HOST_STR		"xxx"

// o2on target cpu & os define
#if defined(TARGET_CPU) && defined(TARGET_OS)
   #define O2_PLATFORM		STR(TARGET_CPU) "-" STR(TARGET_OS)
#else
   #if defined(_M_IX86) || (defined(__MINGW32__) && !defined(__MINGW64__))
      #define O2_PLATFORM 			"x86"
   #elif defined(_M_X64) || (defined(__LP64__) || defined(_LP64))
      #define O2_PLATFORM 			"x64"
   #endif
#endif

#ifdef PLATFORM_WIN32
   #define EOL				L"\r\n"
#elif PLATFORM_LINUX
   #define EOL				L"\n"
#elif PLATFORM_APPLE
   #define EOL				L"\r"
#else
   #define EOL				L"\r\n"
#endif

#define DEFAULT_XML_CHARSET		"utf-8"

#define RECVBUFFSIZE			5120
#define CONNECT_TIMEOUT_S		5
#define SOCKET_TIMEOUT_S		15
#define SELECT_TIMEOUT_MS		3000
#define RECV_SIZE_LIMIT			(3*1024*1024)

#define THREAD_ALIVE_WAIT_MS		300
#define TIMESTR_BUFF_SIZE		32

#define O2_MAX_NAME_LEN			8
#define O2_MAX_COMMENT_LEN		512
#define O2_MAX_KEY_URL_LEN		96
#define O2_MAX_KEY_TITLE_LEN		32
#define O2_MAX_KEY_NOTE_LEN		32
#define O2_SAKUKEY_LIMIT		10
#define O2_BROADCAST_PATH_LIMIT		4




/* for XML Parse Handler */
#ifdef _WIN32
   #define MATCHLNAME(n)	(_wcsicmp(localname, (n)) == 0)
   #define MATCHSTR(n)		(_wcsicmp(str, (n)) == 0)
#else
   #include <string.h>
   #define MATCHLNAME(n)	wcscasecmp(reinterpret_cast<const wchar_t*>(localname), n) == 0
   #define MATCHSTR(n)		wcscasecmp(reinterpret_cast<const wchar_t*>(str), n) == 0
   #define _wcsnicmp(x, y, z)	wcscasecmp(x, y)
#endif

/* debug flag */
#if defined(_DEBUG) || defined(DEBUG)
   #define O2DEBUG 	1
#else
   #define O2DEBUG 	0
#endif

/* */
#if 1 //O2DEBUG
   #define CLEAR_WORKSET NULL
#else
   #define CLEAR_WORKSET (SetProcessWorkingSetSize(GetCurrentProcess(), 0xffffffff, 0xffffffff))
#endif
