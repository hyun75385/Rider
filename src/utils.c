
#include "utils.h"


void update_Tire(BIKE *bike)
{
    bike->front.pose.x = bike->pose.x + cos(bike->theta) * (bike->front.offset.x) - sin(bike->theta) * (bike->front.offset.y);
    bike->front.pose.y = bike->pose.y + sin(bike->theta) * (bike->front.offset.x) + cos(bike->theta) * (bike->front.offset.y);
    bike->back.pose.x = bike->pose.x + cos(bike->theta) * (bike->back.offset.x) - sin(bike->theta) * (bike->back.offset.y);
    bike->back.pose.y = bike->pose.y + sin(bike->theta) * (bike->back.offset.x) + cos(bike->theta) * (bike->back.offset.y);
    return;
}

void update_Center(BIKE *bike)
{
    bike->pose.x = (bike->front.pose.x + bike->back.pose.x) / 2 + bike->front.offset.y * sin(bike->theta);
    bike->pose.y = (bike->front.pose.y + bike->back.pose.y) / 2 - bike->front.offset.y * cos(bike->theta);
    bike->size.x = (int)bike->pose.x-32;
    bike->size.y = (int)bike->pose.y-32;
    return;
}

void update_Body(BIKE *bike)
{
    for (int i = 0; i < 4; i++)
    {
        bike->body.pose[i].x = bike->pose.x + cos(bike->theta) * (bike->body.offset.x + bike->body.r[i].x) - sin(bike->theta) * (bike->body.offset.y + bike->body.r[i].y);
        bike->body.pose[i].y = bike->pose.y + cos(bike->theta) * (bike->body.offset.y + bike->body.r[i].y) + sin(bike->theta) * (bike->body.offset.x + bike->body.r[i].x);
    }
    return;
}

void update_tire_vel(BIKE *bike)
{
    //1. 상대속도, 위치 구하기
    Vecter re_vel, re_pos, are_pos, bre_pos;
    ;
    double inner, cross, sizep, tmp;
    re_vel = vecter_minus(&bike->front.vel, &bike->back.vel);
    re_pos = vecter_minus(&bike->front.pose, &bike->back.pose);
    // printf("repos %f %f revel %f %f\n", re_pos.x, re_pos.y, re_vel.x, re_vel.y);
    inner = InnerProduct(&re_pos, &re_vel);
    sizep = size(&re_pos);

    //2.속도 재조정
    SDL_assert(sizep != 0);
    bike->front.vel.x -= inner * re_pos.x / (2 * sizep * sizep);
    bike->front.vel.y -= inner * re_pos.y / (2 * sizep * sizep);
    bike->back.vel.x += inner * re_pos.x / (2 * sizep * sizep);
    bike->back.vel.y += inner * re_pos.y / (2 * sizep * sizep);
    printf("omega : %f\n", bike->omega);
    //
    //3. 추가 각속도(omega)를 통해 normal 성분한번더 업데이트
    // printf("repos : %f %f \n",re_pos.x,re_pos.y);
    are_pos = vecter_mul(&re_pos, 0.5);
    bre_pos = vecter_mul(&re_pos, -0.5);

    // //front와 back에 더해지는 속도 저장
    // printf("arepos : %f %f brepos %f %f\n",are_pos.x,are_pos.y,bre_pos.x,bre_pos.y);
    are_pos = WxR(-bike->alpa, &are_pos);
    bre_pos = WxR(-bike->alpa, &bre_pos);
    // printf("arepos : %f %f brepos %f %f\n",are_pos.x,are_pos.y,bre_pos.x,bre_pos.y);
    bike->front.vel.x += are_pos.x;
    bike->front.vel.y += are_pos.y;
    bike->back.vel.x += bre_pos.x;
    bike->back.vel.y += bre_pos.y;

    //4.최종 각속도 저장
    tmp = bike->front.vel.x / (-are_pos.y) 
    + bike->front.vel.y / (are_pos.x)
    -(bike->back.vel.x / (-bre_pos.y) + bike->back.vel.y / (bre_pos.x));
    printf("tmp : %f\n", tmp);

}

void update_Center_theta(BIKE *bike)
{
    Vecter re_pos, xuint = {1, 0};
    double sizep;
    re_pos = vecter_minus(&bike->front.pose, &bike->back.pose);
    //3. 외적 구하기(모멘트 계산)
    sizep = size(&re_pos);
    // printf("sizep : %f\n",sizep);
    if (re_pos.y < 0)
        bike->theta2 = acos(InnerProduct(&xuint, &re_pos) / sizep);
    else if (re_pos.y > 0)
        bike->theta2 = 2 * PI - acos(InnerProduct(&xuint, &re_pos) / sizep);
    else
    {
        if (re_pos.x > 0)
            bike->theta2 = 0;
        else
            bike->theta2 = PI;
    }
}

Vecter bounce(const Vecter *v1, const Vecter *normal)
{
    Vecter tan = {-normal->y, normal->x}, v2, tmp1, tmp2;
    // printf("normal x %f y %f tan x %f y %f\n",normal->x,normal->y, tan.x,tan.y);
    double inner, cross;
    inner = InnerProduct(v1, normal);
    cross = InnerProduct(v1, &tan);
    // printf("inner %f, cross %f\n", inner, cross);
    if (inner > 0)
        tmp1 = vecter_mul(normal, inner);
    else
        tmp1 = vecter_mul(normal, -inner * E);

    tmp2 = vecter_mul(&tan, cross);
    return vecter_plus(&tmp1, &tmp2);
}

double distance(const Vecter *p1, const Vecter *p2)
{
    return (sqrt((p1->x - p2->x) * (p1->x - p2->x) + (p1->y - p2->y) * (p1->y - p2->y)));
}

double size(const Vecter *v)
{
    return (sqrt(v->x * v->x + v->y * v->y));
}

bool issameVec(const Vecter *v1, const Vecter *v2)
{
    return (bool)(abs(v1->x - v2->x) < 0.000001 && abs(v1->y - v2->y) < 0.000001);
}

Vecter vecter_minus(const Vecter *v1, const Vecter *v2)
{
    Vecter v;
    v.x = v1->x - v2->x;
    v.y = v1->y - v2->y;
    return v;
}

Vecter vecter_plus(const Vecter *v1, const Vecter *v2)
{
    Vecter v;
    v.x = v1->x + v2->x;
    v.y = v1->y + v2->y;
    return v;
}

Vecter vecter_mul(const Vecter *v1, double m)
{
    Vecter v;
    v.x = v1->x * m;
    v.y = v1->y * m;
    return v;
}

double InnerProduct(const Vecter *v1, const Vecter *v2)
{
    return v1->x * v2->x + v1->y * v2->y;
}

double CrossProduct(const Vecter *v1, const Vecter *v2)
{
    return v1->x * v2->y - v1->y * v2->x;
}

Vecter WxR(double w, const Vecter *r)
{
    Vecter result;
    result.x = (-w) * (r->y);
    result.y = (w) * (r->x);
    return result;
}

void ground(int i,int chechf[]){
    if(chechf[i]!=0)
        return;
    chechf[i]=1;
    flist[i]->dim=0;
    flist[i]->value[0] = 1000;
    flist[i]->limit[0] = 200*i;
    flist[i]->limit[1] = 200*(i+1);
    flist[i]->pose.x = (flist[i]->limit[0] +flist[i]->limit[1])/2;
    flist[i]->pose.y = 1000;
}

void zero(int i,int chechf[]){
    if(chechf[i]!=0)
        return;
    chechf[i]=1;
    flist[i]->dim=0;
    flist[i]->value[0] = 800;
    flist[i]->limit[0] = 200*i;
    flist[i]->limit[1] = 200*(i+1);
    flist[i]->pose.x = (flist[i]->limit[0] +flist[i]->limit[1])/2;
    flist[i]->pose.y = 1000;
}
void first(int i,int chechf[],double val1){   
    if(chechf[i]!=0)
        return;
    chechf[i]=1;
    flist[i]->dim=1;
    flist[i]->value[0] = 800;
    flist[i]->value[1] = val1;
    flist[i]->limit[0] = 200*i;
    flist[i]->limit[1] = 200*(i+1);
    flist[i]->pose.x = (flist[i]->limit[0] +flist[i]->limit[1])/2;
    flist[i]->pose.y = 1000;
}

void second(int i,int chechf[],double val2, double val1){
    if(chechf[i]!=0)
        return;
    chechf[i]=1;
    flist[i]->dim=2;
    flist[i]->value[0] = 800;
    flist[i]->value[1] = val1;
    flist[i]->value[2] = val2;
    flist[i]->limit[0] = 200*i;
    flist[i]->limit[1] = 200*(i)+200;
    flist[i]->pose.x = (flist[i]->limit[0] +flist[i]->limit[1])/2;
    flist[i]->pose.y = 1000;
}
