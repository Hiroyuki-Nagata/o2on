/* Copyright (C) 2006 o2on project. All rights reserved.
 * http://o2on.net/
 */

/*
 * project		: o2on
 * filename		: O2Job_NodeCollector.h
 * description	: 
 *
 */

#pragma once
#include "O2Job.hpp"
#include "O2Logger.hpp"
#include "O2Profile.hpp"
#include "O2NodeDB.hpp"
#include "O2KeyDB.hpp"
#include "O2Client.hpp"
#include "O2Protocol_Kademlia.hpp"
#include "O2Job_PublishOriginal.hpp"
#include "dataconv.hpp"




class O2Job_NodeCollector
	: public O2Job
	, public O2Protocol_Kademlia
{
protected:
	O2Logger				*Logger;
	O2Profile				*Profile;
	O2NodeDB				*NodeDB;
	O2KeyDB					*KeyDB;
	O2Client				*Client;
	O2Job_PublishOriginal	*Job_PublishOriginal;
	CryptoPP::AutoSeededRandomPool rng;

public:
	O2Job_NodeCollector(const wchar_t			*name
						  , time_t					interval
						  , bool					startup
						  , O2Logger				*lgr
						  , O2Profile				*prof
						  , O2NodeDB				*ndb
						  , O2KeyDB					*kdb
						  , O2Client				*client
						  , O2Job_PublishOriginal	*publ)
		: O2Job(name, interval, startup)
		, Logger(lgr)
		, Profile(prof)
		, NodeDB(ndb)
		, KeyDB(kdb)
		, Client(client)
		, Job_PublishOriginal(publ)
	{
	}

	~O2Job_NodeCollector()
	{
	}

	void JobThreadFunc(void)
	{
		uint64 n = O2DEBUG ? 1 : COLLECT_NODE_THRESHOLD;
		if (!Profile->IsPort0() && NodeDB->count() >= n) {
			//SetActvie(false);
			Interval = JOB_INTERVAL_COLLECT_NODE2;
			//Job_PublishOriginal->DoNow();
			//return;
		}

		hashT tmpid;
		tmpid.random();

		O2NodeDB::NodeListT neighbors;
		if (NodeDB->neighbors(tmpid, neighbors, false) == 0)
			return;

		O2NodeDB::NodeListT::iterator it;
		for (it = neighbors.begin(); it != neighbors.end() && IsActive(); it++) {
			it->lastlink = 0;
			it->reset();

			// FINDNODE発行
			O2SocketSession ss;
			ss.ip = it->ip;
			ss.port = it->port;
			O2Protocol_Kademlia pk;
			hashT tmpid;
			tmpid.random();
			MakeRequest_Kademlia_FINDNODE(&ss, Profile, tmpid, ss.sbuff);

			Client->AddRequest(&ss);
			ss.Wait();

			HTTPHeader *header = (HTTPHeader*)ss.data;
			if (CheckResponse(&ss, header, NodeDB, *it)) {
				// Import
				NodeDB->ImportFromXML(NULL, &ss.rbuff[header->length], header->contentlength, NULL);
			}

			if (header) delete header;
		}

		for (it = neighbors.begin(); it != neighbors.end() && IsActive(); it++) {
			if (it->lastlink) {
				// 成功したノードをtouch
				NodeDB->touch(*it);
			}
			else {
				// 失敗したノードをremove
				NodeDB->remove(*it);
				KeyDB->DeleteKeyByNodeID(it->id);
			}
		}
	}
};
