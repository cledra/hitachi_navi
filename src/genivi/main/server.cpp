#include <stdio.h>
#include <signal.h>
#include "genivi-navicore.h"
#include "genivi-mapviewer.h"
#include "NaviTrace.h"

DBus::BusDispatcher dispatcher;

void leave_signal_handler(int sig)
{
    dispatcher.leave();
}

/*static void *dbusServerThread(void *no_arg)
{
    dbusServerLoop(no_arg);
}*/

void dbusServerLoop(void *no_arg)
{
    signal(SIGTERM, leave_signal_handler);
    signal(SIGINT, leave_signal_handler);

    DBus::default_dispatcher = &dispatcher;
    DBus::Connection conn = DBus::Connection::SessionBus();
    conn.request_name("org.agl.gpsnavi");

    Navicore naviCore(conn);
    Mapviewer mapViewer(conn);
    TRACE_DEBUG("DBus server loop initialized");

    dispatcher.enter();
}

/*int createDbusServerThread(void)
{
    pthread_t p;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int ret = pthread_create(&p, &attr, dbusServerThread, NULL);
    if (ret != 0)
        TRACE_ERROR("pthread_create for dbusServerThread failed (err: %s)",
            ret == EAGAIN ? "EAGAIN" :
            ret == EINVAL ? "EINVAL" :
            ret == EPERM   ? "EPERM" : "UNKNOWN");
    pthread_attr_destroy(&attr);

    return ret;
}*/
