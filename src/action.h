#ifndef ACTION_H
#define ACTION_H

#include "init.h"
#include "defs.h"
#include "utils.h"

/* main.h에 정의된 전역 변수 사용 */
extern App app;
extern BIKE bike;

void ActGame(void);
void ActBike(void);
void Updatepose(void);

#endif