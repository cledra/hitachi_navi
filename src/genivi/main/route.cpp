 /*
 * GPS Navigation ---An open source GPS navigation core software
 *
 *
 * Copyright (c) 2016  Hitachi, Ltd.
 *
 * This program is dual licensed under GPL version 2 or a commercial license.
 * See the LICENSE file distributed with this source file.
 */

/*
 * route.c
 *
 *  Created on: 2015/11/05
 *      Author:t.aikawa
 */

#include <stdlib.h>
#include <stdio.h>

#include "navi_genivi.h"
#include "NaviTrace.h"

extern "C" {
    #include "navicore.h"
}

extern NaviContext g_ctx;

void sample_init_demo_route_icon(void)
{
	/*demo_icon_info[0].IconID = 21;	// start flag
	demo_icon_info[0].Longititude	= (INT32)(0*1024.0*3600.0);
	demo_icon_info[0].Latitude		= (INT32)(0*1024.0*3600.0);
	demo_icon_info[1].IconID = 22;	// goal flag
	demo_icon_info[1].Longititude	= (INT32)(0*1024.0*3600.0);
	demo_icon_info[1].Latitude		= (INT32)(0*1024.0*3600.0);
	demo_icon_info[2].IconID = 3;	// pin flag
	demo_icon_info[2].Longititude	= (INT32)(0*1024.0*3600.0);
	demo_icon_info[2].Latitude		= (INT32)(0*1024.0*3600.0);
	demo_disp_info[0] = 0;
	demo_disp_info[1] = 0;
	demo_disp_info[2] = 0;*/

    g_ctx.display.hmi.set_flag_visible(START_FLAG, false);
    g_ctx.display.hmi.set_flag_visible(DEST_FLAG, false);
    g_ctx.display.hmi.set_flag_visible(PIN_FLAG, false);
}
void sample_set_demo_icon_pin_flag(HmiContext &ctxHmi, SMGEOCOORD *geoCood)
{
	/*demo_icon_info[2].IconID = 3;	// pin flag
	demo_icon_info[2].Longititude	= (INT32)(geoCood->longitude);
	demo_icon_info[2].Latitude		= (INT32)(geoCood->latitude);
	demo_disp_info[2] = 1;

	NC_DM_SetIconInfo(demo_icon_info,3);
	NC_DM_SetDynamicUDIDisplay(demo_disp_info,3);*/
    
    ctxHmi.set_pin = 1; /* TODO: improve this */
    g_ctx.display.hmi.set_flag_visible(PIN_FLAG, true,
        geoCood->latitude/1024.0/3600.0, geoCood->longitude/1024.0/3600.0, true);
}

void sample_clear_demo_route_icon(void)
{
	/*demo_disp_info[0] = 0;
	demo_disp_info[1] = 0;
	demo_disp_info[2] = 0;

	//NC_DM_SetIconInfo(demo_icon_info,3);
	NC_DM_SetDynamicUDIDisplay(demo_disp_info,3);*/
    g_ctx.display.hmi.set_flag_visible(START_FLAG, false);
    g_ctx.display.hmi.set_flag_visible(DEST_FLAG, false);
    g_ctx.display.hmi.set_flag_visible(PIN_FLAG, false, 0.0, 0.0, true);
}

int sample_calc_demo_route(DisplayContext &ctxDisplay, Route &route)
{
	SMCARSTATE carState;
	INT32		ret;
	//E_SC_RESULT	ret = e_SC_RESULT_SUCCESS;
	//T_VIEW_INFO* p_vi;
	float st_lat;
	float st_lon;
	float ed_lat;
	float ed_lon;

	if(ctxDisplay.hmi.demo_disp_info[DEST_FLAG] == 0)
        return 1; /* error : no destination set */

	ret = NC_DM_GetCarState(&carState, e_SC_CARLOCATION_NOW);
	if(NC_SUCCESS != ret) {
		printf("NC_DM_GetCarState err (%d)\n", ret);
	}
	// 自車位置緯度経度取得
	st_lon = (float)carState.coord.longitude/1024.0/3600.0;
	st_lat = (float)carState.coord.latitude/1024.0/3600.0;

	ed_lon = (float)ctxDisplay.hmi.demo_icon_info[DEST_FLAG].Longititude/1024.0/3600.0;
	ed_lat = (float)ctxDisplay.hmi.demo_icon_info[DEST_FLAG].Latitude/1024.0/3600.0;

    TRACE_DEBUG("set route from %f:%f to %f:%f", st_lon, st_lat, ed_lon, ed_lat);

	/*demo_icon_info[2].Longititude	= 0;
	demo_icon_info[2].Latitude		= 0;
	demo_disp_info[2] = 0;*/

	SMRPPOINT newPoint[2] = {};

	// 始点
	newPoint[0].coord.latitude = (st_lat*1024.0*3600.0);
	newPoint[0].coord.longitude = (st_lon*1024.0*3600.0);
	newPoint[0].rpPointType = LST_START;
	newPoint[0].rpPointIndex = 0;

	// 終点
	newPoint[1].coord.latitude = (ed_lat*1024.0*3600.0);
	newPoint[1].coord.longitude = (ed_lon*1024.0*3600.0);
	newPoint[1].rpPointType = LST_DEST;
	newPoint[1].rpPointIndex = 1;

    // add for Genivi API:
    WayPoint WPStart(st_lat, st_lon);
    WayPoint WPEnd(ed_lat, ed_lon);
    route.WayPoints.push_back(WPStart);
    route.WayPoints.push_back(WPEnd);
    /* --- */

	NC_RP_PlanSingleRoute(&newPoint[0],2);

	// icon
    /* TODO CLEAN: makefunctions for this */
	/*demo_icon_info[0].IconID = 21;	// start flag
	demo_icon_info[0].Longititude	= (INT32)(st_lon*1024.0*3600.0);
	demo_icon_info[0].Latitude		= (INT32)(st_lat*1024.0*3600.0);

	demo_icon_info[1].IconID = 22;	// goal flag
	demo_icon_info[1].Longititude	= (INT32)(ed_lon*1024.0*3600.0);
	demo_icon_info[1].Latitude		= (INT32)(ed_lat*1024.0*3600.0);

	demo_icon_info[2].IconID = 27;	// pin flag
	demo_icon_info[2].Longititude	= 0;
	demo_icon_info[2].Latitude		= 0;

	demo_disp_info[0] = 1;
	demo_disp_info[1] = 1;
	demo_disp_info[2] = 0;

	NC_DM_SetIconInfo(demo_icon_info,3);
	NC_DM_SetDynamicUDIDisplay(demo_disp_info,3);*/

    ctxDisplay.hmi.set_flag_visible(PIN_FLAG, false);
    ctxDisplay.hmi.set_pin = 0; /* TODO: improve this */
    ctxDisplay.hmi.set_flag_visible(START_FLAG, true, st_lat, st_lon);
    ctxDisplay.hmi.set_flag_visible(DEST_FLAG, true, ed_lat, ed_lon, true);

	//ctxDisplay.sample_hmi_request_mapDraw();
	return 0;
}

