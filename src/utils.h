
#ifndef UTILS_H
#define UTILS_H

#include "defs.h"

/* main.h에 정의된 전역 변수 사용 */
extern App app;
extern BIKE bike;
extern Feature ** flist;
extern int fnum;
extern bool GAME_END;
extern int SCORE;



void update_Tire(BIKE *bike);
void update_Body(BIKE *bike);
double distance(const Vecter *p1, const Vecter *p2);
double size(const Vecter* v);
bool issameVec(const Vecter * v1, const Vecter * v2);
void update_tire_vel(BIKE *bike);
Vecter vecter_minus(const Vecter * v1, const Vecter * v2);
double CrossProduct(const Vecter *v1, const Vecter *v2);
double InnerProduct(const Vecter *v1, const Vecter *v2);
void update_Center(BIKE *bike);
Vecter vecter_plus(const Vecter * v1, const Vecter * v2);
Vecter vecter_mul(const Vecter * v1, double m);
Vecter bounce(const Vecter * v1, const Vecter * normal);
void update_Center_theta(BIKE * bike);
Vecter WxR(double w, const Vecter *  r);
void ground(int i,int chechf[]);
void zero(int i,int chechf[],double);
void first(int i,int chechf[],double,double);
void second(int i,int chechf[],double,double,double);

#endif