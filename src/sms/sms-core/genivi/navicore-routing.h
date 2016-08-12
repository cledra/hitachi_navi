#ifndef NAVICORE_ROUTING_H
#define NAVICORE_ROUTING_H

#include <dbus-c++/dbus.h>
#include "genivi-navicore-routing-adaptor.h"

class NavicoreRouting :
    public org::genivi::navigationcore::Routing_adaptor,
    public DBus::IntrospectableAdaptor,
    public DBus::ObjectAdaptor
{
    public:
        NavicoreRouting( DBus::Connection &connection );

        ::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > GetVersion();
        uint32_t CreateRoute(const uint32_t& sessionHandle);
        void DeleteRoute(const uint32_t& sessionHandle, const uint32_t& routeHandle);
        void SetCostModel(const uint32_t& sessionHandle, const uint32_t& routeHandle, const int32_t& costModel);
        int32_t GetCostModel(const uint32_t& routeHandle);
        std::vector< int32_t > GetSupportedCostModels();
        void SetRoutePreferences(const uint32_t& sessionHandle, const uint32_t& routeHandle, const std::string& countryCode, const std::vector< ::DBus::Struct< int32_t, int32_t > >& roadPreferenceList, const std::vector< ::DBus::Struct< int32_t, int32_t > >& conditionPreferenceList);
        void GetRoutePreferences(const uint32_t& routeHandle, const std::string& countryCode, std::vector< ::DBus::Struct< int32_t, int32_t > >& roadPreferenceList, std::vector< ::DBus::Struct< int32_t, int32_t > >& conditionPreferenceList);
        void GetSupportedRoutePreferences(std::vector< ::DBus::Struct< int32_t, int32_t > >& routePreferencesList, std::vector< ::DBus::Struct< int32_t, int32_t > >& conditionPreferenceList);
        void SetRouteSchedule(const uint32_t& sessionHandle, const uint32_t& routeHandle, const std::map< int32_t, uint32_t >& routeSchedule);
        std::map< int32_t, uint32_t > GetRouteSchedule(const uint32_t& routeHandle, const std::vector< int32_t >& valuesToReturn);
        void SetTransportationMeans(const uint32_t& sessionHandle, const uint32_t& routeHandle, const std::vector< int32_t >& transportationMeansList);
        std::vector< int32_t > GetTransportationMeans(const uint32_t& routeHandle);
        std::vector< int32_t > GetSupportedTransportationMeans();
        void SetExcludedAreas(const uint32_t& sessionHandle, const uint32_t& routeHandle, const std::vector< std::vector< ::DBus::Struct< double, double > > >& excludedAreas);
        std::vector< std::vector< ::DBus::Struct< double, double > > > GetExcludedAreas(const uint32_t& routeHandle);
        void SetWaypoints(const uint32_t& sessionHandle, const uint32_t& routeHandle, const bool& startFromCurrentPosition, const std::vector< std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > >& waypointsList);
        void GetWaypoints(const uint32_t& routeHandle, bool& startFromCurrentPosition, std::vector< std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > >& waypointsList);
        void CalculateRoute(const uint32_t& sessionHandle, const uint32_t& routeHandle);
        void CancelRouteCalculation(const uint32_t& sessionHandle, const uint32_t& routeHandle);
        std::vector< uint32_t > CalculateRoutes(const uint32_t& sessionHandle, const std::vector< uint32_t >& calculatedRoutesList);
        void GetRouteSegments(const uint32_t& routeHandle, const int16_t& detailLevel, const std::vector< int32_t >& valuesToReturn, const uint32_t& numberOfSegments, const uint32_t& offset, uint32_t& totalNumberOfSegments, std::vector< std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > >& routeSegments);
        std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > GetRouteOverview(const uint32_t& routeHandle, const std::vector< int32_t >& valuesToReturn);
        ::DBus::Struct< ::DBus::Struct< double, double >, ::DBus::Struct< double, double > > GetRouteBoundingBox(const uint32_t& routeHandle);
        std::vector< uint32_t > GetAllRoutes();
        void SetBlockedRouteStretches(const uint32_t& sessionHandle, const uint32_t& routeHandle, const std::vector< ::DBus::Struct< uint32_t, uint32_t > >& blockParameters);
        std::vector< ::DBus::Struct< uint32_t, uint32_t > > GetBlockedRouteStretches(const uint32_t& routeHandle);

    private:
        uint32_t lastSession;
};

#endif
