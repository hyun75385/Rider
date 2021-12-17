

#ifndef INIT_H
#define INIT_H

#include "defs.h"
#include "utils.h"

/* main.h에 정의된 전역 변수 사용 */
extern BIKE bike;
extern App app;



void InitSDL(void);

void InitTTF(void);


void QuitSDL(int flag);

void QuitTTF(void);


void InitMemorySet(void);


void InitScoreBoard(void);


void InitPlayer(void);


void InitGameOver(void);


void InitBike(void);
void InitWall(void);

#endif