#include "common.h"

// ---------------------------------------------------------------------------
//	byte2hex 
//	byte2whex
// ---------------------------------------------------------------------------

void byte2hex(const byte *in, uint len, string &out)
{

	out.erase();
	for (uint i = 0; i < len; i++) {
		out += hex[(in[i] >> 4)];
		out += hex[(in[i] & 0x0f)];
	}
}
void byte2whex(const byte *in, uint len, wstring &out)
{

	out.erase();
	for (uint i = 0; i < len; i++) {
		out += whex[(in[i] >> 4)];
		out += whex[(in[i] & 0x0f)];
	}
}




// ---------------------------------------------------------------------------
//	hex2byte 
//	whex2byte
// ---------------------------------------------------------------------------

void hex2byte(const char *in, uint len, byte *out)
{
	for (uint i = 0; i < len; i+=2) {
		char c0 = in[i+0];
		char c1 = in[i+1];
		byte c = (
			((c0 & 0x40 ? (c0 & 0x20 ? c0-0x57 : c0-0x37) : c0-0x30)<<4) |
			((c1 & 0x40 ? (c1 & 0x20 ? c1-0x57 : c1-0x37) : c1-0x30))
			);
		out[i/2] = c;
	}
}
void whex2byte(const wchar_t *in, uint len, byte *out)
{
	for (uint i = 0; i < len; i+=2) {
		wchar_t c0 = in[i+0];
		wchar_t c1 = in[i+1];
		byte c = (
			((c0 & 0x0040 ? (c0 & 0x0020 ? c0-0x0057 : c0-0x0037) : c0-0x0030)<<4) |
			((c1 & 0x0040 ? (c1 & 0x0020 ? c1-0x0057 : c1-0x0037) : c1-0x0030))
			);
		out[i/2] = c;
	}
}

namespace DosMocking {
	long getGmtOffset() 
	{ 
		time_t now = time(NULL);

		struct tm *gm = gmtime(&now);
		time_t gmt = mktime(gm);

		struct tm *loc = localtime(&now);
		time_t local = mktime(loc);

		return static_cast<long>(difftime(local, gmt));
	}
}
