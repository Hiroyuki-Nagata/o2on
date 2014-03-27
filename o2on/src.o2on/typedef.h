﻿/* Copyright (C) 2006 o2on project. All rights reserved.
 * http://o2on.net/
 */

/*
 * project		: 
 * filename		: typedef.h
 * description		: header file for typedef settings 
 *
 */

#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>

/** UNIX only typedef for MSW dirty typedefs...               */
/** @see http://www.jbox.dk/sanos/source/include/win32.h.html */
#ifndef _WIN32
   #include <cstring>
   typedef void*	 HANDLE;
   typedef HANDLE	 HWND;
   typedef unsigned long DWORD;
   typedef unsigned int  UINT;
   #define FALSE	false
   #define TRUE		true
   #define WINAPI
   #define INVALID_HANDLE_VALUE ((HANDLE) -1)

   /* #define GENERIC_READ                     0x80000000 */
   /* #define GENERIC_WRITE                    0x40000000 */
   /* #define GENERIC_EXECUTE                  0x20000000 */
   /* #define GENERIC_ALL                      0x10000000 */

   /* #define FILE_SHARE_READ                  0x00000001 */
   /* #define FILE_SHARE_WRITE                 0x00000002 */
   /* #define FILE_SHARE_DELETE                0x00000004 */

   /* #define CREATE_NEW                       1 */
   /* #define CREATE_ALWAYS                    2 */
   /* #define OPEN_EXISTING                    3 */
   /* #define OPEN_ALWAYS                      4 */
   /* #define TRUNCATE_EXISTING                5 */

   /** Linux or other don't call WINAPI... */
   #define _stricmp(x, y)	strcasecmp(x, y)
   #define Sleep(x)		usleep((x)*1000)

   /** MSW's many many ~_s function series... */
   #define sprintf_s(buffer, buffer_size, stringbuffer, ...) sprintf(buffer, stringbuffer, __VA_ARGS__)
   #define swprintf_s(buffer, buffer_size, stringbuffer, ...) swprintf(buffer, buffer_size, stringbuffer, __VA_ARGS__)
#endif

/* unsigned */
typedef unsigned char				byte;
typedef unsigned short				ushort;
typedef unsigned int				uint;
typedef unsigned long				ulong;

#if defined(_MSC_VER) && (_MSC_VER <= 1500)
    typedef __int64				uint64;
#else
    /* gcc or VC10 */
    #include <stdint.h>
    typedef int64_t				uint64;
#endif

/* char and strings */
typedef std::string				string;
typedef std::wstring				wstring;

/* string container */
typedef std::vector<string>			strarray;
typedef std::vector<wstring>			wstrarray;
typedef std::set<string>			stringset;
typedef std::set<wstring>			wstringset;
typedef std::map<string,string>		        strmap;
typedef std::map<wstring,wstring>		wstrmap;
typedef std::map<string,uint64>			strnummap;
typedef std::map<wstring,uint64>		wstrnummap;

/* tstring */
#if defined(UNICODE) || defined(_UNICODE) || !defined(_WIN32)
   typedef std::wstring				tstring;
   typedef std::wfstream			tfstream;
   typedef std::wifstream			tifstream;
   typedef std::wofstream			tofstream;
#else
   typedef std::string				tstring;
   typedef std::fstream				tfstream;
   typedef std::ifstream			tifstream;
   typedef std::ofstream			tofstream;
#endif

#define FOUND(i) ((i) != tstring::npos)

/* some _T macro trick */
#ifndef _WIN32
   #if defined(UNICODE) || defined(_UNICODE) || !defined(_WIN32)
      #define __T(x)      L ## x
   #else
      #define __T(x)      x
   #endif
    
   #define _T(x)       __T(x)
   #define _TEXT(x)    __T(x)
#endif

/** Some WinSock to BSD socket equivalent */
#ifndef _WIN32
   typedef uint		SOCKET;
   #define INVALID_SOCKET -1
#endif

/** macro expansion */
#define XSTR(x) #x
#define STR(x)  XSTR(x)
