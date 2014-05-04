/* Copyright (C) 2006 o2on project. All rights reserved.
 * http://o2on.net/
 */

/*
 * project		: o2on
 * FILENAME		: wxmain.cpp
 * DESCRIPTION	: o2on client main source code ( equivalent of main.cpp )
 *
 */

#include <math.h>
#include "O2DatDB.hpp"
#include "O2Server_HTTP_P2P.hpp"
#include "O2Server_HTTP_Proxy.hpp"
#include "O2Server_HTTP_Admin.hpp"
#include "O2Job_GetGlobalIP.hpp"
#include "O2Job_QueryDat.hpp"
#include "O2Job_DatCollector.hpp"
#include "O2Job_AskCollection.hpp"
#include "O2Job_PublishKeys.hpp"
#include "O2Job_PublishOriginal.hpp"
#include "O2Job_NodeCollector.hpp"
#include "O2Job_Search.hpp"
#include "O2Job_SearchFriends.hpp"
#include "O2Job_Broadcast.hpp"
#include "O2Job_ClearWorkset.hpp"
#include "O2Job_AutoSave.hpp"
#include "O2PerformanceCounter.hpp"
#include "O2ReportMaker.hpp"
#include "O2Boards.hpp"
#include "O2ProgressInfo.hpp"
#include "O2Version.hpp"
#include "file.hpp"
#include "upnp.hpp"
//#include "resource.hpp"
#include <boost/dynamic_bitset.hpp>
#include <wx/wx.h>

// ---------------------------------------------------------------------------
//	macros
// ---------------------------------------------------------------------------
#define CLASS_NAME			"o2on"
#define UM_TRAYICON			(WM_USER+1)
#define UM_EMERGENCYHALT		(WM_USER+2)
#define UM_SHOWBALOON			(WM_USER+3)
#define UM_SETICON			(WM_USER+4)
#define UM_DLGREFRESH			(WM_USER+5)
#define UM_UPNP_START_TEST		(WM_USER+7)
#define UM_UPNP_END_TEST		(WM_USER+8)
#define IDT_TRAYICON			0
#define IDT_INICONTIMER			1
#define IDT_OUTICONTIMER 		2
#define IDT_PROGRESSTIMER 		3
#define IDT_AUTORESUME	 		4




// ---------------------------------------------------------------------------
//	file-scope variables
// ---------------------------------------------------------------------------
//static HINSTANCE			instance;
static HWND				hwndMain;
static HWND				hwndProgressDlg;
static HWND				hwndUPnPDlg;
static HANDLE				ThreadHandle = NULL;
static UINT				TaskbarRestartMsg;
static int				CurrentProperyPage;
static bool				VisibleOptionDialog;
static bool				PropRet;
static UINT				InIconTimer;
static UINT				OutIconTimer;
static bool				Active;
static bool				P2PStopBySuspend;
static UINT				ResumeTimer;
static HANDLE				UPnPThreadHandle;
static bool				UPnPLoop;
static UPnPService			*UPnPServiceUsingForPortMapping;
static DWORD				TickCount;
static uint64				Send;
static uint64				Recv;
static double				SendRate;
static double				RecvRate;

static O2Logger				*Logger;
static O2Profile			*Profile;
static O2Profile			*ProfBuff;
static O2IPFilter			*IPF_P2P;
static O2IPFilter			*IPF_Proxy;
static O2IPFilter			*IPF_Admin;
static O2DatDB				*DatDB;
static O2DatIO				*DatIO;
static O2NodeDB				*NodeDB;
static O2FriendDB			*FriendDB;
static O2KeyDB				*KeyDB;
static O2KeyDB				*SakuKeyDB;
static O2KeyDB				*QueryDB;
static O2KeyDB				*SakuDB;
static O2IMDB				*IMDB;
static O2IMDB				*BroadcastDB;
static O2Boards				*Boards;
static O2Server_HTTP_P2P		*Server_P2P;
static O2Server_HTTP_Proxy		*Server_Proxy;
static O2Server_HTTP_Admin		*Server_Admin;
static O2Job_GetGlobalIP		*Job_GetGlobalIP;
static O2Job_DatCollector		*Job_DatCollector;
static O2Job_AskCollection		*Job_AskCollection;
static O2Job_QueryDat			*Job_QueryDat;
static O2Job_NodeCollector		*Job_NodeCollector;
static O2Job_PublishKeys		*Job_PublishKeys;
static O2Job_PublishOriginal		*Job_PublishOriginal;
static O2Job_Search			*Job_Search;
static O2Job_SearchFriends		*Job_SearchFriends;
static O2Job_Broadcast			*Job_Broadcast;
static O2Job_ClearWorkset		*Job_ClearWorkset;
static O2Job_AutoSave			*Job_AutoSave;
static O2PerformanceCounter		*PerformanceCounter;
static O2LagQueryQueue			*LagQueryQueue;
static O2ReportMaker			*ReportMaker;
static O2ProgressInfo			ProgressInfo;





// ---------------------------------------------------------------------------
//	function prototypes
// ---------------------------------------------------------------------------
// TODO ...この辺は必要なものから入れ込んでいくスタイル.
//         WindowsのHANDLEウザイ、ただのvoid*のくせにいらんtypedefしすぎやし使いすぎ



// ---------------------------------------------------------------------------
//	wxWidgets main class definition
// ---------------------------------------------------------------------------
class O2Main : public wxApp 
{

public:
  virtual bool OnInit();
  virtual int OnExit();
};

IMPLEMENT_APP(O2Main)

// ---------------------------------------------------------------------------
//	OnInit
//	wxWidgetsエントリ関数
// ---------------------------------------------------------------------------
bool O2Main::OnInit() 
{

    if (!wxApp::OnInit())
	 return false;

    return true;
}

int O2Main::OnExit() 
{
     return 0;
}

