#include <stdio.h>
#include <vector>
#include <string>
#include <tuple>
#include "navicore-guidance.h"

NavicoreGuidance::NavicoreGuidance( DBus::Connection &connection )
    : DBus::ObjectAdaptor(connection, "/org/genivi/navicore_guidance")
{
}

::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > NavicoreGuidance::GetVersion()
{
    //::DBus::Struct<uint16_t, uint16_t, uint16_t, std::string> version;
    //version._1 = 0;
    //version._2 = 1;
    //version._3 = 2;
    //version._4 = std::string("Demo");
    //return version;
}

void NavicoreGuidance::NavicoreGuidance::StartGuidance(const uint32_t& sessionHandle, const uint32_t& routeHandle)
{
    // TODO
}

void NavicoreGuidance::StopGuidance(const uint32_t& sessionHandle)
{
    // TODO
}

void NavicoreGuidance::SetVoiceGuidance(const bool& activate, const std::string& voice)
{
    // TODO
}

void NavicoreGuidance::GetGuidanceDetails(bool& voiceGuidance, bool& vehicleOnTheRoad, bool& isDestinationReached, int32_t& maneuver)
{
    // TODO
}

void NavicoreGuidance::PlayVoiceManeuver()
{
    // TODO
}

void NavicoreGuidance::GetWaypointInformation(const uint16_t& requestedNumberOfWaypoints, uint16_t& numberOfWaypoints, std::vector< ::DBus::Struct< uint32_t, uint32_t, int32_t, int32_t, int16_t, int16_t, bool, uint16_t > >& waypointsList)
{
    // TODO
}

void NavicoreGuidance::GetDestinationInformation(uint32_t& offset, uint32_t& travelTime, int32_t& direction, int32_t& side, int16_t& timeZone, int16_t& daylightSavingTime)
{
    // TODO
}

void NavicoreGuidance::GetManeuversList(const uint16_t& requestedNumberOfManeuvers, const uint32_t& maneuverOffset, uint16_t& numberOfManeuvers, std::vector< ::DBus::Struct< std::string, std::string, uint16_t, int32_t, uint32_t, std::vector< ::DBus::Struct< uint32_t, uint32_t, int32_t, int32_t, std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > > > > >& maneuversList)
{
    // TODO
}

void NavicoreGuidance::SetRouteCalculationMode(const uint32_t& sessionHandle, const int32_t& routeCalculationMode)
{
    // TODO
}

void NavicoreGuidance::SkipNextManeuver(const uint32_t& sessionHandle)
{
    // TODO
}

void NavicoreGuidance::GetGuidanceStatus(int32_t& guidanceStatus, uint32_t& routeHandle)
{
    // TODO
}

void NavicoreGuidance::SetVoiceGuidanceSettings(const int32_t& promptMode)
{
    // TODO
}

int32_t NavicoreGuidance::GetVoiceGuidanceSettings()
{
    // TODO
}
