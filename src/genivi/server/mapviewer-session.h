#ifndef MAPVIEWER_SESSION_H
#define MAPVIEWER_SESSION_H

#include <dbus-c++/dbus.h>
#include "genivi-mapviewer-session-adaptor.h"

class MapviewerSession :
    public org::genivi::mapviewer::Session_adaptor,
    public DBus::IntrospectableAdaptor,
    public DBus::ObjectAdaptor
{
    public:
        MapviewerSession( DBus::Connection &connection );

        ::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > GetVersion();
        uint32_t CreateSession(const std::string& client);
        void DeleteSession(const uint32_t& sessionHandle);
        int32_t GetSessionStatus(const uint32_t& sessionHandle);
        std::vector< ::DBus::Struct< uint32_t, std::string > > GetAllSessions();

    private:
        uint32_t lastSession;
};

#endif
