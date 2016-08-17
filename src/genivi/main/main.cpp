#include <cmath>
#include <stdio.h>
#include <string.h>

#include "navi_genivi.h"
#include "route.h"
#include "guide.h"
#include "server.h"

extern "C" {
    #include "navicore.h"
    #include "glview.h"
    //#include "png.h"
    //#include "font.h"
    //#include "navi.h"
    #include "HMI_Icon.h"
    #include "NaviTrace.h"
    #include "SMCoreMP/MP_Def.h" /* button */
    #include "SMCoreMP/MP_GL.h"  /* button */
}

using namespace std;

#define APP_NAME_TEXT "navi_genivi - sample GPS Navigation with Genivi API (build date: " __DATE__ ", " __TIME__ ")"

#define NAVI_HOME_PATH			    "/home/"
#define NAVI_DATA_PATH			    "navi_data/"
#define NAVI_CONFIG_PATH_JAPAN		NAVI_HOME_PATH NAVI_DATA_PATH "japan_TR9/"
#define NAVI_CONFIG_PATH_UK			NAVI_HOME_PATH NAVI_DATA_PATH "uk_TR6/"
#define NAVI_CONFIG_PATH_GERMANY	NAVI_HOME_PATH NAVI_DATA_PATH "germany_TR6/"
#define NAVI_CONFIG_PATH_NEVADA		NAVI_HOME_PATH NAVI_DATA_PATH "nevada_TR6/"

#define GESTURE_FLICK_TIMER_ID			(1000)
#define GESTURE_LONG_PRESS_TIMER_ID		(1001)

const char *navi_config_hmi_udi_data_path	= NAVI_HOME_PATH NAVI_DATA_PATH "HMI/";
const char *navi_config_hmi_udi_info_file	= NAVI_HOME_PATH NAVI_DATA_PATH "HMI/udi_info";

#define NAVI_DATA_PATH_SIZE     256
//char navi_config_path[NAVI_DATA_PATH_SIZE]={};
//char navi_config_map_db_path[NAVI_DATA_PATH_SIZE]={};
//char navi_config_user_data_path[NAVI_DATA_PATH_SIZE]={};
//char navi_config_map_udi_data_path[NAVI_DATA_PATH_SIZE]={};
//char navi_config_map_udi_info_file[NAVI_DATA_PATH_SIZE]={};

/* keep this to keep compatibility with font.cpp... */
char navi_config_map_font_file[NAVI_DATA_PATH_SIZE]={};

/*static*/ NaviContext g_ctx;

/* function declarations: */
static void sample_hmi_set_pin_mode(int pin);
static int sample_hmi_keyboard_handle_key(unsigned int key, unsigned int state);
void sample_hmi_draw_compass(FLOAT rotate);

/* TODO: Fix this, this is crapy dirty */
/*void sample_hmi_request_update(void)
{
    g_ctx.display.hmi.sample_hmi_request_update();
}*/

/*INT32 BitmapFontCallBack(NCBITMAPFONTINFO* pInfo)
{
	int result = 0;

	initializeBitmapFont();
	setColor(pInfo->color);
	setOutLineColor(pInfo->outLineColor);
	setBkgdColor(pInfo->bkgdColor);

	result = createBitmapFont(
			pInfo->str, pInfo->fontSize, pInfo->outLineFlg,
			&pInfo->pBitMap,
			&pInfo->width, &pInfo->height,
			&pInfo->strWidth, &pInfo->strHeight,
			pInfo->rotation, true);

	if (result != 0) {
		return (0);
	}
	return (1);
}

INT32 ReadImageForFileCallBack(NCBITMAPINFO* pInfo)
{
	pInfo->pBitMap = (UChar *)readPngData(pInfo->path, &pInfo->width, &pInfo->height);
	if (NULL == pInfo->pBitMap) {
		return (0);
	}
	return (1);
}

INT32 SetImageForMemoryCallBack(NCBITMAPINFO* pInfo)
{
	pInfo->pBitMap = (UChar *)setPngData(pInfo->image, pInfo->dataSize, &pInfo->width, &pInfo->height);
	if (NULL == pInfo->pBitMap) {
		return (0);
	}
	return (1);
}

INT32 MapDrawEndCallBack(NCDRAWENDINFO* pInfo)
{
	sample_hmi_draw_compass(pInfo->rotate);
	sample_hmi_request_update();
	return (1);
}*/

////////////////////////////////

////////////////////////////////
/* former button.c            */
////////////////////////////////

/*#include <stdlib.h>
#include <stdio.h>

#include "navicore.h"

#include "SMCoreMP/MP_Def.h"
#include "SMCoreMP/MP_GL.h"

#include "glview.h"
#include "navi.h"
#include "HMI_Icon.h"*/

/*extern GLVContext glv_hmi_context;
extern GLVContext glv_map_context;*/

#define DEMO_BUTTON_OWNPOSI			(0)
#define DEMO_BUTTON_ROUTE			(1)
#define DEMO_BUTTON_COMPASS			(2)
#define DEMO_BUTTON_GUIDE_START		(3)
#define DEMO_BUTTON_GUIDE_END		(4)
#define DEMO_BUTTON_SCALE_UP		(5)
#define DEMO_BUTTON_SCALE_DOWN		(6)
#define DEMO_BUTTON_MAX				(7)

typedef struct _demo_button {
	int			type;
	int			x1,y1;
	int			x2,y2;
	int			w,h;
	int			offset;
	int			visible;
	int			on;
	float		scale;
} DEMO_BUTTON_t;

static DEMO_BUTTON_t	demo_button[DEMO_BUTTON_MAX];
static int current_push_buttonId=-1;
static int old_push_buttonId=-1;
static int current_push_buttonOn=0;
static int old_push_buttonOn=0;

static void sample_hmi_init_button(int WinWidth,int WinHeight)
{
	DEMO_BUTTON_t	*button;

	button = &demo_button[DEMO_BUTTON_OWNPOSI];
	button->type = DEMO_BUTTON_OWNPOSI;
	button->scale = 0.8f;
	button->offset = 10;
	button->h	= 128 * button->scale;
	button->w	= 128 * button->scale;
	button->x1	= WinWidth  - button->w - 20;
	button->y1	= WinHeight - button->h - 30 - 20;
	button->x2	= button->x1 + button->w;
	button->y2	= button->y1 + button->h;
	button->visible = 0;
	button->on = 0;

	button = &demo_button[DEMO_BUTTON_ROUTE];
	button->type = DEMO_BUTTON_ROUTE;
	button->scale = 0.8f;
	button->offset = 10;
	button->h	= 128 * button->scale;
	button->w	= 128 * button->scale;
	button->x1	= WinWidth  - button->w - 20;
	button->y1	= 30;
	button->x2	= button->x1 + button->w;
	button->y2	= button->y1 + button->h;
	button->visible = 0;
	button->on = 0;

	button = &demo_button[DEMO_BUTTON_COMPASS];
	button->type = DEMO_BUTTON_COMPASS;
	button->scale = 0.8f;
	button->offset = 0;
	button->h	= 128 * button->scale;
	button->w	= 128 * button->scale;
	button->x1	= 10;
	button->y1	= 10;
	button->x2	= button->x1 + button->w;
	button->y2	= button->y1 + button->h;
	button->visible = 1;
	button->on = 0;

	button = &demo_button[DEMO_BUTTON_GUIDE_START];
	button->type = DEMO_BUTTON_GUIDE_START;
	button->scale = 0.8f;
	button->offset = 10;
	button->h	= 128 * button->scale;
	button->w	= 128 * button->scale;
	button->x1	= WinWidth  - button->w - 20;
	button->y1	= 30;
	button->x2	= button->x1 + button->w;
	button->y2	= button->y1 + button->h;
	button->visible = 0;
	button->on = 0;

	button = &demo_button[DEMO_BUTTON_GUIDE_END];
	button->type = DEMO_BUTTON_GUIDE_END;
	button->scale = 0.8f;
	button->offset = 10;
	button->h	= 128 * button->scale;
	button->w	= 128 * button->scale;
	button->x1	= WinWidth  - button->w - 20;
	button->y1	= 30;
	button->x2	= button->x1 + button->w;
	button->y2	= button->y1 + button->h;
	button->visible = 0;
	button->on = 0;

	button = &demo_button[DEMO_BUTTON_SCALE_UP];
	button->type = DEMO_BUTTON_SCALE_UP;
	button->scale = 0.8f;
	button->offset = 10;
	button->h	= 128 * button->scale;
	button->w	= 128 * button->scale;
	button->x1	= 30;
	button->y1	= WinHeight - button->h - 30 - 30 - button->h - 20;
	button->x2	= button->x1 + button->w;
	button->y2	= button->y1 + button->h;
	button->visible = 1;
	button->on = 0;

	button = &demo_button[DEMO_BUTTON_SCALE_DOWN];
	button->type = DEMO_BUTTON_SCALE_DOWN;
	button->scale = 0.8f;
	button->offset = 10;
	button->h	= 128 * button->scale;
	button->w	= 128 * button->scale;
	button->x1	= 30;
	button->y1	= WinHeight - button->h - 30 - 20;
	button->x2	= button->x1 + button->w;
	button->y2	= button->y1 + button->h;
	button->visible = 1;
	button->on = 0;
}

int sample_hmi_check_button_area(int x,int y,int on)
{
	DEMO_BUTTON_t	*button;
	int i;
	current_push_buttonId = -1;
	current_push_buttonOn = 0;
	for(i=0;i<DEMO_BUTTON_MAX;i++){
		button = &demo_button[i];
		if(button->visible  != 1) continue;
		if( (button->x1 < x ) && (button->x2 > x) &&
			(button->y1 < y ) && (button->y2 > y) 	){
			printf("check button = %d\n",button->type);
			current_push_buttonId = button->type;
			current_push_buttonOn = on;
			return(button->type);
		}
	}
	return(-1);
}

void sample_hmi_set_button_visible(int type,int visible)
{
	demo_button[type].visible = visible;
}

static void sample_hmi_set_pin_mode(int pin)
{
	g_ctx.display.hmi.set_pin = pin;
}

int sample_hmi_get_pin_mode(void)
{
	return g_ctx.display.hmi.set_pin;
}

void hmiMP_GL_Init(void)
{
#ifdef NAVI_OPENGLES1
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glDisable(GL_LIGHTING);
	glShadeModel(GL_FLAT);
#endif

	glDisable(GL_DITHER);		// ディザを無効化
	glDisable(GL_DEPTH_TEST);

	// 頂点配列の使用を許可
	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

    glClearColor(0.0, 0.0, 0.0, 0.0);
	// 画面クリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void hmiMP_GL_DrawSquares(FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
	static T_POINT squares[4];

	squares[0].x = x;
	squares[0].y = y;
	squares[1].x = x;
	squares[1].y = y + height;
	squares[2].x = x + width;
	squares[2].y = y;
	squares[3].x = x + width;
	squares[3].y = y + height;

	MP_GL_Draw(GL_TRIANGLE_STRIP, squares, 4);
}


void sample_hmi_draw_compass(FLOAT rotate)
{
#if 0
	typedef enum _E_SC_RESULT {
		e_SC_RESULT_SUCCESS = 0x00000000,
	} E_SC_RESULT;

	E_SC_RESULT MP_ICON_Draw(FLOAT x, FLOAT y, FLOAT angle, FLOAT scale, INT32 iconID);
#endif

	MP_GL_PushMatrix();
	MP_GL_BeginBlend();

	{
		DEMO_BUTTON_t	*button;
		float x,y,w,h,offset,scale;

		button = &demo_button[DEMO_BUTTON_COMPASS];
		scale	= button->scale;
		offset	= button->offset;
		w		= button->w;
		h		= button->h;
		x		= button->x1;
		y		= button->y1;
		//hmiMP_GL_DrawSquares(x,y,w - offset,h - offset);
		if(g_ctx.display.hmi.compass == 0){
			mapMP_ICON_Draw(x + (w/2) - offset,y + (h/2) - offset,rotate,scale,89); // 回転用コンパス

		}else{
			mapMP_ICON_Draw(x + (w/2) - offset,y + (h/2) - offset,0.0f,scale,90); // 回転用コンパス
		}
	}

	MP_GL_EndBlend();
	MP_GL_PopMatrix();
}

void sample_hmi_update(GLVContext glv_ctx)
{
	int updateFlag = 0;
	int new_car;

	/* ------------------------------------------ */
	new_car = NC_MP_GetMapMoveWithCar(NC_MP_MAP_MAIN);
	if(g_ctx.display.hmi.move_with_car != new_car){
		updateFlag = 1;
		g_ctx.display.hmi.move_with_car = new_car;
	}
	/* ------------------------------------------ */
	if(g_ctx.display.hmi.old_set_pin != g_ctx.display.hmi.set_pin){
		updateFlag = 1;
		g_ctx.display.hmi.old_set_pin = g_ctx.display.hmi.set_pin;
	}
	/* ------------------------------------------ */
	if(old_push_buttonId != current_push_buttonId){
		updateFlag = 1;
		old_push_buttonId = current_push_buttonId;
	}
	/* ------------------------------------------ */
	if(old_push_buttonOn != current_push_buttonOn){
		updateFlag = 1;
		old_push_buttonOn = current_push_buttonOn;
	}
	/* ------------------------------------------ */
	if(g_ctx.display.hmi.force_update_flag == 1){	/* 強制更新 */
		g_ctx.display.hmi.force_update_flag = 0;
		updateFlag = 1;
	}
	/* ------------------------------------------ */
	if(updateFlag == 0) return;

	printf("----------[HMI Update]--------------\n");
	sample_hmi_set_button_visible(DEMO_BUTTON_OWNPOSI,0);
	sample_hmi_set_button_visible(DEMO_BUTTON_ROUTE,0);
	sample_hmi_set_button_visible(DEMO_BUTTON_GUIDE_START,0);
	sample_hmi_set_button_visible(DEMO_BUTTON_GUIDE_END,0);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* ----------------------------------------------------------- */
    {
		MP_GL_PushMatrix();

		MP_GL_BeginBlend();

		glColor4f(0.0, 0.0, 0.0, 0.6);

		{
			DEMO_BUTTON_t	*button;
			float x,y,w,h,offset,scale;
			button = &demo_button[DEMO_BUTTON_SCALE_UP];
			scale	= button->scale;
			offset	= button->offset;
			w		= button->w;
			h		= button->h;
			x		= button->x1;
			y		= button->y1;
			hmiMP_GL_DrawSquares(x,y,w - offset,h - offset);
			if((current_push_buttonId == DEMO_BUTTON_SCALE_UP) && (old_push_buttonOn == 1)){
				hmiMP_ICON_Draw(x + (w/2) - offset,y + (h/2) - offset,0.0f,scale,26); //
			}else{
				hmiMP_ICON_Draw(x + (w/2) - offset,y + (h/2) - offset,0.0f,scale,25); //
			}
		}

		MP_GL_EndBlend();
		MP_GL_PopMatrix();
    }
    /* ----------------------------------------------------------- */
    {
		MP_GL_PushMatrix();

		MP_GL_BeginBlend();

		glColor4f(0.0, 0.0, 0.0, 0.6);

		{
			DEMO_BUTTON_t	*button;
			float x,y,w,h,offset,scale;
			button = &demo_button[DEMO_BUTTON_SCALE_DOWN];
			scale	= button->scale;
			offset	= button->offset;
			w		= button->w;
			h		= button->h;
			x		= button->x1;
			y		= button->y1;
			hmiMP_GL_DrawSquares(x,y,w - offset,h - offset);
			if((current_push_buttonId == DEMO_BUTTON_SCALE_DOWN) && (old_push_buttonOn == 1)){
				hmiMP_ICON_Draw(x + (w/2) - offset,y + (h/2) - offset,0.0f,scale,24); //
			}else{
				hmiMP_ICON_Draw(x + (w/2) - offset,y + (h/2) - offset,0.0f,scale,23); //
			}
		}

		MP_GL_EndBlend();
		MP_GL_PopMatrix();
    }
    /* ----------------------------------------------------------- */
    if(g_ctx.display.hmi.move_with_car == 0){
		MP_GL_PushMatrix();
		MP_GL_BeginBlend();

		glColor4f(0.0, 0.0, 0.0, 0.6);

		{
			DEMO_BUTTON_t	*button;
			float x,y,w,h,offset,scale;
			button = &demo_button[DEMO_BUTTON_OWNPOSI];
			scale	= button->scale;
			offset	= button->offset;
			w		= button->w;
			h		= button->h;
			x		= button->x1;
			y		= button->y1;
			hmiMP_GL_DrawSquares(x,y,w - offset,h - offset);
			if((current_push_buttonId == DEMO_BUTTON_OWNPOSI) && (old_push_buttonOn == 1)){
				hmiMP_ICON_Draw(x + (w/2) - offset,y + (h/2) - offset,0.0f,scale,30); //
			}else{
				hmiMP_ICON_Draw(x + (w/2) - offset,y + (h/2) - offset,0.0f,scale,29); //
			}
			sample_hmi_set_button_visible(DEMO_BUTTON_OWNPOSI,1);
		}

		MP_GL_EndBlend();
		MP_GL_PopMatrix();
    }
    /* ----------------------------------------------------------- */
    if(NC_DM_IsExistRoute() == 1){
    	/* 経路がある */
    	if(NC_Simulation_IsInSimu() == 1){
    		/* 終了ボタンを表示する */
    		MP_GL_PushMatrix();
    		MP_GL_BeginBlend();
    		glColor4f(0.0, 0.0, 0.0, 0.6);
    		{
    			DEMO_BUTTON_t	*button;
    			float x,y,w,h,offset,scale;
    			button = &demo_button[DEMO_BUTTON_GUIDE_END];
    			scale	= button->scale;
    			offset	= button->offset;
    			w		= button->w;
    			h		= button->h;
    			x		= button->x1;
    			y		= button->y1;
    			hmiMP_GL_DrawSquares(x,y,w - offset,h - offset);
    			if((current_push_buttonId == DEMO_BUTTON_GUIDE_END) && (old_push_buttonOn == 1)){
    				hmiMP_ICON_Draw(x + (w/2) - offset,y + (h/2) - offset,0.0f,scale,22); //
    			}else{
    				hmiMP_ICON_Draw(x + (w/2) - offset,y + (h/2) - offset,0.0f,scale,21); //
    			}
    			sample_hmi_set_button_visible(DEMO_BUTTON_GUIDE_END,1);
    		}

    		MP_GL_EndBlend();
    		MP_GL_PopMatrix();
    	}else{
    		/* 開始ボタンを表示する */
    		MP_GL_PushMatrix();
    		MP_GL_BeginBlend();
    		glColor4f(0.0, 0.0, 0.0, 0.6);
    		{
    			DEMO_BUTTON_t	*button;
    			float x,y,w,h,offset,scale;
    			button = &demo_button[DEMO_BUTTON_GUIDE_START];
    			scale	= button->scale;
    			offset	= button->offset;
    			w		= button->w;
    			h		= button->h;
    			x		= button->x1;
    			y		= button->y1;
    			hmiMP_GL_DrawSquares(x,y,w - offset,h - offset);
    			if((current_push_buttonId == DEMO_BUTTON_GUIDE_START) && (old_push_buttonOn == 1)){
    				hmiMP_ICON_Draw(x + (w/2) - offset,y + (h/2) - offset,0.0f,scale,27); //
    			}else{
    				hmiMP_ICON_Draw(x + (w/2) - offset,y + (h/2) - offset,0.0f,scale,28); //
    			}
    			sample_hmi_set_button_visible(DEMO_BUTTON_GUIDE_START,1);
    		}

    		MP_GL_EndBlend();
    		MP_GL_PopMatrix();
    	}
    }else
    if(g_ctx.display.hmi.set_pin == 1){
		MP_GL_PushMatrix();
		MP_GL_BeginBlend();

		glColor4f(0.0, 0.0, 0.0, 0.6);
		{
			DEMO_BUTTON_t	*button;
			float x,y,w,h,offset,scale;
			button = &demo_button[DEMO_BUTTON_ROUTE];
			scale	= button->scale;
			offset	= button->offset;
			w		= button->w;
			h		= button->h;
			x		= button->x1;
			y		= button->y1;
			hmiMP_GL_DrawSquares(x,y,w - offset,h - offset);
			if((current_push_buttonId == DEMO_BUTTON_ROUTE) && (old_push_buttonOn == 1)){
				hmiMP_ICON_Draw(x + (w/2) - offset,y + (h/2) - offset,0.0f,scale,28); //
			}else{
				hmiMP_ICON_Draw(x + (w/2) - offset,y + (h/2) - offset,0.0f,scale,27); //
			}
			sample_hmi_set_button_visible(DEMO_BUTTON_ROUTE,1);
		}

		MP_GL_EndBlend();
		MP_GL_PopMatrix();
    }
	glvSwapBuffers(glv_ctx);
}

void sample_hmi_init(GLVContext glv_ctx)
{
	int width,height;
	g_ctx.naviGetResolution(&width,&height);
	sample_hmi_init_button(width,height);
	// プロジェクション行列の設定
	MP_GL_MatrixProjection();
	// 座標体系設定
	MP_GL_Orthof(0, width, height, 0, -1, 1);
	MP_GL_MatrixModelView();
	sample_hmi_update(glv_ctx);
}

void sample_hmi_request_action(int buttonId)
{
	switch(buttonId){
	case DEMO_BUTTON_OWNPOSI:
		NC_MP_SetMapMoveWithCar(NC_MP_MAP_MAIN,1);
		glvOnReDraw(g_ctx.display.map_context);
		break;
	case DEMO_BUTTON_ROUTE:
        sample_calc_demo_route(g_ctx.display);
		break;
	case DEMO_BUTTON_COMPASS:
		if(g_ctx.display.hmi.compass== 0){
			g_ctx.display.hmi.compass = 1;
		}else{
			g_ctx.display.hmi.compass = 0;
		}
		NC_MP_SetMapDispMode(NC_MP_MAP_MAIN, g_ctx.display.hmi.compass);
		if(g_ctx.display.hmi.compass == 1){
			NC_MP_SetMapRotate(NC_MP_MAP_MAIN,0);
		}
		glvOnReDraw(g_ctx.display.map_context);
		break;
	case DEMO_BUTTON_GUIDE_START:
		sample_guide_request_start();
		break;
	case DEMO_BUTTON_GUIDE_END:
		sample_guide_request_end();
		break;
	case DEMO_BUTTON_SCALE_UP:
		   if(g_ctx.main_window_mapScale < g_ctx.display.hmi.map_max_scale){
			   g_ctx.main_window_mapScale++;
				   if (g_ctx.main_window_mapScale == 1) {
					   g_ctx.main_window_mapScale++;
				   }
				   if(g_ctx.main_window_mapScale > g_ctx.display.hmi.map_max_scale) g_ctx.main_window_mapScale = g_ctx.display.hmi.map_max_scale;
				   NC_MP_SetMapScaleLevel(NC_MP_MAP_MAIN, g_ctx.main_window_mapScale);
                   glvOnReDraw(g_ctx.display.map_context);
		   }
		break;
	case DEMO_BUTTON_SCALE_DOWN:
  	   if(g_ctx.main_window_mapScale > 0){
  		   g_ctx.main_window_mapScale--;
				   if (g_ctx.main_window_mapScale == 1) {
					   g_ctx.main_window_mapScale--;
				   }
				   if(g_ctx.main_window_mapScale < 0) g_ctx.main_window_mapScale = 0;
				  NC_MP_SetMapScaleLevel(NC_MP_MAP_MAIN, g_ctx.main_window_mapScale);
				  glvOnReDraw(g_ctx.display.map_context);
  	   }
		break;
	default:
		break;
	}
}

static int hmi_onmap_button=0;
static int hmi_onmap_buttonId=-1;

int sample_hmi_button_down(int pointer_sx,int pointer_sy)
{
	hmi_onmap_buttonId = sample_hmi_check_button_area(pointer_sx,pointer_sy,1);
	if(hmi_onmap_buttonId > -1){
		printf("sample_hmi_check_button_area (%d)\n",hmi_onmap_buttonId);
		hmi_onmap_button = 1;
		g_ctx.display.hmi.sample_hmi_request_update();
		return(1);
	}
	return(0);
}

int sample_hmi_button_up(int pointer_sx,int pointer_sy)
{
	if(hmi_onmap_button == 1){
		hmi_onmap_button = 0;
		hmi_onmap_buttonId = sample_hmi_check_button_area(pointer_sx,pointer_sy,0);
		g_ctx.display.hmi.sample_hmi_request_update();
		sample_hmi_request_action(hmi_onmap_buttonId);
		return(1);
	}
	return(0);
}
////////////////////////////////////////////////////

#define PI	(3.1415926)
static float calcAngleBaseS(float x, float y)
{
		double acos1 = acos(x / hypot(x, y));
		double radian = (y < 0) ? PI * 2 - acos1 : acos1;
		float angle = (float) ((radian * 180.0 / PI) + 90.0);
		//if(angle > 360.0) angle -= 360.0;
		return angle;
}

static void usage(void)
{
	printf("Usage:\n");
//	printf("  --map  [ uk | nevada ]      set the map region\n");
//	printf("           ~~                 default\n");
	printf("  --r    [ fhd | hd | agl ]   set the resolution\n");
	printf("                      ~~~     default\n");
	printf("  --width  WIDTH              width  of wayland surface\n");
	printf("  --height HEIGHT             height of wayland surface\n");
	printf("  --data   data_path          set the map data path\n");
	printf("  --help                      this help message\n");
}

static int main_arg(int argc, char *argv[])
{
	int width, height;
	int i,n;
	char *home;
	home = getenv("NAVI_DATA_DIR");

	if(home != 0){
		/*strcpy(navi_config_path, home);
		if ((navi_config_path[strlen(navi_config_path) - 1] != '/') &&
				(sizeof(navi_config_path) > (strlen(navi_config_path) + 1))) {
			strcat(navi_config_path, "/");
		}*/
        g_ctx.navi_config_path = std::string(home);
        if (g_ctx.navi_config_path.back() != '/') g_ctx.navi_config_path += "/";
		g_ctx.region = NAVI_REGION_OPTIONAL;
	}

	for(i = 1; i < argc; i++) {
		if(strcmp(argv[i], "-display") == 0) {
			g_ctx.display.name = std::string(argv[i+1]);
			i++;
		}
		else if(strcmp(argv[i], "-debug") == 0) {
			glvSetDebugFlag(GLV_DEBUG_ON);
		}
		else if (strcmp(argv[i], "--r") == 0) {
			if(strcmp(argv[i+1], "fhd") == 0) {
				g_ctx.resolution = NAVI_RESOLUTION_FullHD;
			}else if(strcmp(argv[i+1], "hd") == 0) {
				g_ctx.resolution = NAVI_RESOLUTION_HD;
			}else if(strcmp(argv[i+1], "agl") == 0) {
				g_ctx.resolution = NAVI_RESOLUTION_AGL_DEMO;
			}
			i++;
		}
		else if (strcmp(argv[i], "--width") == 0) {
			n = sscanf(argv[i+1],"%d", &width);
			if(n ==1){
				g_ctx.resolution = NAVI_RESOLUTION_OPTIONAL;
				g_ctx.WinWidth = width;
			}
			i++;
		}
		else if (strcmp(argv[i], "--height") == 0) {
			n = sscanf(argv[i+1],"%d", &height);
			if(n ==1){
				g_ctx.resolution = NAVI_RESOLUTION_OPTIONAL;
				g_ctx.WinHeight = height;
			}
			i++;
		}
		else if (strcmp(argv[i], "--map") == 0) {
			if(strcmp(argv[i+1], "japan") == 0) {
				g_ctx.region = NAVI_REGION_JAPAN;
			}else if(strcmp(argv[i+1], "uk") == 0) {
				g_ctx.region = NAVI_REGION_UK;
			}else if(strcmp(argv[i+1], "nevada") == 0) {
				g_ctx.region = NAVI_REGION_NEVADA;
			}else if(strcmp(argv[i+1], "germany") == 0) {
				g_ctx.region = NAVI_REGION_GERMANY;
			}
			i++;
		}
		else if (strcmp(argv[i], "--data") == 0) {
			/*strcpy(navi_config_path,argv[i+1]);
			if ((navi_config_path[strlen(navi_config_path) - 1] != '/') &&
					(sizeof(navi_config_path) > (strlen(navi_config_path) + 1))) {
				strcat(navi_config_path, "/");
			}*/
            g_ctx.navi_config_path = std::string(argv[i+1]);
            if (g_ctx.navi_config_path.back() != '/') g_ctx.navi_config_path += "/";
        
			g_ctx.region = NAVI_REGION_OPTIONAL;
			i++;
		}
		else if (strcmp(argv[i], "--help") == 0) {
			usage();
			return(-1);
		}
		else if(strcmp(argv[i], "-hmiimageout") == 0) {
			hmiMP_ICON_CreateResImage();
		}
		else if(strcmp(argv[i], "-hmiimageload") == 0) {
			g_ctx.display.hmi.sample_load_image_file = 1;
		}
		else {
			usage();
			return(-1);
		}
	}
	return(0);
}

void NaviContext::naviStartUpResolution(void)
{
	/* ---------------------------------------------------------------------------------- */
	switch(resolution)
    {
        case NAVI_RESOLUTION_FullHD:	// Full-HD	(1080p)
            g_ctx.WinWidth	= 1920;
            g_ctx.WinHeight	= 1080;
            break;
        case NAVI_RESOLUTION_HD:		// HD		( 720p)
            g_ctx.WinWidth	= 1280;
            g_ctx.WinHeight	= 720;
            break;
        case NAVI_RESOLUTION_AGL_DEMO:		// CES 2016 demo
            g_ctx.WinWidth	= 1080;
            g_ctx.WinHeight	= 1670;
            break;
        default:
            break;
	}
	TRACE_DEBUG("window size: %dx%d", g_ctx.WinWidth, g_ctx.WinHeight);
	/* ---------------------------------------------------------------------------------- */
}

void NaviContext::naviStartUpRegion(void)
{
	/* ---------------------------------------------------------------------------------- */
	switch(region){
        case NAVI_REGION_JAPAN:
            //strcpy(navi_config_path, NAVI_CONFIG_PATH_JAPAN);
            g_ctx.navi_config_path = std::string(NAVI_CONFIG_PATH_JAPAN);
            break;
        case NAVI_REGION_UK:
            //strcpy(navi_config_path, NAVI_CONFIG_PATH_UK);
            g_ctx.navi_config_path = std::string(NAVI_CONFIG_PATH_UK);
            break;
        case NAVI_REGION_GERMANY:
            //strcpy(navi_config_path, NAVI_CONFIG_PATH_GERMANY);
            g_ctx.navi_config_path = std::string(NAVI_CONFIG_PATH_GERMANY);
            break;
        case NAVI_REGION_NEVADA:
            //strcpy(navi_config_path, NAVI_CONFIG_PATH_NEVADA);
            g_ctx.navi_config_path = std::string(NAVI_CONFIG_PATH_NEVADA);
            break;
        default:
            break;
	}

	/*strcat(navi_config_map_db_path			,navi_config_path); strcat(navi_config_map_db_path			,"Map/JPN/");
	strcat(navi_config_user_data_path		,navi_config_path); strcat(navi_config_user_data_path		,"Data");
	strcat(navi_config_map_udi_data_path    ,navi_config_path); strcat(navi_config_map_udi_data_path	,"Data/MD/UDI/");
	strcat(navi_config_map_udi_info_file	,navi_config_path); strcat(navi_config_map_udi_info_file	,"Data/MD/UDI/udi_info");
	strcat(navi_config_map_font_file		,navi_config_path); strcat(navi_config_map_font_file		,"IPAfont00303/ipagp.ttf");*/
    g_ctx.navi_config_map_db_path = g_ctx.navi_config_path + std::string("Map/JPN/");
    g_ctx.navi_config_user_data_path = g_ctx.navi_config_path + std::string("Data");
    g_ctx.navi_config_map_udi_data_path = g_ctx.navi_config_path + std::string("Data/MD/UDI/");
    g_ctx.navi_config_map_udi_info_file = g_ctx.navi_config_path + std::string("Data/MD/UDI/udi_info");
    g_ctx.navi_config_map_font_file = g_ctx.navi_config_path + std::string("IPAfont00303/ipagp.ttf");

    /* TODO: workaround to keep compatibility with font.cpp: */
    //strncpy(navi_config_map_font_file, g_ctx.navi_config_map_font_file.c_str(), NAVI_DATA_PATH_SIZE)[NAVI_DATA_PATH_SIZE-1] = '\0';
    strcpy(&navi_config_map_font_file[0], g_ctx.navi_config_map_font_file.c_str());
    
	TRACE_INFO("navi_config_path             (%s)", navi_config_path.c_str());
	TRACE_INFO("navi_config_map_db_path      (%s)", navi_config_map_db_path.c_str());
	TRACE_INFO("navi_config_user_data_path   (%s)", navi_config_user_data_path.c_str());
	TRACE_INFO("navi_config_map_udi_data_path(%s)", navi_config_map_udi_data_path.c_str());
	TRACE_INFO("navi_config_map_udi_info_file(%s)", navi_config_map_udi_info_file.c_str());
	TRACE_INFO("navi_config_map_font_file    (%s)", navi_config_map_font_file.c_str());

    if(region == NAVI_REGION_JAPAN) {
		display.hmi.map_max_scale = 12;
	} else {
		display.hmi.map_max_scale = 11;
	}
}

/** 初期移動速度割合（8%） */
static float MOVE_RATIO = 0.08f;
/** 移動到達地点倍率（1倍） */
static float GOLE_RATIO = 0.8f;
/** 到達地点補正閾値-短（900） */
static long GOLE_SHORT_CORRECT_LINE = 900;
/** 到達地点補正倍率-短（0.4倍） */
static float GOLE_SHORT_CORRECT_RATIO = 0.4f;
/** 到達地点補正閾値-中（1700） */
static int GOLE_MIDIUM_CORRECT_LINE = 1700;
/** 到達地点補正倍率-中（0.8倍） */
static float GOLE_MIDIUM_CORRECT_RATIO = 0.8f;

static float mAngle;
static float mSpeedRatio;
static double mBaseSpeed;
static double mBaseDistance;
static int mMoveDistance;
static int mGoleDistance;

static void run_scroll(GLVContext glv_ctx,int distanceX,int distanceY)
{
	float	angle;
	int		distance;

	NC_MP_SetMapMoveWithCar(NC_MP_MAP_MAIN,0);
	glvStopTimer(glv_ctx,GESTURE_FLICK_TIMER_ID);

	angle = calcAngleBaseS(-distanceX,-distanceY);
	distance = (int)round(hypot((double)distanceX, (double)distanceY));

	NC_MP_MoveMapDir(NC_MP_MAP_MAIN,angle ,distance);
	NC_MP_RefreshMap(NC_MP_MAP_MAIN);
	glvSwapBuffers(glv_ctx);
}

static void run_flick(GLVContext glv_ctx)
{
	mSpeedRatio = (float) mMoveDistance / (float) mGoleDistance;
	if (1 < mSpeedRatio) {
		glvStopTimer(glv_ctx,GESTURE_FLICK_TIMER_ID);
		return;
	}
	float ratio = (1.0f - mSpeedRatio * mSpeedRatio);
	int distance = (int) round(mBaseSpeed * ratio);

	NC_MP_MoveMapDir(NC_MP_MAP_MAIN,mAngle ,distance);
	NC_MP_RefreshMap(NC_MP_MAP_MAIN);
	glvSwapBuffers(glv_ctx);

	mMoveDistance += distance;

	if (1 > mBaseSpeed * ratio || 0 == distance) {
		//scrollEnd();
		glvStopTimer(glv_ctx,GESTURE_FLICK_TIMER_ID);
	}
}

static int map_long_press_x;
static int map_long_press_y;

int map_gesture(GLVContext glv_ctx,int maps,int eventType,int x,int y,int distanceX,int distanceY,int velocityX,int velocityY)
{
	glvStopTimer(glv_ctx,GESTURE_LONG_PRESS_TIMER_ID);
	switch(eventType)
	 {
	   case    GLV_GESTURE_EVENT_DOWN:
		   TRACE_INFO("GESTURE:[down(%d,%d)]",x,y);
		   glvStartTimer(glv_ctx,GESTURE_LONG_PRESS_TIMER_ID);
		   map_long_press_x = x;
		   map_long_press_y = y;
		   break;
	   case    GLV_GESTURE_EVENT_SINGLE_UP:
		   TRACE_INFO("GESTURE:[single up(%d,%d) vector:%d %d]",x,y,distanceX,distanceY);
		   break;
	   case    GLV_GESTURE_EVENT_SCROLL:
		   TRACE_INFO("GESTURE:[scroll(%d,%d) vector:%d %d]",x,y,distanceX,distanceY);
		   run_scroll(glv_ctx,distanceX,distanceY);
		   break;
	   case    GLV_GESTURE_EVENT_FLING:
		   TRACE_INFO("GESTURE:[flick(%d,%d) vector:%d %d velocity:%d %d]",x,y,distanceX,distanceY,velocityX,velocityY);
		   {
				mMoveDistance = 0;
				mBaseDistance = hypot((double)velocityX, (double)velocityY) * GOLE_RATIO;
				mGoleDistance = round(mBaseDistance);
				if (GOLE_SHORT_CORRECT_LINE > mGoleDistance) {
					mGoleDistance *= GOLE_SHORT_CORRECT_RATIO;
				} else if (GOLE_MIDIUM_CORRECT_LINE > mGoleDistance) {
					mGoleDistance *= GOLE_MIDIUM_CORRECT_RATIO;
				}

				mBaseSpeed = mGoleDistance * MOVE_RATIO;
				mAngle = calcAngleBaseS(-velocityX, -velocityY);
		   }
		   NC_MP_SetMapMoveWithCar(NC_MP_MAP_MAIN,0);
		   run_flick(glv_ctx);
		   glvStartTimer(glv_ctx,GESTURE_FLICK_TIMER_ID);
		   break;
	   case    GLV_GESTURE_EVENT_SCROLL_STOP:
		   TRACE_INFO("GESTURE:[scroll end(%d,%d) vector:%d %d]",x,y,distanceX,distanceY);
		   run_scroll(glv_ctx,distanceX,distanceY);
		   break;
	   default:
		   break;
	 }
	return(GLV_OK);
}

int map_timer(GLVContext glv_ctx, int maps, int group, int id)
{
	if(id == GESTURE_FLICK_TIMER_ID){
		run_flick(glv_ctx);
	}else if(GESTURE_LONG_PRESS_TIMER_ID){
		SMGEOCOORD	geoCood;
		if(NC_Simulation_IsInSimu() != 0){
			/* シミュレーション中 */
			return(GLV_OK);
		}
		TRACE_DEBUG("TIMER:long press");
		NC_MP_ScreenToGeoCode(NC_MP_MAP_MAIN,map_long_press_x, map_long_press_y, &geoCood);
		TRACE_INFO("TIMER:latitude(%ld) , longitude(%ld)",geoCood.latitude,geoCood.longitude);
		sample_set_demo_icon_pin_flag(g_ctx.display.hmi, &geoCood);
		NC_MP_RefreshMap(NC_MP_MAP_MAIN);
		glvSwapBuffers(glv_ctx);
	}
	return(GLV_OK);
}

int map_init(GLVContext glv_ctx, int maps)
{
	NC_MP_SetUDIResource(/*(Char*)*/g_ctx.navi_config_map_udi_data_path.c_str(), /*(Char*)*/g_ctx.navi_config_map_udi_info_file.c_str());

	NC_MP_InitResource(maps);

	NC_MP_SetMapViewport(maps,0,0, g_ctx.WinWidth, g_ctx.WinHeight);

	sample_init_demo_route_icon();

	return(GLV_OK);
}

int map_reshape(GLVContext glv_ctx, int maps, int width, int height)
{
	NC_MP_SetMapViewport(maps,0,0,width,height);
	return(GLV_OK);
}

int map_redraw(GLVContext glv_ctx, int maps)
{
	NC_MP_RefreshMap(maps);
	return(GLV_OK);
}

int hmi_init(GLVContext glv_ctx, int maps)
{
    hmiMP_GL_Init();

    if(g_ctx.display.hmi.sample_load_image_file == 1){
    	TRACE_INFO("load image file (%s)",navi_config_hmi_udi_data_path);
    	//	HMIで使用するイメージはイメージファイルを読み込んで使用する
    	hmiMP_ICON_Load(const_cast<char*>(navi_config_hmi_udi_data_path), const_cast<char*>(navi_config_hmi_udi_info_file));
    }else{
    	//	HMIで使用するイメージはプログラム中のイメージデータを使用する
    	hmiMP_ICON_Initialize();
    }

	sample_hmi_init(glv_ctx);

	return(GLV_OK);
}

int hmi_update(GLVContext glv_ctx,int maps)
{
	sample_hmi_update(glv_ctx);
	return(GLV_OK);
}


int main(int argc, char *argv[])
{
	GLVDisplay	glv_dpy;
	GLVWindow	glv_map_window;
	GLVWindow	glv_hmi_window;
	int rc;

	rc = main_arg(argc,argv);
	if(0 != rc){
		return -1;
	}

    TRACE_INFO("%s", APP_NAME_TEXT);

    dbusServerLoop(NULL); // add CDR
#if 0
	g_ctx.naviStartUpResolution();  // CDR - navicore (1) - NaviContext
	g_ctx.naviStartUpRegion();      // CDR - navicore (1) - NaviContext

	glv_dpy = glvOpenDisplay(g_ctx.display.name);   // CDR - navicore (1) - NaviContext.display
	if(!glv_dpy){
		TRACE_ERROR("glvOpenDisplay() failed");
		return -1;
	}
	/* ----------------------------------------------------------------------------------------------- */
	NC_MP_SetBitmapFontCB(BitmapFontCallBack);                  // CDR - navicore (1) - NaviContext
	NC_MP_SetImageReadForFileCB(ReadImageForFileCallBack);      // CDR - navicore (1) - NaviContext
	NC_MP_SetImageReadForImageCB(SetImageForMemoryCallBack);    // CDR - navicore (1) - NaviContext
	NC_MP_SetMapDrawEndCB(MapDrawEndCallBack);                  // CDR - navicore (1) - NaviContext

	rc = NC_Initialize(g_ctx.WinWidth, g_ctx.WinHeight,         // CDR - navicore (1) - NaviContext
            navi_config_user_data_path, navi_config_map_db_path, "locatorPath");
	if(NC_SUCCESS != rc){
		TRACE_ERROR("NC_Initialize error");
		TRACE_ERROR("\ttuser data path: %s", navi_config_user_data_path);
		TRACE_ERROR("\tmap db path: %s", navi_config_map_db_path);
		return -1;
	}

	NC_MP_SetMapMoveWithCar(NC_MP_MAP_MAIN,1);                          // CDR - mapviewer (3) - MapContext (vect)
	NC_MP_SetMapScaleLevel(NC_MP_MAP_MAIN, g_ctx.main_window_mapScale); // CDR - mapviewer (3) - MapContext (vect)
	/* ----------------------------------------------------------------------------------------------- */
	glv_input_func.keyboard_key = sample_hmi_keyboard_handle_key;       // CDR - navicore (1) - NaviContext
	glv_input_func.touch_down   = sample_hmi_button_down;               // CDR - navicore (1) - NaviContext
	glv_input_func.touch_up     = sample_hmi_button_up;                 // CDR - navicore (1) - NaviContext

	glv_map_window = glvCreateNativeWindow(glv_dpy, 0, 0, g_ctx.WinWidth, g_ctx.WinHeight, NULL);           // CDR - mapviewInstance (4) - MapContext (vect)
	glv_hmi_window = glvCreateNativeWindow(glv_dpy, 0, 0, g_ctx.WinWidth, g_ctx.WinHeight, glv_map_window); // CDR - navicore (1) - NaviContext

	glvInitTimer();

	/* ----------------------------------------------------------------------------------------------- */
	g_ctx.display.SurfaceViewEventFunc.init		= map_init;     // CDR - mapviewInstance (4) - MapContext (vect)
	g_ctx.display.SurfaceViewEventFunc.reshape	= map_reshape;
	g_ctx.display.SurfaceViewEventFunc.redraw	= map_redraw;
	g_ctx.display.SurfaceViewEventFunc.update	= NULL;
	g_ctx.display.SurfaceViewEventFunc.timer	= map_timer;
	g_ctx.display.SurfaceViewEventFunc.gesture	= map_gesture;

	g_ctx.display.map_context = glvCreateSurfaceView(glv_map_window, NC_MP_MAP_MAIN, &g_ctx.display.SurfaceViewEventFunc);  // CDR - mapviewInstance (4)

	g_ctx.display.hmi.hmi_SurfaceViewEventFunc.init		= hmi_init; // CDR - mapviewInstance (4)
	g_ctx.display.hmi.hmi_SurfaceViewEventFunc.reshape	= NULL;
	g_ctx.display.hmi.hmi_SurfaceViewEventFunc.redraw	= NULL;
	g_ctx.display.hmi.hmi_SurfaceViewEventFunc.update	= hmi_update;
	g_ctx.display.hmi.hmi_SurfaceViewEventFunc.timer	= NULL;
	g_ctx.display.hmi.hmi_SurfaceViewEventFunc.gesture	= NULL;

	g_ctx.display.hmi.hmi_context = glvCreateSurfaceView(glv_hmi_window, NC_MP_MAP_MAIN, &g_ctx.display.hmi.hmi_SurfaceViewEventFunc);  // CDR - mapviewInstance (4)

	/* ----------------------------------------------------------------------------------------------- */
	glvCreateTimer(g_ctx.display.map_context,1000,GESTURE_FLICK_TIMER_ID     ,GLV_TIMER_REPEAT   ,  50);    // CDR - navicore (1) - NaviContext.display
	glvCreateTimer(g_ctx.display.map_context,1000,GESTURE_LONG_PRESS_TIMER_ID,GLV_TIMER_ONLY_ONCE, 700);    // CDR - navicore (1) - NaviContext.display
	/* ----------------------------------------------------------------------------------------------- */

	sample_createGuideThread(&g_ctx.display); // CDR Route (2) - joinable thread, join at route destruction

    createDbusServerThread();

	glvOnReDraw(g_ctx.display.map_context);

	glvEventLoop(glv_dpy);

	glvDestroyNativeWindow(glv_map_window);
	glvDestroyNativeWindow(glv_hmi_window);

	glvCloseDisplay(glv_dpy);
#endif
	return(0);
}

/*static int sample_hmi_keyboard_handle_key(
                    unsigned int key,
                    unsigned int state)
{
	if(g_ctx.display.map_context == 0) {
		return 0;
	}
    
    TRACE_INFO("Key is %d, state is %d", key, state);
    if(state == GLV_KEYBOARD_KEY_STATE_PRESSED) {
    	switch(key) {
    	case 103:		// '↑'
		   if(g_ctx.main_window_mapScale < g_ctx.display.hmi.map_max_scale){
			   g_ctx.main_window_mapScale++;
				   if (g_ctx.main_window_mapScale == 1) {
					   g_ctx.main_window_mapScale++;
				   }
				   if(g_ctx.main_window_mapScale > g_ctx.display.hmi.map_max_scale)
                        g_ctx.main_window_mapScale = g_ctx.display.hmi.map_max_scale;
				  NC_MP_SetMapScaleLevel(NC_MP_MAP_MAIN, g_ctx.main_window_mapScale);
				  glvOnReDraw(g_ctx.display.map_context);
		   }
			break;
		case 108:		// '↓'
     	   if(g_ctx.main_window_mapScale > 0){
     		   g_ctx.main_window_mapScale--;
				   if (g_ctx.main_window_mapScale == 1) {
					   g_ctx.main_window_mapScale--;
				   }
				   if(g_ctx.main_window_mapScale < 0)
                       g_ctx.main_window_mapScale = 0;
				  NC_MP_SetMapScaleLevel(NC_MP_MAP_MAIN, g_ctx.main_window_mapScale);
				  glvOnReDraw(g_ctx.display.map_context);
     	   }
     	   break;
		case 25:		// 'p'
			// route search
            sample_calc_demo_route(g_ctx.display);
			break;
		case 30:		// 'a'
			// own posi mode
			NC_MP_SetMapMoveWithCar(NC_MP_MAP_MAIN,1);
			glvOnReDraw(g_ctx.display.map_context);
			break;
    	}
    }
    return(0);
}*/
