#include <dbus-c++/dbus.h>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include "navicore.h"
#include "mapviewer.h"
#include "../main/NaviTrace.h"
#include "../main/genivi-mapviewer-constants.h"
#include "../main/genivi-navicore-constants.h"

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include <unistd.h> /* for sleep */

#define DEFAULT_W 1280
#define DEFAULT_H 720

DBus::BusDispatcher dispatcher;

static void usage(void)
{
	printf("Usage:\n");
	printf("  -r, --resolution  [fhd|hd|agl]    set the resolution\n");
	printf("  -w, --width       WIDTH           width  of wayland surface\n");
	printf("  -h, --height      HEIGHT          height of wayland surface\n");
	printf("      --help                        this help message\n");
}

static struct option long_options[] = {
    {"width",       required_argument, 0,  'w' },
    {"height",      required_argument, 0,  'h' },
    {"resolution",  required_argument, 0,  'r' },
    {"help",        no_argument,       0,  'p' },
    {0,         0,                 0,  0 }
};

int main(int argc, char * argv[])
{
    ::DBus::Struct< uint16_t, uint16_t > resolution;
    resolution._1 = DEFAULT_W;
    resolution._2 = DEFAULT_H;
    int opt;

    while ((opt = getopt_long(argc, argv, "w:h:r:p", long_options, NULL)) != -1)
    {
        switch (opt)
        {
            case 'w':
                resolution._1 = atoi(optarg);
                break;
            case 'h':
                resolution._2 = atoi(optarg);
                break;
            case 'r':
                if (!strcmp(optarg, "hd")) {
                    resolution._1 = 1280;
                    resolution._2 = 720;
                } else if (!strcmp(optarg, "fhd")) {
                    resolution._1 = 1920;
                    resolution._2 = 1080;
                } else if (!strcmp(optarg, "agl")) {
                    resolution._1 = 1080;
                    resolution._2 = 1670;
                }
                break;
            case 'p':
                usage();
                return 0;
            default: break;
        }
    }

    DBus::default_dispatcher = &dispatcher;
    DBus::Connection conn = DBus::Connection::SessionBus();
    Navicore navicore(conn, "/org/genivi/navicore", "org.agl.gpsnavi");
    MapViewer mapviewer(conn, "/org/genivi/mapviewer", "org.agl.gpsnavi");
    uint32_t navicoreSession, mapViewerSession, mapViewInstance;
    ::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > version;

    /** Navicore::GuidanceGetVersion() */
    version = navicore.GuidanceGetVersion();
    TRACE_INFO("Navicore guidance version : %s : .%" PRIu16 ".%" PRIu16 ".%" PRIu16,
        version._4.c_str(), version._3, version._2, version._1);

    /** Navicore::RoutingGetVersion() */
    version = navicore.RoutingGetVersion();
    TRACE_INFO("Navicore routing version : %s : .%" PRIu16 ".%" PRIu16 ".%" PRIu16,
        version._4.c_str(), version._3, version._2, version._1);

    /** Mapviewer::MapViewerControlGetVersion() */
    version = mapviewer.MapViewerControlGetVersion();
    TRACE_INFO("MapViewer version : %s : .%" PRIu16 ".%" PRIu16 ".%" PRIu16,
        version._4.c_str(), version._3, version._2, version._1);

    /** Navicore::CreateSession() */
    navicoreSession = navicore.CreateSession(std::string("My Navicore Session"));
    TRACE_INFO("navicore.CreateSession() --> %" PRIu32, navicoreSession);

    /** Navicore::GetAllSessions() */
    std::vector< ::DBus::Struct< uint32_t, std::string > > navicoreAllSessions =
        navicore.GetAllSessions();
    TRACE_INFO("Current navicore sessions:");
    for (std::vector< ::DBus::Struct< uint32_t, std::string > >::iterator it =
        navicoreAllSessions.begin(); it != navicoreAllSessions.end(); it++)
    {
        TRACE_INFO("\t%" PRIu32 ", %s", it->_1, it->_2.c_str());
    }

    /** Mapviewer::CreateSession() */
    mapViewerSession = mapviewer.CreateSession(std::string("My MapViewer Session"));
    TRACE_INFO("mapviewer.CreateSession() --> %" PRIu32, mapViewerSession);

    /** Mapviewer::CreateMapViewInstance() */
    mapViewInstance = mapviewer.CreateMapViewInstance(mapViewerSession, resolution, MAPVIEWER_MAIN_MAP);
    TRACE_INFO("mapviewer.CreateMapViewInstance(%" PRIu16 ":%" PRIu16 ") --> %" PRIu32,
        resolution._1, resolution._2, mapViewInstance);

    /** Navicore::CreateRoute() */
    uint32_t route = navicore.CreateRoute(navicoreSession);
    TRACE_INFO("CreateRoute %" PRIu32 " -> %" PRIu32, navicoreSession, route);
    sleep(2);

    /** Navicore::GetAllRoutes() */
    std::vector<uint32_t> navicoreAllRoutes = navicore.GetAllRoutes();
    TRACE_INFO("Current navicore routes:");
    for (std::vector<uint32_t>::iterator it =
        navicoreAllRoutes.begin(); it != navicoreAllRoutes.end(); it++)
    {
        TRACE_INFO("\t%" PRIu32, *it);
    }

    /** Navicore::SetWaypoints() */
    TRACE_INFO("SetWaypoints (start from current position + 2 points)");
    {
        std::vector< std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > > wpList;

        // far away from original position :
        std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > point1;
        ::DBus::Struct< uint8_t, ::DBus::Variant > point1Lat;
        ::DBus::Struct< uint8_t, ::DBus::Variant > point1Lon;
        
        point1Lat._1 = NAVICORE_LATITUDE;
        point1Lat._2.writer().append_double(35.594820421);
        point1Lon._1 = NAVICORE_LONGITUDE;
        point1Lon._2.writer().append_double(139.720988227);
        
        point1[NAVICORE_LATITUDE] = point1Lat;
        point1[NAVICORE_LONGITUDE] = point1Lon;
        wpList.push_back(point1);

        // close to original position :
        std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > point2;
        ::DBus::Struct< uint8_t, ::DBus::Variant > point2Lat;
        ::DBus::Struct< uint8_t, ::DBus::Variant > point2Lon;
        
        point2Lat._1 = NAVICORE_LATITUDE;
        point2Lat._2.writer().append_double(35.589478895);
        point2Lon._1 = NAVICORE_LONGITUDE;
        point2Lon._2.writer().append_double(139.72986084);
        
        point2[NAVICORE_LATITUDE] = point2Lat;
        point2[NAVICORE_LONGITUDE] = point2Lon;
        wpList.push_back(point2);

        navicore.SetWaypoints(navicoreSession, route, true, wpList);
    }
    sleep(2);

    /** Navicore::CalculateRoute() */
    TRACE_INFO("CalculateRoute");
    navicore.CalculateRoute(navicoreSession, route);
    sleep(2);

    /** Navicore::GetWayPoints() */
    TRACE_INFO("GetWayPoints");
    {
        std::vector< std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > > wpList2;
        bool startFromCurrentPosition;
        navicore.GetWaypoints(route, startFromCurrentPosition, wpList2);

        std::vector< std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > >::const_iterator wp_map;
        for (wp_map = wpList2.begin(); wp_map != wpList2.end(); wp_map++)
        {
            std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > >::const_iterator map;
            double latitude, longitude;
            for (map = (*wp_map).begin(); map != (*wp_map).end(); map++)
            {
                if ((*map).first == NAVICORE_LATITUDE)
                {
                    latitude = (*map).second._2.reader().get_double();
                }
                else if ((*map).first == NAVICORE_LONGITUDE)
                {
                    longitude = (*map).second._2.reader().get_double();
                }
            }
            TRACE_INFO("\t(%f, %f)", latitude, longitude);
        }
    }
    sleep(5);

    /** Navicore::SetSimulationMode() */
    TRACE_INFO("SetSimulationMode true");
    navicore.SetSimulationMode(navicoreSession, true);
    sleep(2);

    /** Navicore::StartSimulation() */
    TRACE_INFO("StartSimulation");
    navicore.StartSimulation(navicoreSession);
    sleep(2);

    /** Navicore::GetSimulationStatus() */
    TRACE_INFO("GetSimulationStatus : %" PRIu32, navicore.GetSimulationStatus());

    sleep(10);

    /** Mapviewer::SetCameraHeadingAngle() */
    TRACE_INFO("SetCameraHeadingAngle");
    mapviewer.SetCameraHeadingAngle(mapViewerSession, mapViewInstance, 0);
    sleep(10);

    /** Navicore::GetPosition() */
    TRACE_INFO("GetPosition");
    {
        std::vector< int32_t > requestValues;
        requestValues.push_back(NAVICORE_LATITUDE);
        requestValues.push_back(NAVICORE_LONGITUDE);
        requestValues.push_back(NAVICORE_TIMESTAMP); /* note : not supported yet */
        requestValues.push_back(NAVICORE_HEADING);
        requestValues.push_back(NAVICORE_SPEED);
        requestValues.push_back(NAVICORE_SIMULATION_MODE);
        std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > > getPosRet;
        std::map< int32_t, ::DBus::Struct< uint8_t, ::DBus::Variant > >::iterator pr_map;
        getPosRet = navicore.GetPosition(requestValues);
        for (pr_map = getPosRet.begin(); pr_map != getPosRet.end(); pr_map++)
        {
            if (pr_map->first == NAVICORE_LATITUDE) {
                TRACE_INFO("\tlatitude:\t%f", pr_map->second._2.reader().get_double());
            } else if (pr_map->first == NAVICORE_LONGITUDE) {
                TRACE_INFO("\tlongitude:\t%f", pr_map->second._2.reader().get_double());
            } else if (pr_map->first == NAVICORE_TIMESTAMP) {
                TRACE_INFO("\ttimestamp:\%" PRIu32, pr_map->second._2.reader().get_uint32());
            } else if (pr_map->first == NAVICORE_HEADING) {
                TRACE_INFO("\theading:\t%" PRIu32, pr_map->second._2.reader().get_uint32());
            } else if (pr_map->first == NAVICORE_SPEED) {
                TRACE_INFO("\tspeed:\t%" PRId32, pr_map->second._2.reader().get_int32());
            } else if (pr_map->first == NAVICORE_SIMULATION_MODE) {
                TRACE_INFO("\tsimulation:\t%d", pr_map->second._2.reader().get_bool());
            }
        }
    }
    sleep(2);

    /** Mapviewer::SetCameraHeadingTrackUp() */
    TRACE_INFO("SetCameraHeadingTrackUp");
    mapviewer.SetCameraHeadingTrackUp(mapViewerSession, mapViewInstance);

    sleep(10);

    /** Mapviewer::GetScaleList() */
    std::vector< ::DBus::Struct< uint16_t, uint16_t, int32_t, uint32_t > > scaleList;
    std::vector< ::DBus::Struct< uint16_t, uint16_t, int32_t, uint32_t > >::iterator it;
    TRACE_INFO("Possible zoom values:");
    {
        scaleList = mapviewer.GetScaleList(mapViewInstance);
        for (it = scaleList.begin(); it != scaleList.end(); it++)
        {
            TRACE_INFO("\t%" PRIu16 " (%" PRIu16 ", %" PRIu32 ", %" PRIu32 ")",
                it->_1, it->_2, it->_3, it->_4);
        }
    }
    sleep(2);
    
    /** Mapviewer::GetMapViewScale() */
    uint8_t old_scale;
    
    {
        int32_t minmax;
        mapviewer.GetMapViewScale(mapViewInstance, old_scale, minmax);
        TRACE_INFO("GetMapViewScale: Current zoom: %" PRIu16 " (%" PRIu16 ", %" PRIu32 ", %" PRIu32 "), minmax: %" PRId32,
            scaleList[old_scale]._1, scaleList[old_scale]._2, scaleList[old_scale]._3, scaleList[old_scale]._4,
            minmax);
    }

    /** Mapviewer::SetMapViewScale() */
    for (it = scaleList.begin(); it != scaleList.end(); it++)
    {
        sleep(2);
        TRACE_INFO("setting zoom %" PRIu16, it->_1);
        mapviewer.SetMapViewScale(mapViewerSession, mapViewInstance, it->_1);
    }

    sleep(2);
    TRACE_INFO("setting zoom %" PRIu16, scaleList[old_scale]._1);
    mapviewer.SetMapViewScale(mapViewerSession, mapViewInstance, scaleList[old_scale]._1);
    sleep(2);

    /** Mapviewer::SetFollowCarMode() / Mapviewer::GetFollowCarMode() */
    mapviewer.SetFollowCarMode(mapViewerSession, mapViewInstance, false);
    TRACE_INFO("GetFollowCarMode: %d", mapviewer.GetFollowCarMode(mapViewInstance));
    sleep(10);
    mapviewer.SetFollowCarMode(mapViewerSession, mapViewInstance, true);
    TRACE_INFO("GetFollowCarMode: %d", mapviewer.GetFollowCarMode(mapViewInstance));
    sleep(10);

    /** Navicore::PauseSimulation() */
    TRACE_INFO("PauseSimulation");
    navicore.PauseSimulation(navicoreSession);
    sleep(2);

    /** Navicore::SetSimulationMode() */
    TRACE_INFO("SetSimulationMode false");
    navicore.SetSimulationMode(navicoreSession, false);
    sleep(2);

    /** Navicore::DeleteRoute() */
    TRACE_INFO("DeleteRoute %" PRIu32 ", %" PRIu32, navicoreSession, route);
    navicore.DeleteRoute(navicoreSession, route);

    /** Mapviewer::ReleaseMapViewInstance() */
    TRACE_INFO("calling ReleaseMapViewInstance");
    mapviewer.ReleaseMapViewInstance(mapViewerSession, mapViewInstance);

    /** Mapviewer::DeleteSession() */
    TRACE_INFO("calling DeleteSession (mapViewer)");
    mapviewer.DeleteSession(mapViewerSession);

    /** Navicore::DeleteSession() */
    TRACE_INFO("calling DeleteSession (navicore)");
    navicore.DeleteSession(navicoreSession);

    TRACE_WARN("end");
    return 0;
}
