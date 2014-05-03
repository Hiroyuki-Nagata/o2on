﻿/* Copyright (C) 2006 o2on project. All rights reserved.
 * http://o2on.net/
 */

/*
 * project		: o2on
 * filename		: O2Job_PublishOriginal.h
 * description	: 
 *
 */

#pragma once
#include "O2Job.hpp"
#include "O2Logger.hpp"
#include "O2Profile.hpp"
#include "O2NodeDB.hpp"
#include "O2KeyDB.hpp"
#include "O2DatIO.hpp"
#include "O2Client.hpp"
#include "O2Protocol_Kademlia.hpp"
#include "dataconv.hpp"
#include "stopwatch.hpp"




class O2Job_PublishOriginal
	: public O2Job
	, public O2Protocol_Kademlia
{
protected:
	O2Logger	*Logger;
	O2Profile	*Profile;
	O2NodeDB	*NodeDB;
	O2KeyDB		*KeyDB;
	O2KeyDB		*SakuDB;
	O2DatIO		*DatIO;
	O2DatDB		*DatDB;
	O2Client	*Client;

	typedef std::map<O2Node,O2KeyList> O2NodeKeyMap;
	typedef O2NodeKeyMap::iterator O2NodeKeyMapIt;
	typedef hashListT::iterator hashListTIt;
public:
	O2Job_PublishOriginal(const wchar_t	*name
						, time_t		interval
						, bool			startup
						, O2Logger		*lgr
						, O2Profile		*prof
						, O2NodeDB		*ndb
						, O2KeyDB		*kdb
						, O2KeyDB		*sakudb
						, O2DatIO		*dio
						, O2DatDB		*datdb
						, O2Client		*client)
		: O2Job(name, interval, startup)
		, Logger(lgr)
		, Profile(prof)
		, NodeDB(ndb)
		, KeyDB(kdb)
		, SakuDB(sakudb)
		, DatIO(dio)
		, DatDB(datdb)
		, Client(client)
	{
	}

	~O2Job_PublishOriginal()
	{
	}

	void JobThreadFunc(void)
	{
		if (Profile->GetP2PPort() == 0) {
			Active = false;
			return;
		}

		if (NodeDB->count() < PUBLISH_START_NODE_COUNT)
			return;

//stopwatch sw("[[ PublishOriginal ]]");
		O2KeyList keylist;
		O2KeyListIt kit;
		
		hashListT hashlist;
		hashListTIt hit;
		// 所有datのキーをpublish
		if (DatIO->GetLocalFileKeys(keylist, PUBLISH_ORIGINAL_TT, PUBLISHNUM_PER_THREAD) > 0) {
			Publish(keylist, "dat", hashlist);
			//Publish関数から返却されるリストのハッシュは重複しているので一つにまとめる。
			hashlist.sort();
			hashlist.unique();
			for (hit = hashlist.begin(); hit != hashlist.end(); hit++) {
				DatDB->AddUpdateQueue(*hit); //lastpublish更新
			}
			hashlist.clear();
		}

		// 削除依頼のキーをpublish
		if (SakuDB->GetKeyList(keylist, time(NULL)-PUBLISH_ORIGINAL_TT) > 0) {
			for (kit = keylist.begin(); kit != keylist.end(); kit++) {
				Profile->GetID(kit->nodeid);
				kit->ip = Profile->GetIP();
				kit->port = Profile->GetP2PPort();
			}
			Publish(keylist, "saku", hashlist);
			for (hit = hashlist.begin(); hit != hashlist.end(); hit++) {
				SakuDB->SetDate(*hit, time(NULL)); //date更新
			}
			hashlist.clear();
		}
	}
	void Publish(O2KeyList &keylist, const char *category, hashListT &hashlist)
	{
		O2NodeKeyMap nkmap;

stopwatch *sw = new stopwatch("PublishOriginal: MakeMap");
		O2KeyListIt kit;
		for (kit = keylist.begin(); kit != keylist.end() && IsActive(); kit++) {
			O2NodeDB::NodeListT neighbors;
			if (NodeDB->neighbors(kit->hash, neighbors, false) == 0)
				break;

			O2NodeKeyMapIt nkit;
			std::pair<O2NodeKeyMapIt,bool> ret;

			O2NodeDB::NodeListT::iterator it;
			for (it = neighbors.begin(); it != neighbors.end() && IsActive(); it++) {
				nkit = nkmap.find(*it);
				if (nkit == nkmap.end()) {
					ret = nkmap.insert(O2NodeKeyMap::value_type(*it, O2KeyList()));
					nkit = ret.first;
				}
				nkit->second.push_back(*kit);
				Sleep(1);
			}
		}
		keylist.clear();
delete sw;

		if (nkmap.empty())
			return;

		O2NodeDB::NodeListT proceededNodes;

		O2NodeKeyMapIt nkit;
		for (nkit = nkmap.begin(); nkit != nkmap.end() && IsActive(); nkit++) {
			string xml;
			if (KeyDB->ExportToXML(nkit->second, xml) == 0)
				continue;

			O2Node node(nkit->first);
			node.lastlink = 0;
			node.reset();

			// STORE発行
			O2SocketSession ss;
			ss.ip = node.ip;
			ss.port = node.port;
			O2Protocol_Kademlia pk;
			MakeRequest_Kademlia_STORE(&ss, Profile, category, xml.size(), ss.sbuff);
			ss.sbuff += xml;

			Client->AddRequest(&ss);
			ss.Wait();

			HTTPHeader *header = (HTTPHeader*)ss.data;
			if (CheckResponse(&ss, header, NodeDB, node)) {
				O2KeyListIt kit;
				for (kit = nkit->second.begin(); kit != nkit->second.end() && IsActive(); kit++) {
					hashlist.push_back(kit->hash);
				}
			}

			proceededNodes.push_back(node);

			if (header) delete header;
			Sleep(IsActive() ? PUBLISH_INTERVAL_MS : 0);
		}

		O2NodeDB::NodeListT::iterator it;
		for (it = proceededNodes.begin(); it != proceededNodes.end() && IsActive(); it++) {
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
#if 0
		// 所有datのキーでループ
		O2KeyListIt it;
		for (it = keylist.begin(); it != keylist.end() && IsActive(); it++) {
			string xml;
			if (KeyDB->ExportToXML(*it, xml) == 0)
				continue;
/*
			TRACEA("+++++++++++++++++++++++++++++++++++++++++++++++++\n");
			TRACEA(" Publish Original\n");
			TRACEA("+++++++++++++++++++++++++++++++++++++++++++++++++\n");
			TRACEA(xml.c_str());
			TRACEA("\n");
*/
			O2NodeDB::NodeListT neighbors;
			if (NodeDB->neighbors(it->hash, neighbors, false) == 0)
				break;

			// キーの近隣ノードでループ
			uint publishcount = 0;
			size_t i;
			for (i = 0; i < neighbors.size() && publishcount < 2 && IsActive(); i++) {
				neighbors[i].lastlink = 0;
				neighbors[i].reset();

				// STORE発行
				O2SocketSession ss;
				ss.ip = neighbors[i].ip;
				ss.port = neighbors[i].port;
				O2Protocol_Kademlia pk;
				MakeRequest_Kademlia_STORE(&ss, Profile, xml.size(), ss.sbuff);
				ss.sbuff += xml;

				Client->AddRequest(&ss);
				ss.Wait();

				HTTPHeader *header = (HTTPHeader*)ss.data;
				if (CheckResponse(&ss, header, NodeDB, neighbors[i])) {
					publishcount++;
				}

				if (header) delete header;
				Sleep(IsActive() ? PUBLISH_INTERVAL_MS : 0);
			}

			size_t n = i;

			for (i = 0; i < n && IsActive(); i++) {
				if (neighbors[i].lastlink) {
					// 成功したノードをtouch
					NodeDB->touch(neighbors[i]);
				}
				else {
					// 失敗したノードをremove
					NodeDB->remove(neighbors[i]);
					KeyDB->DeleteKeyByNodeID(neighbors[i].id);
				}
			}

			if (publishcount) {
				DatDB->AddUpdateQueue(it->hash);
			}
		}
#endif
	}
};
