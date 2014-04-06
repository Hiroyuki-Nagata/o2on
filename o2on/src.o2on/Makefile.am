# o2on is not GPL
AUTOMAKE_OPTIONS 	= foreign subdir-objects
ACLOCAL_AMFLAGS		= -I m4

# For Boost compile option
AM_CPPFLAGS		= $(BOOST_CPPFLAGS) -DTARGET_CPU=@target_cpu@ -DTARGET_OS=@target_os@
AM_LDFLAGS		= $(BOOST_REGEX_LDFLAGS) $(BOOST_THREADS_LDFLAGS)
LIBS			= $(BOOST_REGEX_LIBS) $(BOOST_THREAD_LIBS)

# For o2on sources
bin_PROGRAMS		= o2on
o2on_CXXFLAGS		= @CXXFLAGS@ -I$(top_builddir)/o2on/babel
o2on_SOURCES		= \
KademliaKBucket.h	  \
KademliaNode.h		  \
KademliaRoutingTable.h	  \
O2Boards.cpp		  \
O2Boards.h		  \
O2Client.cpp		  \
O2Client.h		  \
O2Client_HTTP.h		  \
O2DatDB.cpp		  \
O2DatDB.h		  \
O2DatIO.cpp		  \
O2DatIO.h		  \
O2DatIndex.h		  \
O2DatPath.h		  \
O2Define.h		  \
O2FriendDB.h		  \
O2IMDB.cpp		  \
O2IMDB.h		  \
O2IPFilter.cpp		  \
O2IPFilter.h		  \
O2Job.h			  \
O2JobSchedule.h		  \
O2Job_AskCollection.h	  \
O2Job_AutoSave.h	  \
O2Job_Broadcast.h	  \
O2Job_ClearWorkset.h	  \
O2Job_DatCollector.h	  \
O2Job_GetGlobalIP.h	  \
O2Job_GetGlobalIP2.h	  \
O2Job_NodeCollector.h	  \
O2Job_PublishKeys.h	  \
O2Job_PublishOriginal.	  \
O2Job_QueryDat.h	  \
O2Job_Search.h		  \
O2Job_SearchFriends.h	  \
O2Key.h			  \
O2KeyDB.cpp		  \
O2KeyDB.h		  \
O2LagQueryQueue.h	  \
O2Logger.cpp		  \
O2Logger.h		  \
O2Node.h		  \
O2NodeDB.cpp		  \
O2NodeDB.h		  \
O2NodeKBucket.h		  \
O2PerformanceCounter.h	  \
O2Profile.cpp		  \
O2Profile.h		  \
O2ProgressInfo.h	  \
O2Protocol.h		  \
O2Protocol_Dat.h	  \
O2Protocol_Kademlia.h	  \
O2ReportMaker.cpp	  \
O2ReportMaker.h		  \
O2SAX2Parser.cpp	  \
O2SAX2Parser.h		  \
O2Scheduler.h		  \
O2Server.cpp		  \
O2Server.h		  \
O2Server_HTTP.h		  \
O2Server_HTTP_Admin.h	  \
O2Server_HTTP_P2P.h	  \
O2Server_HTTP_Proxy.h	  \
O2SocketSession.h	  \
O2Version.h		  \
barray.h		  \
dataconv.cpp		  \
dataconv.h		  \
debug.h			  \
event.h			  \
file.cpp		  \
file.h			  \
headertest.cpp		  \
httpheader.cpp		  \
httpheader.h		  \
main.cpp		  \
mutex.h			  \
resource.h		  \
resource.rc		  \
rsa.h			  \
sha.h			  \
common.cpp		  \
common.h		  \
simplehttpsocket.h	  \
simplessdpsocket.h	  \
simpletcpsocket.h	  \
simpleudpsocket.h	  \
stopwatch.h		  \
typedef.h		  \
upnp.h			  \
upnp_description.h    

# if target can use win32 thread, use win32 thread
if USE_PTHREAD
o2on_SOURCES		+= \
../pevents/pevents.cpp	   \
../pevents/pevents.h
endif
