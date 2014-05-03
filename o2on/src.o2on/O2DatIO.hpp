/* Copyright (C) 2006 o2on project. All rights reserved.
 * http://o2on.net/
 */

/*
 * project		: o2on
 * filename		: O2DatIO.h
 * description	: dat I/O class
 *
 */

#pragma once
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/convenience.hpp>
#include "O2DatPath.hpp"
#include "O2DatDB.hpp"
#include "O2Logger.hpp"
#include "O2Profile.hpp"
#include "O2Key.hpp"
#include "mutex.hpp"
#include "O2ProgressInfo.hpp"
#include "sqlite3.h"




class O2DatIO
{
protected:
	O2DatDB			*DatDB;
	O2Logger		*Logger;
	O2Profile		*Profile;
	uint64			ClusterSize;
	HWND			hwndEmergencyHaltCallback;
	UINT			msgEmergencyHaltCallback;
	O2ProgressInfo		*ProgressInfo;

#ifdef _WIN32 /** HANDLE for win32 thread */
	HANDLE			RebuildDBThreadHandle;
	HANDLE			ReindexThreadHandle;
	HANDLE			AnalyzeThreadHandle;
#else /** For POSIX thread processing */
	pthread_t		RebuildDBThreadHandle;
	pthread_t		ReindexThreadHandle;
	pthread_t		AnalyzeThreadHandle;
	enum O2DatIOHandleEnum { REBUILDDB = 0, REINDEX = 1, ANALYZE = 2 }; // 列挙型の定義
	neosmart::neosmart_event_t handles[3];
#endif

	bool			LoopRebuildDB;

protected:
	uint64 GetDiskFileSize(uint64 size);

public:
	O2DatIO(O2DatDB *db, O2Logger *lgr, O2Profile *prof, O2ProgressInfo *proginfo);
	~O2DatIO();

	bool CheckQuarterOverflow(uint64 add_size);
	void SetEmergencyHaltCallbackMsg(HWND hwnd, UINT msg);

	bool KakoHantei(const O2DatPath &datpath);
	bool KakoHantei(const char *dat, uint64 len, bool is_be);
	bool CheckDat(const char *in, uint64 inlen);
	bool CheckDat2(const char *in, uint64 inlen);
	bool GetTitle(O2DatPath &datpath);
	uint64 GetSize(const O2DatPath &datpath);

	bool Load(const O2DatPath &datpath, uint64 offset, string &out);
	bool Load(const hashT &hash, uint64 offset, string &out, O2DatPath &datpath);
	bool RandomGet(string &out, O2DatPath &datpath);
	bool RandomGetInBoard(const wchar_t *domain, const wchar_t *bbsname, string &out, O2DatPath &datpath);
	bool Delete(const hashListT &hashlist);

	uint64 Put(O2DatPath &datpath, const char *dat, uint64 len, uint64 startpos);
	bool ExportToXML(const wchar_t *domain, const wchar_t *bbsname, string &out);
	bool Dat2HTML(const hashT &hash, string &out, string &encoding);

	size_t GetLocalFileKeys(O2KeyList &keylist, time_t publish_tt, size_t limit);

public:
	void RebuildDB(void);
	void StopRebuildDB(void);
	void RebuildDBThread(const wchar_t *dir, uint level, O2DatRecList &reclist);
	void Reindex(void);
	void Analyze(void);

#ifdef _WIN32 /** for win32 thread */
	static uint WINAPI StaticRebuildDBThread(void *data);
	static uint WINAPI StaticReindexThread(void *data);
	static uint WINAPI StaticAnalyzeThread(void *data);
#else   /** for POSIX thread processing */
	static void* StaticRebuildDBThread(void *data);
	static void* StaticReindexThread(void *data);
	static void* StaticAnalyzeThread(void *data);
#endif
};
