/* Copyright (C) 2006 o2on project. All rights reserved.
 * http://o2on.net/
 */

/*
 * project		: 
 * filename		: debug.h
 * description		: debug log defination etc... 
 *
 */

#if (defined(_DEBUG) || defined(DEBUG)) && !defined(__GNUC__) && !defined(__clang__)
   #define ASSERT(x) \
    	if (!(x)) { \
    		MyTRACE("Assertion failed! in %s (%d)\n", \
    			__FILE__, __LINE__); \
    		DebugBreak(); \
    	}
   #define VERIFY(x)	ASSERT(x)
   #define SLASH() /
   #define TRACE	OutputDebugString
   #define TRACEA	OutputDebugStringA
   #define TRACEW	OutputDebugStringW
#elif !defined(_DEBUG) && !defined(DEBUG) && !defined(__GNUC__) && !defined(__clang__) 
   #define ASSERT(x)
   #define VERIFY(x)	x
   #define SLASH() /
   #define TRACE  SLASH()SLASH() /** This macro work with VC++ */
   #define TRACEA SLASH()SLASH() /** but, gcc or clang is not  */
   #define TRACEW SLASH()SLASH() /** work...:<                 */
#elif (defined(_DEBUG) || defined(DEBUG)) && (defined(__GNUC__) || defined(__clang__))
   #define ASSERT(x) \
    	if (!(x)) { \
    		MyTRACE("Assertion failed! in %s (%d)\n", \
    			__FILE__, __LINE__); \
    		DebugBreak(); \
    	}
   #define VERIFY(x)	ASSERT(x)
   #define SLASH() /
   #define TRACE(x)  printf("%s(): %s", __func__, #x)
   #define TRACEA(x) printf("%s(): %s", __func__, #x)
   #define TRACEW(x) wprintf("%s(): %s", __func__, #x)
#else
   #define ASSERT(x)
   #define VERIFY(x)	x
   #define SLASH() /
   #define TRACE(x)  do {} while (0)
   #define TRACEA(x) do {} while (0)
   #define TRACEW(x) do {} while (0)
#endif
