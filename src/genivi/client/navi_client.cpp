#include <dbus-c++/dbus.h>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <getopt.h>
#include "navicore.h"
#include "mapviewer.h"
#include "../main/NaviTrace.h"
#include "../main/genivi-mapviewer-constants.h"
#include "../main/genivi-navicore-constants.h"

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include <unistd.h> /* for sleep */

DBus::BusDispatcher dispatcher;

static struct option long_options[] = {
    {"width",   required_argument, 0,  'w' },
    {"height",  required_argument, 0,  'h' },
    {0,         0,                 0,  0 }
};

int main(int argc, char * argv[])
{
    DBus::default_dispatcher = &dispatcher;
    DBus::Connection conn = DBus::Connection::SessionBus();
    Navicore navicore(conn, "/org/genivi/navicore", "org.agl.gpsnavi");
    MapViewer mapviewer(conn, "/org/genivi/mapviewer", "org.agl.gpsnavi");
    uint32_t navicoreSession, mapViewerSession, mapViewInstance;
    ::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > version;
    ::DBus::Struct< uint16_t, uint16_t > resolution;
    resolution._1 = 1280;
    resolution._2 = 720;
    int opt;

    while ((opt = getopt_long(argc, argv, "w:h:", long_options, NULL)) != -1)
    {
        switch (opt)
        {
            case 'w':
                resolution._1 = atoi(optarg);
                break;
            case 'h':
                resolution._2 = atoi(optarg);
                break;
            default: break;
        }
    }

    version = navicore.GuidanceGetVersion();
    TRACE_INFO("Navicore version : %s : .%" PRIu16 ".%" PRIu16 ".%" PRIu16,
        version._4.c_str(), version._3, version._2, version._1);
    
    version = mapviewer.MapViewerControlGetVersion();
    TRACE_INFO("MapViewer version : %s : .%" PRIu16 ".%" PRIu16 ".%" PRIu16,
        version._4.c_str(), version._3, version._2, version._1);

    navicoreSession = navicore.CreateSession(std::string("AW-navicore"));
    TRACE_INFO("navicore.CreateSession() --> %" PRIu32, navicoreSession);

    mapViewerSession = mapviewer.CreateSession(std::string("AW-mapviewer"));
    TRACE_INFO("mapviewer.CreateSession() --> %" PRIu32, mapViewerSession);

    mapViewInstance = mapviewer.CreateMapViewInstance(mapViewerSession, resolution, MAPVIEWER_MAIN_MAP);
    TRACE_INFO("mapviewer.CreateMapViewInstance(%" PRIu16 ":%" PRIu16 ") --> %" PRIu32,
        resolution._1, resolution._2, mapViewInstance);

    uint32_t route = navicore.CreateRoute(navicoreSession);
    TRACE_INFO("CreateRoute %" PRIu32 " -> %" PRIu32, navicoreSession, route);
    sleep(2);

    std::vector< std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > > wpList;
    std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > point1;
    ::DBus::Struct< uint8_t, ::DBus::Variant > point1Lat;
    ::DBus::Struct< uint8_t, ::DBus::Variant > point1Lon;
    
    /*point1Lat._1 = NAVICORE_LATITUDE;
    point1Lat._2.writer().append_double(35.590429959);
    point1Lon._1 = NAVICORE_LONGITUDE;
    point1Lon._2.writer().append_double(139.730397407);*/
    
    /*point1Lat._1 = NAVICORE_LATITUDE;
    point1Lat._2.writer().append_double(35.588711209);
    point1Lon._1 = NAVICORE_LONGITUDE;
    point1Lon._2.writer().append_double(139.728857693);*/
    
    point1Lat._1 = NAVICORE_LATITUDE;
    point1Lat._2.writer().append_double(35.594820421);
    point1Lon._1 = NAVICORE_LONGITUDE;
    point1Lon._2.writer().append_double(139.720988227);
    
    point1[NAVICORE_LATITUDE] = point1Lat;
    point1[NAVICORE_LONGITUDE] = point1Lon;
    wpList.push_back(point1);

    TRACE_INFO("SetWaypoints (start from current position + 1 point)");
    navicore.SetWaypoints(navicoreSession, route, true, wpList);
    sleep(2);

    TRACE_INFO("SetSimulationMode true");
    navicore.SetSimulationMode(navicoreSession, true);
    sleep(2);

    TRACE_INFO("StartSimulation");
    navicore.StartSimulation(navicoreSession);
    sleep(2);

    TRACE_INFO("GetSimulationStatus : %" PRIu32, navicore.GetSimulationStatus());

    sleep(10);

    /*TRACE_INFO("SetCameraHeadingTrackUp");
    mapviewer.SetCameraHeadingTrackUp(mapViewerSession, mapViewInstance);

    sleep(10);*/

    TRACE_INFO("SetCameraHeadingAngle");
    mapviewer.SetCameraHeadingAngle(mapViewerSession, mapViewInstance, 0);
    sleep(10);

    std::vector< ::DBus::Struct< uint16_t, uint16_t, int32_t, uint32_t > > scaleList;
    std::vector< ::DBus::Struct< uint16_t, uint16_t, int32_t, uint32_t > >::iterator it;
    scaleList = mapviewer.GetScaleList(mapViewInstance);

    TRACE_INFO("Possible zoom values:");
    for (it = scaleList.begin(); it != scaleList.end(); it++)
    {
        TRACE_INFO("%" PRIu16 " (%" PRIu16 ", %" PRIu32 ", %" PRIu32 ")",
            it->_1, it->_2, it->_3, it->_4);
    }

    sleep(2);
    uint8_t old_scale, current_scale;
    int32_t minmax;
    mapviewer.GetMapViewScale(mapViewInstance, old_scale, minmax);
    TRACE_INFO("Current zoom: %" PRIu16 " (%" PRIu16 ", %" PRIu32 ", %" PRIu32 "), minmax: %" PRId32,
        scaleList[old_scale]._1, scaleList[old_scale]._2, scaleList[old_scale]._3, scaleList[old_scale]._4,
        minmax);

    for (it = scaleList.begin(); it != scaleList.end(); it++)
    {
        sleep(2);
        TRACE_INFO("setting zoom %" PRIu16, it->_1);
        mapviewer.SetMapViewScale(mapViewerSession, mapViewInstance, it->_1);

        mapviewer.GetMapViewScale(mapViewInstance, current_scale, minmax);
        TRACE_INFO("Current zoom: %" PRIu16 " (%" PRIu16 ", %" PRIu32 ", %" PRIu32 "), minmax: %" PRId32,
            scaleList[current_scale]._1, scaleList[current_scale]._2, scaleList[current_scale]._3, scaleList[current_scale]._4,
            minmax);
    }

    sleep(2);
    TRACE_INFO("setting zoom %" PRIu16, scaleList[old_scale]._1);
    mapviewer.SetMapViewScale(mapViewerSession, mapViewInstance, scaleList[old_scale]._1);
    sleep(2);

    mapviewer.SetFollowCarMode(mapViewerSession, mapViewInstance, false);
    TRACE_INFO("GetFollowCarMode: %d", mapviewer.GetFollowCarMode(mapViewInstance));
    sleep(10);
    mapviewer.SetFollowCarMode(mapViewerSession, mapViewInstance, true);
    TRACE_INFO("GetFollowCarMode: %d", mapviewer.GetFollowCarMode(mapViewInstance));
    sleep(10);

    TRACE_INFO("PauseSimulation");
    navicore.PauseSimulation(navicoreSession);
    sleep(2);

    TRACE_INFO("SetSimulationMode false");
    navicore.SetSimulationMode(navicoreSession, false);
    sleep(2);

    TRACE_INFO("DeleteRoute %" PRIu32 ", %" PRIu32, navicoreSession, route);
    navicore.DeleteRoute(navicoreSession, route);

    TRACE_INFO("calling ReleaseMapViewInstance");
    mapviewer.ReleaseMapViewInstance(mapViewerSession, mapViewInstance);
    TRACE_INFO("calling DeleteSession (mapViewer)");
    mapviewer.DeleteSession(mapViewerSession);
    TRACE_INFO("calling DeleteSession (navicore)");
    navicore.DeleteSession(navicoreSession);

    TRACE_WARN("end");

/*
    try
    {
      client.Cat("foobar");
    }
    catch(DBus::Error &e)
    {
      cout << "Caught error: " << e.name() << endl
       << "\t" << e.message() << endl;
    }
*/
  return 0;
}
