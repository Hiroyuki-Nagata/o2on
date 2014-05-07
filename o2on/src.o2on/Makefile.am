# o2on is not GPL
AUTOMAKE_OPTIONS 	= foreign subdir-objects
ACLOCAL_AMFLAGS		= -I m4

# For Boost compile option
AM_CPPFLAGS		= $(BOOST_CPPFLAGS) @WX_CXXFLAGS@ -DTARGET_CPU=@target_cpu@ -DTARGET_OS=@target_os@
AM_LDFLAGS		= $(BOOST_REGEX_LDFLAGS) $(BOOST_THREADS_LDFLAGS)
LIBS			= $(BOOST_REGEX_LIBS) $(BOOST_THREAD_LIBS) $(BOOST_FILESYSTEM_LIBS) @WX_LIBS@ @LIBS@

# For o2on sources
bin_PROGRAMS		= o2on
o2on_CXXFLAGS		= @CXXFLAGS@ -I$(top_builddir)/o2on/babel
o2on_SOURCES		= \
KademliaKBucket.hpp	  \
KademliaNode.hpp	  \
KademliaRoutingTable.hpp  \
O2Agent.hpp		  \
O2Boards.cpp		  \
O2Boards.hpp		  \
O2Client.cpp		  \
O2Client.hpp		  \
O2Client_HTTP.hpp	  \
O2DatDB.cpp		  \
O2DatDB.hpp		  \
O2DatIO.cpp		  \
O2DatIO.hpp		  \
O2DatIndex.hpp		  \
O2DatPath.hpp		  \
O2Define.hpp		  \
O2FriendDB.hpp		  \
O2IMDB.cpp		  \
O2IMDB.hpp		  \
O2IPFilter.cpp		  \
O2IPFilter.hpp		  \
O2Job.hpp		  \
O2JobSchedule.hpp	  \
O2Job_AskCollection.hpp	  \
O2Job_AutoSave.hpp	  \
O2Job_Broadcast.hpp	  \
O2Job_ClearWorkset.hpp	  \
O2Job_DatCollector.hpp	  \
O2Job_GetGlobalIP.hpp	  \
O2Job_GetGlobalIP2.hpp	  \
O2Job_NodeCollector.hpp	  \
O2Job_PublishKeys.hpp	  \
O2Job_PublishOriginal.	  \
O2Job_QueryDat.hpp	  \
O2Job_Search.hpp	  \
O2Job_SearchFriends.hpp	  \
O2Key.hpp		  \
O2KeyDB.cpp		  \
O2KeyDB.hpp		  \
O2LagQueryQueue.hpp	  \
O2Logger.cpp		  \
O2Logger.hpp		  \
O2Node.hpp		  \
O2NodeDB.cpp		  \
O2NodeDB.hpp		  \
O2NodeKBucket.hpp	  \
O2PerformanceCounter.hpp  \
O2Profile.cpp		  \
O2Profile.hpp		  \
O2ProgressInfo.hpp	  \
O2Protocol.hpp		  \
O2Protocol_Dat.hpp	  \
O2Protocol_Kademlia.hpp	  \
O2ReportMaker.cpp	  \
O2ReportMaker.hpp	  \
O2SAX2Parser.cpp	  \
O2SAX2Parser.hpp	  \
O2Scheduler.hpp		  \
O2Server.cpp		  \
O2Server.hpp		  \
O2Server_HTTP.hpp	  \
O2Server_HTTP_Admin.hpp	  \
O2Server_HTTP_P2P.hpp	  \
O2Server_HTTP_Proxy.hpp	  \
O2SocketSession.hpp	  \
O2Zlib.hpp		  \
O2Version.hpp		  \
barray.hpp		  \
dataconv.cpp		  \
dataconv.hpp		  \
debug.hpp		  \
event.hpp		  \
file.cpp		  \
file.hpp		  \
headertest.cpp		  \
httpheader.cpp		  \
httpheader.hpp		  \
mutex.hpp		  \
resource.hpp		  \
resource.rc		  \
rsa.hpp			  \
sha.hpp			  \
common.cpp		  \
common.hpp		  \
simplehttpsocket.hpp	  \
simplessdpsocket.hpp	  \
simpletcpsocket.hpp	  \
simpleudpsocket.hpp	  \
stopwatch.hpp		  \
typedef.hpp		  \
upnp.hpp		  \
upnp_description.hpp	  \
../babel/babel.cpp	  \
../babel/balel.h


# if use wxWidgets use wxmain.cpp, else use main.cpp(MFC)
if USE_WXWIDGETS
o2on_SOURCES		+= wxmain.cpp
else
o2on_SOURCES		+= main.cpp
endif


# if target can use win32 thread, use win32 thread
if USE_PTHREAD

o2on_CXXFLAGS		+= -I../pevents
o2on_SOURCES		+= \
../pevents/pevents.cpp	   \
../pevents/pevents.h

AM_CPPFLAGS		+= -DWFMO
endif
