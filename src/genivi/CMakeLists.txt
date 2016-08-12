cmake_minimum_required(VERSION 2.8.11)
INCLUDE(FindPkgConfig)

project(dbus-test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

pkg_check_modules(DBUSCXX REQUIRED dbus-c++-1)

include_directories( ${DBUSCXX_INCLUDE_DIRS} )

add_executable(client client/client.cpp)

add_executable(server server/server.cpp
                      server/mapviewer-session.cpp
                      server/mapviewer-control.cpp
                      server/navicore-mapmatching.cpp
                      server/navicore-guidance.cpp
                      server/navicore-session.cpp
                      server/navicore-routing.cpp )

target_link_libraries( client ${DBUSCXX_LIBRARIES} )
target_link_libraries( server ${DBUSCXX_LIBRARIES} )

install(TARGETS server
        RUNTIME DESTINATION bin
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                    GROUP_EXECUTE GROUP_READ
                    WORLD_EXECUTE WORLD_READ
        COMPONENT base)
