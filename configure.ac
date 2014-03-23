#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([o2on], [2.4.5], [newserver002@gmail.com])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE
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
                ;;
        *-*-linux*)
                AC_DEFINE(PLATFORM_LINUX, 1, [Platform is Linux])
                PLATFORM_STRING="Linux"
                AC_SUBST(PLATFORM_STRING)
                ;;
        *-*-darwin*)
                AC_DEFINE(PLATFORM_APPLE, 1, [Platform is Apple])
                PLATFORM_STRING="Apple"
                AC_SUBST(PLATFORM_STRING)
                ;;
        *-*-*bsd*)
                AC_DEFINE(PLATFORM_BSD, 1, [Platform is BSD])
                PLATFORM_STRING="BSD"
                AC_SUBST(PLATFORM_STRING)
                ;;
        *)
                AC_MSG_WARN([*** Please add $host to configure.ac checks!])
                ;;
esac

# Checks for compiler
AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])

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
fi

# Boost
BOOST_REQUIRE([1.49.0])
BOOST_REGEX
BOOST_THREADS

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_CHECK_FUNCS([gethostbyaddr gethostbyname inet_ntoa memset mkdir pow select socket strchr strrchr strspn strstr strtol strtoul])

AC_CONFIG_FILES([Makefile
		 o2on/Makefile
                 o2on/src.browser/Makefile
                 o2on/src.o2on/Makefile])
AC_OUTPUT
