#include <stdio.h>
#include <vector>
#include <string>
#include <tuple>
#include "genivi-mapviewer.h"
#include "NaviTrace.h"
#include "navi_genivi.h"

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

using namespace std;

extern NaviContext g_ctx;

/* TODO: bring these functions here, or put them in the correct class: */
extern int map_init(GLVContext glv_ctx, int maps);
extern int map_reshape(GLVContext glv_ctx, int maps, int width, int height);
extern int map_redraw(GLVContext glv_ctx, int maps);
extern int map_timer(GLVContext glv_ctx, int maps, int group, int id);
extern int map_gesture(GLVContext glv_ctx,int maps,int eventType,int x,int y,int distanceX,int distanceY,int velocityX,int velocityY);
extern int hmi_init(GLVContext glv_ctx, int maps);
extern int hmi_update(GLVContext glv_ctx,int maps);


std::vector<MapviewerSession>::iterator DisplayContext::retrieveMapviewerSessionIt(const uint32_t& sessionHandle)
{
    std::vector<MapviewerSession>::iterator it;
    for (it = MapviewerSessionList.begin(); it != MapviewerSessionList.end(); it++)
    {
        if (it->handle == sessionHandle) return it;
    }
    return MapviewerSessionList.end();
}

std::vector<MapviewInstance>::iterator MapviewerSession::retrieveMapviewInstanceIt(const uint32_t& mapViewInstanceHandle)
{
    std::vector<MapviewInstance>::iterator it;
    for (it = views.begin(); it != views.end(); it++)
    {
        if (it->handle == mapViewInstanceHandle) return it;
    }
    return views.end();
}

Mapviewer::Mapviewer( DBus::Connection &connection )
    : DBus::ObjectAdaptor(connection, "/org/genivi/mapviewer"),
      lastSession(0)
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
    lastSession++;

    TRACE_INFO("SESSION ADAPTOR - Created session %d [%s]", lastSession, client.c_str());

    MapviewerSession s(lastSession, client);
    g_ctx.display.MapviewerSessionList.push_back(s);
    
    return s.handle;
}

void Mapviewer::DeleteSession(const uint32_t& sessionHandle)
{
    TRACE_INFO("SESSION ADAPTOR - Delete session %" PRIu32, sessionHandle);

    std::vector<struct MapviewerSession>::iterator it = g_ctx.display.retrieveMapviewerSessionIt(sessionHandle);
    if (it != g_ctx.display.MapviewerSessionList.end())
    {
        // TODO: actually delete mapviewerSession
        g_ctx.display.MapviewerSessionList.erase(it);
    }
    else
    {
        TRACE_ERROR("No MapviewerSession %" PRIu32, sessionHandle);
    }
}

int32_t Mapviewer::GetSessionStatus(const uint32_t& sessionHandle)
{
    int32_t ret = 1; // available
    TRACE_INFO("SESSION ADAPTOR - GetSessionStatus %" PRIu32, sessionHandle);

    std::vector<struct MapviewerSession>::iterator it = g_ctx.display.retrieveMapviewerSessionIt(sessionHandle);
    if (it != g_ctx.display.MapviewerSessionList.end())
    {
        TRACE_DEBUG("SESSION ADAPTOR - Session %" PRIu32 " is present in list (%s)", sessionHandle, it->client.c_str());
        ret = 0; // not available
    }

    TRACE_DEBUG("return %d", ret);
    return ret;
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

    std::vector<struct MapviewerSession>::iterator map = g_ctx.display.retrieveMapviewerSessionIt(sessionHandle);
    if (map != g_ctx.display.MapviewerSessionList.end())
    {
        // TODO: for now, we only support 1 view per session
        if (map->lastViewInstance != 0) return 0;
        
        map->lastViewInstance++;
        handle = map->lastViewInstance;
        MapviewInstance newInstance(map->lastViewInstance, map->client, mapViewType, mapViewSize._1, mapViewSize._2);

        // TODO: init
        NC_MP_SetMapMoveWithCar(map->handle, 1);
        NC_MP_SetMapScaleLevel(map->handle, g_ctx.main_window_mapScale); // TODO: see if we can move 'main_window_mapScale' to class MapviewerSession

        g_ctx.display.glv_map_window = glvCreateNativeWindow(g_ctx.display.glvDisplay, 0, 0,
            /*g_ctx.WinWidth, g_ctx.WinHeight*/ newInstance.w, newInstance.h, NULL); // TODO: see if we can move 'glv_map_window' to class MapviewerSession
        g_ctx.display.hmi.glv_hmi_window = glvCreateNativeWindow(g_ctx.display.glvDisplay, 0, 0,
            /*g_ctx.WinWidth, g_ctx.WinHeight*/ newInstance.w, newInstance.h, g_ctx.display.glv_map_window); // TODO: see if we can move 'glv_hmi_window' to class MapviewerSession

        glvInitTimer();

        g_ctx.display.SurfaceViewEventFunc.init		= map_init;
        g_ctx.display.SurfaceViewEventFunc.reshape	= map_reshape;
        g_ctx.display.SurfaceViewEventFunc.redraw	= map_redraw;
        g_ctx.display.SurfaceViewEventFunc.update	= NULL;
        g_ctx.display.SurfaceViewEventFunc.timer	= map_timer;
        g_ctx.display.SurfaceViewEventFunc.gesture	= map_gesture;

        g_ctx.display.map_context = glvCreateSurfaceView(
            g_ctx.display.glv_map_window, NC_MP_MAP_MAIN, &g_ctx.display.SurfaceViewEventFunc);

        g_ctx.display.hmi.hmi_SurfaceViewEventFunc.init		= hmi_init;
        g_ctx.display.hmi.hmi_SurfaceViewEventFunc.reshape	= NULL;
        g_ctx.display.hmi.hmi_SurfaceViewEventFunc.redraw	= NULL;
        g_ctx.display.hmi.hmi_SurfaceViewEventFunc.update	= hmi_update;
        g_ctx.display.hmi.hmi_SurfaceViewEventFunc.timer	= NULL;
        g_ctx.display.hmi.hmi_SurfaceViewEventFunc.gesture	= NULL;

        g_ctx.display.hmi.hmi_context = glvCreateSurfaceView(
            g_ctx.display.hmi.glv_hmi_window, NC_MP_MAP_MAIN, &g_ctx.display.hmi.hmi_SurfaceViewEventFunc);

        glvCreateTimer(g_ctx.display.map_context, 1000, GESTURE_FLICK_TIMER_ID, GLV_TIMER_REPEAT, 50);
        glvCreateTimer(g_ctx.display.map_context, 1000, GESTURE_LONG_PRESS_TIMER_ID, GLV_TIMER_ONLY_ONCE, 700);

        glvOnReDraw(g_ctx.display.map_context);

        pthread_create(&map->p, NULL, glvEventLoop_, g_ctx.display.glvDisplay);

        map->views.push_back(newInstance);
    }
    else
    {
        TRACE_ERROR("No MapviewerSession %" PRIu32, sessionHandle);
        return 0;
    }

    return handle;
}


void Mapviewer::ReleaseMapViewInstance(
    const uint32_t& sessionHandle,
    const uint32_t& mapViewInstanceHandle)
{
    TRACE_INFO("Release instance view %" PRIu32 " in session %" PRIu32, mapViewInstanceHandle, sessionHandle);

    std::vector<struct MapviewerSession>::iterator map = g_ctx.display.retrieveMapviewerSessionIt(sessionHandle);
    if (map != g_ctx.display.MapviewerSessionList.end())
    {
        //retrieveMapviewInstanceIt
        std::vector<struct MapviewInstance>::iterator itt = map->retrieveMapviewInstanceIt(mapViewInstanceHandle);
        if (itt != map->views.end())
        {
            // TODO: actually delete MapviewInstance
            if (g_ctx.display.glv_map_window)
            {
                glvDestroyNativeWindow(g_ctx.display.glv_map_window);
                g_ctx.display.glv_map_window = NULL;
            }
            if (g_ctx.display.hmi.glv_hmi_window)
            {
                glvDestroyNativeWindow(g_ctx.display.hmi.glv_hmi_window);
                g_ctx.display.hmi.glv_hmi_window = NULL;
            }
    
            map->views.erase(itt);
            TRACE_DEBUG("delete done.");
        }
        else
        {
            TRACE_ERROR("No MapviewInstance %" PRIu32 " in session %" PRIu32, mapViewInstanceHandle, sessionHandle);
        }

        // TODO: for now, we only support 1 view per session
        map->lastViewInstance = 0;
    }
    else
    {
        TRACE_ERROR("No MapviewerSession %" PRIu32, sessionHandle);
    }
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
    TRACE_INFO("view %" PRIu32 " in session %" PRIu32 ", followCarMode %d", mapViewInstanceHandle, sessionHandle, followCarMode);

    std::vector<struct MapviewerSession>::iterator map = g_ctx.display.retrieveMapviewerSessionIt(sessionHandle);
    if (map == g_ctx.display.MapviewerSessionList.end())
    {
        TRACE_ERROR("No MapviewerSession %" PRIu32, sessionHandle);
        return;
    }

    std::vector<struct MapviewInstance>::iterator it = map->retrieveMapviewInstanceIt(mapViewInstanceHandle);
    if (it == map->views.end())
    {
        TRACE_ERROR("No MapviewInstance %" PRIu32 "in session %" PRIu32, mapViewInstanceHandle, sessionHandle);
        return;
    }

    NC_MP_SetMapMoveWithCar(map->handle, followCarMode);
}

bool Mapviewer::GetFollowCarMode(const uint32_t& mapViewInstanceHandle)
{
    uint32_t sessionHandle = NC_MP_MAP_MAIN; // TODO: replace by the currently used map

    TRACE_INFO("view %" PRIu32, mapViewInstanceHandle);

    std::vector<struct MapviewerSession>::iterator map = g_ctx.display.retrieveMapviewerSessionIt(sessionHandle);
    if (map == g_ctx.display.MapviewerSessionList.end())
    {
        TRACE_ERROR("No MapviewerSession %" PRIu32, sessionHandle);
        return 0;
    }

    std::vector<struct MapviewInstance>::iterator it = map->retrieveMapviewInstanceIt(mapViewInstanceHandle);
    if (it == map->views.end())
    {
        TRACE_ERROR("No MapviewInstance %" PRIu32 "in session %" PRIu32, mapViewInstanceHandle, sessionHandle);
        return 0;
    }

    return NC_MP_GetMapMoveWithCar(map->handle);
}

void Mapviewer::SetCameraPosition(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const ::DBus::Struct< double, double, double >& position)
{
    // TODO: difficult. How to retrieve current camera position ?
    // Use NC_MP_MoveMapDir and NC_MP_ScreenToGeoCode
}

::DBus::Struct< double, double, double > Mapviewer::GetCameraPosition(const uint32_t& mapViewInstanceHandle)
{
    // TODO: difficult. How to retrieve current camera position ?
    // Use NC_MP_MoveMapDir and NC_MP_ScreenToGeoCode
}

void Mapviewer::SetCameraHeadingAngle(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int32_t& heading)
{
    TRACE_INFO("view %" PRIu32 " in session %" PRIu32 ", heading %" PRIu32, mapViewInstanceHandle, sessionHandle, heading);

    std::vector<struct MapviewerSession>::iterator map = g_ctx.display.retrieveMapviewerSessionIt(sessionHandle);
    if (map == g_ctx.display.MapviewerSessionList.end())
    {
        TRACE_ERROR("No MapviewerSession %" PRIu32, sessionHandle);
        return;
    }

    std::vector<struct MapviewInstance>::iterator it = map->retrieveMapviewInstanceIt(mapViewInstanceHandle);
    if (it == map->views.end())
    {
        TRACE_ERROR("No MapviewInstance %" PRIu32 "in session %" PRIu32, mapViewInstanceHandle, sessionHandle);
        return;
    }

    g_ctx.display.hmi.compass = 0;
    NC_MP_SetMapDispMode(map->handle, g_ctx.display.hmi.compass);
    NC_MP_SetMapRotate(map->handle, heading);

    glvOnReDraw(g_ctx.display.map_context);
}

void Mapviewer::SetCameraHeadingToTarget(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const ::DBus::Struct< double, double >& target)
{
    // TODO
}

void Mapviewer::SetCameraHeadingTrackUp(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle)
{
    TRACE_INFO("view %" PRIu32 " in session %" PRIu32, mapViewInstanceHandle, sessionHandle);

    std::vector<struct MapviewerSession>::iterator map = g_ctx.display.retrieveMapviewerSessionIt(sessionHandle);
    if (map == g_ctx.display.MapviewerSessionList.end())
    {
        TRACE_ERROR("No MapviewerSession %" PRIu32, sessionHandle);
        return;
    }

    std::vector<struct MapviewInstance>::iterator it = map->retrieveMapviewInstanceIt(mapViewInstanceHandle);
    if (it == map->views.end())
    {
        TRACE_ERROR("No MapviewInstance %" PRIu32 "in session %" PRIu32, mapViewInstanceHandle, sessionHandle);
        return;
    }

    g_ctx.display.hmi.compass = 1;
    NC_MP_SetMapDispMode(map->handle, g_ctx.display.hmi.compass);
    NC_MP_SetMapRotate(map->handle, 0);

    glvOnReDraw(g_ctx.display.map_context);
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

#define METER 0x0032 // TODO: why were the XML constants not generated ?
std::vector< ::DBus::Struct< uint16_t, uint16_t, int32_t, uint32_t > > Mapviewer::GetScaleList(const uint32_t& mapViewInstanceHandle)
{
    std::vector< ::DBus::Struct< uint16_t, uint16_t, int32_t, uint32_t > > ret;
    for (int i=0; i<=g_ctx.display.hmi.map_max_scale; i++)
    {
        ::DBus::Struct< uint16_t, uint16_t, int32_t, uint32_t > newVal;
        newVal._1 = newVal._2 = i;
        newVal._3 = METER; // TODO: dummy value
        newVal._4 = 1000;  // TODO: dummy value
        ret.push_back(newVal);
    }

    return ret;
}

void Mapviewer::SetMapViewScale(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const uint16_t& scaleID)
{
    TRACE_INFO("view %" PRIu32 " in session %" PRIu32 ", scaleID %" PRIu16, mapViewInstanceHandle, sessionHandle, scaleID);

    std::vector<struct MapviewerSession>::iterator map = g_ctx.display.retrieveMapviewerSessionIt(sessionHandle);
    if (map == g_ctx.display.MapviewerSessionList.end())
    {
        TRACE_ERROR("No MapviewerSession %" PRIu32, sessionHandle);
        return;
    }

    std::vector<struct MapviewInstance>::iterator it = map->retrieveMapviewInstanceIt(mapViewInstanceHandle);
    if (it == map->views.end())
    {
        TRACE_ERROR("No MapviewInstance %" PRIu32 "in session %" PRIu32, mapViewInstanceHandle, sessionHandle);
        return;
    }

    if (scaleID > g_ctx.display.hmi.map_max_scale)
        g_ctx.main_window_mapScale = g_ctx.display.hmi.map_max_scale;
    else if (scaleID < 0)
        g_ctx.main_window_mapScale = 0;
    else
        g_ctx.main_window_mapScale = scaleID;

    NC_MP_SetMapScaleLevel(map->handle, g_ctx.main_window_mapScale);
}

void Mapviewer::SetMapViewScaleByDelta(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const int16_t& scaleDelta)
{
    // TODO
}

void Mapviewer::SetMapViewScaleByMetersPerPixel(const uint32_t& sessionHandle, const uint32_t& mapViewInstanceHandle, const double& metersPerPixel)
{
    // TODO
}


#define MIN 0x0040 // TODO: why were the XML constants not generated ?
#define MID 0x0041 // TODO: why were the XML constants not generated ?
#define MAX 0x0042 // TODO: why were the XML constants not generated ?
void Mapviewer::GetMapViewScale(const uint32_t& mapViewInstanceHandle, uint8_t& scaleID, int32_t& isMinMax)
{
    uint32_t sessionHandle = NC_MP_MAP_MAIN; // TODO: replace by the currently used map

    TRACE_INFO("view %" PRIu32 "in currently used map", mapViewInstanceHandle);

    std::vector<struct MapviewerSession>::iterator map = g_ctx.display.retrieveMapviewerSessionIt(sessionHandle);
    if (map == g_ctx.display.MapviewerSessionList.end())
    {
        TRACE_ERROR("No MapviewerSession %" PRIu32, sessionHandle);
        return;
    }

    std::vector<struct MapviewInstance>::iterator it = map->retrieveMapviewInstanceIt(mapViewInstanceHandle);
    if (it == map->views.end())
    {
        TRACE_ERROR("No MapviewInstance %" PRIu32 "in session %" PRIu32, mapViewInstanceHandle, sessionHandle);
        return;
    }

    scaleID = NC_MP_GetMapScaleLevel(map->handle);

    if (g_ctx.main_window_mapScale != scaleID) TRACE_ERROR(" ");

    if (scaleID == 0) isMinMax = MIN;
    else if (scaleID == g_ctx.display.hmi.map_max_scale) isMinMax = MAX;
    else isMinMax = MID;
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
