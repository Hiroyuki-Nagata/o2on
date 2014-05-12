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
#include "O2Agent.hpp"
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
#include <wx/snglinst.h>
#include <wx/progdlg.h>
#include <pevents.h>

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

static O2Agent				*Agent;
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

private:
	static bool  InitializeApp();
	static void  FinalizeApp(void);
	static void* FinalizeAppThread(void *param);

	wxSingleInstanceChecker* m_checker;
	static neosmart::neosmart_event_t threadHandle;
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

	if (!O2DEBUG) {
		const wxString name = wxString::Format(_("o2on-%s"), wxGetUserId().c_str());
		m_checker = new wxSingleInstanceChecker(name);
		if ( m_checker->IsAnotherRunning()) {
			// 他のプロセスが走っていた場合終了
			return false;
		}
	}

	if (!InitializeApp())
		return false;

	/**
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!hwndProgressDlg || !IsDialogMessage(hwndProgressDlg, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	*/

	//if (Mutex) CloseHandle(Mutex);

	//return ((int)msg.wParam);
	return true;
}

int O2Main::OnExit() 
{
	return 0;
}

// ---------------------------------------------------------------------------
//	InitializeApp
//	アプリケーションの初期化処理
// ---------------------------------------------------------------------------
bool
O2Main::InitializeApp()
{
#ifdef _WIN32 /** 終了時のプロセス優先度：最初の方でシャットダウンするアプリケーション用  */
	SetProcessShutdownParameters(0x3FF, 0);
#endif

	// ログ出力用
	wxString msg;
	const wxString msgTitle = wxT("o2on初期化");

/**     commctl関連：移植は不可

	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_PROGRESS_CLASS;
	InitCommonControlsEx(&icex);

	CoInitialize(NULL); 
*/	

#ifdef _WIN32 /** winsock の初期化 */
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);
#endif

	// Xerces-Cの初期化
	XMLPlatformUtils::Initialize();

#ifdef _WIN32 // タスクバー関連か、WIN32APIなので移植は難しい
	TaskbarRestartMsg = RegisterWindowMessage(_T("TaskbarCreated"));
	if (!O2DEBUG) {
		ChangeToModuleDir();
	}
#endif

	//
	//	Logger
	//
	Logger = new O2Logger(NULL/*L"logs"*/);

	//
	//	Profile
	//
	Profile = new O2Profile(Logger, true);
	if (!Profile->MakeConfDir()) {
		msg = wxString::Format(wxT("ディレクトリ「%s」の作成に失敗しました\n起動を中止します"),
				       wxString(Profile->GetConfDirW()).c_str());
		wxMessageBox(msgTitle, msg, wxICON_ERROR | wxOK);
		return false;
	}

	if (!Profile->MakeDBDir()) {
		msg = wxString::Format(wxT("ディレクトリ「%s」の作成に失敗しました\n起動を中止します"),
				       wxString(Profile->GetDBDirW()).c_str());
		wxMessageBox(msgTitle, msg, wxICON_ERROR | wxOK);
		return false;
	}
	if (!Profile->MakeCacheRoot()) {
		msg = wxString::Format(wxT("ディレクトリ「%s」の作成に失敗しました\n起動を中止します"),
				       wxString(Profile->GetCacheRootW()).c_str());
		wxMessageBox(msgTitle, msg, wxICON_ERROR | wxOK);
		return false;
	}
	if (!Profile->CheckAdminRoot()) {
		msg = wxString::Format(wxT("ディレクトリ「%s」が存在しません\n起動を中止します"),
				       wxString(Profile->GetAdminRootW()).c_str());
		wxMessageBox(msgTitle, msg, wxICON_ERROR | wxOK);
		return false;
	}
	Logger->SetLimit(LOGGER_LOG, Profile->GetLogLimit());
	Logger->SetLimit(LOGGER_NETLOG, Profile->GetNetLogLimit());
	Logger->SetLimit(LOGGER_HOKANLOG, Profile->GetHokanLogLimit());
	Logger->SetLimit(LOGGER_IPFLOG, Profile->GetIPFLogLimit());

#if 0 && defined(_DEBUG)
	string s;
	Profile->SetIP(inet_addr("192.168.0.99"));
	Profile->GetEncryptedProfile(s);
	Profile->SetIP(0);
	TRACEA(s.c_str());
	TRACEA("\n");
#endif

	//
	//	IPFilter
	//
	IPF_P2P	= new O2IPFilter(L"P2P", Logger);
	if (!IPF_P2P->Load(Profile->GetIPF_P2PFilePath())) {
		IPF_P2P->setdefault(O2_ALLOW);
	}
	IPF_Proxy	= new O2IPFilter(L"Proxy", Logger);
	if (!IPF_Proxy->Load(Profile->GetIPF_ProxyFilePath())) {
		IPF_Proxy->setdefault(O2_DENY);
		IPF_Proxy->add(true, O2_ALLOW, L"127.0.0.1/255.255.255.255");
		IPF_Proxy->add(true, O2_ALLOW, L"192.168.0.0/255.255.0.0");
	}
	IPF_Admin	= new O2IPFilter(L"Admin", Logger);
	if (!IPF_Admin->Load(Profile->GetIPF_AdminFilePath())) {
		IPF_Admin->setdefault(O2_DENY);
		IPF_Admin->add(true, O2_ALLOW, L"127.0.0.1/255.255.255.255");
		IPF_Admin->add(true, O2_ALLOW, L"192.168.0.0/255.255.0.0");
	}
	//
	//	DatDB
	//
	wstring dbfilename(Profile->GetDBDirW());
	dbfilename += L"\\dat.db";
	DatDB = new O2DatDB(Logger, dbfilename.c_str());
	if (!DatDB->create_table(false)) {
		wxMessageBox(msgTitle,
			     wxT("DBオープンに失敗しました\n起動を中止します"),
			     wxICON_ERROR | wxOK);
		return false;
	}
	DatDB->StartUpdateThread();

	//
	//	Agent
	//
	Agent = new O2Agent(L"Agent", Logger);
	Agent->SetRecvSizeLimit(RECV_SIZE_LIMIT);
	Agent->ClientStart();

	//
	//	DBs
	//
	DatIO = new O2DatIO(DatDB, Logger, Profile, &ProgressInfo);
	NodeDB = new O2NodeDB(Logger, Profile, Agent);
	hashT myID;
	Profile->GetID(myID);
	NodeDB->SetSelfNodeID(myID);
	NodeDB->SetSelfNodePort(Profile->GetP2PPort());
#if 0 && defined(_DEBUG)
	StopWatch *sw = new StopWatch("BIGNODE");
	NodeDB->Load(L"doc\\BIGNodeList.xml");
	delete sw;
	sw = new StopWatch("BIGNODE");
	NodeDB->Save(L"doc\\BIGNodeList_save.xml");
	delete sw;
	return false;
#endif
	NodeDB->Load(Profile->GetNodeFilePath());
	FriendDB = new O2FriendDB(Logger, NodeDB);
	FriendDB->Load(Profile->GetFriendFilePath());
	KeyDB = new O2KeyDB(L"KeyDB", false, Logger);
	KeyDB->SetSelfNodeID(myID);
	KeyDB->SetLimit(Profile->GetKeyLimit());
	SakuKeyDB = new O2KeyDB(L"SakuKeyDB", false, Logger);
	SakuKeyDB->SetSelfNodeID(myID);
	SakuKeyDB->SetLimit(O2_SAKUKEY_LIMIT);
	QueryDB = new O2KeyDB(L"QueryDB", true, Logger);
	QueryDB->Load(Profile->GetQueryFilePath());
	QueryDB->SetLimit(Profile->GetQueryLimit());
	SakuDB = new O2KeyDB(L"SakuDB", true, Logger);
	SakuDB->Load(Profile->GetSakuFilePath());
	IMDB = new O2IMDB(Logger);
	IMDB->Load(Profile->GetIMFilePath());
	BroadcastDB = new O2IMDB(Logger);
	LagQueryQueue = new O2LagQueryQueue(Logger, Profile, QueryDB);
	// Boards
	wstring brdfile(Profile->GetConfDirW());
	brdfile += L"\\2channel.brd";
	wstring exbrdfile(Profile->GetConfDirW());
	exbrdfile += L"\\BoardEx.xml";
	Boards = new O2Boards(Logger, Profile, Agent, brdfile.c_str(), exbrdfile.c_str());
	if (!Boards->Load()) {
		brdfile += L".default";
		Boards->Load(brdfile.c_str());
		Boards->Save();
		if (!Boards->LoadEx()) {
			Boards->EnableExAll();
		}
	}
	else {
		Boards->LoadEx();
	}
	Profile->SetDatStorageFlag(Boards->SizeEx() ? true : false);

	//
	//	Jobs
	//
	PerformanceCounter = new O2PerformanceCounter(
		L"PerformanceCounter",
		JOB_INTERVAL_PERFORMANCE_COUNTER,
		false,
		Logger);
	PerformanceCounter->Load(Profile->GetReportFilePath());
	Job_GetGlobalIP = new O2Job_GetGlobalIP(
		L"GetGlobalIP",
		JOB_INTERVAL_GET_GLOBAL_IP,
		true,
		Logger,
		Profile,
		NodeDB,
		Agent);
	Job_QueryDat = new O2Job_QueryDat(
		L"QueryDat",
		JOB_INTERVAL_QUERY_DAT,
		false,
		Logger,
		Profile,
		NodeDB,
		KeyDB,
		QueryDB,
		DatIO,
		Agent);
	Job_DatCollector = new O2Job_DatCollector(
		L"DatCollector",
		JOB_INTERVAL_DAT_COLLECTOR,
		false,
		Logger,
		Profile,
		NodeDB,
		KeyDB,
		QueryDB,
		DatIO,
		Boards,
		Agent);
	Job_AskCollection = new O2Job_AskCollection(
		L"AskCollection",
		JOB_INTERVAL_ASK_COLLECTION,
		false,
		Logger,
		Profile,
		NodeDB,
		KeyDB,
		Boards,
		Agent);
	Job_PublishKeys = new O2Job_PublishKeys(
		L"PublishKey",
		JOB_INTERVAL_PUBLISH_KEYS,
		false,
		Logger,
		Profile,
		NodeDB,
		KeyDB,
		SakuKeyDB,
		Agent);
	Job_PublishOriginal = new O2Job_PublishOriginal(
		L"PublishOriginal",
		JOB_INTERVAL_PUBLISH_ORIGINAL,
		false,
		Logger,
		Profile,
		NodeDB,
		KeyDB,
		SakuDB,
		DatIO,
		DatDB,
		Agent);
	Job_NodeCollector = new O2Job_NodeCollector(
		L"NodeCollector",
		JOB_INTERVAL_COLLECT_NODE,
		false,
		Logger,
		Profile,
		NodeDB,
		KeyDB,
		Agent,
		Job_PublishOriginal);
	Job_Search = new O2Job_Search(
		L"Search",
		JOB_INTERVAL_SEARCH,
		false,
		Logger,
		Profile,
		NodeDB,
		KeyDB,
		QueryDB,
		Agent,
		Job_QueryDat);
	Job_SearchFriends = new O2Job_SearchFriends(
		L"SearchFriends",
		JOB_INTERVAL_SEARCHFRIENDS,
		true,
		Logger,
		Profile,
		NodeDB,
		FriendDB,
		Agent);
	Job_Broadcast = new O2Job_Broadcast(
		L"Broadcast",
		JOB_INTERVAL_BROADCAST,
		false,
		Logger,
		Profile,
		NodeDB,
		KeyDB,
		BroadcastDB,
		Agent);
	Job_ClearWorkset = new O2Job_ClearWorkset(
		L"ClearWorkset",
		JOB_INTERVAL_WORKSET_CLEAR,
		false);
	Job_AutoSave = new O2Job_AutoSave(
		L"AutoSave",
		JOB_INTERVAL_AUTO_SAVE,
		false,
		Profile,
		NodeDB);

	Agent->Add(PerformanceCounter);
	Agent->Add(Job_GetGlobalIP);
	Agent->Add(Job_QueryDat);
	Agent->Add(Job_DatCollector);
	Agent->Add(Job_AskCollection);
	Agent->Add(Job_PublishKeys);
	Agent->Add(Job_PublishOriginal);
	Agent->Add(Job_NodeCollector);
	Agent->Add(Job_Search);
	Agent->Add(Job_SearchFriends);
	Agent->Add(Job_Broadcast);
	Agent->Add(Job_ClearWorkset);
	Agent->Add(Job_AutoSave);

	//
	//	Servers
	//
	Server_P2P = new O2Server_HTTP_P2P(
		Logger,
		IPF_P2P,
		Profile,
		DatIO,
		Boards,
		NodeDB,
		KeyDB,
		SakuKeyDB,
		QueryDB,
		IMDB,
		BroadcastDB,
		Job_Broadcast);
	Server_P2P->SetMultiLinkRejection(false);
	Server_P2P->SetSessionLimit(Profile->GetP2PSessionLimit());
	Server_P2P->SetRecvSizeLimit(RECV_SIZE_LIMIT);
	Server_P2P->SetPort(Profile->GetP2PPort());

	Server_Proxy = new O2Server_HTTP_Proxy(
		Logger,
		IPF_Proxy,
		Profile,
		DatIO,
		Boards,
		LagQueryQueue);
	Server_Proxy->SetMultiLinkRejection(false);
	Server_Proxy->SetPort(Profile->GetProxyPort());

	Server_Admin = new O2Server_HTTP_Admin(
		Logger,
		IPF_Admin,
		Profile,
		DatDB,
		DatIO,
		NodeDB,
		FriendDB,
		KeyDB,
		SakuKeyDB,
		QueryDB,
		SakuDB,
		IMDB,
		BroadcastDB,
		IPF_P2P,
		IPF_Proxy,
		IPF_Admin,
		Job_Broadcast,
		Agent,
		Boards);
	Server_Admin->SetMultiLinkRejection(false);
	Server_Admin->SetPort(Profile->GetAdminPort());

	//
	//	ReportMaker
	//
	ReportMaker = new O2ReportMaker(
		Logger,
		Profile,
		DatDB,
		DatIO,
		NodeDB,
		FriendDB,
		KeyDB,
		SakuKeyDB,
		QueryDB,
		SakuDB,
		IMDB,
		BroadcastDB,
		IPF_P2P,
		IPF_Proxy,
		IPF_Admin,
		PerformanceCounter,
		Server_P2P,
		Server_Proxy,
		Server_Admin,
		Agent,
		Job_QueryDat,
		Job_Broadcast);
	ReportMaker->PushJob(Job_GetGlobalIP);
	ReportMaker->PushJob(Job_NodeCollector);
	ReportMaker->PushJob(Job_PublishKeys);
	ReportMaker->PushJob(Job_PublishOriginal);
	ReportMaker->PushJob(Job_Search);
	ReportMaker->PushJob(Job_SearchFriends);
	ReportMaker->PushJob(Job_Broadcast);
	ReportMaker->PushJob(Job_QueryDat);
	ReportMaker->PushJob(Job_DatCollector);
	ReportMaker->PushJob(Job_AskCollection);

	Server_P2P->SetReportMaker(ReportMaker);
	Server_Admin->SetReportMaker(ReportMaker);

/**     メインウィンドウクラス登録...この辺もwxWidgets使えば移植できるはず
	WNDCLASSEX wc;
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= MainWindowProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= instance;
	wc.hIcon			= LoadIcon(instance, MAKEINTRESOURCE(IDI_O2ON));
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= NULL;//(HBRUSH)(COLOR_MENU + 1);
	wc.lpszMenuName 	= NULL;
	wc.lpszClassName	= _T(CLASS_NAME);
	wc.hIconSm			= NULL;

	if (!RegisterClassEx(&wc))
		return false;

	// メインウィンドウ作成
	hwndMain = CreateWindowEx(
		WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
		_T(CLASS_NAME),
		_T(APP_NAME),
		WS_POPUP,
		-1, -1, 0, 0,
		NULL,
		(HMENU)0,
		instance,
		NULL);

	if (!hwndMain)
		return false;

	// コールバックメッセージ登録
	LagQueryQueue->SetBaloonCallbackMsg(hwndMain, UM_SHOWBALOON);
	Server_P2P->SetIconCallbackMsg(hwndMain, UM_SETICON);
	Server_P2P->SetBaloonCallbackMsg(hwndMain, UM_SHOWBALOON);
	Server_Proxy->SetIconCallbackMsg(hwndMain, UM_SETICON);
	Server_Admin->SetIconCallbackMsg(hwndMain, UM_SETICON);
	Server_Admin->SetBaloonCallbackMsg(hwndMain, UM_SHOWBALOON);
	Agent->SetIconCallbackMsg(hwndMain, UM_SETICON);
	Job_DatCollector->SetBaloonCallbackMsg(hwndMain, UM_SHOWBALOON);
	Job_QueryDat->SetBaloonCallbackMsg(hwndMain, UM_SHOWBALOON);

	// ProxyとAdminを起動
	if (!StartProxy(L"\n\no2onの起動を中止します"))
		return false;
	if (!StartAdmin(L"\n\no2onの起動を中止します"))
		return false;

	// トレイアイコン追加
	AddTrayIcon(IDI_DISABLE);

	if (!CheckPort()) {
		ShowTrayBaloon(_T("o2on"),
			_T("オプションでポート番号を設定してください"),
			5*1000, NIIF_INFO);
	}
	else {
		// 自動起動
		if (Profile->IsP2PAutoStart())
			StartP2P(true);
	}

	CLEAR_WORKSET;
	return true;
*/
	return true;
}


// ---------------------------------------------------------------------------
//	FinalizeApp
//	アプリケーションの終了処理
// ---------------------------------------------------------------------------
void
O2Main::FinalizeApp(void)
{
	wxProgressDialog o2ProgressDlg(wxT("o2on"), wxT("o2on終了..."),
				       0, NULL, wxPD_APP_MODAL | wxPD_CAN_ABORT);

	ThreadHandle = neosmart::CreateEvent();
	pthread_t thread;
	pthread_create(&thread, NULL, FinalizeAppThread, NULL);

	MSG msg;
	while (ThreadHandle) {
		while (PeekMessage(&msg, hwndProgressDlg, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		wxSleep(1);
	}

/**
	ThreadHandle =
		(HANDLE)_beginthreadex(NULL, 0, FinalizeAppThread, NULL, 0, NULL);

	MSG msg;
	while (ThreadHandle) {
		while (PeekMessage(&msg, hwndProgressDlg, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Sleep(1);
	}

	HWND hwnd;
	while ((hwnd = FindWindow(_T("o2browser"), NULL)) != NULL) {
		PostMessage(hwnd, WM_CLOSE, 0, 0);
	}
*/
}

void* 
O2Main::FinalizeAppThread(void *param)
{
	CoInitialize(NULL);

	DatIO->StopRebuildDB();

	ProgressInfo.Reset(true, false);
	ProgressInfo.AddMax(7);

	if (Profile->IsBaloon_P2P())
		ShowTrayBaloon(L"o2on", L"o2onを終了しています…", 5*1000, NIIF_INFO);

	ProgressInfo.SetMessage(L"P2Pを終了しています");
	StopP2P(false);
	ProgressInfo.AddPos(1);

	ProgressInfo.SetMessage(L"Agentを終了しています");
	Agent->ClientStop();
	ProgressInfo.AddPos(1);

	ProgressInfo.SetMessage(L"Proxyを終了しています");
	Server_Proxy->Stop();
	ProgressInfo.AddPos(1);

	ProgressInfo.SetMessage(L"Adminを終了しています");
	Server_Admin->Stop();
	ProgressInfo.AddPos(1);

	ProgressInfo.SetMessage(L"設定を保存しています");

	Profile->Save();
	NodeDB->Save(Profile->GetNodeFilePath());
	FriendDB->Save(Profile->GetFriendFilePath());
	QueryDB->Save(Profile->GetQueryFilePath());
	SakuDB->Save(Profile->GetSakuFilePath());
	IMDB->Save(Profile->GetIMFilePath(), false);
	IPF_P2P->Save(Profile->GetIPF_P2PFilePath());
	IPF_Proxy->Save(Profile->GetIPF_ProxyFilePath());
	IPF_Admin->Save(Profile->GetIPF_AdminFilePath());
	PerformanceCounter->Save(Profile->GetReportFilePath());
	Boards->Save();
	Boards->SaveEx();

	ProgressInfo.AddPos(1);

	if (Agent)
		delete Agent;

	if (ReportMaker)
		delete ReportMaker;

	if (PerformanceCounter)
		delete PerformanceCounter;
	if (Job_GetGlobalIP)
		delete Job_GetGlobalIP;
	if (Job_QueryDat)
		delete Job_QueryDat;
	if (Job_DatCollector)
		delete Job_DatCollector;
	if (Job_AskCollection)
		delete Job_AskCollection;
	if (Job_PublishKeys)
		delete Job_PublishKeys;
	if (Job_PublishOriginal)
		delete Job_PublishOriginal;
	if (Job_NodeCollector)
		delete Job_NodeCollector;
	if (Job_Search)
		delete Job_Search;
	if (Job_SearchFriends)
		delete Job_SearchFriends;
	if (Job_Broadcast)
		delete Job_Broadcast;
	if (Job_ClearWorkset)
		delete Job_ClearWorkset;
	if (Job_AutoSave)
		delete Job_AutoSave;

	if (Server_P2P)
		delete Server_P2P;
	if (Server_Proxy)
		delete Server_Proxy;
	if (Server_Admin)
		delete Server_Admin;

	if (DatIO)
		delete DatIO;
	if (NodeDB)
		delete NodeDB;
	if (FriendDB)
		delete FriendDB;
	if (KeyDB)
		delete KeyDB;
	if (SakuKeyDB)
		delete SakuKeyDB;
	if (QueryDB)
		delete QueryDB;
	if (SakuDB)
		delete SakuDB;
	if (IMDB)
		delete IMDB;
	if (BroadcastDB)
		delete BroadcastDB;
	if (LagQueryQueue)
		delete LagQueryQueue;
	if (Boards)
		delete Boards;

	if (DatDB) {
		DatDB->StopUpdateThread();
		delete DatDB;
	}

	if (IPF_P2P)
		delete IPF_P2P;
	if (IPF_Proxy)
		delete IPF_Proxy;
	if (IPF_Admin)
		delete IPF_Admin;

	if (Profile)
		delete Profile;
	if (Logger)
		delete Logger;

	ProgressInfo.Reset(false, false);
	DeleteTrayIcon();
	XMLPlatformUtils::Terminate();
	WSACleanup();
	CoUninitialize(); 

	CloseHandle(ThreadHandle);
	ThreadHandle = NULL;
	return (0);
}
