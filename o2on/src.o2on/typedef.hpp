/* Copyright (C) 2006 o2on project. All rights reserved.
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

#ifdef _WIN32
   /* for XML Parse Handler */
   #define MATCHLNAME(n)	(_wcsicmp(localname, (n)) == 0)
   #define MATCHSTR(n)		(_wcsicmp(str, (n)) == 0)
#else
   /** UNIX only typedef for MSW dirty typedefs...               */
   /** @see http://www.jbox.dk/sanos/source/include/win32.h.html */
   #include <cstring>
   #include <cstdlib>
   #include <iostream>
   #include <time.h>
   #include <utime.h>
   #include <sys/stat.h>
   #include <netdb.h>
   #include <wchar.h>
   #include <boost/filesystem.hpp>
   #include <string.h>

   typedef void*	 HANDLE;
   typedef HANDLE	 HWND;
   typedef HANDLE	 HQUERY;
   typedef HANDLE	 HCOUNTER;
   typedef unsigned long DWORD;
   typedef unsigned int  UINT;
   typedef struct _FILETIME {
      DWORD dwLowDateTime;
      DWORD dwHighDateTime;
   } FILETIME, *PFILETIME;
   #define FALSE	 false
   #define TRUE		 true
   #define WINAPI       /** Linux or other don't call WINAPI... */
   #define _utimbuf	 utimbuf
   #define _stat         stat
   #define _stat64       stat

   #define INVALID_HANDLE_VALUE ((HANDLE) -1)

   #define GENERIC_READ                     0x80000000
   #define GENERIC_WRITE                    0x40000000
   #define GENERIC_EXECUTE                  0x20000000
   #define GENERIC_ALL                      0x10000000

   #define FILE_SHARE_READ                  0x00000001
   #define FILE_SHARE_WRITE                 0x00000002
   #define FILE_SHARE_DELETE                0x00000004

   #define FILE_ATTRIBUTE_READONLY     	    0x00000001
   #define FILE_ATTRIBUTE_NORMAL	    0x00000080
   #define FILE_ATTRIBUTE_TEMPORARY	    0x00000100

   #define CREATE_NEW                       1
   #define CREATE_ALWAYS                    2
   #define OPEN_EXISTING                    3
   #define OPEN_ALWAYS                      4
   #define TRUNCATE_EXISTING                5

   #define _stricmp(x, y)	    strcasecmp(x, y)
   #define _mkdir(x)		    mkdir(x)
   #define _wmkdir(x)		    boost::filesystem::create_directory(x)
   #define _wutime(x, y)	    utime(x, y)
   #define _tstat(x, y)		    stat(x, y)
   #define _wtoi(x)		    wcstol(x, NULL, 10)
   #define _access_s(x, y)	    access(x, y)
   #define wcscpy_s(dest, buf, src) wcscpy(dest, src)
   #define wcstok_s(x, y, z)        wcstok(x, y, z)
   #define strtok_s(x, y, z)        strtok_r(x, y, z)
   #define MATCHLNAME(n)	    wcscasecmp(reinterpret_cast<const wchar_t*>(localname), n) == 0
   #define MATCHSTR(n)		    wcscasecmp(reinterpret_cast<const wchar_t*>(str), n) == 0
   #define _wcsnicmp(x, y, z)	    wcscasecmp(x, y) 
   #define _strtoui64(x, y, z)	    strtoull(x, y, z)
   #define _wcstoui64(x, y, z)	    wcstoul(x, y, z)
   #define _fseeki64(x, y, z)       fseeko(x, y, z)

   /** MSW's many many ~_s function series... */
   #define vsprintf_s(str, format, ...)				vsprintf(str, format, __VA_ARGS__)
   #define sprintf_s(buffer, buffer_size, stringbuffer, ...)	sprintf(buffer, stringbuffer, __VA_ARGS__)
   #define swprintf_s(buffer, buffer_size, stringbuffer, ...)	swprintf(buffer, buffer_size, stringbuffer, __VA_ARGS__)
   #define fopen_s(pFile,filename,mode)				((*(pFile))=fopen((filename),(mode)))==NULL
#endif

/** When o2on don't use wxWidgets in MinGW/Cygwin */
#if defined(_WIN32) && !defined(__WXWINDOWS__)
   /** Sleep equivalent */
   #define Sleep(x)		    usleep((x)*1000)

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
#else
   #include <wx/utils.h>
   #define Sleep(x)		    wxMilliSleep(x)
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

/** Some WinSock to BSD socket equivalent */
#ifndef _WIN32
   typedef uint		SOCKET;
   typedef hostent	HOSTENT;
   #define INVALID_SOCKET -1
   #define ioctlsocket(x,y,z) ioctl(x,y,z)
#endif

/** MAX_PATH defination */
#ifdef _WIN32
   #if defined(WIN32) && !defined(__MINGW32__) && !defined(__MINGW64__)
      #define MAX_PATH _MAX_PATH
   #else  // MSVC
      #define MAX_PATH FILENAME_MAX
   #endif // MinGW32,64
#else
   #define _MAX_PATH _POSIX_PATH_MAX
   #define MAX_PATH _POSIX_PATH_MAX
#endif 

/** macro expansion */
#define XSTR(x) #x
#define STR(x)  XSTR(x)

#ifndef _WIN32
   #define __STR2WSTR(str)    L##str
   #define _STR2WSTR(str)     __STR2WSTR(str)
#endif

/** windows OutputDebugString is std::cerr */
#ifndef _WIN32
   #define OutputDebugStringA(x) std::cerr << x
   #define OutputDebugStringW(x) std::cerr << x
#endif
