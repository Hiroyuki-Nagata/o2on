﻿/* Copyright (C) 2006 o2on project. All rights reserved.
 * http://o2on.net/
 */

/*
 * project		: o2on
 * filename		: O2Protocol.h
 * description	: 
 *
 */

#pragma once
#include "httpheader.hpp"
#include "O2Profile.hpp"
#include "O2Node.hpp"
#include "O2Version.hpp"




#define O2PROTOPATH_DAT					"dat"
#define O2PROTOPATH_COLLECTION			"collection"
#define O2PROTOPATH_IM					"im"
#define O2PROTOPATH_BROADCAST			"broadcast"
#define O2PROTOPATH_PROFILE				"profile"
#define O2PROTOPATH_KADEMLIA_PING		"ping"
#define O2PROTOPATH_KADEMLIA_STORE		"store"
#define O2PROTOPATH_KADEMLIA_FINDNODE	"findnode"
#define O2PROTOPATH_KADEMLIA_FINDVALUE	"findvalue"

#define X_O2_NODE_ID					"X-O2-Node-ID"
#define X_O2_NODE_FLAGS					"X-O2-Node-Flags"
#define X_O2_RSAPUBLICKEY				"X-O2-RSA-Public-Key"
#define X_O2_PORT						"X-O2-Port"
#define X_O2_NODENAME					"X-O2-Node-Name"
#define X_O2_TARGET_KEY					"X-O2-Target-Key"
#define X_O2_TARGET_BOARD				"X-O2-Target-Board"
#define X_O2_DATPATH					"X-O2-DAT-Path"
#define X_O2_ORG_DAT_URL				"X-O2-Original-DAT-URL"
#define X_O2_KEY_CATEGORY				"X-O2-Key-Category"




class O2Protocol
{
public:
	O2Protocol(void)
	{
	}

	~O2Protocol()
	{
	}

	// -----------------------------------------------------------------------
	//	MakeURL
	// -----------------------------------------------------------------------
	void MakeURL(const ulong ip
			   , const ushort port
			   , const char *path
			   , string &url)
	{
		string ipstr;
		ulong2ipstr(ip, ipstr);

		char portstr[10];
		sprintf_s(portstr, 10, "%d", port);

		url = "http://";
		url += ipstr;
		url += ":";
		url += portstr;
		url += "/";
		url += path;
	}

	// -----------------------------------------------------------------------
	//	AddRequestHeaderFields
	// -----------------------------------------------------------------------
	void AddRequestHeaderFields(HTTPHeader &header
							  , const O2Profile *profile)
	{
		string pubkey;
		profile->GetPubkeyA(pubkey);

		hashT hash;
		profile->GetID(hash);
		string hashstr;
		hash.to_string(hashstr);

		string flags;
		profile->GetFlags(flags);

		header.AddFieldString("Connection", "close");
		header.AddFieldString("Host", header.host.c_str());
		header.AddFieldString("User-Agent", profile->GetUserAgentA());
		header.AddFieldString(X_O2_NODE_ID, hashstr.c_str());
		header.AddFieldString(X_O2_RSAPUBLICKEY, pubkey.c_str());
		header.AddFieldNumeric(X_O2_PORT, profile->GetP2PPort());
		header.AddFieldString(X_O2_NODENAME, profile->GetNodeNameA());
		header.AddFieldString(X_O2_NODE_FLAGS, flags.c_str());
	}

	// -----------------------------------------------------------------------
	//	AddResponseHeaderFields
	// -----------------------------------------------------------------------
	void AddResponseHeaderFields(HTTPHeader &header
							  , const O2Profile *profile)
	{
		string pubkey;
		profile->GetPubkeyA(pubkey);

		hashT hash;
		profile->GetID(hash);
		string hashstr;
		hash.to_string(hashstr);

		string flags;
		profile->GetFlags(flags);

		header.AddFieldString("Connection", "close");
		header.AddFieldString("Server", profile->GetUserAgentA());
		header.AddFieldString(X_O2_NODE_ID, hashstr.c_str());
		header.AddFieldString(X_O2_RSAPUBLICKEY, pubkey.c_str());
		header.AddFieldNumeric(X_O2_PORT, profile->GetP2PPort());
		header.AddFieldString(X_O2_NODENAME, profile->GetNodeNameA());
		header.AddFieldString(X_O2_NODE_FLAGS, flags.c_str());
	}

	// -----------------------------------------------------------------------
	//	AddContentFields
	// -----------------------------------------------------------------------
	void AddContentFields(HTTPHeader &header
						, const size_t content_length
						, const char *content_type
						, const char *charset)
	{
		string tmp = content_type;
		if (charset) tmp += string("; charset=") + charset;
		header.AddFieldString("Content-Type", tmp.c_str());
		header.AddFieldNumeric("Content-Length", content_length);
	}

	// -----------------------------------------------------------------------
	//	GetTargetKeyFromHeader
	// -----------------------------------------------------------------------
	bool GetTargetKeyFromHeader(const HTTPHeader &header
							  , hashT &target)
	{
		strmap::const_iterator it = header.fields.find(X_O2_TARGET_KEY);
		if (it == header.fields.end())
			return false;
		target.assign(it->second.c_str(), it->second.size());
		return true;
	}

	// -----------------------------------------------------------------------
	//	GetTargetBoardFromHeader
	// -----------------------------------------------------------------------
	bool GetTargetBoardFromHeader(const HTTPHeader &header
								, wstring &board)
	{
		strmap::const_iterator it = header.fields.find(X_O2_TARGET_BOARD);
		if (it == header.fields.end())
			return false;
		ascii2unicode(it->second, board);
		return true;
	}

	// -----------------------------------------------------------------------
	//	GetNodeInfoFromHeader
	// -----------------------------------------------------------------------
	bool GetNodeInfoFromHeader(const HTTPHeader &header
							 , const ulong ip
							 , const ushort port
							 , O2Node &node)
	{
		strmap::const_iterator it;

		//ID
		it = header.fields.find(X_O2_NODE_ID);
		if (it == header.fields.end())
			return false;
		node.id.assign(it->second.c_str(), it->second.size());

		//IP
		node.ip = ip;

		//Port
		if (header.GetType() == HTTPHEADERTYPE_REQUEST) {
			it = header.fields.find(X_O2_PORT);
			if (it == header.fields.end())
				return false;
			node.port = (ushort)strtoul(it->second.c_str(), NULL, 10);
		}
		else
			node.port = port;

		//RSA Public Key
		it = header.fields.find(X_O2_RSAPUBLICKEY);
		if (it == header.fields.end())
			return false;
		node.pubkey.assign(it->second.c_str(), it->second.size());

		//NodeName
		it = header.fields.find(X_O2_NODENAME);
		if (it != header.fields.end())
			ToUnicode(_T(DEFAULT_XML_CHARSET), it->second, node.name);

		//Flags
		it = header.fields.find(X_O2_NODE_FLAGS);
		if (it != header.fields.end())
			ascii2unicode(it->second, node.flags);

		//User-Agent
		byte status;
		switch (header.GetType()) {
			case HTTPHEADERTYPE_REQUEST:
				it = header.fields.find("User-Agent");
				status = O2_NODESTATUS_LINKEDFROM;
				break;
			case HTTPHEADERTYPE_RESPONSE:
				it = header.fields.find("Server");
				status = O2_NODESTATUS_LINKEDTO;
				break;
			default:
				return false;
		}
		if (it == header.fields.end())
			return false;
		ascii2unicode(it->second, node.ua);

		// ノードのユーザーエージェントから情報を取得
		// O2/0.2 (o2on/0.02.0027; Win32)
		// O2/0.2 (opy2on/0.00.0001; Linux x86_64)
		//など「O2/プロトコルバージョン (アプリ名/アプリバージョン; 環境等)」形式であること
		size_t ProtoNamePos = node.ua.find(L"O2/", 0);
		if (ProtoNamePos != 0) {
			// プロトコル名が見つからない
			return false;
		}
		size_t AppNamePeriodPos = node.ua.find(L" ", ProtoNamePos);
		if ( AppNamePeriodPos == wstring::npos ) {
			// プロトコル・アプリ名の区切りが見つからない
			return false;
		}
		node.proto_ver = node.ua.substr( 3, AppNamePeriodPos - 3 );
		size_t AppNameStartPos = node.ua.find(L"(", AppNamePeriodPos);
		if (( AppNameStartPos == wstring::npos ) || ((AppNameStartPos + 1) == node.ua.size() )) {
			// アプリ名の開始位置が見つからない
			return false;
		}
		AppNameStartPos++;
		size_t AppNameEndPos = node.ua.find(L"/", AppNameStartPos);
		if ( AppNameEndPos == wstring::npos ) {
			// アプリ名の終了位置が見つからない
			return false;
		}
		node.app_name = node.ua.substr( AppNameStartPos, AppNameEndPos - AppNameStartPos );
 		if ((wcscmp(node.app_name.c_str(), _T(APP_NAME)) == 0) && ((AppNameEndPos + 1) < node.ua.size() )) {
			// 同じアプリならバージョン番号を取得
			AppNameEndPos++;
			size_t VerEndPos = node.ua.find(L";", AppNameEndPos);
			if ( VerEndPos == wstring::npos ) {
				// バージョン番号の終了位置が見つからない
				return false;
			}
			node.app_ver = node.ua.substr(AppNameEndPos, VerEndPos - AppNameEndPos);
		}
		else 
		{
			wchar_t tmpW[128];
			swprintf_s(tmpW, 128, L"異なるアプリ:%s\n", node.app_name.c_str());
			TRACEW(tmpW);
		}

		return true;
	}

	// -----------------------------------------------------------------------
	//	MakeResponse_
	// -----------------------------------------------------------------------
	void MakeResponse_200(const O2Profile *profile, string &out)
	{
		HTTPHeader header(HTTPHEADERTYPE_RESPONSE);
		header.status = 200;
		AddResponseHeaderFields(header, profile);
		header.Make(out);
	}
	void MakeResponse_302(const O2Profile *profile, const char *location, string &out)
	{
		HTTPHeader header(HTTPHEADERTYPE_RESPONSE);
		header.status = 302;
		AddResponseHeaderFields(header, profile);
		header.AddFieldString("Location", location);
		header.Make(out);
	}
	void MakeResponse_304(const O2Profile *profile, string &out)
	{
		HTTPHeader header(HTTPHEADERTYPE_RESPONSE);
		header.status = 304;
		AddResponseHeaderFields(header, profile);
		header.Make(out);
	}
	void MakeResponse_400(const O2Profile *profile, string &out)
	{
		HTTPHeader header(HTTPHEADERTYPE_RESPONSE);
		header.status = 400;
		AddResponseHeaderFields(header, profile);
		header.Make(out);
	}
	void MakeResponse_403(const O2Profile *profile, string &out)
	{
		HTTPHeader header(HTTPHEADERTYPE_RESPONSE);
		header.status = 403;
		AddResponseHeaderFields(header, profile);
		header.Make(out);
	}
	void MakeResponse_404(const O2Profile *profile, string &out)
	{
		HTTPHeader header(HTTPHEADERTYPE_RESPONSE);
		header.status = 404;
		AddResponseHeaderFields(header, profile);
		header.Make(out);
	}
};
