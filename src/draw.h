
#ifndef DRAW_H
#define DRAW_H

#include "defs.h"
#include "utils.h"

/* main.h에 정의된 전역 변수 사용 */
extern App app;
extern BIKE bike;
extern char score_text[BUFSIZE];
extern int score;

void ClearWindow(void);

void ShowWindow(void);

void DrawGame(void);

void DrawGameOver(void);


void RenderTire(const BIKE * bike);
void RenderBikeBody(const BIKE *bike);


#endif