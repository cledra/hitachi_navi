#ifndef SERVER_H
#define SERVER_H

int createDbusServerThread(void);
void dbusServerLoop(Navicore **naviCore, Mapviewer **mapViewer);

#endif
