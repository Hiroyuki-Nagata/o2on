#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([o2on], [2.4.5], [newserver002@gmail.com])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_SRCDIR([o2on/src.browser/resource.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_LANG_PUSH([C++])
AC_PROG_MKDIR_P
AC_LIBTOOL_WIN32_DLL
LT_INIT

# Checks for HostOS
case "$host" in
        *-*-mingw*|*-*-cygwin*)
                AC_DEFINE(PLATFORM_WIN32, 1, [Platform is Win32])
                PLATFORM_STRING="Win32"
                AC_SUBST(PLATFORM_STRING)
		AM_CONDITIONAL(USE_PTHREAD, false)
                ;;
        *-*-linux*)
                AC_DEFINE(PLATFORM_LINUX, 1, [Platform is Linux])
                PLATFORM_STRING="Linux"
                AC_SUBST(PLATFORM_STRING)
		AM_CONDITIONAL(USE_PTHREAD, true)
                ;;
        *-*-darwin*)
                AC_DEFINE(PLATFORM_APPLE, 1, [Platform is Apple])
                PLATFORM_STRING="Apple"
                AC_SUBST(PLATFORM_STRING)
		AM_CONDITIONAL(USE_PTHREAD, true)
                ;;
        *-*-*bsd*)
                AC_DEFINE(PLATFORM_BSD, 1, [Platform is BSD])
                PLATFORM_STRING="BSD"
                AC_SUBST(PLATFORM_STRING)
		AM_CONDITIONAL(USE_PTHREAD, true)
                ;;
        *)
                AC_MSG_WARN([*** Please add $host to configure.ac Checks!])
                ;;
esac

# Checks for TargetCPU
case $target in
	x86_64-*)
	AC_DEFINE([_LARGEFILE_SOURCE], [], [Large file support])
	AC_DEFINE([_FILE_OFFSET_BITS], [64], [Large file support])
	;;
esac


# Checks for compiler
AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])
# If Clang++ & Mac OS X, add -stdlib=libc++...
AC_CHECK_DECLS([__clang__],
	       [AC_CHECK_DECLS([__APPLE__], 
	       [AM_CXXFLAGS="-stdlib=libc++ $CXXFLAGS $AM_CXXFLAGS"])
	       ])

#
# Checks for libraries.
#

# libxerces-c
AC_CHECK_HEADER([xercesc/util/XercesVersion.hpp], 
                [AM_CPPFLAGS="$CPPFLAGS $AM_CPPFLAGS"], 
                [AC_MSG_ERROR([xerces include file not found. o2on need xerces-c.])])

AC_CHECK_LIB([xerces-c], 
	     [main], 
	     [has_xerces_c=1],
	     [has_xerces_c=0])

if test $has_xerces_c = 0; then
  AC_MSG_ERROR("ERROR: libxerces-c not found. o2on need xerces-c.")
else
  LIBS="$LIBS -lxerces-c"
fi

# libcryptopp(crypto++)
AC_CHECK_HEADER([cryptopp/sha.h], 
                [AM_CPPFLAGS="$CPPFLAGS $AM_CPPFLAGS"], 
                [AC_MSG_ERROR([cryptopp include file not found. o2on need cryptopp.])])

AC_CHECK_LIB([cryptopp], 
	     [main], 
	     [has_cryptopp=1],
             [has_cryptopp=0])

if test $has_cryptopp = 0; then
  AC_MSG_ERROR("ERROR: cryptopp not found. o2on need cryptopp.")
else
  LIBS="$LIBS -lcryptopp"
fi

# Checks for zlib
AC_CHECK_HEADER([zlib.h], 
                [AM_CPPFLAGS="$CPPFLAGS $AM_CPPFLAGS"], 
                [AC_MSG_ERROR([zlib include file not found. o2on need zlib.])])

AC_CHECK_LIB(z,gzopen,has_zlib=1,has_zlib=0)
if test $has_zlib = 0; then
  AC_MSG_ERROR("ERROR: zlib not found. o2on need zlib.")
else
  LIBS="$LIBS -lz"
fi

# Checks for pthread
AC_CHECK_LIB(pthread,pthread_create,has_pthread=1,has_pthread=0)
if test $has_pthread = 0; then
  AC_MSG_ERROR("ERROR: pthread not found. o2on need pthread.")
fi

# Checks for sqlite3
AC_CHECK_HEADER([sqlite3.h], 
                [AM_CPPFLAGS="$CPPFLAGS $AM_CPPFLAGS"], 
                [AC_MSG_ERROR([sqlite3 include file not found. o2on need sqlite3.])])

AC_CHECK_LIB(sqlite3,sqlite3_open,has_sqlite3=1,has_sqlite3=0)
if test $has_sqlite3 = 0; then
  AC_MSG_ERROR("ERROR: sqlite3 not found. o2on need sqlite3.")
else
  LIBS="$LIBS -lsqlite3"
fi

# Boost
BOOST_REQUIRE([1.49.0])
BOOST_REGEX
BOOST_THREADS
BOOST_FILESYSTEM

# wxWidgets
AM_OPTIONS_WXCONFIG
reqwx=2.8.0
AM_PATH_WXCONFIG($reqwx, wxWin=1)
if test "$wxWin" != 1; then
	case "$host" in
	        *-*-mingw*|*-*-cygwin*)
	 	AC_MSG_WARN([o2on will be build without wxWidgets...(use MFC)])
	 	AM_CONDITIONAL(USE_WXWIDGETS, false)
	 	;;
	        *)
	 	AC_MSG_ERROR([
	 		wxWidgets must be installed on your system.
	 
	 		Please check that wx-config is in path, the directory
	 		where wxWidgets libraries are installed (returned by
	 		'wx-config --libs' or 'wx-config --static --libs' command)
	 		is in LD_LIBRARY_PATH or equivalent variable and
	 		wxWidgets version is $reqwx or above.
	 		])
	        ;;
	esac
else
	AM_CONDITIONAL(USE_WXWIDGETS, true)
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_CHECK_FUNCS([gethostbyaddr gethostbyname inet_ntoa memset mkdir pow select socket strchr strrchr strspn strstr strtol strtoul])

# Checks for subdirectory
if test ! -d o2on/pevents/; then
   AC_MSG_RESULT([pevents is not found. exec git submodule...])
   git submodule init;
   git submodule update;
fi

AC_CONFIG_FILES([Makefile
		 o2on/Makefile
                 o2on/src.browser/Makefile
                 o2on/src.o2on/Makefile])
AC_OUTPUT

# Trace all options
AC_MSG_RESULT([])
AC_MSG_RESULT([])
AC_MSG_RESULT([Configured $PACKAGE_STRING for $host])
AC_MSG_RESULT([CXX     :	$CXX])
AC_MSG_RESULT([CPPFLAGS:	$CPPFLAGS])
AC_MSG_RESULT([CXXFLAGS:	$CXXFLAGS])
AC_MSG_RESULT([LIBS    :	$LIBS])