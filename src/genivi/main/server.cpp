/*
 * GPS Navigation ---An open source GPS navigation core software
 *
 *
 * Copyright (c) 2016  Aisin AW, Ltd
 *
 * This program is dual licensed under GPL version 2 or a commercial license.
 * See the LICENSE file distributed with this source file.
 */
#include <stdio.h>
#include <signal.h>
#include "NaviTrace.h"
#include "server.h"

DBus::BusDispatcher dispatcher;

void leave_signal_handler(int sig)
{
    dispatcher.leave();
}

void dbusServerLoop(Navicore **naviCore, Mapviewer **mapViewer)
{
    signal(SIGTERM, leave_signal_handler);
    signal(SIGINT, leave_signal_handler);

    DBus::default_dispatcher = &dispatcher;
    DBus::Connection conn = DBus::Connection::SessionBus();
    conn.request_name("org.agl.gpsnavi");

    *naviCore = new Navicore(conn);
    *mapViewer = new Mapviewer(conn);
    TRACE_DEBUG("DBus server loop initialized");

    dispatcher.enter();
}
