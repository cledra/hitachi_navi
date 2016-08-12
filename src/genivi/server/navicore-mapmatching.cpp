#include <stdio.h>
#include <vector>
#include <string>
#include <tuple>
#include "navicore-mapmatching.h"

NavicoreMapmatching::NavicoreMapmatching( DBus::Connection &connection )
    : DBus::ObjectAdaptor(connection, "/org/genivi/navicore_mapmatching")
{
}

::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > NavicoreMapmatching::GetVersion()
{
    //::DBus::Struct<uint16_t, uint16_t, uint16_t, std::string> version;
    //version._1 = ;
    //version._2 = ;
    //version._3 = ;
    //version._4 = std::string("");
    //return version;
}

void NavicoreMapmatching::SetSimulationMode(const uint32_t& sessionHandle, const bool& activate)
{
    // TODO
}

int32_t NavicoreMapmatching::GetSimulationStatus()
{
    // TODO
}

void NavicoreMapmatching::AddSimulationStatusListener()
{
    // TODO
}

void NavicoreMapmatching::RemoveSimulationStatusListener()
{
    // TODO
}

void NavicoreMapmatching::SetSimulationSpeed(const uint32_t& sessionHandle, const uint8_t& speedFactor)
{
    // TODO
}

uint8_t NavicoreMapmatching::GetSimulationSpeed()
{
    // TODO
}

void NavicoreMapmatching::AddSimulationSpeedListener()
{
    // TODO
}

void NavicoreMapmatching::RemoveSimulationSpeedListener()
{
    // TODO
}

void NavicoreMapmatching::StartSimulation(const uint32_t& sessionHandle)
{
    // TODO
}

void NavicoreMapmatching::PauseSimulation(const uint32_t& sessionHandle)
{
    // TODO
}

std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > >
    NavicoreMapmatching::GetPosition(const std::vector< int32_t >& valuesToReturn)
{
    // TODO
}

void NavicoreMapmatching::SetPosition(
    const uint32_t& sessionHandle,
    const std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > >& position)
{
    // TODO
}

std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > >
    NavicoreMapmatching::GetAddress(const std::vector< int32_t >& valuesToReturn)
{
    // TODO
}

std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > >
    NavicoreMapmatching::GetPositionOnSegment(const std::vector< int32_t >& valuesToReturn)
{
    // TODO
}

std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > >
    NavicoreMapmatching::GetStatus(const std::vector< int32_t >& valuesToReturn)
{
    // TODO
}
