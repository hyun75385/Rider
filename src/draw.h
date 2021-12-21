
#ifndef DRAW_H
#define DRAW_H

#include "defs.h"
#include "utils.h"

/* main.h에 정의된 전역 변수 사용 */
extern App app;
extern BIKE bike;
extern Feature ** flist;
extern int fnum;
extern bool GAME_END;
extern int SCORE;
extern Entity background;
extern char score_text[20];
extern Text score_board;  




void ClearWindow(void);

void ShowWindow(void);

void DrawGame(void);

void DrawGameOver(void);


void RenderTire(const BIKE * bike);
void RenderBikeBody( BIKE *bike);
void RenderFeature_0(const Feature * f);
void RenderFeature_1(const Feature * f);
void RenderFeature_2(const Feature *f);
void RenderScoreBoard(Text *object);


#endif