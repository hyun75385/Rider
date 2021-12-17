
#ifndef UTILS_H
#define UTILS_H

#include "defs.h"

/* main.h에 정의된 전역 변수 사용 */
extern App app;
extern BIKE bike;

extern char score_text[BUFSIZE];
extern int score;

void update_Tire(BIKE *bike);
void update_Body(BIKE *bike);
double distance(const Vecter *p1, const Vecter *p2);
double size(const Vecter* v);

#endif