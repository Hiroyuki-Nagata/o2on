/* Copyright (C) 2006 o2on project. All rights reserved.
 * http://o2on.s69.xrea.com/
 */

/*
 * project		: o2on
 * filename		: O2DatIndex.h
 * description	: Index file
 *
 */

#pragma once
#include "define.h"
#include "typedef.h"
#include "hash.h"
#include <list>
#include <map>

#define O2_MAX_INDEX_PATHLEN 48




// ---------------------------------------------------------------------------
//	O2DatIndexRecord
//	�e�T�u�f�B���N�g���ɍ��.index�t�@�C���̃��R�[�h��`
//
//	�p�r�F
//	�Edat���̂Ɋ܂܂�Ȃ�����ۑ�����
//	�Edat�̃L�[���������ɕ������邽�߂̏���ۑ�����
//
//	���A���C�����g���l�����Đ݌v���邱��
// ---------------------------------------------------------------------------

struct O2DatIndexRecord {
	uint64		id;
	uint64		size;
	uint64		lastmodified;
	uint		resnum;
	byte		hash[HASHSIZE];
	wchar_t		url[O2_MAX_KEY_URL_LEN];
	wchar_t		note[O2_MAX_KEY_NOTE_LEN];
	byte		reserved[16];
};

typedef std::map<hashT,O2DatIndexRecord> O2DatIndexRecordMap;
typedef O2DatIndexRecordMap::iterator O2DatIndexRecordMapIt;

typedef std::list<O2DatIndexRecord> O2DatIndexRecordList;
typedef O2DatIndexRecordList::iterator O2DatIndexRecordListIt;




// ---------------------------------------------------------------------------
//	O2RootIndexRecord
//	�L���b�V�����[�g�ɍ��.index�t�@�C���̃��R�[�h��`
//
//	�p�r�F
//	�E�T�u�f�B���N�g���̃p�X��ۑ�����
//	�E�T�u�f�B���N�g������dat���Adat�T�C�Y���ۑ�����B�N���������ɕ������邽��
//	�E���s���̓��������Ɏ��B�I�����ɕۑ�
//
//	���A���C�����g���l�����Đ݌v���邱��
// ---------------------------------------------------------------------------

struct O2RootIndexRecord {
	wchar_t		indexfile[O2_MAX_INDEX_PATHLEN]; //ex) \2ch.net\news\1234
	uint64		indexfilesize;
	uint64		datfilesize;
	uint64		datfilenum;
	time_t		lastcheck;
};

typedef std::map<wstring,O2RootIndexRecord> O2IndexMap;
typedef O2IndexMap::iterator O2IndexMapIt;




// ---------------------------------------------------------------------------
//	O2OffsetMap
//	�E[�L�[�n�b�V��] - [index�t�@�C����,�t�@�C�����I�t�Z�b�g] �̃}�b�v
//	�E���������Ɏ���
// ---------------------------------------------------------------------------

struct O2IndexFileOffset {
	wstring indexfile;
	uint64 offset;
};

typedef std::map<hashT,O2IndexFileOffset> O2OffsetMap;
typedef O2OffsetMap::iterator O2OffsetMapIt;

struct O2PublishIndexFileOffset {
	hashT hash;
	wstring indexfile;
	uint64 offset;

	bool operator==(const hashT &h) const {
		return (hash == h);
	}
	bool operator==(const O2PublishIndexFileOffset &src) const {
		return (hash == src.hash);
	}
};

typedef std::list<O2PublishIndexFileOffset> O2PublishIndexFileList;
typedef O2PublishIndexFileList::iterator O2PublishIndexFileListIt;
