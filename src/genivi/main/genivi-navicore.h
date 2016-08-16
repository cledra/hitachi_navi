#ifndef GENIVI_NAVICORE_H
#define GENIVI_NAVICORE_H

#include <dbus-c++/dbus.h>
#include "genivi-navigationcore-adaptor.h"

class Navicore :
    public org::genivi::navigationcore::Session_adaptor,
    public org::genivi::navigationcore::Routing_adaptor,
    public org::genivi::navigationcore::MapMatchedPosition_adaptor,
    public org::genivi::navigationcore::Guidance_adaptor,
    public DBus::IntrospectableAdaptor,
    public DBus::ObjectAdaptor
{
    public:
        Navicore( DBus::Connection &connection );

        // Session interface
        ::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > SessionGetVersion();
        uint32_t CreateSession(const std::string& client);
        void DeleteSession(const uint32_t& sessionHandle);
        int32_t GetSessionStatus(const uint32_t& sessionHandle);
        std::vector< ::DBus::Struct< uint32_t, std::string > > GetAllSessions();

        // Routing interface
        ::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > RoutingGetVersion();
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

        // MapMatchedPosition interface
        ::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > PositionGetVersion();
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

        // Guidance interface
        ::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > GuidanceGetVersion();
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

    private:
        uint32_t lastSession;
};

#endif // GENIVI_NAVICORE_H
