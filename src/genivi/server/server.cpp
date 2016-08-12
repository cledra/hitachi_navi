#include <stdio.h>
#include <signal.h>
#include "navicore-session.h"
#include "navicore-routing.h"
#include "navicore-guidance.h"
#include "navicore-mapmatching.h"
#include "mapviewer-session.h"
#include "mapviewer-control.h"

DBus::BusDispatcher dispatcher;

void leave_signal_handler(int sig)
{
    dispatcher.leave();
}

int main(int argc, char *argv[])
{
    signal(SIGTERM, leave_signal_handler);
    signal(SIGINT, leave_signal_handler);

    DBus::default_dispatcher = &dispatcher;
    DBus::Connection conn = DBus::Connection::SessionBus();
    conn.request_name("org.agl.gpsnavi");

    NavicoreSession nc_session(conn);
    NavicoreRouting nc_routing(conn);
    NavicoreMapmatching nc_mapmatching(conn);
    NavicoreGuidance nc_guidance(conn);
    MapviewerSession mv_session(conn);
    MapviewerControl mv_control(conn);

    dispatcher.enter();
}
