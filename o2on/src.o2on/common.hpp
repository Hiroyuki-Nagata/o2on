/* Copyright (C) 2014 o2on project. All rights reserved.
 * http://o2on.net/
 */

/*
 * project		: o2on
 * filename		: commmon.h
 * description		: base utility code
 *
 */
#include "typedef.hpp"

extern void byte2hex(const byte *in, uint len, string &out);
extern void byte2whex(const byte *in, uint len, wstring &out);
extern void hex2byte(const char *in, uint len, byte *out);
extern void whex2byte(const wchar_t *in, uint len, byte *out);

namespace DosMocking 
{
  static long getGmtOffset();
}
