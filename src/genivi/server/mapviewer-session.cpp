#include <stdio.h>
#include <vector>
#include <string>
#include <tuple>
#include "mapviewer-session.h"

MapviewerSession::MapviewerSession( DBus::Connection &connection )
    : DBus::ObjectAdaptor(connection, "/org/genivi/mapviewer_session"),
      lastSession(0)
{
}

::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > MapviewerSession::GetVersion()
{
    ::DBus::Struct<uint16_t, uint16_t, uint16_t, std::string> version;
    version._1 = 0;
    version._2 = 1;
    version._3 = 2;
    version._4 = std::string("Demo");
    return version;
}

uint32_t MapviewerSession::CreateSession(const std::string& client)
{
    lastSession++;
    fprintf(stderr,"SESSION ADAPTOR - Created session %d [%s]\n",lastSession,client.c_str());
    return lastSession;
}

void MapviewerSession::DeleteSession(const uint32_t& sessionHandle)
{
    fprintf(stderr,"SESSION ADAPTOR - Deleted session %d\n",sessionHandle);
}

int32_t MapviewerSession::GetSessionStatus(const uint32_t& sessionHandle)
{
    return sessionHandle%2;
}

std::vector< ::DBus::Struct< uint32_t, std::string > > MapviewerSession::GetAllSessions()
{
    std::vector< ::DBus::Struct< uint32_t, std::string > > list;
    ::DBus::Struct< uint32_t, std::string > a,b;
    a._1 = 1; a._2 = std::string("Session 1");
    b._1 = 2; b._2 = std::string("Session b");
    list.push_back( a );
    list.push_back( b );
    return list;
}
