#include <stdio.h>
#include <vector>
#include <string>
#include <tuple>
#include <pthread.h>
#include "genivi-mapviewer.h"
#include "genivi-mapviewer-constants.h"
#include "NaviTrace.h"

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

extern "C" { // CDR
    #include "glview.h"
    #include "navicore.h"
}

#include "button.h"
#include "navi.h"
#include "navi_genivi.h"

#include "MapviewInstance.hpp"
#include "Guide.hpp"

extern int main_window_mapScale;
extern GLVWindow glv_map_window;
extern GLVWindow glv_hmi_window;
extern GLVDisplay glvDisplay;
extern GLVEVENTFUNC_t SurfaceViewEventFunc;
extern GLVEVENTFUNC_t hmi_SurfaceViewEventFunc;
extern GLVContext glv_map_context;
extern GLVContext glv_hmi_context;
extern int hmi_compass;
extern int hmiMAP_MAX_SCALE;
extern int WinWidth;
extern int WinHeight;

extern int map_init(GLVContext glv_ctx,int maps);
extern int map_reshape(GLVContext glv_ctx,int maps,int width, int height);
extern int map_redraw(GLVContext glv_ctx,int maps);
extern int map_timer(GLVContext glv_ctx,int maps,int group,int id);
extern int map_gesture(GLVContext glv_ctx,int maps,int eventType,int x,int y,int distanceX,int distanceY,int velocityX,int velocityY);
extern int hmi_init(GLVContext glv_ctx,int maps);
extern int hmi_update(GLVContext glv_ctx,int maps);


Mapviewer::Mapviewer( DBus::Connection &connection )
    : DBus::ObjectAdaptor(connection, "/org/genivi/mapviewer"),
      lastSession(0),lastViewInstance(0),Mapview(NULL),client("")
{
}

::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > Mapviewer::SessionGetVersion()
{
    ::DBus::Struct<uint16_t, uint16_t, uint16_t, std::string> version;
    version._1 = 3;
    version._2 = 0;
    version._3 = 0;
    version._4 = std::string("21-01-2014");
    return version;
}

uint32_t Mapviewer::CreateSession(const std::string& client)
{
    if (lastSession != 0) // we only handle 1 session for now
    {
        TRACE_ERROR(" ");
        return 0;
    }

    lastSession++;
    this->client = client;
    
    TRACE_INFO("SESSION ADAPTOR - Created session %d [%s]", lastSession, client.c_str());
    return lastSession;
}

void Mapviewer::DeleteSession(const uint32_t& sessionHandle)
{
    if (sessionHandle != lastSession) // we only handle 1 session for now
    {
        TRACE_ERROR(" ");
        return;
    }

    lastSession = 0;
    TRACE_INFO("SESSION ADAPTOR - Deleted session %d\n", sessionHandle);
}

int32_t Mapviewer::GetSessionStatus(const uint32_t& sessionHandle)
{
    if (sessionHandle == 1 && lastSession != 1) // we only handle 1 session for now
        return 1; // available
    return 0; // not available
}

std::vector< ::DBus::Struct< uint32_t, std::string > > Mapviewer::GetAllSessions()
{
    std::vector< ::DBus::Struct< uint32_t, std::string > > list;
    ::DBus::Struct< uint32_t, std::string > a;
    a._1 = lastSession; a._2 = client;
    list.push_back( a );
    return list;
}

// Configuration interface
::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > Mapviewer::ConfigurationGetVersion()
{
    ::DBus::Struct<uint16_t, uint16_t, uint16_t, std::string> version;
    version._1 = 3;
    version._2 = 0;
    version._3 = 0;
    version._4 = std::string("21-01-2014");
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
    TRACE_WARN("TODO: implement this function");
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
    version._1 = 3;
    version._2 = 1;
    version._3 = 0;
    version._4 = std::string("03-03-2014");
    return version;
}

static void *glvEventLoop_(void *arg)
{
    glvEventLoop(arg);
    return NULL;
}

uint32_t Mapviewer::CreateMapViewInstance(
    const uint32_t& sessionHandle,
    const ::DBus::Struct< uint16_t, uint16_t >& mapViewSize,
    const int32_t& mapViewType)
{
    uint32_t handle;

    TRACE_INFO("MapViewInstance creation for MapViewerSession %" PRIu32, sessionHandle);
    TRACE_INFO("\tSize: %" PRIu16 "x%" PRIu16 ", type: %" PRIu32, mapViewSize._1, mapViewSize._2, mapViewType);

    if (sessionHandle == 0 || sessionHandle != lastSession
        || lastViewInstance != 0) // for now, we only support 1 view per session
    {
        TRACE_ERROR("Fail to create MapViewInstance for session %" PRIu32, sessionHandle);
        return 0;
    }

    if (mapViewSize._1 > WinWidth || mapViewSize._2 > WinHeight)
    {
        TRACE_ERROR("Incompatible map size (%" PRIu16 ":%" PRIu16 " vs %" PRIu16 ":%" PRIu16 ")",
            mapViewSize._1, mapViewSize._2, WinWidth, WinHeight);
        return 0;
    }
    
    lastViewInstance++;
    Mapview = new MapviewInstance(lastViewInstance, client, mapViewType, mapViewSize._1, mapViewSize._2);

    NC_MP_SetMapMoveWithCar(lastSession, 1);
    NC_MP_SetMapScaleLevel(lastSession, main_window_mapScale);

    glv_map_window = glvCreateNativeWindow(glvDisplay, 0, 0, Mapview->w, Mapview->h, NULL);
    glv_hmi_window = glvCreateNativeWindow(glvDisplay, 0, 0, Mapview->w, Mapview->h, glv_map_window);

    glvInitTimer();

    SurfaceViewEventFunc.init		= map_init;
    SurfaceViewEventFunc.reshape	= map_reshape;
    SurfaceViewEventFunc.redraw	    = map_redraw;
    SurfaceViewEventFunc.update	    = NULL;
    SurfaceViewEventFunc.timer	    = map_timer;
    SurfaceViewEventFunc.gesture	= map_gesture;

    glv_map_context = glvCreateSurfaceView(glv_map_window, lastSession, &SurfaceViewEventFunc);

    hmi_SurfaceViewEventFunc.init		= hmi_init;
    hmi_SurfaceViewEventFunc.reshape	= NULL;
    hmi_SurfaceViewEventFunc.redraw	    = NULL;
    hmi_SurfaceViewEventFunc.update	    = hmi_update;
    hmi_SurfaceViewEventFunc.timer	    = NULL;
    hmi_SurfaceViewEventFunc.gesture	= NULL;

    glv_hmi_context = glvCreateSurfaceView(glv_hmi_window, lastSession, &hmi_SurfaceViewEventFunc);

    glvCreateTimer(glv_map_context, 1000, GESTURE_FLICK_TIMER_ID, GLV_TIMER_REPEAT, 50);
    glvCreateTimer(glv_map_context, 1000, GESTURE_LONG_PRESS_TIMER_ID, GLV_TIMER_ONLY_ONCE, 700);

    sample_createGuideThread();

    glvOnReDraw(glv_map_context);

    if (pthread_create(&p, NULL, glvEventLoop_, glvDisplay) != 0)
        TRACE_ERROR("pthread_create failed");

    return lastViewInstance;
}


void Mapviewer::ReleaseMapViewInstance(
    const uint32_t& sessionHandle,
    const uint32_t& mapViewInstanceHandle)
{
    TRACE_INFO("Release instance view %" PRIu32 " in session %" PRIu32, mapViewInstanceHandle, sessionHandle);

    if (sessionHandle != 1 || mapViewInstanceHandle != 1 || lastViewInstance != 1) // we only support 1 view instance for now
    {
        TRACE_ERROR(" ");
        return;
    }

    /* First, destroy Glv thread: */
    TRACE_DEBUG("destroy Glv thread:");
    void *res;
    if (pthread_cancel(p) != 0)
        TRACE_ERROR("Fail to cancel thread");
    else if (pthread_join(p, &res) != 0)
        TRACE_ERROR("Fail to join canceled thread");
    else if (res == PTHREAD_CANCELED)
        TRACE_INFO("thread was successfully canceled");
    else
        TRACE_ERROR("thread was not properly canceled (%p)", res);

    /* Destroy Guide thread: */
    TRACE_DEBUG("destroy Guide thread:");
    sample_destroyGuideThread();

    if (glv_map_window)
    {
        TRACE_DEBUG("destroy map window:");
        glvDestroyNativeWindow(glv_map_window);
        glv_map_window = NULL;
    }
    if (glv_hmi_window)
    {
        TRACE_DEBUG("destroy hmi window:");
        glvDestroyNativeWindow(glv_hmi_window);
        glv_hmi_window = NULL;
    }

    lastViewInstance = 0; // we only handle one instance for now
}

int32_t Mapviewer::GetMapViewType(
    const uint32_t& mapViewInstanceHandle)
{
    TRACE_WARN("TODO: implement this function");
}

std::vector< int32_t > Mapviewer::GetSupportedMapViewTypes()
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::SetTargetPoint(
    const uint32_t& sessionHandle,
    const uint32_t& mapViewInstanceHandle,
    const ::DBus::Struct< double, double, double >& targetPoint)
{
    TRACE_WARN("TODO: implement this function");
}

::DBus::Struct< double, double, double > Mapviewer::GetTargetPoint(
    const uint32_t& mapViewInstanceHandle)
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::SetFollowCarMode(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const bool& followCarMode)
{
    TRACE_INFO("view %" PRIu32 " in session %" PRIu32 ", followCarMode %d", mapViewInstanceHandle, sessionHandle, followCarMode);

    if (sessionHandle != 1 || mapViewInstanceHandle != 1
        || lastSession != 1) return;

    NC_MP_SetMapMoveWithCar(lastSession, followCarMode);
}

bool Mapviewer::GetFollowCarMode(const uint32_t& mapViewInstanceHandle)
{
    TRACE_INFO("view %" PRIu32, mapViewInstanceHandle);

    if (mapViewInstanceHandle != 1 || lastSession != 1)
    {
        TRACE_ERROR(" ");
        return false;
    }

    return NC_MP_GetMapMoveWithCar(lastSession);
}

void Mapviewer::SetCameraPosition(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const ::DBus::Struct< double, double, double >& position)
{
    TRACE_WARN("TODO: implement this function");
}

::DBus::Struct< double, double, double > Mapviewer::GetCameraPosition(const uint32_t& mapViewInstanceHandle)
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::SetCameraHeadingAngle(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& heading)
{
    TRACE_INFO("view %" PRIu32 " in session %" PRIu32 ", heading %" PRIu32, mapViewInstanceHandle, sessionHandle, heading);

    if (sessionHandle != 1 || mapViewInstanceHandle != 1
        || lastSession != 1) return;

    hmi_compass = 1;
    TRACE_DEBUG("NC_MP_SetMapDispMode %" PRIu32 ", %d", lastSession, hmi_compass);
    NC_MP_SetMapDispMode(lastSession, hmi_compass);

    /* WARNING: this API exists but is not effective */
    TRACE_DEBUG("NC_MP_SetMapRotate %" PRIu32 "%" PRId32, lastSession, heading);
    NC_MP_SetMapRotate(lastSession, heading); 
    
    TRACE_DEBUG("glvOnReDraw");
    glvOnReDraw(glv_map_context);
}

void Mapviewer::SetCameraHeadingToTarget(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const ::DBus::Struct< double, double >& target)
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::SetCameraHeadingTrackUp(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle)
{
    TRACE_INFO("view %" PRIu32 " in session %" PRIu32, mapViewInstanceHandle, sessionHandle);

    if (sessionHandle != 1 || mapViewInstanceHandle != 1
        || lastSession != 1) return;

    hmi_compass = 0;
    NC_MP_SetMapDispMode(lastSession, hmi_compass);
    NC_MP_SetMapRotate(lastSession, 0);

    glvOnReDraw(glv_map_context);
}

void Mapviewer::GetCameraHeading(const uint32_t& mapViewInstanceHandle, int32_t& headingType, int32_t& headingAngle, ::DBus::Struct< double, double >& target)
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::SetCameraTiltAngle(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& tilt)
{
    TRACE_WARN("TODO: implement this function");
}

int32_t Mapviewer::GetCameraTiltAngle(const uint32_t& mapViewInstanceHandle)
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::SetCameraRollAngle(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& roll)
{
    TRACE_WARN("TODO: implement this function");
}

int32_t Mapviewer::GetCameraRollAngle(const uint32_t& mapViewInstanceHandle)
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::SetCameraDistanceFromTargetPoint(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const uint32_t& distance)
{
    TRACE_WARN("TODO: implement this function");
}

uint32_t Mapviewer::GetCameraDistanceFromTargetPoint(const uint32_t& mapViewInstanceHandle)
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::SetMapViewScaleMode(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& scaleMode)
{
    TRACE_WARN("TODO: implement this function");
}

int32_t Mapviewer::GetMapViewScaleMode(const uint32_t& mapViewInstanceHandle)
{
    TRACE_WARN("TODO: implement this function");
}

std::vector< int32_t > Mapviewer::GetSupportedMapViewScaleModes(const uint32_t& mapViewInstanceHandle)
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::AddMapViewScaleChangedListener()
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::RemoveMapViewScaleChangedListener()
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::SetCameraHeight(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const uint32_t& height)
{
    TRACE_WARN("TODO: implement this function");
}

uint32_t Mapviewer::GetCameraHeight(const uint32_t& mapViewInstanceHandle)
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::SetMapViewPerspective(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& perspective)
{
    TRACE_WARN("TODO: implement this function");
}

int32_t Mapviewer::GetMapViewPerspective(const uint32_t& mapViewInstanceHandle)
{
    TRACE_INFO("mapViewInstanceHandle: %" PRIu32, mapViewInstanceHandle);
    return MAPVIEWER_2D;
}

std::vector< int32_t > Mapviewer::GetSupportedMapViewPerspectives()
{
    TRACE_INFO(" ");
    std::vector<int32_t> ret;
    ret.push_back(MAPVIEWER_2D);
    return ret;
}

void Mapviewer::SetMapViewObjectVisibility(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::map< int32_t, bool >& objectVisibilityList)
{
    TRACE_WARN("TODO: implement this function");
}

std::map< int32_t, bool > Mapviewer::GetMapViewObjectVisibility(const uint32_t& mapViewInstanceHandle)
{
    TRACE_WARN("TODO: implement this function");
}

std::vector< int32_t > Mapviewer::GetSupportedMapViewObjectVisibilities(const uint32_t& mapViewInstanceHandle)
{
    TRACE_WARN("TODO: implement this function");
}

std::vector< ::DBus::Struct< uint16_t, uint16_t, int32_t, uint32_t > > Mapviewer::GetScaleList(const uint32_t& mapViewInstanceHandle)
{
    std::vector< ::DBus::Struct< uint16_t, uint16_t, int32_t, uint32_t > > ret;
    TRACE_DEBUG("hmiMAP_MAX_SCALE = %d", (int)hmiMAP_MAX_SCALE);
    for (int i=0; i<=hmiMAP_MAX_SCALE; i++)
    {
        ::DBus::Struct< uint16_t, uint16_t, int32_t, uint32_t > newVal;
        newVal._1 = newVal._2 = i;
        newVal._3 = MAPVIEWER_METER; // TODO: dummy value
        newVal._4 = 1000;  // TODO: dummy value
        TRACE_DEBUG("pushing %d", i);
        ret.push_back(newVal);
    }

    return ret;
}

void Mapviewer::SetMapViewScale(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const uint16_t& scaleID)
{
    TRACE_INFO("view %" PRIu32 " in session %" PRIu32 ", scaleID %" PRIu16, mapViewInstanceHandle, sessionHandle, scaleID);

    if (sessionHandle != 1 || mapViewInstanceHandle != 1
        || lastSession != 1) return;

    if (scaleID > hmiMAP_MAX_SCALE)
        main_window_mapScale = hmiMAP_MAX_SCALE;
    else if (scaleID < 0)
        main_window_mapScale = 0;
    else
        main_window_mapScale = scaleID;

    NC_MP_SetMapScaleLevel(lastSession, main_window_mapScale);
    glvOnReDraw(glv_map_context);
}

void Mapviewer::SetMapViewScaleByDelta(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int16_t& scaleDelta)
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::SetMapViewScaleByMetersPerPixel(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const double& metersPerPixel)
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::GetMapViewScale(const uint32_t& mapViewInstanceHandle, uint8_t& scaleID, int32_t& isMinMax)
{
    TRACE_INFO("view %" PRIu32 "in currently used map", mapViewInstanceHandle);

    if (mapViewInstanceHandle != 1 || lastSession != 1) return;

    scaleID = NC_MP_GetMapScaleLevel(lastSession);

    if (main_window_mapScale != scaleID) TRACE_ERROR(" ");

    if (scaleID == 0) isMinMax = MAPVIEWER_MIN;
    else if (scaleID == hmiMAP_MAX_SCALE) isMinMax = MAPVIEWER_MAX;
    else isMinMax = MAPVIEWER_MID;
}

void Mapviewer::SetMapViewBoundingBox(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const ::DBus::Struct< ::DBus::Struct< double, double >, ::DBus::Struct< double, double > >& boundingBox)
{
    TRACE_WARN("TODO: implement this function");
}

::DBus::Struct< ::DBus::Struct< double, double >, ::DBus::Struct< double, double > > Mapviewer::GetMapViewBoundingBox(const uint32_t& mapViewInstanceHandle)
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::SetMapViewSaveArea(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const ::DBus::Struct< double, double, double, double >& saveArea)
{
    TRACE_WARN("TODO: implement this function");
}

::DBus::Struct< double, double, double, double > Mapviewer::GetMapViewSaveArea(const uint32_t& mapViewInstanceHandle)
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::SetMapViewPan(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& panningAction, const std::vector< ::DBus::Struct< uint16_t, uint16_t > >& pixelCoordinates)
{
    TRACE_WARN("TODO: implement this function");
}

std::vector< ::DBus::Struct< uint16_t, uint16_t > > Mapviewer::GetMapViewPan(const uint32_t& mapViewInstanceHandle, const int32_t& valueToReturn)
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::SetMapViewRotation(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& rotationAngle, const int32_t& rotationAnglePerSecond)
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::GetMapViewRotation(const uint32_t& mapViewInstanceHandle, int32_t& rotationAngle, int32_t& rotationAnglePerFrame)
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::SetMapViewVisibilityMode(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& visibilityMode)
{
    TRACE_WARN("TODO: implement this function");
}

int32_t Mapviewer::GetMapViewVisibilityMode(const uint32_t& mapViewInstanceHandle)
{
    TRACE_WARN("TODO: implement this function");
}

std::vector< int32_t > Mapviewer::GetSupportedMapViewVisibilityModes()
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::SetMapViewPerformanceLevel(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& performanceLevel)
{
    TRACE_WARN("TODO: implement this function");
}

int32_t Mapviewer::GetMapViewPerformanceLevel(const uint32_t& mapViewInstanceHandle)
{
    TRACE_WARN("TODO: implement this function");
}

std::vector< int32_t > Mapviewer::GetSupportedMapViewPerformanceLevels()
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::DisplayRoute(
    const uint32_t& sessionHandle,
    const uint32_t& mapViewInstanceHandle,
    const uint32_t& routeHandle,
    const bool& highlighted)
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::HideRoute(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const uint32_t& routeHandle)
{
    TRACE_WARN("TODO: implement this function");
}

std::vector< ::DBus::Struct< uint32_t, bool > > Mapviewer::GetDisplayedRoutes(const uint32_t& mapViewInstanceHandle)
{
    TRACE_WARN("TODO: implement this function");
}

std::vector< uint32_t > Mapviewer::GetPoiCategoriesVisible(const uint32_t& mapViewInstanceHandle)
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::SetPoiCategoriesVisible(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::vector< uint32_t >& poiCategoryIds)
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::SetPoiCategoriesVisibleWithinLimits(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::vector< uint32_t >& poiCategoryIds, const uint8_t& minScaleID, const uint8_t& maxScaleID)
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::SetPoiCategoriesNotVisible(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::vector< uint32_t >& poiCategoryIds)
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::SetTrafficIncidentsVisibility(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const bool& visible)
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::SetMapViewTheme(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& mapViewTheme)
{
    TRACE_WARN("TODO: implement this function");
}

int32_t Mapviewer::GetMapViewTheme(const uint32_t& mapViewInstanceHandle)
{
    TRACE_WARN("TODO: implement this function");
}

std::vector< int32_t > Mapviewer::GetSupportedMapViewThemes()
{
    TRACE_WARN("TODO: implement this function");
}

std::vector< ::DBus::Struct< double, double > > Mapviewer::ConvertPixelCoordsToGeoCoords(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::vector< ::DBus::Struct< uint16_t, uint16_t > >& pixelCoordinates)
{
    TRACE_WARN("TODO: implement this function");
}

std::vector< ::DBus::Struct< uint16_t, uint16_t > > Mapviewer::ConvertGeoCoordsToPixelCoords(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const std::vector< ::DBus::Struct< double, double > >& geoCoordinates)
{
    TRACE_WARN("TODO: implement this function");
}

std::vector< uint32_t > Mapviewer::DisplayCustomElements(
    const uint32_t& sessionHandle,
    const uint32_t& mapViewInstanceHandle,
    const std::vector< ::DBus::Struct< std::string, std::string, ::DBus::Struct< double, double >, ::DBus::Struct< int16_t, int16_t > > >& customElements)
{
    TRACE_WARN("TODO: implement this function");
}

void Mapviewer::HideCustomElements(
    const uint32_t& sessionHandle,
    const uint32_t& mapViewInstanceHandle,
    const std::vector< uint32_t >& customElementHandles)
{
    TRACE_WARN("TODO: implement this function");
}

std::map< uint32_t, ::DBus::Struct< std::string, std::string, ::DBus::Struct< double, double >, ::DBus::Struct< int16_t, int16_t > > >
    Mapviewer::GetDisplayedCustomElements(
        const uint32_t& mapViewInstanceHandle)
{
    TRACE_WARN("TODO: implement this function");
}

std::vector< ::DBus::Struct< int32_t, ::DBus::Struct< double, double >, ::DBus::Struct< uint8_t, ::DBus::Variant > > >
    Mapviewer::SelectElementsOnMap(
        const uint32_t& mapViewInstanceHandle,
        const ::DBus::Struct< uint16_t, uint16_t >& pixelCoordinate,
        const std::vector< int32_t >& selectableTypes,
        const uint16_t& maxNumberOfSelectedElements)
{
    TRACE_WARN("TODO: implement this function");
}
