/* Copyright (C) 2006 o2on project. All rights reserved.
 * http://o2on.net/
 */

/*
 * project		: 
 * filename		: file.h
 * description	: 
 *
 */

#pragma once

#ifdef _WIN32
   #include <windows.h>
   #include "file_msw.hpp"
#else
   #include "file_unx.hpp"
#endif
