#ifndef NAVI_GENIVI_H
#define NAVI_GENIVI_H

#define GESTURE_FLICK_TIMER_ID			1000
#define GESTURE_LONG_PRESS_TIMER_ID		1001

void naviStartUpResolution(int resolution); 
void naviStartUpRegion(int region);
INT32 BitmapFontCallBack(NCBITMAPFONTINFO* pInfo);
INT32 ReadImageForFileCallBack(NCBITMAPINFO* pInfo);
INT32 SetImageForMemoryCallBack(NCBITMAPINFO* pInfo);
INT32 MapDrawEndCallBack(NCDRAWENDINFO* pInfo);

#endif
