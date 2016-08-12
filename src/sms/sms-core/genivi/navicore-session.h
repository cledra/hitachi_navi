#ifndef NAVICORE_SESSION_H
#define NAVICORE_SESSION_H

#include <dbus-c++/dbus.h>
#include "genivi-navicore-session-adaptor.h"

class NavicoreSession :
    public org::genivi::navigationcore::Session_adaptor,
    public DBus::IntrospectableAdaptor,
    public DBus::ObjectAdaptor
{
    public:
        NavicoreSession( DBus::Connection &connection );

        ::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > GetVersion();
        uint32_t CreateSession(const std::string& client);
        void DeleteSession(const uint32_t& sessionHandle);
        int32_t GetSessionStatus(const uint32_t& sessionHandle);
        std::vector< ::DBus::Struct< uint32_t, std::string > > GetAllSessions();

    private:
        uint32_t lastSession;
};

#endif
