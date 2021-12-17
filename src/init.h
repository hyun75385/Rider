

#ifndef INIT_H
#define INIT_H

#include "defs.h"
#include "utils.h"

/* main.h에 정의된 전역 변수 사용 */
extern BIKE bike;
extern App app;
extern Feature ** flist;
extern int fnum;



void InitSDL(void);
void QuitSDL(int flag);
void InitMemorySet(void);
void InitTTF(void);
void QuitTTF(void);

void InitBike(void);
void InitWall(void);
void InitFeature(void);


#endif