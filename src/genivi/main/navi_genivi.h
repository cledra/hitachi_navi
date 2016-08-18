/*
 * GPS Navigation ---An open source GPS navigation core software
 *
 *
 * Copyright (c) 2016  AWTCE, sa
 *
 * This program is dual licensed under GPL version 2 or a commercial license.
 * See the LICENSE file distributed with this source file.
 */

/*
 *  NavicoreGenivi.hpp
 *
 *  Created on: 2016/03/14
 *      Author:
 */

#ifndef NAVI_GENIVI_H_
#define NAVI_GENIVI_H_

// TODO : include all the GENIVI navigation interfaces
//#include "NaviSession.hpp"
//#include "NaviMapViewerControl.hpp"
/*#include "GeniviNaviSession.hpp"
#include "GeniviNaviMapMatchedPosition.hpp"
#include "GeniviNaviRouting.hpp"*/

#include <string>
#include <vector>

extern "C" {
    #include "navicore.h"
    #include "glview.h"
    #include "SMCoreMP/MP_Def.h"
    #include "SMCoreMP/MP_GL.h"
}

#define GESTURE_FLICK_TIMER_ID      1000
#define GESTURE_LONG_PRESS_TIMER_ID 1001

typedef enum
{
    NAVI_REGION_OPTIONAL    = -1,
    NAVI_REGION_JAPAN       = 0,
    NAVI_REGION_UK,
    NAVI_REGION_GERMANY,
    NAVI_REGION_NEVADA
} NAVI_REGION_e;


typedef enum
{
    NAVI_RESOLUTION_OPTIONAL    = -1,   // --width WIDTH --height HEIGHT
    NAVI_RESOLUTION_FullHD      = 0,    // Full-HD	(1080p)
    NAVI_RESOLUTION_HD,                 // HD		( 720p)
    NAVI_RESOLUTION_AGL_DEMO            // CES 2016 demo
} NAVI_RESOLUTION_e;


typedef enum
{
    START_FLAG = 0,
    DEST_FLAG = 1,
    PIN_FLAG = 2
} HMI_FLAG_INDEX_e;


class WayPoint
{
    public:
        WayPoint(double lat_, double lon_):
            lat(lat_), lon(lon_) {}

        double lat;
        double lon;
};


class Route
{
    public:
        Route(uint32_t handle_):
            handle(handle_){}
        

        uint32_t handle;
        std::vector<WayPoint> WayPoints;
};


class MapviewInstance
{
    public:
        MapviewInstance(uint32_t handle_, std::string client_, int32_t ViewType_, int16_t w_, int16_t h_):
            handle(handle_),
            ViewType(ViewType_),
            w(w_), h(h_) {
                client = std::string(client_);
            }

        uint32_t handle;
        int32_t ViewType;
        int16_t w;
        int16_t h;
        std::string client;
};


class MapviewerSession
{
    public:
        MapviewerSession(uint32_t handle_, std::string client_):
            handle(handle_),
            lastViewInstance(0) {
                client = std::string(client_);
            }
        std::vector<MapviewInstance>::iterator retrieveMapviewInstanceIt(const uint32_t& mapViewInstanceHandle);

        uint32_t handle;
        std::string client;
        std::vector<MapviewInstance> views;
        uint32_t lastViewInstance;
        pthread_t p;
};


class HmiContext
{
    public:
        HmiContext():
            sample_load_image_file(0),
            move_with_car(-1),
            old_set_pin(-1),
            set_pin(-1),
            compass(0),
            force_update_flag(0),
            map_max_scale(0),
            hmi_context(0),
            glv_hmi_window(NULL) {}
        void sample_hmi_request_update(void) { glvOnUpdate(hmi_context); }
        void set_flag_visible(HMI_FLAG_INDEX_e flag, Bool visible, double lat = 0.0, double lon = 0.0, Bool commit = false);

        int sample_load_image_file;
        int move_with_car;
        int old_set_pin;
        int set_pin;
        int compass;
        int force_update_flag;
        int map_max_scale;
        GLVContext hmi_context = 0;
        GLVEVENTFUNC_t hmi_SurfaceViewEventFunc;
        GLVWindow glv_hmi_window;
        SMMAPDYNUDI     demo_icon_info[ICON_CNT_MAX];
        unsigned char   demo_disp_info[ICON_CNT_MAX];
};


class DisplayContext
{
    public:
        DisplayContext(): hmi(),
            name(""),
            map_context(0),
            glvDisplay(NULL),
            glv_map_window(NULL)/*,
            mapviewerSession(NULL)*/ {}
        void sample_hmi_request_mapDraw(void){ glvOnReDraw(map_context); }
        std::vector<MapviewerSession>::iterator retrieveMapviewerSessionIt(const uint32_t& sessionHandle);

        HmiContext hmi;
        std::string name;
        GLVContext map_context = 0;
        GLVEVENTFUNC_t SurfaceViewEventFunc;
        GLVDisplay glvDisplay;
        GLVWindow glv_map_window;
        //MapviewerSession *mapviewerSession;
        std::vector<struct MapviewerSession> MapviewerSessionList;
};


class NaviContext
{
    public:
        NaviContext():
            resolution(NAVI_RESOLUTION_AGL_DEMO),
            region(NAVI_REGION_JAPAN),
            WinWidth(1280),
            WinHeight(720),
            main_window_mapScale(2), // TODO: move this to display / hmi
            /*hmi()*/
            navi_config_path(""),
            navi_config_map_db_path(""),
            navi_config_user_data_path(""),
            navi_config_map_udi_data_path(""),
            navi_config_map_udi_info_file(""),
            navi_config_map_font_file(""),
            display(),
            lastRoute(0) {}
        void naviStartUpRegion(void);
        void naviStartUpResolution(void);
        void naviGetResolution(int *w,int *h) {
            *w = WinWidth;
            *h = WinHeight;
        }
        void naviGetRegion(NAVI_REGION_e *r) {
            *r = region;
        }
        std::vector<Route>::iterator retrieveRouteIt(const uint32_t& routeHandle);

        NAVI_RESOLUTION_e resolution;
        NAVI_REGION_e region;
        int WinWidth;
        int WinHeight;
        int main_window_mapScale; // TODO: move to MapViewInstance ?
        /*char navi_config_path[NAVI_DATA_PATH_SIZE];
        char navi_config_user_data_path[NAVI_DATA_PATH_SIZE];*/
        std::string navi_config_path;
        std::string navi_config_map_db_path;
        std::string navi_config_user_data_path;
        std::string navi_config_map_udi_data_path;
        std::string navi_config_map_udi_info_file;
        std::string navi_config_map_font_file;
        //std::shared_ptr<CommonAPI::ClientId> myPtrClientId;
        //v4::org::genivi::navigation::NavigationTypes::Handle myNaviHandle;
        //v4::org::genivi::navigation::navigationcore::GeniviNaviMapMatchedPosition myNaviMapMatchedPosition;
        //v4::org::genivi::navigation::navigationcore::GeniviNaviSession myGeniviNaviSession;
        //v4::org::genivi::navigation::navigationcore::GeniviNaviRouting myGeniviNaviRouting;
        //HmiContext hmi;
        DisplayContext display;
        uint32_t lastRoute;
        std::vector<Route> routes;
};


/*class ClientIdImp : public CommonAPI::ClientId
{
public:
        virtual ~ClientIdImp() {}
        bool operator==(ClientId& clientIdToCompare) { return hashCode() == clientIdToCompare.hashCode(); }
        std::size_t hashCode() { return (std::size_t)id; }

private:
        int id;
};*/

#endif /* NAVI_GENIVI_H_ */
