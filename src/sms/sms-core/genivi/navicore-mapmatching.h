#ifndef NAVICORE_MAPMATCHING_H
#define NAVICORE_MAPMATCHING_H

#include <dbus-c++/dbus.h>
#include "genivi-navicore-mapmatchedposition-adaptor.h"

class NavicoreMapmatching :
    public org::genivi::navigationcore::MapMatchedPosition_adaptor,
    public DBus::IntrospectableAdaptor,
    public DBus::ObjectAdaptor
{
    public:
        NavicoreMapmatching( DBus::Connection &connection );

        ::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > GetVersion();
        void SetSimulationMode(const uint32_t& sessionHandle, const bool& activate);
        int32_t GetSimulationStatus();
        void AddSimulationStatusListener();
        void RemoveSimulationStatusListener();
        void SetSimulationSpeed(const uint32_t& sessionHandle, const uint8_t& speedFactor);
        uint8_t GetSimulationSpeed();
        void AddSimulationSpeedListener();
        void RemoveSimulationSpeedListener();
        void StartSimulation(const uint32_t& sessionHandle);
        void PauseSimulation(const uint32_t& sessionHandle);
        std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > GetPosition(const std::vector< int32_t >& valuesToReturn);
        void SetPosition(const uint32_t& sessionHandle, const std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > >& position);
        std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > GetAddress(const std::vector< int32_t >& valuesToReturn);
        std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > GetPositionOnSegment(const std::vector< int32_t >& valuesToReturn);
        std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > GetStatus(const std::vector< int32_t >& valuesToReturn);

    private:
        uint32_t lastSession;
};

#endif
