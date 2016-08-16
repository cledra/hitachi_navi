/***************************************************************************
 *   Copyright (C) 2007,2010 by Rick L. Vinyard, Jr.                       *
 *   rvinyard@cs.nmsu.edu                                                  *
 *                                                                         *
 *   This file is part of the dbus-cxx library.                            *
 *                                                                         *
 *   The dbus-cxx library is free software; you can redistribute it and/or *
 *   modify it under the terms of the GNU General Public License           *
 *   version 3 as published by the Free Software Foundation.               *
 *                                                                         *
 *   The dbus-cxx library is distributed in the hope that it will be       *
 *   useful, but WITHOUT ANY WARRANTY; without even the implied warranty   *
 *   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   *
 *   General Public License for more details.                              *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this software. If not see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/
#include <dbus-c++/dbus.h>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include "navicore.h"
#include "mapviewer.h"

DBus::BusDispatcher dispatcher;

int main()
{
    DBus::default_dispatcher = &dispatcher;
    DBus::Connection conn = DBus::Connection::SessionBus();
    Navicore navicore(conn, "/org/genivi/navicore", "org.agl.gpsnavi");
    MapViewer mapviewer(conn, "/org/genivi/mapviewer", "org.agl.gpsnavi");

    ::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > version;

    version = navicore.GuidanceGetVersion();
    std::cout << "Navicore  : " << version._4 << " : " << version._3 << "." << version._2 << "." << version._1 << std::endl;
    
    version = mapviewer.MapViewerControlGetVersion();
    std::cout << "Mapviewer : " << version._4 << " : " << version._3 << "." << version._2 << "." << version._1 << std::endl;

    std::cout << "CreateSession() -->" << navicore.CreateSession(std::string("Test1")) << std::endl;
    std::cout << "CreateSession() -->" << navicore.CreateSession(std::string("Test2")) << std::endl;

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
