#ifndef ACTION_H
#define ACTION_H

#include "init.h"
#include "defs.h"
#include "utils.h"

/* main.h에 정의된 전역 변수 사용 */
extern App app;
extern BIKE bike;
extern Feature ** flist;
extern int fnum;
extern bool GAME_END;


void ActGame(void);
void ActBike(void);
void Updatepose(void);
void physics(void);
int closestFeature(void);
void detect_contact(const Feature *f);
double cal_crash_tire(double x, const Feature *f, const Tire * tire);
double cal_y(double x, const Feature *f);
double cal_diff(double x, const Feature *f);
Vecter cal_normal(double diff);
double cal_circle_y(double x, const Tire * tire);
Vecter adjust_pose(void);
Vecter Force(void);

#endif