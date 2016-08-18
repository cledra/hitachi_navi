#ifndef ROUTE_H
#define ROUTE_H

#include "navi_genivi.h"

int sample_calc_demo_route(DisplayContext &ctxDisplay, Route &route);
void sample_set_demo_icon_pin_flag(HmiContext &ctxHmi, SMGEOCOORD *geoCood);
void sample_init_demo_route_icon(void);
void sample_clear_demo_route_icon(void);

#endif
