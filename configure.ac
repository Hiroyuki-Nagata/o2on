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

# Checks for libraries.

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
