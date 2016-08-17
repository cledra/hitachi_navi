#include <stdio.h>
#include <vector>
#include <string>
#include <tuple>
#include "genivi-navicore.h"
#include "NaviTrace.h"
#include "navi_genivi.h"
#include "route.h"

extern "C" {
    #include "font.h"
    #include "png.h"
}

// TODO: put in a session handle
GLVWindow	glv_hmi_window;

extern NaviContext g_ctx;
extern void sample_hmi_draw_compass(FLOAT rotate); // TODO: move this to button.cpp
extern int sample_hmi_button_down(int pointer_sx,int pointer_sy);  // TODO: move this to button.cpp
extern int sample_hmi_button_up(int pointer_sx,int pointer_sy);  // TODO: move this to button.cpp

static INT32 BitmapFontCallBack(NCBITMAPFONTINFO* pInfo)
{
	int result = 0;

	initializeBitmapFont();
	setColor(pInfo->color);
	setOutLineColor(pInfo->outLineColor);
	setBkgdColor(pInfo->bkgdColor);

	result = createBitmapFont(
			pInfo->str, pInfo->fontSize, pInfo->outLineFlg,
			&pInfo->pBitMap,
			&pInfo->width, &pInfo->height,
			&pInfo->strWidth, &pInfo->strHeight,
			pInfo->rotation, true);

	if (result != 0) {
		return (0);
	}
	return (1);
}

static INT32 ReadImageForFileCallBack(NCBITMAPINFO* pInfo)
{
	pInfo->pBitMap = (UChar *)readPngData(pInfo->path, &pInfo->width, &pInfo->height);
	if (NULL == pInfo->pBitMap) {
		return (0);
	}
	return (1);
}

static INT32 SetImageForMemoryCallBack(NCBITMAPINFO* pInfo)
{
	pInfo->pBitMap = (UChar *)setPngData(pInfo->image, pInfo->dataSize, &pInfo->width, &pInfo->height);
	if (NULL == pInfo->pBitMap) {
		return 0;
	}
	return 1;
}

static void sample_hmi_request_update(void) /* TODO: Fix this, this is crapy dirty */
{
    g_ctx.display.hmi.sample_hmi_request_update();
}

static INT32 MapDrawEndCallBack(NCDRAWENDINFO* pInfo)
{
	sample_hmi_draw_compass(pInfo->rotate);
	sample_hmi_request_update();
	return 1;
}

static int sample_hmi_keyboard_handle_key(
                    unsigned int key,
                    unsigned int state)
{
	if(g_ctx.display.map_context == 0) {
		return 0;
	}
    
    TRACE_INFO("Key is %d, state is %d", key, state);
    if(state == GLV_KEYBOARD_KEY_STATE_PRESSED) {
    	switch(key) {
    	case 103:		// '↑'
		   if(g_ctx.main_window_mapScale < g_ctx.display.hmi.map_max_scale){
			   g_ctx.main_window_mapScale++;
				   if (g_ctx.main_window_mapScale == 1) {
					   g_ctx.main_window_mapScale++;
				   }
				   if(g_ctx.main_window_mapScale > g_ctx.display.hmi.map_max_scale)
                        g_ctx.main_window_mapScale = g_ctx.display.hmi.map_max_scale;
				  NC_MP_SetMapScaleLevel(NC_MP_MAP_MAIN, g_ctx.main_window_mapScale);
				  glvOnReDraw(g_ctx.display.map_context);
		   }
			break;
		case 108:		// '↓'
     	   if(g_ctx.main_window_mapScale > 0){
     		   g_ctx.main_window_mapScale--;
				   if (g_ctx.main_window_mapScale == 1) {
					   g_ctx.main_window_mapScale--;
				   }
				   if(g_ctx.main_window_mapScale < 0)
                       g_ctx.main_window_mapScale = 0;
				  NC_MP_SetMapScaleLevel(NC_MP_MAP_MAIN, g_ctx.main_window_mapScale);
				  glvOnReDraw(g_ctx.display.map_context);
     	   }
     	   break;
		case 25:		// 'p'
			// route search
            sample_calc_demo_route(g_ctx.display);
			break;
		case 30:		// 'a'
			// own posi mode
			NC_MP_SetMapMoveWithCar(NC_MP_MAP_MAIN,1);
			glvOnReDraw(g_ctx.display.map_context);
			break;
    	}
    }
    return(0);
}

static int internal_create_session(NaviContext *ctx)
{
    //GLVDisplay	glv_dpy;
	//GLVWindow	glv_map_window;
	//GLVWindow	glv_hmi_window;
	int rc;

	ctx->naviStartUpResolution(); 
	ctx->naviStartUpRegion();

	ctx->display.glvDisplay = glvOpenDisplay((char*)ctx->display.name.c_str());
	if(!ctx->display.glvDisplay){
		TRACE_ERROR("glvOpenDisplay() failed");
		return -1;
	}

	NC_MP_SetBitmapFontCB(BitmapFontCallBack);
	NC_MP_SetImageReadForFileCB(ReadImageForFileCallBack);
	NC_MP_SetImageReadForImageCB(SetImageForMemoryCallBack);
	NC_MP_SetMapDrawEndCB(MapDrawEndCallBack);

	rc = NC_Initialize(g_ctx.WinWidth, g_ctx.WinHeight,
            g_ctx.navi_config_user_data_path.c_str(), g_ctx.navi_config_map_db_path.c_str(), "locatorPath");
	if(NC_SUCCESS != rc){
		TRACE_ERROR("NC_Initialize error");
		TRACE_ERROR("\ttuser data path: %s", ctx->navi_config_user_data_path.c_str());
		TRACE_ERROR("\tmap db path: %s", ctx->navi_config_map_db_path.c_str());
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

Navicore::Navicore( DBus::Connection &connection )
    : DBus::ObjectAdaptor(connection, "/org/genivi/navicore"),
      lastSession(0)
{
}

::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > Navicore::SessionGetVersion()
{
    ::DBus::Struct<uint16_t, uint16_t, uint16_t, std::string> version;
    version._1 = 0;
    version._2 = 1;
    version._3 = 2;
    version._4 = std::string("Session");
    return version;
}

uint32_t Navicore::CreateSession(const std::string& client)
{
    lastSession++;

    if (lastSession == 1) // we only handle 1 session for now
    {
        if (internal_create_session(&g_ctx) < 0) // error
        {
            TRACE_ERROR("fail to create session (%s)", client.c_str());
            return 0;
        }
    }
    
    TRACE_INFO("SESSION ADAPTOR - Created session %d [%s]", lastSession,client.c_str());
    return lastSession;
}

void Navicore::DeleteSession(const uint32_t& sessionHandle)
{
    if (sessionHandle == 1) // we only handle 1 session for now
    {
        //TODO: delete Mapviewer if it's not already done
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
        glvCloseDisplay(g_ctx.display.glvDisplay);
        g_ctx.display.glvDisplay = NULL;
    }

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
    ::DBus::Struct< uint32_t, std::string > a,b;
    a._1 = 1; a._2 = std::string("Session 1");
    b._1 = 2; b._2 = std::string("Session b");
    list.push_back( a );
    list.push_back( b );
    return list;
}

::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > Navicore::RoutingGetVersion()
{
    ::DBus::Struct<uint16_t, uint16_t, uint16_t, std::string> version;
    version._1 = 0;
    version._2 = 1;
    version._3 = 2;
    version._4 = std::string("Routing");
    return version;
}

uint32_t Navicore::CreateRoute(const uint32_t& sessionHandle)
{
    // TODO
}

void Navicore::DeleteRoute(const uint32_t& sessionHandle, const uint32_t& routeHandle)
{
    // TODO
}

void Navicore::SetCostModel(const uint32_t& sessionHandle, const uint32_t& routeHandle, const int32_t& costModel)
{
    // TODO
}

int32_t Navicore::GetCostModel(const uint32_t& routeHandle)
{
    // TODO
}

std::vector< int32_t > Navicore::GetSupportedCostModels()
{
    // TODO
}

void Navicore::SetRoutePreferences(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle,
    const std::string& countryCode,
    const std::vector< ::DBus::Struct< int32_t, int32_t > >& roadPreferenceList,
    const std::vector< ::DBus::Struct< int32_t, int32_t > >& conditionPreferenceList)
{
    // TODO
}
void Navicore::GetRoutePreferences(
    const uint32_t& routeHandle,
    const std::string& countryCode,
    std::vector< ::DBus::Struct< int32_t, int32_t > >& roadPreferenceList,
    std::vector< ::DBus::Struct< int32_t, int32_t > >& conditionPreferenceList)
{
    // TODO
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
    // TODO
}

std::map< int32_t, uint32_t > Navicore::GetRouteSchedule(
    const uint32_t& routeHandle,
    const std::vector< int32_t >& valuesToReturn)
{
    // TODO
}

void Navicore::SetTransportationMeans(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle,
    const std::vector< int32_t >& transportationMeansList)
{
    // TODO
}

std::vector< int32_t > Navicore::GetTransportationMeans(
    const uint32_t& routeHandle)
{
    // TODO
}

std::vector< int32_t > Navicore::GetSupportedTransportationMeans()
{
    // TODO
}

void Navicore::SetExcludedAreas(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle,
    const std::vector< std::vector< ::DBus::Struct< double, double > > >& excludedAreas)
{
    // TODO
}

std::vector< std::vector< ::DBus::Struct< double, double > > > Navicore::GetExcludedAreas(
    const uint32_t& routeHandle)
{
    // TODO
}

void Navicore::SetWaypoints(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle,
    const bool& startFromCurrentPosition,
    const std::vector< std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > >& waypointsList)
{
    // TODO
}

void Navicore::GetWaypoints(
    const uint32_t& routeHandle,
    bool& startFromCurrentPosition,
    std::vector< std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > >& waypointsList)
{
    // TODO
}

void Navicore::CalculateRoute(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle)
{
    // TODO
}

void Navicore::CancelRouteCalculation(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle)
{
    // TODO
}

std::vector< uint32_t > Navicore::CalculateRoutes(
    const uint32_t& sessionHandle,
    const std::vector< uint32_t >& calculatedRoutesList)
{
    // TODO
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
    // TODO
}

std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > Navicore::GetRouteOverview(
    const uint32_t& routeHandle,
    const std::vector< int32_t >& valuesToReturn)
{
    // TODO
}

::DBus::Struct< ::DBus::Struct< double, double >, ::DBus::Struct< double, double > > Navicore::GetRouteBoundingBox(
    const uint32_t& routeHandle)
{
    // TODO
}

std::vector< uint32_t > Navicore::GetAllRoutes()
{
    // TODO
}

void Navicore::SetBlockedRouteStretches(
    const uint32_t& sessionHandle,
    const uint32_t& routeHandle,
    const std::vector< ::DBus::Struct< uint32_t, uint32_t > >& blockParameters)
{
    // TODO
}

std::vector< ::DBus::Struct< uint32_t, uint32_t > > Navicore::GetBlockedRouteStretches(
    const uint32_t& routeHandle)
{
    // TODO
}

::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > Navicore::PositionGetVersion()
{
    ::DBus::Struct<uint16_t, uint16_t, uint16_t, std::string> version;
    version._1 = 0;
    version._2 = 0;
    version._3 = 0;
    version._4 = std::string("MapMatchedPosition");
    return version;
}

void Navicore::SetSimulationMode(const uint32_t& sessionHandle, const bool& activate)
{
    // TODO
}

int32_t Navicore::GetSimulationStatus()
{
    // TODO
}

void Navicore::AddSimulationStatusListener()
{
    // TODO
}

void Navicore::RemoveSimulationStatusListener()
{
    // TODO
}

void Navicore::SetSimulationSpeed(const uint32_t& sessionHandle, const uint8_t& speedFactor)
{
    // TODO
}

uint8_t Navicore::GetSimulationSpeed()
{
    // TODO
}

void Navicore::AddSimulationSpeedListener()
{
    // TODO
}

void Navicore::RemoveSimulationSpeedListener()
{
    // TODO
}

void Navicore::StartSimulation(const uint32_t& sessionHandle)
{
    // TODO
}

void Navicore::PauseSimulation(const uint32_t& sessionHandle)
{
    // TODO
}

std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > >
    Navicore::GetPosition(const std::vector< int32_t >& valuesToReturn)
{
    // TODO
}

void Navicore::SetPosition(
    const uint32_t& sessionHandle,
    const std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > >& position)
{
    // TODO
}

std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > >
    Navicore::GetAddress(const std::vector< int32_t >& valuesToReturn)
{
    // TODO
}

std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > >
    Navicore::GetPositionOnSegment(const std::vector< int32_t >& valuesToReturn)
{
    // TODO
}

std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > >
    Navicore::GetStatus(const std::vector< int32_t >& valuesToReturn)
{
    // TODO
}

::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > Navicore::GuidanceGetVersion()
{
    ::DBus::Struct<uint16_t, uint16_t, uint16_t, std::string> version;
    version._1 = 0;
    version._2 = 1;
    version._3 = 2;
    version._4 = std::string("Guidance");
    return version;
}

void Navicore::Navicore::StartGuidance(const uint32_t& sessionHandle, const uint32_t& routeHandle)
{
    // TODO
}

void Navicore::StopGuidance(const uint32_t& sessionHandle)
{
    // TODO
}

void Navicore::SetVoiceGuidance(const bool& activate, const std::string& voice)
{
    // TODO
}

void Navicore::GetGuidanceDetails(bool& voiceGuidance, bool& vehicleOnTheRoad, bool& isDestinationReached, int32_t& maneuver)
{
    // TODO
}

void Navicore::PlayVoiceManeuver()
{
    // TODO
}

void Navicore::GetWaypointInformation(const uint16_t& requestedNumberOfWaypoints, uint16_t& numberOfWaypoints, std::vector< ::DBus::Struct< uint32_t, uint32_t, int32_t, int32_t, int16_t, int16_t, bool, uint16_t > >& waypointsList)
{
    // TODO
}

void Navicore::GetDestinationInformation(uint32_t& offset, uint32_t& travelTime, int32_t& direction, int32_t& side, int16_t& timeZone, int16_t& daylightSavingTime)
{
    // TODO
}

void Navicore::GetManeuversList(const uint16_t& requestedNumberOfManeuvers, const uint32_t& maneuverOffset, uint16_t& numberOfManeuvers, std::vector< ::DBus::Struct< std::string, std::string, uint16_t, int32_t, uint32_t, std::vector< ::DBus::Struct< uint32_t, uint32_t, int32_t, int32_t, std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > > > > >& maneuversList)
{
    // TODO
}

void Navicore::SetRouteCalculationMode(const uint32_t& sessionHandle, const int32_t& routeCalculationMode)
{
    // TODO
}

void Navicore::SkipNextManeuver(const uint32_t& sessionHandle)
{
    // TODO
}

void Navicore::GetGuidanceStatus(int32_t& guidanceStatus, uint32_t& routeHandle)
{
    // TODO
}

void Navicore::SetVoiceGuidanceSettings(const int32_t& promptMode)
{
    // TODO
}

int32_t Navicore::GetVoiceGuidanceSettings()
{
    // TODO
}
