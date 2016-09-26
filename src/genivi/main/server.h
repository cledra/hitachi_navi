/*
 * GPS Navigation ---An open source GPS navigation core software
 *
 *
 * Copyright (c) 2016  Aisin AW, Ltd
 *
 * This program is dual licensed under GPL version 2 or a commercial license.
 * See the LICENSE file distributed with this source file.
 */
#ifndef SERVER_H
#define SERVER_H

#include "genivi-navicore.h"
#include "genivi-mapviewer.h"

void dbusServerLoop(Navicore **naviCore, Mapviewer **mapViewer);

#endif
