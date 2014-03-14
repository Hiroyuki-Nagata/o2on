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
AC_PROG_MKDIR_P
AC_LIBTOOL_WIN32_DLL
LT_INIT

# Checks for libraries.

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
