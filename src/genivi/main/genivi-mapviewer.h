#ifndef GENIVI_MAPVIEWER_H
#define GENIVI_MAPVIEWER_H

#include <dbus-c++/dbus.h>
#include "genivi-mapviewer-adaptor.h"

class Mapviewer :
    public org::genivi::mapviewer::Session_adaptor,
    public org::genivi::mapviewer::Configuration_adaptor,
    public org::genivi::mapviewer::MapViewerControl_adaptor,
    public DBus::IntrospectableAdaptor,
    public DBus::ObjectAdaptor
{
    public:
        Mapviewer( DBus::Connection &connection );

        // Session interface
        ::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > SessionGetVersion();
        uint32_t CreateSession(const std::string& client);
        void DeleteSession(const uint32_t& sessionHandle);
        int32_t GetSessionStatus(const uint32_t& sessionHandle);
        std::vector< ::DBus::Struct< uint32_t, std::string > > GetAllSessions();

        // Configuration interface
        ::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > ConfigurationGetVersion();
        void SetUnitsOfMeasurement(const std::map< int32_t, int32_t >& unitsOfMeasurementList);
        std::map< int32_t, int32_t > GetUnitsOfMeasurement();
        std::map< int32_t, std::vector< int32_t > > GetSupportedUnitsOfMeasurement();
        void SetTimeFormat(const int32_t& timeFormat);
        int32_t GetTimeFormat();
        std::vector< int32_t > GetSupportedTimeFormats();
        void SetCoordinatesFormat(const int32_t& coordinatesFormat);
        int32_t GetCoordinatesFormat();
        std::vector< int32_t > GetSupportedCoordinatesFormats();
        void SetLocale(const std::string& languageCode, const std::string& countryCode, const std::string& scriptCode);
        void GetLocale(std::string& languageCode, std::string& countryCode, std::string& scriptCode);
        std::vector< ::DBus::Struct< std::string, std::string, std::string > > GetSupportedLocales();

        // MapViewerControl interface
        ::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > MapViewerControlGetVersion();
        uint32_t CreateMapViewInstance(const uint32_t& sessionHandle, const ::DBus::Struct< uint16_t, uint16_t >& mapViewSize, const int32_t& mapViewType);
        void ReleaseMapViewInstance(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle);
        int32_t GetMapViewType(const uint32_t& mapViewInstanceHandle);
        std::vector< int32_t > GetSupportedMapViewTypes();
        void SetTargetPoint(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const ::DBus::Struct< double, double, double >& targetPoint);
        ::DBus::Struct< double, double, double > GetTargetPoint(const uint32_t& mapViewInstanceHandle);
        void SetFollowCarMode(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const bool& followCarMode);
        bool GetFollowCarMode(const uint32_t& mapViewInstanceHandle);
        void SetCameraPosition(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const ::DBus::Struct< double, double, double >& position);
        ::DBus::Struct< double, double, double > GetCameraPosition(const uint32_t& mapViewInstanceHandle);
        void SetCameraHeadingAngle(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& heading);
        void SetCameraHeadingToTarget(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const ::DBus::Struct< double, double >& target);
        void SetCameraHeadingTrackUp(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle);
        void GetCameraHeading(const uint32_t& mapViewInstanceHandle, int32_t& headingType, int32_t& headingAngle, ::DBus::Struct< double, double >& target);
        void SetCameraTiltAngle(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& tilt);
        int32_t GetCameraTiltAngle(const uint32_t& mapViewInstanceHandle);
        void SetCameraRollAngle(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& roll);
        int32_t GetCameraRollAngle(const uint32_t& mapViewInstanceHandle);
        void SetCameraDistanceFromTargetPoint(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const uint32_t& distance);
        uint32_t GetCameraDistanceFromTargetPoint(const uint32_t& mapViewInstanceHandle);
        void SetMapViewScaleMode(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& scaleMode);
        int32_t GetMapViewScaleMode(const uint32_t& mapViewInstanceHandle);
        std::vector< int32_t > GetSupportedMapViewScaleModes(const uint32_t& mapViewInstanceHandle);
        void AddMapViewScaleChangedListener();
        void RemoveMapViewScaleChangedListener();
        void SetCameraHeight(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const uint32_t& height);
        uint32_t GetCameraHeight(const uint32_t& mapViewInstanceHandle);
        void SetMapViewPerspective(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& perspective);
        int32_t GetMapViewPerspective(const uint32_t& mapViewInstanceHandle);
        std::vector< int32_t > GetSupportedMapViewPerspectives();
        void SetMapViewObjectVisibility(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::map< int32_t, bool >& objectVisibilityList);
        std::map< int32_t, bool > GetMapViewObjectVisibility(const uint32_t& mapViewInstanceHandle);
        std::vector< int32_t > GetSupportedMapViewObjectVisibilities(const uint32_t& mapViewInstanceHandle);
        std::vector< ::DBus::Struct< uint16_t, uint16_t, int32_t, uint32_t > > GetScaleList(const uint32_t& mapViewInstanceHandle);
        void SetMapViewScale(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const uint16_t& scaleID);
        void SetMapViewScaleByDelta(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int16_t& scaleDelta);
        void SetMapViewScaleByMetersPerPixel(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const double& metersPerPixel);
        void GetMapViewScale(const uint32_t& mapViewInstanceHandle, uint8_t& scaleID, int32_t& isMinMax);
        void SetMapViewBoundingBox(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const ::DBus::Struct< ::DBus::Struct< double, double >, ::DBus::Struct< double, double > >& boundingBox);
        ::DBus::Struct< ::DBus::Struct< double, double >, ::DBus::Struct< double, double > > GetMapViewBoundingBox(const uint32_t& mapViewInstanceHandle);
        void SetMapViewSaveArea(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const ::DBus::Struct< double, double, double, double >& saveArea);
        ::DBus::Struct< double, double, double, double > GetMapViewSaveArea(const uint32_t& mapViewInstanceHandle);
        void SetMapViewPan(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& panningAction, const std::vector< ::DBus::Struct< uint16_t, uint16_t > >& pixelCoordinates);
        std::vector< ::DBus::Struct< uint16_t, uint16_t > > GetMapViewPan(const uint32_t& mapViewInstanceHandle, const int32_t& valueToReturn);
        void SetMapViewRotation(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& rotationAngle, const int32_t& rotationAnglePerSecond);
        void GetMapViewRotation(const uint32_t& mapViewInstanceHandle, int32_t& rotationAngle, int32_t& rotationAnglePerFrame);
        void SetMapViewVisibilityMode(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& visibilityMode);
        int32_t GetMapViewVisibilityMode(const uint32_t& mapViewInstanceHandle);
        std::vector< int32_t > GetSupportedMapViewVisibilityModes();
        void SetMapViewPerformanceLevel(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& performanceLevel);
        int32_t GetMapViewPerformanceLevel(const uint32_t& mapViewInstanceHandle);
        std::vector< int32_t > GetSupportedMapViewPerformanceLevels();
        void DisplayRoute(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const uint32_t& routeHandle, const bool& highlighted);
        void HideRoute(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const uint32_t& routeHandle);
        std::vector< ::DBus::Struct< uint32_t, bool > > GetDisplayedRoutes(const uint32_t& mapViewInstanceHandle);
        std::vector< uint32_t > GetPoiCategoriesVisible(const uint32_t& mapViewInstanceHandle);
        void SetPoiCategoriesVisible(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::vector< uint32_t >& poiCategoryIds);
        void SetPoiCategoriesVisibleWithinLimits(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::vector< uint32_t >& poiCategoryIds, const uint8_t& minScaleID, const uint8_t& maxScaleID);
        void SetPoiCategoriesNotVisible(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::vector< uint32_t >& poiCategoryIds);
        void SetTrafficIncidentsVisibility(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const bool& visible);
        void SetMapViewTheme(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& mapViewTheme);
        int32_t GetMapViewTheme(const uint32_t& mapViewInstanceHandle);
        std::vector< int32_t > GetSupportedMapViewThemes();
        std::vector< ::DBus::Struct< double, double > > ConvertPixelCoordsToGeoCoords(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::vector< ::DBus::Struct< uint16_t, uint16_t > >& pixelCoordinates);
        std::vector< ::DBus::Struct< uint16_t, uint16_t > > ConvertGeoCoordsToPixelCoords(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::vector< ::DBus::Struct< double, double > >& geoCoordinates);
        std::vector< uint32_t > DisplayCustomElements(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::vector< ::DBus::Struct< std::string, std::string, ::DBus::Struct< double, double >, ::DBus::Struct< int16_t, int16_t > > >& customElements);
        void HideCustomElements(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::vector< uint32_t >& customElementHandles);
        std::map< uint32_t, ::DBus::Struct< std::string, std::string, ::DBus::Struct< double, double >, ::DBus::Struct< int16_t, int16_t > > > GetDisplayedCustomElements(const uint32_t& mapViewInstanceHandle);
        std::vector< ::DBus::Struct< int32_t, ::DBus::Struct< double, double >, ::DBus::Struct< uint8_t, ::DBus::Variant > > > SelectElementsOnMap(const uint32_t& mapViewInstanceHandle, const ::DBus::Struct< uint16_t, uint16_t >& pixelCoordinate, const std::vector< int32_t >& selectableTypes, const uint16_t& maxNumberOfSelectedElements);

        static const int INVALID=0x0000;
        static const int ALL=0xffff;
        static const int AVAILABLE=0x0001;
        static const int NOT_AVAILABLE=0x0002;
        static const int TIME_FORMAT=0x0003;
        static const int 12H=0x0004;
        static const int 24H=0x0005;
        static const int COORDINATES_FORMAT=0x0006;
        static const int DEGREES=0x0007;
        static const int MINUTES=0x0008;
        static const int SECONDS=0x0009;
        static const int MAIN_MAP=0x0010;
        static const int SPLIT_SCREEN=0x0011;
        static const int 2D=0x0020;
        static const int 3D=0x0021;
        static const int LOCALE=0x0025;
        static const int UNITS_OF_MEASUREMENT=0x0030;
        static const int LENGTH=0x0031;
        static const int METER=0x0032;
        static const int MILE=0x0033;
        static const int KM=0x0034;
        static const int YARD=0x0035;
        static const int FOOT=0x0036;
        static const int MIN=0x0040;
        static const int MAX=0x0041;
        static const int MID=0x0042;
        static const int VISIBLE=0x0043;
        static const int INVISIBLE=0x0044;
        static const int FROZEN=0x0045;
        static const int LEVEL1=0x0050;
        static const int LEVEL2=0x0051;
        static const int LEVEL3=0x0052;
        static const int LEVEL4=0x0053;
        static const int LEVEL5=0x0054;
        static const int THEME_1=0x0060;
        static const int THEME_2=0x0061;
        static const int THEME_3=0x0062;
        static const int CONSTANT_ANGLE=0x0070;
        static const int TRACK_UP=0x0071;
        static const int TOWARDS_TARGET=0x0072;
        static const int PAN_START=0x0100;
        static const int PAN_TO=0x0101;
        static const int PAN_END=0x0102;
        static const int BUILDINGS=0x0080;
        static const int TERRAIN=0x0081;
        static const int AUTOMATIC=0x0110;
        static const int MANUAL=0x0111;
        static const int HYBRID=0x0112;
        static const int CUSTOM_ELEMENT=0x0120;
        static const int CURRENT_POSITION=0x0121;
        static const int WAYPOINT=0x0122;
        static const int POI=0x0123;
        static const int TRAFFIC_INCIDENT=0x0124;
        static const int ROUTE=0x0125;
        static const int GEOCOORDINATES=0x0126;
        
    private:
        uint32_t lastSession;
};

#endif // GENIVI_MAPVIEWER_H
