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
#include "navicore-session.h"

DBus::BusDispatcher dispatcher;

int main()
{
    DBus::default_dispatcher = &dispatcher;
    DBus::Connection conn = DBus::Connection::SessionBus();
    NavicoreSession nc_Session(conn, "/org/genivi/navicore_session", "org.agl.gpsnavi");

    printf("CreateSession() --> %d\n", nc_Session.CreateSession(std::string("Test1")) );
    printf("CreateSession() --> %d\n", nc_Session.CreateSession(std::string("Test2")) );
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
