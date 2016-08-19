#include <stdio.h>
#include <vector>
#include <string>
#include <tuple>
#include "genivi-navicore.h"
#include "genivi-navicore-constants.h"
#include "NaviTrace.h"

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

extern "C" { // CDR
    #include "glview.h"
    #include "navicore.h"
    //#include "navi.h"
}

#include "navi.h"
#include "navi_genivi.h"
#include "Route.hpp"

extern Navicore  *naviCore;
extern int resolution;
extern int region;
extern char *dpyName;
extern int WinWidth;
extern int WinHeight;
extern char navi_config_path[];
extern char navi_config_map_db_path[];
extern char navi_config_user_data_path[];
extern char navi_config_map_udi_data_path[];
extern char navi_config_map_udi_info_file[];
extern char navi_config_map_font_file[];
extern SMMAPDYNUDI demo_icon_info[];
extern unsigned char demo_disp_info[];
extern GLVDisplay glvDisplay;


Navicore::Navicore( DBus::Connection &connection )
    : DBus::ObjectAdaptor(connection, "/org/genivi/navicore"),
      lastSession(0),lastRoute(0),client(""),IsSimulationMode(false),
      SimulationStatus(SIMULATION_STATUS_NO_SIMULATION)
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

uint32_t Navicore::CreateSession_internal(void)
{
	int rc;

	naviStartUpResolution(resolution); 
	naviStartUpRegion(region);

	glvDisplay = glvOpenDisplay(dpyName);
	if(!glvDisplay){
		TRACE_ERROR("glvOpenDisplay() failed");
		return -1;
	}

	NC_MP_SetBitmapFontCB(BitmapFontCallBack);
	NC_MP_SetImageReadForFileCB(ReadImageForFileCallBack);
	NC_MP_SetImageReadForImageCB(SetImageForMemoryCallBack);
	NC_MP_SetMapDrawEndCB(MapDrawEndCallBack);

	rc = NC_Initialize(WinWidth, WinHeight,
            navi_config_user_data_path, navi_config_map_db_path, "locatorPath");
	if(NC_SUCCESS != rc){
		TRACE_ERROR("NC_Initialize error");
		TRACE_ERROR("\ttuser data path: %s", navi_config_user_data_path);
		TRACE_ERROR("\tmap db path: %s", navi_config_map_db_path);
		return -1;
	}

	//NC_MP_SetMapMoveWithCar(NC_MP_MAP_MAIN,1);                          // CDR - mapviewer (3) - MapContext (vect)
	//NC_MP_SetMapScaleLevel(NC_MP_MAP_MAIN, g_ctx.main_window_mapScale); // CDR - mapviewer (3) - MapContext (vect)

	glv_input_func.keyboard_key = sample_hmi_keyboard_handle_key;       // CDR - navicore (1) - NaviContext
	glv_input_func.touch_down   = sample_hmi_button_down;               // CDR - navicore (1) - NaviContext
	glv_input_func.touch_up     = sample_hmi_button_up;                 // CDR - navicore (1) - NaviContext

	//glv_map_window = glvCreateNativeWindow(ctx->display.glvDisplay, 0, 0, g_ctx.WinWidth, g_ctx.WinHeight, NULL);           // CDR - mapviewinstance (4)
	//glv_hmi_window = glvCreateNativeWindow(ctx->display.glvDisplay, 0, 0, g_ctx.WinWidth, g_ctx.WinHeight, glv_map_window); // CDR - mapviewinstance (4)

	//glvInitTimer(); // CDR - mapviewinstance (4)

	/*g_ctx.display.SurfaceViewEventFunc.init		= map_init;     // CDR - mapviewInstance (4) - MapContext (vect)
	g_ctx.display.SurfaceViewEventFunc.reshape	= map_reshape;
	g_ctx.display.SurfaceViewEventFunc.redraw	= map_redraw;
	g_ctx.display.SurfaceViewEventFunc.update	= NULL;
	g_ctx.display.SurfaceViewEventFunc.timer	= map_timer;
	g_ctx.display.SurfaceViewEventFunc.gesture	= map_gesture;

	g_ctx.display.map_context = glvCreateSurfaceView(glv_map_window, NC_MP_MAP_MAIN, &g_ctx.display.SurfaceViewEventFunc);  // CDR - mapviewInstance (4)

	g_ctx.display.hmi.hmi_SurfaceViewEventFunc.init		= hmi_init; // CDR - mapviewInstance (4)
	g_ctx.display.hmi.hmi_SurfaceViewEventFunc.reshape	= NULL;
	g_ctx.display.hmi.hmi_SurfaceViewEventFunc.redraw	= NULL;
	g_ctx.display.hmi.hmi_SurfaceViewEventFunc.update	= hmi_update;
	g_ctx.display.hmi.hmi_SurfaceViewEventFunc.timer	= NULL;
	g_ctx.display.hmi.hmi_SurfaceViewEventFunc.gesture	= NULL;

	g_ctx.display.hmi.hmi_context = glvCreateSurfaceView(glv_hmi_window, NC_MP_MAP_MAIN, &g_ctx.display.hmi.hmi_SurfaceViewEventFunc);  // CDR - mapviewInstance (4)

	glvCreateTimer(g_ctx.display.map_context, 1000, GESTURE_FLICK_TIMER_ID, GLV_TIMER_REPEAT, 50);          // CDR - mapviewInstance (4)
	glvCreateTimer(g_ctx.display.map_context, 1000, GESTURE_LONG_PRESS_TIMER_ID, GLV_TIMER_ONLY_ONCE, 700); // CDR - mapviewInstance (4)
*/
    return 0;
}

uint32_t Navicore::CreateSession(const std::string& client)
{
    if (lastSession != 0) // we only handle 1 session for now
    {
        TRACE_ERROR(" ");
        return 0;
    }

    if (CreateSession_internal(/*&g_ctx*/) < 0) // error
    {
        TRACE_ERROR("fail to create session (%s)", client.c_str());
        return 0;
    }

    lastSession++;
    this->client = client;
    
    TRACE_INFO("SESSION ADAPTOR - Created session %d [%s]", lastSession, client.c_str());
    return lastSession;
}

void Navicore::DeleteSession(const uint32_t& sessionHandle)
{
    if (sessionHandle != lastSession) // we only handle 1 session for now
    {
        TRACE_ERROR(" ");
        return;
    }

    //TODO: delete Mapviewer if it's not already done
    /*if (g_ctx.display.glv_map_window)
    {
        glvDestroyNativeWindow(g_ctx.display.glv_map_window);
        g_ctx.display.glv_map_window = NULL;
    }
    if (g_ctx.display.hmi.glv_hmi_window)
    {
        glvDestroyNativeWindow(g_ctx.display.hmi.glv_hmi_window);
        g_ctx.display.hmi.glv_hmi_window = NULL;
    }*/
    TRACE_DEBUG("calling NC_Finalize()");
    NC_Finalize();
    glvCloseDisplay(glvDisplay);
    glvDisplay = NULL;

    lastSession = 0;
    TRACE_INFO("SESSION ADAPTOR - Deleted session %d\n", sessionHandle);
}

int32_t Navicore::GetSessionStatus(const uint32_t& sessionHandle)
{
    if (sessionHandle == 1 && !NC_IsInitialized()) // we only handle 1 session for now
        return 1; // available
    return 0; // not available
}

std::vector< ::DBus::Struct< uint32_t, std::string > > Navicore::GetAllSessions()
{
    std::vector< ::DBus::Struct< uint32_t, std::string > > list;
    ::DBus::Struct< uint32_t, std::string > a;
    a._1 = lastSession; a._2 = client;
    list.push_back(a);
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

std::vector<Route>::iterator Navicore::retrieveRouteIt(const uint32_t& routeHandle)
{
    std::vector<Route>::iterator it;
    for (it = Routes.begin(); it != Routes.end(); it++)
    {
        if (it->handle == routeHandle) return it;
    }
    return Routes.end();
}

uint32_t Navicore::CreateRoute(const uint32_t& sessionHandle)
{
    if (lastRoute != 0) return 0; // we only manage 1 route for now

    lastRoute++;
    TRACE_INFO("Create route %" PRIu32 " for session %" PRIu32, lastRoute, sessionHandle);
    Route newRoute(lastRoute);
    Routes.push_back(newRoute);
    return newRoute.handle;
}

void Navicore::DeleteRoute(const uint32_t& sessionHandle, const uint32_t& routeHandle)
{
    TRACE_INFO("Delete route %" PRIu32 " for session %" PRIu32, routeHandle, sessionHandle);

    std::vector<Route>::iterator it = retrieveRouteIt(routeHandle);
    
    if (it != Routes.end())
    {
        // TODO: actually delete route
        Routes.erase(it);
        //NC_RP_DeleteRouteResult();
        // TODO: stop guidance if any
        lastRoute = 0; // we only manage 1 route for now
    }
    else
    {
        TRACE_ERROR("No route %" PRIu32 " for session %" PRIu32, routeHandle, sessionHandle);
    }
}

void Navicore::SetCostModel(const uint32_t& sessionHandle, const uint32_t& routeHandle, const int32_t& costModel)
{
    TRACE_WARN("TODO: implement this function");
}

int32_t Navicore::GetCostModel(const uint32_t& routeHandle)
{
    TRACE_WARN("TODO: implement this function");
}

std::vector< int32_t > Navicore::GetSupportedCostModels()
{
    TRACE_WARN("TODO: implement this function");
}

void Navicore::SetRoutePreferences(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle,
    const std::string& countryCode,
    const std::vector< ::DBus::Struct< int32_t, int32_t > >& roadPreferenceList,
    const std::vector< ::DBus::Struct< int32_t, int32_t > >& conditionPreferenceList)
{
    TRACE_WARN("TODO: implement this function");
}
void Navicore::GetRoutePreferences(
    const uint32_t& routeHandle,
    const std::string& countryCode,
    std::vector< ::DBus::Struct< int32_t, int32_t > >& roadPreferenceList,
    std::vector< ::DBus::Struct< int32_t, int32_t > >& conditionPreferenceList)
{
    TRACE_WARN("TODO: implement this function");
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
    TRACE_WARN("TODO: implement this function");
}

std::map< int32_t, uint32_t > Navicore::GetRouteSchedule(
    const uint32_t& routeHandle,
    const std::vector< int32_t >& valuesToReturn)
{
    TRACE_WARN("TODO: implement this function");
}

void Navicore::SetTransportationMeans(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle,
    const std::vector< int32_t >& transportationMeansList)
{
    TRACE_WARN("TODO: implement this function");
}

std::vector< int32_t > Navicore::GetTransportationMeans(
    const uint32_t& routeHandle)
{
    TRACE_WARN("TODO: implement this function");
}

std::vector< int32_t > Navicore::GetSupportedTransportationMeans()
{
    TRACE_WARN("TODO: implement this function");
}

void Navicore::SetExcludedAreas(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle,
    const std::vector< std::vector< ::DBus::Struct< double, double > > >& excludedAreas)
{
    TRACE_WARN("TODO: implement this function");
}

std::vector< std::vector< ::DBus::Struct< double, double > > > Navicore::GetExcludedAreas(
    const uint32_t& routeHandle)
{
    TRACE_WARN("TODO: implement this function");
}

void Navicore::SetIconVisibility(IconIndex index, bool visible,
    double lat, double lon, bool commit)
{
    /* start flag, dest flag and pin flag: */
    static const int icon_id[] =
        { IconNum::FLAG_START_NUM, IconNum::FLAG_DEST_NUM, IconNum::FLAG_PIN_NUM };

    demo_icon_info[index].IconID = icon_id[index];
    demo_icon_info[index].Latitude		= (INT32)(lat*1024.0*3600.0);
    demo_icon_info[index].Longititude	= (INT32)(lon*1024.0*3600.0);
    demo_disp_info[index] = visible ? 1 : 0;

    if (commit)
    {
        NC_DM_SetIconInfo(demo_icon_info, 3);
        NC_DM_SetDynamicUDIDisplay(demo_disp_info, 3);
    }
}

void Navicore::SetWaypoints(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle,
    const bool& startFromCurrentPosition,
    const std::vector< std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > >& waypointsList)
{
    double newLat, newLon;
    SMRPPOINT *NcPointsTab;
    size_t size;
    int index = 0;

    TRACE_INFO("route %" PRIu32 " for session %" PRIu32, routeHandle, sessionHandle);

    if (sessionHandle != lastSession) return; // we only handle 1 session for now

    std::vector<struct Route>::iterator route = retrieveRouteIt(routeHandle);
    if (route == Routes.end())
    {
        TRACE_ERROR("No Route %" PRIu32 "in session 1", routeHandle);
        return ;
    }

    /* mask Pin flag: */
    SetIconVisibility(FLAG_PIN_IDX, false);

    size = waypointsList.size();
    if (startFromCurrentPosition) size++;
    NcPointsTab = (SMRPPOINT*) malloc(size*sizeof(SMRPPOINT));
    TRACE_DEBUG("alloc array of size %zu (startFromCurrentPosition = %d)", size, startFromCurrentPosition);

    if (startFromCurrentPosition)
    {
        SMCARSTATE carState;
        
        if (NC_DM_GetCarState(&carState, e_SC_CARLOCATION_NOW) == NC_SUCCESS)
        {
            /* Add waypoint in the Genivi table: */
            newLat = carState.coord.latitude/1024.0/3600.0;
            newLon = carState.coord.longitude/1024.0/3600.0;
            TRACE_DEBUG("Add waypoint in the Genivi table: %f, %f", newLat, newLon);
            WayPoint newWayPoint(newLat, newLon);
            route->WayPoints.push_back(newWayPoint);
            TRACE_DEBUG("\tadded: %f, %f", newWayPoint.lat, newWayPoint.lon);
            /* --- */

            /* Add waypoint in the NC table: */
            TRACE_DEBUG("Add waypoint in the NC table");
            NcPointsTab[index].coord.latitude = carState.coord.latitude;
            NcPointsTab[index].coord.longitude = carState.coord.longitude;
            NcPointsTab[index].rpPointType = LST_START;
            NcPointsTab[index].rpPointIndex = index;
            index++;
            /* --- */

            /* Display Start icon: */
            /*demo_icon_info[START_FLAG].IconID = 21;	// start flag
            demo_icon_info[START_FLAG].Longititude	= (INT32)carState.coord.longitude;
            demo_icon_info[START_FLAG].Latitude		= (INT32)carState.coord.latitude;
            demo_disp_info[START_FLAG] = 1;*/
            SetIconVisibility(FLAG_START_IDX, true, newLat, newLon);
            /* --- */
        }
        else
        {
            TRACE_ERROR("unable to get current car position");
        }
    }

    std::vector< std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > >::const_iterator wp_map;
    for (wp_map = waypointsList.begin(); wp_map != waypointsList.end(); wp_map++)
    {
        std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > >::const_iterator map;
        for (map = (*wp_map).begin(); map != (*wp_map).end(); map++)
        {
            if ((*map).first == NAVICORE_LATITUDE)
                newLat = (*map).second._2.reader().get_double();
            else if ((*map).first == NAVICORE_LONGITUDE)
                newLon = (*map).second._2.reader().get_double();
        }

        /* Add waypoint in the Genivi table: */
        TRACE_DEBUG("Add waypoint in the Genivi table: %f, %f", newLat, newLon);
        WayPoint newWayPoint(newLat, newLon);
        route->WayPoints.push_back(newWayPoint);
        TRACE_DEBUG("\tadded: %f, %f", newWayPoint.lat, newWayPoint.lon);
        /* --- */

        /* Add waypoint in the NC table: */
        TRACE_DEBUG("Add waypoint in the NC table");
        NcPointsTab[index].coord.latitude = newLat*1024.0*3600.0;
        NcPointsTab[index].coord.longitude = newLon*1024.0*3600.0;
        if (index == 0)
        {
            NcPointsTab[index].rpPointType = LST_START;
            // Display Start icon:
            /*demo_icon_info[START_FLAG].IconID = 21;	// start flag
            demo_icon_info[START_FLAG].Latitude		= (INT32)(newLat*1024.0*3600.0);
            demo_icon_info[START_FLAG].Longititude	= (INT32)(newLon*1024.0*3600.0);
            demo_disp_info[START_FLAG] = 1;*/
            SetIconVisibility(FLAG_START_IDX, true, newLat, newLon);
        }
        else if (index == (size-1))
        {
            NcPointsTab[index].rpPointType = LST_DEST;
            // Display Dest icon and update display:
            /*demo_icon_info[DEST_FLAG].IconID = 22;	// goal flag
            demo_icon_info[DEST_FLAG].Latitude		= (INT32)(newLat*1024.0*3600.0);
            demo_icon_info[DEST_FLAG].Longititude	= (INT32)(newLon*1024.0*3600.0);
            demo_disp_info[DEST_FLAG] = 1;*/
            SetIconVisibility(FLAG_DEST_IDX, true, newLat, newLon, true);
        }
        else
        {
            NcPointsTab[index].rpPointType = LST_NORMAL;
        }
        NcPointsTab[index].rpPointIndex = index;
        index++;
        /* --- */
    }

    NC_RP_PlanSingleRoute(&NcPointsTab[0], size);

    sample_hmi_set_pin_mode(0);
	sample_hmi_request_mapDraw();
}

void Navicore::GetWaypoints(
    const uint32_t& routeHandle,
    bool& startFromCurrentPosition,
    std::vector< std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > >& waypointsList)
{
    TRACE_WARN("TODO: implement this function");
}

void Navicore::CalculateRoute(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle)
{
    TRACE_WARN("TODO: implement this function");
}

void Navicore::CancelRouteCalculation(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle)
{
    TRACE_WARN("TODO: implement this function");
}

std::vector< uint32_t > Navicore::CalculateRoutes(
    const uint32_t& sessionHandle,
    const std::vector< uint32_t >& calculatedRoutesList)
{
    TRACE_WARN("TODO: implement this function");
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
    TRACE_WARN("TODO: implement this function");
}

std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > Navicore::GetRouteOverview(
    const uint32_t& routeHandle,
    const std::vector< int32_t >& valuesToReturn)
{
    TRACE_WARN("TODO: implement this function");
}

::DBus::Struct< ::DBus::Struct< double, double >, ::DBus::Struct< double, double > > Navicore::GetRouteBoundingBox(
    const uint32_t& routeHandle)
{
    TRACE_WARN("TODO: implement this function");
}

std::vector< uint32_t > Navicore::GetAllRoutes()
{
    std::vector<uint32_t> ret;
    for (std::vector<Route>::iterator it=Routes.begin(); it != Routes.end(); it++)
    {
        ret.push_back((*it).handle);
    }
    return ret;
}

void Navicore::SetBlockedRouteStretches(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle,
    const std::vector< ::DBus::Struct< uint32_t, uint32_t > >& blockParameters)
{
    TRACE_WARN("TODO: implement this function");
}

std::vector< ::DBus::Struct< uint32_t, uint32_t > > Navicore::GetBlockedRouteStretches(
    const uint32_t& routeHandle)
{
    TRACE_WARN("TODO: implement this function");
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
    TRACE_INFO("activate = %d (%d)", activate, IsSimulationMode);
    if (sessionHandle != lastSession) return;

    if (activate == IsSimulationMode) return; // nothing to do

    if (!activate && SimulationStatus != SIMULATION_STATUS_NO_SIMULATION)
    {
        TRACE_DEBUG("calling sample_guide_request_end");
        sample_guide_request_end();
        SimulationStatus = SIMULATION_STATUS_NO_SIMULATION;
    }
    else if (activate)
    {
        SimulationStatus = SIMULATION_STATUS_FIXED_POSITION;
    }

    IsSimulationMode = activate;
    TRACE_DEBUG("new state: %d, %d", IsSimulationMode, (int)SimulationStatus);
}

int32_t Navicore::GetSimulationStatus()
{
    TRACE_INFO("SimulationStatus: %d", (int)SimulationStatus);
    return SimulationStatus;
}

void Navicore::AddSimulationStatusListener()
{
    TRACE_WARN("TODO: implement this function");
}

void Navicore::RemoveSimulationStatusListener()
{
    TRACE_WARN("TODO: implement this function");
}

void Navicore::SetSimulationSpeed(const uint32_t& sessionHandle, const uint8_t& speedFactor)
{
    TRACE_WARN("TODO: implement this function");
}

uint8_t Navicore::GetSimulationSpeed()
{
    TRACE_WARN("TODO: implement this function");
}

void Navicore::AddSimulationSpeedListener()
{
    TRACE_WARN("TODO: implement this function");
}

void Navicore::RemoveSimulationSpeedListener()
{
    TRACE_WARN("TODO: implement this function");
}

void Navicore::StartSimulation(const uint32_t& sessionHandle)
{
    TRACE_INFO("current status: %d, %d", IsSimulationMode, (int)SimulationStatus);
    if (sessionHandle != lastSession) return;

    if (!IsSimulationMode) return; // activate simulation first
    else if (SimulationStatus == SIMULATION_STATUS_RUNNING) return; // nothing to do

    TRACE_DEBUG("calling sample_guide_request_start");
    sample_guide_request_start();
    SimulationStatus = SIMULATION_STATUS_RUNNING;

    TRACE_DEBUG("new state: %d, %d", IsSimulationMode, (int)SimulationStatus);
}

void Navicore::PauseSimulation(const uint32_t& sessionHandle)
{
    TRACE_WARN("TODO: implement this function");
}

std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > >
    Navicore::GetPosition(const std::vector< int32_t >& valuesToReturn)
{
    TRACE_WARN("TODO: implement this function");
}

void Navicore::SetPosition(
    const uint32_t& sessionHandle,
    const std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > >& position)
{
    TRACE_WARN("TODO: implement this function");
}

std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > >
    Navicore::GetAddress(const std::vector< int32_t >& valuesToReturn)
{
    TRACE_WARN("TODO: implement this function");
}

std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > >
    Navicore::GetPositionOnSegment(const std::vector< int32_t >& valuesToReturn)
{
    TRACE_WARN("TODO: implement this function");
}

std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > >
    Navicore::GetStatus(const std::vector< int32_t >& valuesToReturn)
{
    TRACE_WARN("TODO: implement this function");
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
    TRACE_WARN("TODO: implement this function");
}

void Navicore::StopGuidance(const uint32_t& sessionHandle)
{
    TRACE_WARN("TODO: implement this function");
}

void Navicore::SetVoiceGuidance(const bool& activate, const std::string& voice)
{
    TRACE_WARN("TODO: implement this function");
}

void Navicore::GetGuidanceDetails(bool& voiceGuidance, bool& vehicleOnTheRoad, bool& isDestinationReached, int32_t& maneuver)
{
    TRACE_WARN("TODO: implement this function");
}

void Navicore::PlayVoiceManeuver()
{
    TRACE_WARN("TODO: implement this function");
}

void Navicore::GetWaypointInformation(const uint16_t& requestedNumberOfWaypoints, uint16_t& numberOfWaypoints, std::vector< ::DBus::Struct< uint32_t, uint32_t, int32_t, int32_t, int16_t, int16_t, bool, uint16_t > >& waypointsList)
{
    TRACE_WARN("TODO: implement this function");
}

void Navicore::GetDestinationInformation(uint32_t& offset, uint32_t& travelTime, int32_t& direction, int32_t& side, int16_t& timeZone, int16_t& daylightSavingTime)
{
    TRACE_WARN("TODO: implement this function");
}

void Navicore::GetManeuversList(const uint16_t& requestedNumberOfManeuvers, const uint32_t& maneuverOffset, uint16_t& numberOfManeuvers, std::vector< ::DBus::Struct< std::string, std::string, uint16_t, int32_t, uint32_t, std::vector< ::DBus::Struct< uint32_t, uint32_t, int32_t, int32_t, std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > > > > >& maneuversList)
{
    TRACE_WARN("TODO: implement this function");
}

void Navicore::SetRouteCalculationMode(const uint32_t& sessionHandle, const int32_t& routeCalculationMode)
{
    TRACE_WARN("TODO: implement this function");
}

void Navicore::SkipNextManeuver(const uint32_t& sessionHandle)
{
   TRACE_WARN("TODO: implement this function");
}

void Navicore::GetGuidanceStatus(int32_t& guidanceStatus, uint32_t& routeHandle)
{
    TRACE_WARN("TODO: implement this function");
}

void Navicore::SetVoiceGuidanceSettings(const int32_t& promptMode)
{
    TRACE_WARN("TODO: implement this function");
}

int32_t Navicore::GetVoiceGuidanceSettings()
{
    TRACE_WARN("TODO: implement this function");
}
