#ifndef NAVICORE_GUIDANCE_H
#define NAVICORE_GUIDANCE_H

#include <dbus-c++/dbus.h>
#include "genivi-navicore-guidance-adaptor.h"

class NavicoreGuidance :
    public org::genivi::navigationcore::Guidance_adaptor,
    public DBus::IntrospectableAdaptor,
    public DBus::ObjectAdaptor
{
    public:
        NavicoreGuidance( DBus::Connection &connection );

        ::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > GetVersion();
        void StartGuidance(const uint32_t& sessionHandle, const uint32_t& routeHandle);
        void StopGuidance(const uint32_t& sessionHandle);
        void SetVoiceGuidance(const bool& activate, const std::string& voice);
        void GetGuidanceDetails(bool& voiceGuidance, bool& vehicleOnTheRoad, bool& isDestinationReached, int32_t& maneuver);
        void PlayVoiceManeuver();
        void GetWaypointInformation(const uint16_t& requestedNumberOfWaypoints, uint16_t& numberOfWaypoints, std::vector< ::DBus::Struct< uint32_t, uint32_t, int32_t, int32_t, int16_t, int16_t, bool, uint16_t > >& waypointsList);
        void GetDestinationInformation(uint32_t& offset, uint32_t& travelTime, int32_t& direction, int32_t& side, int16_t& timeZone, int16_t& daylightSavingTime);
        void GetManeuversList(const uint16_t& requestedNumberOfManeuvers, const uint32_t& maneuverOffset, uint16_t& numberOfManeuvers, std::vector< ::DBus::Struct< std::string, std::string, uint16_t, int32_t, uint32_t, std::vector< ::DBus::Struct< uint32_t, uint32_t, int32_t, int32_t, std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > > > > >& maneuversList);
        void SetRouteCalculationMode(const uint32_t& sessionHandle, const int32_t& routeCalculationMode);
        void SkipNextManeuver(const uint32_t& sessionHandle);
        void GetGuidanceStatus(int32_t& guidanceStatus, uint32_t& routeHandle);
        void SetVoiceGuidanceSettings(const int32_t& promptMode);
        int32_t GetVoiceGuidanceSettings();
};

#endif
