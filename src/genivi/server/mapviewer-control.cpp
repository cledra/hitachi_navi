#include <stdio.h>
#include <vector>
#include <string>
#include <tuple>
#include "mapviewer-control.h"

MapviewerControl::MapviewerControl( DBus::Connection &connection )
    : DBus::ObjectAdaptor(connection, "/org/genivi/mapviewer_control")
{
}

::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > MapviewerControl::GetVersion()
{
    // TODO
}

uint32_t MapviewerControl::CreateMapViewInstance(
    const uint32_t& sessionHandle,
    const ::DBus::Struct< uint16_t, uint16_t >& mapViewSize,
    const int32_t& mapViewType)
{
    // TODO
}

void MapviewerControl::ReleaseMapViewInstance(
    const uint32_t& sessionHandle,
    const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

int32_t MapviewerControl::GetMapViewType(
    const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

std::vector< int32_t > MapviewerControl::GetSupportedMapViewTypes()
{
    // TODO
}

void MapviewerControl::SetTargetPoint(
    const uint32_t& sessionHandle,
    const uint32_t& mapViewInstanceHandle,
    const ::DBus::Struct< double, double, double >& targetPoint)
{
    // TODO
}

::DBus::Struct< double, double, double > MapviewerControl::GetTargetPoint(
    const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void MapviewerControl::SetFollowCarMode(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const bool& followCarMode)
{
    // TODO
}

bool MapviewerControl::GetFollowCarMode(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void MapviewerControl::SetCameraPosition(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const ::DBus::Struct< double, double, double >& position)
{
    // TODO
}

::DBus::Struct< double, double, double > MapviewerControl::GetCameraPosition(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void MapviewerControl::SetCameraHeadingAngle(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& heading)
{
    // TODO
}

void MapviewerControl::SetCameraHeadingToTarget(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const ::DBus::Struct< double, double >& target)
{
    // TODO
}

void MapviewerControl::SetCameraHeadingTrackUp(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void MapviewerControl::GetCameraHeading(const uint32_t& mapViewInstanceHandle, int32_t& headingType, int32_t& headingAngle, ::DBus::Struct< double, double >& target)
{
    // TODO
}

void MapviewerControl::SetCameraTiltAngle(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& tilt)
{
    // TODO
}

int32_t MapviewerControl::GetCameraTiltAngle(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void MapviewerControl::SetCameraRollAngle(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& roll)
{
    // TODO
}

int32_t MapviewerControl::GetCameraRollAngle(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void MapviewerControl::SetCameraDistanceFromTargetPoint(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const uint32_t& distance)
{
    // TODO
}

uint32_t MapviewerControl::GetCameraDistanceFromTargetPoint(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void MapviewerControl::SetMapViewScaleMode(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& scaleMode)
{
    // TODO
}

int32_t MapviewerControl::GetMapViewScaleMode(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

std::vector< int32_t > MapviewerControl::GetSupportedMapViewScaleModes(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void MapviewerControl::AddMapViewScaleChangedListener()
{
    // TODO
}

void MapviewerControl::RemoveMapViewScaleChangedListener()
{
    // TODO
}

void MapviewerControl::SetCameraHeight(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const uint32_t& height)
{
    // TODO
}

uint32_t MapviewerControl::GetCameraHeight(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void MapviewerControl::SetMapViewPerspective(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& perspective)
{
    // TODO
}

int32_t MapviewerControl::GetMapViewPerspective(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

std::vector< int32_t > MapviewerControl::GetSupportedMapViewPerspectives()
{
    // TODO
}

void MapviewerControl::SetMapViewObjectVisibility(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::map< int32_t, bool >& objectVisibilityList)
{
    // TODO
}

std::map< int32_t, bool > MapviewerControl::GetMapViewObjectVisibility(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

std::vector< int32_t > MapviewerControl::GetSupportedMapViewObjectVisibilities(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

std::vector< ::DBus::Struct< uint16_t, uint16_t, int32_t, uint32_t > > MapviewerControl::GetScaleList(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void MapviewerControl::SetMapViewScale(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const uint16_t& scaleID)
{
    // TODO
}

void MapviewerControl::SetMapViewScaleByDelta(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int16_t& scaleDelta)
{
    // TODO
}

void MapviewerControl::SetMapViewScaleByMetersPerPixel(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const double& metersPerPixel)
{
    // TODO
}

void MapviewerControl::GetMapViewScale(const uint32_t& mapViewInstanceHandle, uint8_t& scaleID, int32_t& isMinMax)
{
    // TODO
}

void MapviewerControl::SetMapViewBoundingBox(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const ::DBus::Struct< ::DBus::Struct< double, double >, ::DBus::Struct< double, double > >& boundingBox)
{
    // TODO
}

::DBus::Struct< ::DBus::Struct< double, double >, ::DBus::Struct< double, double > > MapviewerControl::GetMapViewBoundingBox(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void MapviewerControl::SetMapViewSaveArea(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const ::DBus::Struct< double, double, double, double >& saveArea)
{
    // TODO
}

::DBus::Struct< double, double, double, double > MapviewerControl::GetMapViewSaveArea(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void MapviewerControl::SetMapViewPan(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& panningAction, const std::vector< ::DBus::Struct< uint16_t, uint16_t > >& pixelCoordinates)
{
    // TODO
}

std::vector< ::DBus::Struct< uint16_t, uint16_t > > MapviewerControl::GetMapViewPan(const uint32_t& mapViewInstanceHandle, const int32_t& valueToReturn)
{
    // TODO
}

void MapviewerControl::SetMapViewRotation(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& rotationAngle, const int32_t& rotationAnglePerSecond)
{
    // TODO
}

void MapviewerControl::GetMapViewRotation(const uint32_t& mapViewInstanceHandle, int32_t& rotationAngle, int32_t& rotationAnglePerFrame)
{
    // TODO
}

void MapviewerControl::SetMapViewVisibilityMode(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& visibilityMode)
{
    // TODO
}

int32_t MapviewerControl::GetMapViewVisibilityMode(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

std::vector< int32_t > MapviewerControl::GetSupportedMapViewVisibilityModes()
{
    // TODO
}

void MapviewerControl::SetMapViewPerformanceLevel(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& performanceLevel)
{
    // TODO
}

int32_t MapviewerControl::GetMapViewPerformanceLevel(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

std::vector< int32_t > MapviewerControl::GetSupportedMapViewPerformanceLevels()
{
    // TODO
}

void MapviewerControl::DisplayRoute(
    const uint32_t& sessionHandle,
    const uint32_t& mapViewInstanceHandle,
    const uint32_t& routeHandle,
    const bool& highlighted)
{
    // TODO
}

void MapviewerControl::HideRoute(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const uint32_t& routeHandle)
{
    // TODO
}

std::vector< ::DBus::Struct< uint32_t, bool > > MapviewerControl::GetDisplayedRoutes(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

std::vector< uint32_t > MapviewerControl::GetPoiCategoriesVisible(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

void MapviewerControl::SetPoiCategoriesVisible(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::vector< uint32_t >& poiCategoryIds)
{
    // TODO
}

void MapviewerControl::SetPoiCategoriesVisibleWithinLimits(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::vector< uint32_t >& poiCategoryIds, const uint8_t& minScaleID, const uint8_t& maxScaleID)
{
    // TODO
}

void MapviewerControl::SetPoiCategoriesNotVisible(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::vector< uint32_t >& poiCategoryIds)
{
    // TODO
}

void MapviewerControl::SetTrafficIncidentsVisibility(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const bool& visible)
{
    // TODO
}

void MapviewerControl::SetMapViewTheme(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& mapViewTheme)
{
    // TODO
}

int32_t MapviewerControl::GetMapViewTheme(const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

std::vector< int32_t > MapviewerControl::GetSupportedMapViewThemes()
{
    // TODO
}

std::vector< ::DBus::Struct< double, double > > MapviewerControl::ConvertPixelCoordsToGeoCoords(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::vector< ::DBus::Struct< uint16_t, uint16_t > >& pixelCoordinates)
{
    // TODO
}

std::vector< ::DBus::Struct< uint16_t, uint16_t > > MapviewerControl::ConvertGeoCoordsToPixelCoords(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::vector< ::DBus::Struct< double, double > >& geoCoordinates)
{
    // TODO
}

std::vector< uint32_t > MapviewerControl::DisplayCustomElements(
    const uint32_t& sessionHandle,
    const uint32_t& mapViewInstanceHandle,
    const std::vector< ::DBus::Struct< std::string, std::string, ::DBus::Struct< double, double >, ::DBus::Struct< int16_t, int16_t > > >& customElements)
{
    // TODO
}

void MapviewerControl::HideCustomElements(
    const uint32_t& sessionHandle,
    const uint32_t& mapViewInstanceHandle,
    const std::vector< uint32_t >& customElementHandles)
{
    // TODO
}

std::map< uint32_t, ::DBus::Struct< std::string, std::string, ::DBus::Struct< double, double >, ::DBus::Struct< int16_t, int16_t > > >
    MapviewerControl::GetDisplayedCustomElements(
        const uint32_t& mapViewInstanceHandle)
{
    // TODO
}

std::vector< ::DBus::Struct< int32_t, ::DBus::Struct< double, double >, ::DBus::Struct< uint8_t, ::DBus::Variant > > >
    MapviewerControl::SelectElementsOnMap(
        const uint32_t& mapViewInstanceHandle,
        const ::DBus::Struct< uint16_t, uint16_t >& pixelCoordinate,
        const std::vector< int32_t >& selectableTypes,
        const uint16_t& maxNumberOfSelectedElements)
{
    // TODO
}
