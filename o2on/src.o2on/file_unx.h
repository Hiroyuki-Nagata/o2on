/* Copyright (C) 2006 o2on project. All rights reserved.
 * http://o2on.net/
 */

/*
 * project		: 
 * filename		: file_unx.h
 * description	: file open/read/write for unix
 *
 */

#pragma once

#include "dataconv.h"

#define MODE_R	0x00000001
#define MODE_W	0x00000002
#define MODE_A	0x00000003

// ---------------------------------------------------------------------------
//
//	class File
//
//	移植時の注意：
//	  Read時  : バイト単位の共有ロックをかける (Read=OK   / Write=Wait)
//	  Write時 : バイト単位の排他ロックをかける (Read=Wait / Write=Wait)
//
//	  ※Wait無しですぐエラーを返すような実装はダメ
//
// ---------------------------------------------------------------------------
class File
{
protected:
	#warning "TODO: Implement file_unx.h"
	// TODO: UNIX移植版では正直にvoid*を使ったほうが良いかも
	//HANDLE hFile;

public:
	File(void)
	//: hFile(INVALID_HANDLE_VALUE)
	{
	}

	~File()
	{
		close();
	}

	bool open(const char *filename, uint mode)
	{
		// TODO
		return true;
	};

	bool open(const wchar_t *filename, uint mode)
	{
		// TODO
		return true;
	};

	bool open_(const char *filenameA, const wchar_t *filenameW, uint mode)
	{
		// TODO
		return true;
	};

	uint64 read(void *buffer, uint siz)
	{
		// TODO
		DWORD ret;
		return (ret);
	};

	uint64 write(void *buffer, uint siz)
	{
		// TODO
		DWORD ret;
		return (ret);
	};

	uint64 seek(uint64 offset, DWORD origin)
	{
		// TODO
		DWORD ret;
		return (ret);
	};

	uint64 ftell(void)
	{
		// TODO
		DWORD ret;
		return (ret);
	};

	void close(void)
	{
		// TODO
	};

	uint64 size(void)
	{
		// TODO
		DWORD ret;
		return (ret);
	};

private:
	File(const File& rhs);
	File& operator=(const File& rhs);
};




// ---------------------------------------------------------------------------
//
//	class MappedFile
//
//	移植時の注意：排他ロックをかけ、後続のアクセスを待たせるように実装すること
//				  Wait無しですぐエラーを返すような実装はダメ。
//
// ---------------------------------------------------------------------------
class MappedFile
{
protected:
	/** TODO: このへんもUNIX移植版ではtypedefされてない型で正直に作る
	HANDLE			hFile;
	HANDLE			hMap;
	void			*addrP;
	OVERLAPPED		*ov;
	ULARGE_INTEGER	fsize;
	*/
public:
	MappedFile(void)
	{
		// TODO
	};

	~MappedFile()
	{
		// TODO
	};

	void *open(const char *filename, DWORD mapbyte_, bool writemode)
	{
		// TODO
	};

	void *open(const wchar_t *filename, DWORD mapbyte_, bool writemode)
	{
		// TODO
	};

	void *open_(const char *filenameA, const wchar_t *filenameW, DWORD mapbyte_, bool writemode)
	{
		// TODO
	};

	void close(void)
	{
		// TODO
	};

	uint64 size(void)
	{
		// TODO
		DWORD ret;
		return (ret);
	};

	DWORD allocG(void)
	{
		// TODO
		DWORD ret;
		return (ret);
	};

private:
	MappedFile(const MappedFile& rhs);
	MappedFile& operator=(const MappedFile& rhs);
};
