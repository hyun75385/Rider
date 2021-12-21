#ifndef DEFS_H
#define DEFS_H

#include "ctype.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

/*상수 매크로 정의*/
#define FPS 60              /**< 게임 FPS*/
#define BUFSIZE 1024        /**< 문자열 버퍼 크기*/
#define DEBUG_MODE 0

#if (DEBUG_MODE==1)
#define SCREEN_WIDTH 3000    /**< 화면 너비(픽셀)640*/
#elif (DEBUG_MOD==0)
#define SCREEN_WIDTH 960    /**< 화면 너비(픽셀)640*/
#endif
#define SCREEN_HEIGHT 960   /**< 화면 높이(픽셀)960*/

#define FEATURENUM 10

#define MAP_WIDTH 10000
#define MAP_HEIGHT 960

#define PLAYER_WIDTH 24     /**< 플레이어 객체 너비(픽셀)*/
#define PLAYER_HEIGHT 24    /**< 플레이어 객체 높이(픽셀)*/
#define PLAYER_SPEED 0.2     /**< 가속도*/
#define PLAYER_BRAKE 0.01  /*브레이크*/
#define PLAYER_MAXSPEED 20   /**< 최대 속도*/
#define PLAYER_ROT 0.005   /**< 각가속도*/
#define PLAYER_MAXROT 0.1   /**< 최대 각속도*/


#define GRAVITY -0.1
#define E 0.3//충돌계수
#define PI 3.14159265359
#define GAP 4

#define abs(X) (((X)>=0) ? (X) : (-(X)))

#define FONTSIZE 20 /**< 출력할 문자열 폰트 크기*/
typedef enum {false,true} bool;
typedef enum {front=1,back,body} part_num;

typedef struct {
    SDL_Renderer *renderer; /**< 렌더링 관리를 위한 구조체*/
    SDL_Window *window;     /**< 창 관리를 위한 구조체*/
    TTF_Font *font;         /**< 폰트 관리를 위한 구조체*/
    int key_space;             /**< 스페이스 키가 눌린 상태를 저장하는 변수*/
} App;

typedef struct vecter{
    double x;
    double y;
}Vecter;




typedef struct circle
{  
    Vecter offset;
    Vecter pose;
    Vecter vel;
    double radius;
    double min_d; //for collaspe detect
    part_num part;

}Tire;

typedef struct rect{
    Vecter offset;
    Vecter pose[4];
    Vecter r[4];
    Vecter vel;
    double min_d;
    part_num part;
}RECT;


typedef struct bike{
    SDL_Texture * texture;
    SDL_Rect size;
    Vecter pose;
    Vecter vel;
    Vecter acc;
    
    double theta; //rad // for randering

    double theta2;
    double omega;
    double alpa;


    RECT body;
    Tire front;
    Tire back;

}BIKE;


typedef struct feature{
    int dim;
    Vecter pose;
    double  value[4]; //계수 저장
    double limit[2]; //x1,x2저장
}Feature;

typedef struct crash{
    double iner;
    Vecter normal;
    Vecter pose;
    part_num part;
}Crash;
#endif