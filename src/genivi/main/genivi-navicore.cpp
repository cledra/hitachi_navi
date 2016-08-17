#include <stdio.h>
#include <vector>
#include <string>
#include <tuple>
#include "genivi-navicore.h"

Navicore::Navicore( DBus::Connection &connection )
    : DBus::ObjectAdaptor(connection, "/org/genivi/navicore"),
      lastSession(0)
{
}

::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > Navicore::SessionGetVersion()
{
    ::DBus::Struct<uint16_t, uint16_t, uint16_t, std::string> version;
    version._1 = 3;
    version._2 = 0;
    version._3 = 0;
    version._4 = std::string("22-01-2014");
    return version;
}

uint32_t Navicore::CreateSession(const std::string& client)
{
    lastSession++;
    fprintf(stderr,"SESSION ADAPTOR - Created session %d [%s]\n",lastSession,client.c_str());
    return lastSession;
}

void Navicore::DeleteSession(const uint32_t& sessionHandle)
{
    fprintf(stderr,"SESSION ADAPTOR - Deleted session %d\n",sessionHandle);
}

int32_t Navicore::GetSessionStatus(const uint32_t& sessionHandle)
{
    return sessionHandle%2;
}

std::vector< ::DBus::Struct< uint32_t, std::string > > Navicore::GetAllSessions()
{
    std::vector< ::DBus::Struct< uint32_t, std::string > > list;
    ::DBus::Struct< uint32_t, std::string > a,b;
    a._1 = 1; a._2 = std::string("Session 1");
    b._1 = 2; b._2 = std::string("Session b");
    list.push_back( a );
    list.push_back( b );
    return list;
}

::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > Navicore::RoutingGetVersion()
{
    ::DBus::Struct<uint16_t, uint16_t, uint16_t, std::string> version;
    version._1 = 3;
    version._2 = 0;
    version._3 = 0;
    version._4 = std::string("22-01-2014");
    return version;
}

uint32_t Navicore::CreateRoute(const uint32_t& sessionHandle)
{
    // TODO
}

void Navicore::DeleteRoute(const uint32_t& sessionHandle, const uint32_t& routeHandle)
{
    // TODO
}

void Navicore::SetCostModel(const uint32_t& sessionHandle, const uint32_t& routeHandle, const int32_t& costModel)
{
    // TODO
}

int32_t Navicore::GetCostModel(const uint32_t& routeHandle)
{
    // TODO
}

std::vector< int32_t > Navicore::GetSupportedCostModels()
{
    // TODO
}

void Navicore::SetRoutePreferences(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle,
    const std::string& countryCode,
    const std::vector< ::DBus::Struct< int32_t, int32_t > >& roadPreferenceList,
    const std::vector< ::DBus::Struct< int32_t, int32_t > >& conditionPreferenceList)
{
    // TODO
}
void Navicore::GetRoutePreferences(
    const uint32_t& routeHandle,
    const std::string& countryCode,
    std::vector< ::DBus::Struct< int32_t, int32_t > >& roadPreferenceList,
    std::vector< ::DBus::Struct< int32_t, int32_t > >& conditionPreferenceList)
{
    // TODO
}

void Navicore::GetSupportedRoutePreferences(
    std::vector< ::DBus::Struct< int32_t, int32_t > >& routePreferencesList,
    std::vector< ::DBus::Struct< int32_t, int32_t > >& conditionPreferenceList)
{

}

void Navicore::SetRouteSchedule(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle,
    const std::map< int32_t, uint32_t >& routeSchedule)
{
    // TODO
}

std::map< int32_t, uint32_t > Navicore::GetRouteSchedule(
    const uint32_t& routeHandle,
    const std::vector< int32_t >& valuesToReturn)
{
    // TODO
}

void Navicore::SetTransportationMeans(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle,
    const std::vector< int32_t >& transportationMeansList)
{
    // TODO
}

std::vector< int32_t > Navicore::GetTransportationMeans(
    const uint32_t& routeHandle)
{
    // TODO
}

std::vector< int32_t > Navicore::GetSupportedTransportationMeans()
{
    // TODO
}

void Navicore::SetExcludedAreas(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle,
    const std::vector< std::vector< ::DBus::Struct< double, double > > >& excludedAreas)
{
    // TODO
}

std::vector< std::vector< ::DBus::Struct< double, double > > > Navicore::GetExcludedAreas(
    const uint32_t& routeHandle)
{
    // TODO
}

void Navicore::SetWaypoints(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle,
    const bool& startFromCurrentPosition,
    const std::vector< std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > >& waypointsList)
{
    // TODO
}

void Navicore::GetWaypoints(
    const uint32_t& routeHandle,
    bool& startFromCurrentPosition,
    std::vector< std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > >& waypointsList)
{
    // TODO
}

void Navicore::CalculateRoute(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle)
{
    // TODO
}

void Navicore::CancelRouteCalculation(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle)
{
    // TODO
}

std::vector< uint32_t > Navicore::CalculateRoutes(
    const uint32_t& sessionHandle,
    const std::vector< uint32_t >& calculatedRoutesList)
{
    // TODO
}

void Navicore::GetRouteSegments(
    const uint32_t& routeHandle,
    const int16_t& detailLevel,
    const std::vector< int32_t >& valuesToReturn,
                       const uint32_t& numberOfSegments,
                       const uint32_t& offset,
                       uint32_t& totalNumberOfSegments,
                       std::vector< std::map< int32_t,
                       ::DBus::Struct< uint8_t, ::DBus::Variant > > >& routeSegments)
{
    // TODO
}

std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > Navicore::GetRouteOverview(
    const uint32_t& routeHandle,
    const std::vector< int32_t >& valuesToReturn)
{
    // TODO
}

::DBus::Struct< ::DBus::Struct< double, double >, ::DBus::Struct< double, double > > Navicore::GetRouteBoundingBox(
    const uint32_t& routeHandle)
{
    // TODO
}

std::vector< uint32_t > Navicore::GetAllRoutes()
{
    // TODO
}

void Navicore::SetBlockedRouteStretches(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle,
    const std::vector< ::DBus::Struct< uint32_t, uint32_t > >& blockParameters)
{
    // TODO
}

std::vector< ::DBus::Struct< uint32_t, uint32_t > > Navicore::GetBlockedRouteStretches(
    const uint32_t& routeHandle)
{
    // TODO
}

::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > Navicore::PositionGetVersion()
{
    ::DBus::Struct<uint16_t, uint16_t, uint16_t, std::string> version;
    version._1 = 3;
    version._2 = 0;
    version._3 = 0;
    version._4 = std::string("21-01-2014");
    return version;
}

void Navicore::SetSimulationMode(const uint32_t& sessionHandle, const bool& activate)
{
    // TODO
}

int32_t Navicore::GetSimulationStatus()
{
    // TODO
}

void Navicore::AddSimulationStatusListener()
{
    // TODO
}

void Navicore::RemoveSimulationStatusListener()
{
    // TODO
}

void Navicore::SetSimulationSpeed(const uint32_t& sessionHandle, const uint8_t& speedFactor)
{
    // TODO
}

uint8_t Navicore::GetSimulationSpeed()
{
    // TODO
}

void Navicore::AddSimulationSpeedListener()
{
    // TODO
}

void Navicore::RemoveSimulationSpeedListener()
{
    // TODO
}

void Navicore::StartSimulation(const uint32_t& sessionHandle)
{
    // TODO
}

void Navicore::PauseSimulation(const uint32_t& sessionHandle)
{
    // TODO
}

std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > >
    Navicore::GetPosition(const std::vector< int32_t >& valuesToReturn)
{
    // TODO
}

void Navicore::SetPosition(
    const uint32_t& sessionHandle,
    const std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > >& position)
{
    // TODO
}

std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > >
    Navicore::GetAddress(const std::vector< int32_t >& valuesToReturn)
{
    // TODO
}

std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > >
    Navicore::GetPositionOnSegment(const std::vector< int32_t >& valuesToReturn)
{
    // TODO
}

std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > >
    Navicore::GetStatus(const std::vector< int32_t >& valuesToReturn)
{
    // TODO
}

::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > Navicore::GuidanceGetVersion()
{
    ::DBus::Struct<uint16_t, uint16_t, uint16_t, std::string> version;
    version._1 = 3;
    version._2 = 1;
    version._3 = 0;
    version._4 = std::string("03-03-2014");
    return version;
}

void Navicore::Navicore::StartGuidance(const uint32_t& sessionHandle, const uint32_t& routeHandle)
{
    // TODO
}

void Navicore::StopGuidance(const uint32_t& sessionHandle)
{
    // TODO
}

void Navicore::SetVoiceGuidance(const bool& activate, const std::string& voice)
{
    // TODO
}

void Navicore::GetGuidanceDetails(bool& voiceGuidance, bool& vehicleOnTheRoad, bool& isDestinationReached, int32_t& maneuver)
{
    // TODO
}

void Navicore::PlayVoiceManeuver()
{
    // TODO
}

void Navicore::GetWaypointInformation(const uint16_t& requestedNumberOfWaypoints, uint16_t& numberOfWaypoints, std::vector< ::DBus::Struct< uint32_t, uint32_t, int32_t, int32_t, int16_t, int16_t, bool, uint16_t > >& waypointsList)
{
    // TODO
}

void Navicore::GetDestinationInformation(uint32_t& offset, uint32_t& travelTime, int32_t& direction, int32_t& side, int16_t& timeZone, int16_t& daylightSavingTime)
{
    // TODO
}

void Navicore::GetManeuversList(const uint16_t& requestedNumberOfManeuvers, const uint32_t& maneuverOffset, uint16_t& numberOfManeuvers, std::vector< ::DBus::Struct< std::string, std::string, uint16_t, int32_t, uint32_t, std::vector< ::DBus::Struct< uint32_t, uint32_t, int32_t, int32_t, std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > > > > >& maneuversList)
{
    // TODO
}

void Navicore::SetRouteCalculationMode(const uint32_t& sessionHandle, const int32_t& routeCalculationMode)
{
    // TODO
}

void Navicore::SkipNextManeuver(const uint32_t& sessionHandle)
{
    // TODO
}

void Navicore::GetGuidanceStatus(int32_t& guidanceStatus, uint32_t& routeHandle)
{
    // TODO
}

void Navicore::SetVoiceGuidanceSettings(const int32_t& promptMode)
{
    // TODO
}

int32_t Navicore::GetVoiceGuidanceSettings()
{
    // TODO
}
