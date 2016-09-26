/*
 * GPS Navigation ---An open source GPS navigation core software
 *
 *
 * Copyright (c) 2016  Aisin AW, Ltd
 *
 * This program is dual licensed under GPL version 2 or a commercial license.
 * See the LICENSE file distributed with this source file.
 */
#ifndef NAVI_GENIVI_H
#define NAVI_GENIVI_H

#define GESTURE_FLICK_TIMER_ID			1000
#define GESTURE_LONG_PRESS_TIMER_ID		1001

#define NAVI_REGION_OPTIONAL	-1
#define NAVI_REGION_JAPAN		0
#define NAVI_REGION_UK			1
#define NAVI_REGION_GERMANY	    2
#define NAVI_REGION_NEVADA		3

void naviStartUpResolution(int resolution);
void naviStartUpRegion(int region);
INT32 BitmapFontCallBack(NCBITMAPFONTINFO* pInfo);
INT32 ReadImageForFileCallBack(NCBITMAPINFO* pInfo);
INT32 SetImageForMemoryCallBack(NCBITMAPINFO* pInfo);
INT32 MapDrawEndCallBack(NCDRAWENDINFO* pInfo);

#endif
