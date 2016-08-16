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

extern "C" {
    #include "navicore.h"
    #include "glview.h"
    #include "SMCoreMP/MP_Def.h"
    #include "SMCoreMP/MP_GL.h"
}

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
            map_max_scale(0) {}

        int sample_load_image_file;
        int move_with_car;
        int old_set_pin;
        int set_pin;
        int compass;
        int force_update_flag;
        int map_max_scale;
};


class DisplayContext
{
    public:
        DisplayContext(): hmi(),
            name(NULL),
            map_context(0),
            hmi_context(0) {}
        void sample_hmi_request_update(void) { glvOnUpdate(hmi_context); }
        void sample_hmi_request_mapDraw(void){ glvOnReDraw(map_context); }

        HmiContext hmi;
        char *name = NULL;
        GLVContext map_context = 0;
        GLVContext hmi_context = 0;
        GLVEVENTFUNC_t SurfaceViewEventFunc;
        GLVEVENTFUNC_t hmi_SurfaceViewEventFunc;
};


class NaviContext
{
    public:
        NaviContext():
            resolution(NAVI_RESOLUTION_AGL_DEMO),
            region(NAVI_REGION_JAPAN),
            WinWidth(1280),
            WinHeight(720),
            main_window_mapScale(2),
            /*hmi()*/
            display() {}
        void naviStartUpRegion(void);
        void naviStartUpResolution(void);
        void naviGetResolution(int *w,int *h) {
            *w = WinWidth;
            *h = WinHeight;
        }
        void naviGetRegion(NAVI_REGION_e *r) {
            *r = region;
        }

        NAVI_RESOLUTION_e resolution;
        NAVI_REGION_e region;
        int WinWidth;
        int WinHeight;
        int main_window_mapScale;
        //std::shared_ptr<CommonAPI::ClientId> myPtrClientId;
        //v4::org::genivi::navigation::NavigationTypes::Handle myNaviHandle;
        //v4::org::genivi::navigation::navigationcore::GeniviNaviMapMatchedPosition myNaviMapMatchedPosition;
        //v4::org::genivi::navigation::navigationcore::GeniviNaviSession myGeniviNaviSession;
        //v4::org::genivi::navigation::navigationcore::GeniviNaviRouting myGeniviNaviRouting;
        //HmiContext hmi;
        DisplayContext display;
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
