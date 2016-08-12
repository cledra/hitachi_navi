#include <stdio.h>
#include <vector>
#include <string>
#include <tuple>
#include "navicore-routing.h"

NavicoreRouting::NavicoreRouting( DBus::Connection &connection )
    : Routing_adaptor(),
      DBus::ObjectAdaptor(connection, "/org/genivi/navicore_routing"),
      lastSession(0)
{
}

::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > NavicoreRouting::GetVersion()
{
    ::DBus::Struct<uint16_t, uint16_t, uint16_t, std::string> version;
    version._1 = 0;
    version._2 = 1;
    version._3 = 2;
    version._4 = std::string("Navicore-Routing");
    return version;
}

uint32_t NavicoreRouting::CreateRoute(const uint32_t& sessionHandle)
{
    // TODO
}

void NavicoreRouting::DeleteRoute(const uint32_t& sessionHandle, const uint32_t& routeHandle)
{
    // TODO
}

void NavicoreRouting::SetCostModel(const uint32_t& sessionHandle, const uint32_t& routeHandle, const int32_t& costModel)
{
    // TODO
}

int32_t NavicoreRouting::GetCostModel(const uint32_t& routeHandle)
{
    // TODO
}

std::vector< int32_t > NavicoreRouting::GetSupportedCostModels()
{
    // TODO
}

void NavicoreRouting::SetRoutePreferences(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle,
    const std::string& countryCode,
    const std::vector< ::DBus::Struct< int32_t, int32_t > >& roadPreferenceList,
    const std::vector< ::DBus::Struct< int32_t, int32_t > >& conditionPreferenceList)
{
    // TODO
}
void NavicoreRouting::GetRoutePreferences(
    const uint32_t& routeHandle,
    const std::string& countryCode,
    std::vector< ::DBus::Struct< int32_t, int32_t > >& roadPreferenceList,
    std::vector< ::DBus::Struct< int32_t, int32_t > >& conditionPreferenceList)
{
    // TODO
}

void NavicoreRouting::GetSupportedRoutePreferences(
    std::vector< ::DBus::Struct< int32_t, int32_t > >& routePreferencesList,
    std::vector< ::DBus::Struct< int32_t, int32_t > >& conditionPreferenceList)
{

}

void NavicoreRouting::SetRouteSchedule(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle,
    const std::map< int32_t, uint32_t >& routeSchedule)
{
    // TODO
}

std::map< int32_t, uint32_t > NavicoreRouting::GetRouteSchedule(
    const uint32_t& routeHandle,
    const std::vector< int32_t >& valuesToReturn)
{
    // TODO
}

void NavicoreRouting::SetTransportationMeans(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle,
    const std::vector< int32_t >& transportationMeansList)
{
    // TODO
}

std::vector< int32_t > NavicoreRouting::GetTransportationMeans(
    const uint32_t& routeHandle)
{
    // TODO
}

std::vector< int32_t > NavicoreRouting::GetSupportedTransportationMeans()
{
    // TODO
}

void NavicoreRouting::SetExcludedAreas(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle,
    const std::vector< std::vector< ::DBus::Struct< double, double > > >& excludedAreas)
{
    // TODO
}

std::vector< std::vector< ::DBus::Struct< double, double > > > NavicoreRouting::GetExcludedAreas(
    const uint32_t& routeHandle)
{
    // TODO
}

void NavicoreRouting::SetWaypoints(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle,
    const bool& startFromCurrentPosition,
    const std::vector< std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > >& waypointsList)
{
    // TODO
}

void NavicoreRouting::GetWaypoints(
    const uint32_t& routeHandle,
    bool& startFromCurrentPosition,
    std::vector< std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > >& waypointsList)
{
    // TODO
}

void NavicoreRouting::CalculateRoute(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle)
{
    // TODO
}

void NavicoreRouting::CancelRouteCalculation(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle)
{
    // TODO
}

std::vector< uint32_t > NavicoreRouting::CalculateRoutes(
    const uint32_t& sessionHandle,
    const std::vector< uint32_t >& calculatedRoutesList)
{
    // TODO
}

void NavicoreRouting::GetRouteSegments(
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

std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > NavicoreRouting::GetRouteOverview(
    const uint32_t& routeHandle,
    const std::vector< int32_t >& valuesToReturn)
{
    // TODO
}

::DBus::Struct< ::DBus::Struct< double, double >, ::DBus::Struct< double, double > > NavicoreRouting::GetRouteBoundingBox(
    const uint32_t& routeHandle)
{
    // TODO
}

std::vector< uint32_t > NavicoreRouting::GetAllRoutes()
{
    // TODO
}

void NavicoreRouting::SetBlockedRouteStretches(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle,
    const std::vector< ::DBus::Struct< uint32_t, uint32_t > >& blockParameters)
{
    // TODO
}

std::vector< ::DBus::Struct< uint32_t, uint32_t > > NavicoreRouting::GetBlockedRouteStretches(
    const uint32_t& routeHandle)
{
    // TODO
}
