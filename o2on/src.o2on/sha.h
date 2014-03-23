/* Copyright (C) 2006 o2on project. All rights reserved.
 * http://o2on.net/
 */

/*
 * project		: 
 * filename		: sha.h
 * description	: 
 *
 */

#pragma once

#ifdef _WIN32
   #include "../cryptopp/sha.h"
#else
   #include <cryptopp/sha.h>
#endif

#include "barray.h"
#include <list>

#define HASHSIZE		(CryptoPP::SHA1::DIGESTSIZE)	// 20 byte
#define HASH_BITLEN		(CryptoPP::SHA1::DIGESTSIZE*8)	// 160 bit

typedef barrayT<HASHSIZE>	hashT;
typedef std::list<hashT>	hashListT;
