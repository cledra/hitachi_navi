#include <dbus-c++/dbus.h>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <getopt.h>
#include "navicore.h"
#include "mapviewer.h"
#include "../main/NaviTrace.h"
#include "../main/genivi-mapviewer-constants.h"

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
    ::DBus::Struct< uint16_t, uint16_t > resolution;
    resolution._1 = 1280;
    resolution._2 = 720;
    char opt;

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

    ::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > version;

    version = navicore.GuidanceGetVersion();
    TRACE_INFO("Navicore version : %s : .%" PRIu16 ".%" PRIu16 ".%" PRIu16,
        version._4.c_str(), version._3, version._2, version._1);
    
    version = mapviewer.MapViewerControlGetVersion();
    TRACE_INFO("MapViewer version : %s : .%" PRIu16 ".%" PRIu16 ".%" PRIu16,
        version._4.c_str(), version._3, version._2, version._1);

    navicoreSession = navicore.CreateSession(std::string("AW-navicore"));
    TRACE_INFO("navicore.CreateSession() --> %" PRIu32, navicoreSession);

    sleep(3);

    mapViewerSession = mapviewer.CreateSession(std::string("AW-mapviewer"));
    TRACE_INFO("mapviewer.CreateSession() --> %" PRIu32, mapViewerSession);

    sleep(3);

    mapViewInstance = mapviewer.CreateMapViewInstance(mapViewerSession, resolution, MAPVIEWER_MAIN_MAP);
    TRACE_INFO("mapviewer.CreateMapViewInstance(%" PRIu16 ":%" PRIu16 ") --> %" PRIu32,
        resolution._1, resolution._2, mapViewInstance);

    sleep(10);

    TRACE_INFO("calling ReleaseMapViewInstance");
    mapviewer.ReleaseMapViewInstance(mapViewerSession, mapViewInstance);
    sleep(2);
    TRACE_INFO("calling DeleteSession (mapViewer)");
    mapviewer.DeleteSession(mapViewerSession);
    sleep(2);
    TRACE_INFO("calling DeleteSession (navicore)");
    navicore.DeleteSession(navicoreSession);
    sleep(2);

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
