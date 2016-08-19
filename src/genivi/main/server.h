#ifndef SERVER_H
#define SERVER_H

#include "genivi-navicore.h"
#include "genivi-mapviewer.h"

int createDbusServerThread(void);
void dbusServerLoop(Navicore **naviCore, Mapviewer **mapViewer);

#endif
