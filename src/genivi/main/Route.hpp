#ifndef ROUTE_HPP
#define ROUTE_HPP

#include "WayPoint.hpp"

class Route
{
    public:
        Route(uint32_t handle_):
            handle(handle_){}
        

        uint32_t handle;
        std::vector<WayPoint> WayPoints;
};

#endif
