#ifndef WAYPOINT_HPP
#define WAYPOINT_HPP

class WayPoint
{
    public:
        WayPoint(double lat_, double lon_):
            lat(lat_), lon(lon_) {}

        double lat;
        double lon;
};

#endif
