
#include "action.h"

bool contact_state = false;
int inter = 0; //index
Vecter forces[10];
double iner_p[10];

void ActGame(void)
{
    contact_state = false;
    // physics (Collapse & gravity) & update speed acc omega alpa
    physics();
    ActBike(); //update speed, omega
    Updatepose();

    return;
}

void physics(void)
{
    bike.acc.y = -GRAVITY;
    inter = closestFeature();
    if (inter > 0)
    {
        contact_state = false;
        detect_contact(flist[inter - 1]);
        detect_contact(flist[inter]);
        if (inter < FEATURENUM - 1)
            detect_contact(flist[inter + 1]);
    }
    if (contact_state)
    {
        bike.vel.y = 0;
        return;
    }
    //change vel
    bike.vel.y += bike.acc.y;
}

int closestFeature(void)
{
    double minDis = 1000000000;
    int index = -1;
    for (int i = 0; i < FEATURENUM; i++)
    {
        if (minDis > abs(flist[i]->limit[0] - bike.pose.x))
        {
            minDis = abs(flist[i]->limit[0] - bike.pose.x);
            index = i;
        }
    }
    return index;
}

void detect_contact(const Feature *f)
{
    //Tire1
    Vecter P;
    double boundary[2] = {-1, -1}; //x값
    double max_x = -1, max_d, dis;
    double iner1, iner2;
    for (P.x = f->limit[0]; P.x < f->limit[1]; P.x += 1)
    {
        P.y = cal_y(P.x, f);
        if (boundary[0] == -1 && distance(&P, &bike.front.pose) < bike.front.radius)
        {
            boundary[0] = P.x;
            max_x = P.x;
            max_d = distance(&P, &bike.front.pose);
        }
        if (boundary[0] != -1 && boundary[1] == -0.5)
        {
            dis = distance(&P, &bike.front.pose);
            if (max_d < dis)
            {
                max_x = P.x;
                max_d = dis;
            }
            if (dis > bike.front.radius)
            {
                boundary[1] = P.x;
                break;
            }
        }
    }
    if (max_x != -1)
    { //update되었다는것
        iner1 = cal_crash_tire(max_x, f, &bike.front);
        contact_state = true;
        printf("iner1 %f f.x %f f.y %f \n", iner1, bike.front.pose.x, bike.front.pose.y);
    }
    boundary[0] = -1;
    boundary[1] = -1;
    max_x = -1;
    for (P.x = f->limit[0]; P.x < f->limit[1]; P.x += 0.5)
    {
        P.y = cal_y(P.x, f);
        if (boundary[0] == -1 && distance(&P, &bike.back.pose) < bike.back.radius)
        {
            boundary[0] = P.x;
            max_x = P.x;
            max_d = distance(&P, &bike.back.pose);
        }
        if (boundary[0] != -1 && boundary[1] == -1)
        {
            dis = distance(&P, &bike.back.pose);
            if (max_d < dis)
            {
                max_x = P.x;
                max_d = dis;
            }
            if (dis > bike.back.radius)
            {
                boundary[1] = P.x;
                break;
            }
        }
    }
    if (max_x != -1)
    { //update되었다는것
        iner2 = cal_crash_tire(max_x, f, &bike.back);
        contact_state = true;
        printf("iner2 %f b.x %f b.y %f \n", iner2, bike.back.pose.x, bike.back.pose.y);
    }
}

double cal_crash_tire(double x, const Feature *f, const Tire *tire)
{
    double diff = cal_diff(x, f), iner_pro = 0;
    Vecter normal = cal_normal(diff); // 보라색 벡터
    Vecter green, red_brown;
    red_brown.x = x;
    red_brown.y = cal_y(x, f);
    green.x = red_brown.x - x;
    green.y = red_brown.y - cal_circle_y(x, tire);
    iner_pro = normal.x * green.x + normal.y * green.y;
    return iner_pro;
}

double cal_y(double x, const Feature *f)
{
    SDL_assert(f->dim <= 3);
    double y = 0;
    for (int i = f->dim; i > -1; i--)
    {
        y += pow(x, i) * f->value[i];
    }
    return y;
}

double cal_diff(double x, const Feature *f) //기울기 계산
{
    SDL_assert(f->dim <= 3);
    double y = 0;
    for (int i = f->dim - 1; i > -1; i--)
    {
        y += pow(x, i) * f->value[i] * (i + 1);
    }
    return y;
}

Vecter cal_normal(double diff)
{
    Vecter n;
    n.y = 1 / sqrt(1 + diff * diff);     //y는 무조건 양의 방향
    n.x = -diff / sqrt(1 + diff * diff); //x는 음의방향일 수도
    return n;
}

double cal_circle_y(double x, const Tire *tire)
{
    return tire->pose.y - sqrt(tire->radius * tire->radius + pow((tire->pose.x - x), 2));
}

void ActBike(void)
{
    if (contact_state)
    {
        if (app.key_space)
        {
            if (bike.vel.x * cos(bike.theta) >= 0 && size(&bike.vel) < PLAYER_MAXSPEED) //방향이 다를때
            {
                bike.vel.x += PLAYER_SPEED * cos(bike.theta);
                bike.vel.y += PLAYER_SPEED * sin(bike.theta);
            }
            else if (bike.vel.x * cos(bike.theta) <= 0)
            {
                bike.vel.x += PLAYER_SPEED * cos(bike.theta);
                bike.vel.y += PLAYER_SPEED * sin(bike.theta);
            }
            // else { else시 그냥 속도 유지
            //     bike.vel.x+= PLAYER_SPEED * cos(bike.theta);
            //     bike.vel.y+= PLAYER_SPEED * sin(bike.theta);
            // }
        }
        if (!app.key_space)
        {
            if (bike.vel.x * cos(bike.theta) >= 0 && size(&bike.vel) - PLAYER_SPEED > 0)
            {
                bike.vel.x -= PLAYER_SPEED * cos(bike.theta);
                bike.vel.y -= PLAYER_SPEED * sin(bike.theta);
            }
            else if (bike.vel.x * cos(bike.theta) <= 0 && size(&bike.vel) - PLAYER_SPEED > 0)
            { //방향이 다를때
                bike.vel.x += PLAYER_SPEED * cos(bike.theta);
                bike.vel.y += PLAYER_SPEED * sin(bike.theta);
            }
            else
            {
                bike.vel.x = 0;
                bike.vel.y = 0;
            }
        }
    }
    else
    {
        if (app.key_space)
        {
            if (bike.omega < PLAYER_MAXROT)
            {
                bike.omega += PLAYER_ROT;
            }
            // else {
            //     bike.vel.x+= PLAYER_SPEED * cos(bike.theta);
            //     bike.vel.y+= PLAYER_SPEED * sin(bike.theta);
            // }
        }
        if (!app.key_space)
        {
            if (bike.omega > 0 && bike.omega - PLAYER_ROT > 0)
            {
                bike.omega -= PLAYER_ROT;
            }
            else if (bike.omega < 0 && bike.omega + PLAYER_ROT < 0)
            {
                bike.omega += PLAYER_ROT;
            }
            else
            {
                bike.omega = 0;
            }
        }
    }
    return;
}

void Updatepose(void)
{
    bike.pose.x += bike.vel.x;
    bike.pose.y += bike.vel.y;
    bike.theta2 += bike.omega;
    bike.theta = -1 * bike.theta2;
    // printf("THETA and OMEGA : %f %f",bike.theta, bike)
    update_Tire(&bike);
    update_Body(&bike);
}
