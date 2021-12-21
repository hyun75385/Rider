
#ifndef MAIN_H
#define MAIN_H

#include "defs.h"

#include "init.h"
#include "input.h"
#include "action.h"
#include "draw.h"

/*사용하는 전역 변수*/
/// @ingroup GlobalVariables
App app;                        /**< 프로그램 전체 단위로 관리하는 전역 객체 모음 */
/// @ingroup GlobalVariables
char score_text[20];       /**< 스코어보드에 출력할 문자열*/
/// @ingroup GlobalVariables

/// @ingroup GlobalVariables
BIKE bike;                        /**< 바이크의 속성을 설명하는 BIKE형 구조체 > */
/// @ingroup GlobalVariables
Feature ** flist;
bool GAME_END;
int SCORE;
Entity background;
Text score_board;  
char score_text[20];   
#endif