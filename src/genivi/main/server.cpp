#include <stdio.h>
#include <signal.h>
#include "navicore.h"
#include "mapviewer.h"

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

    Navicore naviCore(conn);
    Mapviewer mapViewer(conn);

    dispatcher.enter();
}
