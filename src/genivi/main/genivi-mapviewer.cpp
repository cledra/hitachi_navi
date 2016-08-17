#include <stdio.h>
#include <vector>
#include <string>
#include <tuple>
#include "genivi-mapviewer.h"
#include "NaviTrace.h"

using namespace std;

class MapviewerSession
{
    public:
        MapviewerSession(int32_t id_, string client_):
            id(id_){
                client = string(client_);
            }

        int32_t id;
        string client;
};

class MapviewInstance
{
    public:
        MapviewInstance(int32_t id_, string client_):
            id(id_){
                client = string(client_);
            }

        int32_t id;
        string client;
};

static std::vector<struct MapviewerSession> MapviewerSessionList;

Mapviewer::Mapviewer( DBus::Connection &connection )
    : DBus::ObjectAdaptor(connection, "/org/genivi/mapviewer"),
      lastSession(0)
{
}

::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > Mapviewer::SessionGetVersion()
{
    TRACE_WARN("TODO: implement this function");
    ::DBus::Struct<uint16_t, uint16_t, uint16_t, std::string> version;
    version._1 = 0;
    version._2 = 1;
    version._3 = 2;
    version._4 = std::string("Session");
    return version;
}

uint32_t Mapviewer::CreateSession(const std::string& client)
{
    lastSession++;
    //fprintf(stderr,"SESSION ADAPTOR - Created session %d [%s]\n",lastSession,client.c_str());
    TRACE_INFO("SESSION ADAPTOR - Created session %d [%s]", lastSession, client.c_str());

    MapviewerSession s(lastSession, client);
    MapviewerSessionList.push_back(s);
    
    return lastSession;
}

void Mapviewer::DeleteSession(const uint32_t& sessionHandle)
{
    TRACE_INFO("SESSION ADAPTOR - Deleted session %d", sessionHandle);

    std::vector<struct MapviewerSession>::iterator it;
    for (it = MapviewerSessionList.begin() ; it != MapviewerSessionList.end(); it++)
    {
        if (it->id == sessionHandle) break;
    }
    if (it != MapviewerSessionList.end())
    {
        TRACE_DEBUG("SESSION ADAPTOR - Delete session %d (%s)", sessionHandle, it->client.c_str());
        MapviewerSessionList.erase(it);
    }
}

int32_t Mapviewer::GetSessionStatus(const uint32_t& sessionHandle)
{
    std::vector<struct MapviewerSession>::iterator it;
    int i=0;
    for (it = MapviewerSessionList.begin() ; it != MapviewerSessionList.end(); it++)
    {
        if (it->id != sessionHandle) i++;
        else break;
    }
    if (i < MapviewerSessionList.size())
    {
        TRACE_DEBUG("SESSION ADAPTOR - Session %d is present in list at index %d (%s)", sessionHandle, i, it->client.c_str());
        return 0; // not available
    }

    return 1; // available
}

std::vector< ::DBus::Struct< uint32_t, std::string > > Mapviewer::GetAllSessions()
{
    TRACE_WARN("TODO: implement this function");
    std::vector< ::DBus::Struct< uint32_t, std::string > > list;
    ::DBus::Struct< uint32_t, std::string > a,b;
    a._1 = 1; a._2 = std::string("Session 1");
    b._1 = 2; b._2 = std::string("Session b");
    list.push_back( a );
    list.push_back( b );
    return list;
}

// Configuration interface
::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > Mapviewer::ConfigurationGetVersion()
{
    TRACE_WARN("TODO: implement this function");
    ::DBus::Struct<uint16_t, uint16_t, uint16_t, std::string> version;
    version._1 = 0;
    version._2 = 0;
    version._3 = 0;
    version._4 = std::string("Configuration");
    return version;
}

void Mapviewer::SetUnitsOfMeasurement(const std::map< int32_t, int32_t >& unitsOfMeasurementList)
{
    TRACE_WARN("TODO: implement this function");
}

std::map< int32_t, int32_t > Mapviewer::GetUnitsOfMeasurement()
{
    TRACE_WARN("TODO: implement this function");
}

std::map< int32_t, std::vector< int32_t > > Mapviewer::GetSupportedUnitsOfMeasurement()
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::SetTimeFormat(const int32_t& timeFormat)
{
    TRACE_WARN("TODO: implement this function");
}

int32_t Mapviewer::GetTimeFormat()
{
    TRACE_WARN("TODO: implement this function");
}

std::vector< int32_t > Mapviewer::GetSupportedTimeFormats()
{
    // TODO
}

void Mapviewer::SetCoordinatesFormat(const int32_t& coordinatesFormat)
{
    TRACE_WARN("TODO: implement this function");
}

int32_t Mapviewer::GetCoordinatesFormat()
{
    TRACE_WARN("TODO: implement this function");
}

std::vector< int32_t > Mapviewer::GetSupportedCoordinatesFormats()
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::SetLocale(const std::string& languageCode, const std::string& countryCode, const std::string& scriptCode)
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::GetLocale(std::string& languageCode, std::string& countryCode, std::string& scriptCode)
{
    TRACE_WARN("TODO: implement this function");
}

std::vector< ::DBus::Struct< std::string, std::string, std::string > > Mapviewer::GetSupportedLocales()
{
    TRACE_WARN("TODO: implement this function");
}

::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > Mapviewer::MapViewerControlGetVersion()
{
    ::DBus::Struct<uint16_t, uint16_t, uint16_t, std::string> version;
    version._1 = 0;
    version._2 = 1;
    version._3 = 2;
    version._4 = std::string("MapviewerControl");
    return version;
}

uint32_t Mapviewer::CreateMapViewInstance(
    const uint32_t& sessionHandle,
    const ::DBus::Struct< uint16_t, uint16_t >& mapViewSize,
    const int32_t& mapViewType)
{
    // TODO
}

void Mapviewer::ReleaseMapViewInstance(
    const uint32_t& sessionHandle,
    const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

int32_t Mapviewer::GetMapViewType(
    const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

std::vector< int32_t > Mapviewer::GetSupportedMapViewTypes()
{
    // TODO
}

void Mapviewer::SetTargetPoint(
    const uint32_t& sessionHandle,
    const uint32_t& mapViewInstanceHandle,
    const ::DBus::Struct< double, double, double >& targetPoint)
{
    // TODO
}

::DBus::Struct< double, double, double > Mapviewer::GetTargetPoint(
    const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void Mapviewer::SetFollowCarMode(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const bool& followCarMode)
{
    // TODO
}

bool Mapviewer::GetFollowCarMode(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void Mapviewer::SetCameraPosition(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const ::DBus::Struct< double, double, double >& position)
{
    // TODO
}

::DBus::Struct< double, double, double > Mapviewer::GetCameraPosition(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void Mapviewer::SetCameraHeadingAngle(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& heading)
{
    // TODO
}

void Mapviewer::SetCameraHeadingToTarget(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const ::DBus::Struct< double, double >& target)
{
    // TODO
}

void Mapviewer::SetCameraHeadingTrackUp(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void Mapviewer::GetCameraHeading(const uint32_t& mapViewInstanceHandle, int32_t& headingType, int32_t& headingAngle, ::DBus::Struct< double, double >& target)
{
    // TODO
}

void Mapviewer::SetCameraTiltAngle(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& tilt)
{
    // TODO
}

int32_t Mapviewer::GetCameraTiltAngle(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void Mapviewer::SetCameraRollAngle(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& roll)
{
    // TODO
}

int32_t Mapviewer::GetCameraRollAngle(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void Mapviewer::SetCameraDistanceFromTargetPoint(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const uint32_t& distance)
{
    // TODO
}

uint32_t Mapviewer::GetCameraDistanceFromTargetPoint(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void Mapviewer::SetMapViewScaleMode(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& scaleMode)
{
    // TODO
}

int32_t Mapviewer::GetMapViewScaleMode(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

std::vector< int32_t > Mapviewer::GetSupportedMapViewScaleModes(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void Mapviewer::AddMapViewScaleChangedListener()
{
    // TODO
}

void Mapviewer::RemoveMapViewScaleChangedListener()
{
    // TODO
}

void Mapviewer::SetCameraHeight(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const uint32_t& height)
{
    // TODO
}

uint32_t Mapviewer::GetCameraHeight(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void Mapviewer::SetMapViewPerspective(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& perspective)
{
    // TODO
}

int32_t Mapviewer::GetMapViewPerspective(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

std::vector< int32_t > Mapviewer::GetSupportedMapViewPerspectives()
{
    // TODO
}

void Mapviewer::SetMapViewObjectVisibility(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::map< int32_t, bool >& objectVisibilityList)
{
    // TODO
}

std::map< int32_t, bool > Mapviewer::GetMapViewObjectVisibility(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

std::vector< int32_t > Mapviewer::GetSupportedMapViewObjectVisibilities(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

std::vector< ::DBus::Struct< uint16_t, uint16_t, int32_t, uint32_t > > Mapviewer::GetScaleList(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void Mapviewer::SetMapViewScale(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const uint16_t& scaleID)
{
    // TODO
}

void Mapviewer::SetMapViewScaleByDelta(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int16_t& scaleDelta)
{
    // TODO
}

void Mapviewer::SetMapViewScaleByMetersPerPixel(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const double& metersPerPixel)
{
    // TODO
}

void Mapviewer::GetMapViewScale(const uint32_t& mapViewInstanceHandle, uint8_t& scaleID, int32_t& isMinMax)
{
    // TODO
}

void Mapviewer::SetMapViewBoundingBox(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const ::DBus::Struct< ::DBus::Struct< double, double >, ::DBus::Struct< double, double > >& boundingBox)
{
    // TODO
}

::DBus::Struct< ::DBus::Struct< double, double >, ::DBus::Struct< double, double > > Mapviewer::GetMapViewBoundingBox(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void Mapviewer::SetMapViewSaveArea(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const ::DBus::Struct< double, double, double, double >& saveArea)
{
    // TODO
}

::DBus::Struct< double, double, double, double > Mapviewer::GetMapViewSaveArea(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void Mapviewer::SetMapViewPan(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& panningAction, const std::vector< ::DBus::Struct< uint16_t, uint16_t > >& pixelCoordinates)
{
    // TODO
}

std::vector< ::DBus::Struct< uint16_t, uint16_t > > Mapviewer::GetMapViewPan(const uint32_t& mapViewInstanceHandle, const int32_t& valueToReturn)
{
    // TODO
}

void Mapviewer::SetMapViewRotation(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& rotationAngle, const int32_t& rotationAnglePerSecond)
{
    // TODO
}

void Mapviewer::GetMapViewRotation(const uint32_t& mapViewInstanceHandle, int32_t& rotationAngle, int32_t& rotationAnglePerFrame)
{
    // TODO
}

void Mapviewer::SetMapViewVisibilityMode(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& visibilityMode)
{
    // TODO
}

int32_t Mapviewer::GetMapViewVisibilityMode(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

std::vector< int32_t > Mapviewer::GetSupportedMapViewVisibilityModes()
{
    // TODO
}

void Mapviewer::SetMapViewPerformanceLevel(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& performanceLevel)
{
    // TODO
}

int32_t Mapviewer::GetMapViewPerformanceLevel(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

std::vector< int32_t > Mapviewer::GetSupportedMapViewPerformanceLevels()
{
    // TODO
}

void Mapviewer::DisplayRoute(
    const uint32_t& sessionHandle,
    const uint32_t& mapViewInstanceHandle,
    const uint32_t& routeHandle,
    const bool& highlighted)
{
    // TODO
}

void Mapviewer::HideRoute(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const uint32_t& routeHandle)
{
    // TODO
}

std::vector< ::DBus::Struct< uint32_t, bool > > Mapviewer::GetDisplayedRoutes(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

std::vector< uint32_t > Mapviewer::GetPoiCategoriesVisible(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void Mapviewer::SetPoiCategoriesVisible(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::vector< uint32_t >& poiCategoryIds)
{
    // TODO
}

void Mapviewer::SetPoiCategoriesVisibleWithinLimits(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::vector< uint32_t >& poiCategoryIds, const uint8_t& minScaleID, const uint8_t& maxScaleID)
{
    // TODO
}

void Mapviewer::SetPoiCategoriesNotVisible(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::vector< uint32_t >& poiCategoryIds)
{
    // TODO
}

void Mapviewer::SetTrafficIncidentsVisibility(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const bool& visible)
{
    // TODO
}

void Mapviewer::SetMapViewTheme(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& mapViewTheme)
{
    // TODO
}

int32_t Mapviewer::GetMapViewTheme(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

std::vector< int32_t > Mapviewer::GetSupportedMapViewThemes()
{
    // TODO
}

std::vector< ::DBus::Struct< double, double > > Mapviewer::ConvertPixelCoordsToGeoCoords(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::vector< ::DBus::Struct< uint16_t, uint16_t > >& pixelCoordinates)
{
    // TODO
}

std::vector< ::DBus::Struct< uint16_t, uint16_t > > Mapviewer::ConvertGeoCoordsToPixelCoords(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::vector< ::DBus::Struct< double, double > >& geoCoordinates)
{
    // TODO
}

std::vector< uint32_t > Mapviewer::DisplayCustomElements(
    const uint32_t& sessionHandle,
    const uint32_t& mapViewInstanceHandle,
    const std::vector< ::DBus::Struct< std::string, std::string, ::DBus::Struct< double, double >, ::DBus::Struct< int16_t, int16_t > > >& customElements)
{
    // TODO
}

void Mapviewer::HideCustomElements(
    const uint32_t& sessionHandle,
    const uint32_t& mapViewInstanceHandle,
    const std::vector< uint32_t >& customElementHandles)
{
    // TODO
}

std::map< uint32_t, ::DBus::Struct< std::string, std::string, ::DBus::Struct< double, double >, ::DBus::Struct< int16_t, int16_t > > >
    Mapviewer::GetDisplayedCustomElements(
        const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

std::vector< ::DBus::Struct< int32_t, ::DBus::Struct< double, double >, ::DBus::Struct< uint8_t, ::DBus::Variant > > >
    Mapviewer::SelectElementsOnMap(
        const uint32_t& mapViewInstanceHandle,
        const ::DBus::Struct< uint16_t, uint16_t >& pixelCoordinate,
        const std::vector< int32_t >& selectableTypes,
        const uint16_t& maxNumberOfSelectedElements)
{
    // TODO
}
