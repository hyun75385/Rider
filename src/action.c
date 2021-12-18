
#include "action.h"

bool contact_state = false; //뒷바퀴 지면 닿는지 여부
int inter = 0;              //index

int collapse = 0;
int fcrash = 0;
Crash crashs[10];
Crash finalcrash[10];

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
    bool flag;
    Vecter adjust;
    fcrash = 0;
    collapse = 0;
    bike.acc.y = -GRAVITY;
    contact_state = false;
    inter = closestFeature();
    if (inter == 0)
    {
        detect_contact(flist[inter]);
        detect_contact(flist[inter + 1]);
    }
    else
    {
        detect_contact(flist[inter - 1]);
        detect_contact(flist[inter]);
        if (inter < FEATURENUM - 1)
            detect_contact(flist[inter + 1]);
    }

    for (int i = 0; i < collapse; i++)
    {
        flag = true;
        for (int j = 0; j < collapse; j++)
        {
            if(i==j)
                continue;
            if (crashs[i].part == crashs[j].part && issameVec(&crashs[i].normal, &crashs[j].normal))
            {
                flag = false;
                if (crashs[i].iner > crashs[j].iner)
                {
                    finalcrash[fcrash++] = crashs[i];
                    break;
                }
            }
        }
        if (flag)
        {
            finalcrash[fcrash++] = crashs[i];
        }
    }

    for (int i = 0; i < fcrash; i++)
    {
        printf("part : %d iner : %f, pose x : %.1f y : %.1f, normal x : %.10f, normal y : %.10f\n",finalcrash[i].part, finalcrash[i].iner, finalcrash[i].pose.x, finalcrash[i].pose.y, finalcrash[i].normal.x, finalcrash[i].normal.y);
    }
    printf("\n");

    adjust = adjust_pose();

    if (contact_state)
    {
        bike.vel.y = 0;

    }

    //change vel
    printf("x,y %f %f \n",adjust.x,adjust.y);
    bike.pose.x += adjust.x;
    bike.pose.y += adjust.y;
    bike.vel.y += bike.acc.y;
}

Vecter adjust_pose(void){
    Vecter max={0,0}; //조정 값은 무조건 양수
    for (int i = 0; i < fcrash; i++){
        if((finalcrash[i].iner*finalcrash[i].normal.x) > max.x){
            max.x = -finalcrash[i].iner*finalcrash[i].normal.x;
        }
        if((finalcrash[i].iner*finalcrash[i].normal.y) > max.y){
            max.y = -finalcrash[i].iner*finalcrash[i].normal.y;
        }
    }
    return max;
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
    double min_x = -1, min_d, dis;
    double iner1, iner2;
    for (P.x = f->limit[0]; P.x < f->limit[1]; P.x += 1)
    {
        P.y = cal_y(P.x, f);
        if (boundary[0] == -1 && distance(&P, &bike.front.pose) < bike.front.radius)
        {
            boundary[0] = P.x;
            min_x = P.x;
            min_d = distance(&P, &bike.front.pose);
        }
        if (boundary[0] != -1 && boundary[1] == -1)
        {
            dis = distance(&P, &bike.front.pose);
            if (min_d > dis)
            {
                min_x = P.x;
                min_d = dis;
            }
            if (dis > bike.front.radius)
            {
                boundary[1] = P.x;
                break;
            }
        }
    }
    if (min_x != -1)
    { //update되었다는것
        iner1 = cal_crash_tire(min_x, f, &bike.front);
        // printf("iner1 %f max x %f distance %f \n", iner1, min_x, min_d);
    }
    boundary[0] = -1;
    boundary[1] = -1;
    min_x = -1;
    for (P.x = f->limit[0]; P.x < f->limit[1]; P.x += 1)
    {
        P.y = cal_y(P.x, f);
        if (boundary[0] == -1 && distance(&P, &bike.back.pose) < bike.back.radius)
        {
            boundary[0] = P.x;
            min_x = P.x;
            min_d = distance(&P, &bike.back.pose);
        }
        if (boundary[0] != -1 && boundary[1] == -1)
        {
            dis = distance(&P, &bike.back.pose);
            if (min_d > dis)
            {
                min_x = P.x;
                min_d = dis;
            }
            if (dis > bike.back.radius)
            {
                boundary[1] = P.x;
                break;
            }
        }
    }
    if (min_x != -1)
    { //update되었다는것
        iner2 = cal_crash_tire(min_x, f, &bike.back);
        contact_state = true;
        // printf("iner2 %f max x %f distance %f \n", iner2, min_x, min_d);
    }
}

double cal_crash_tire(double x, const Feature *f, const Tire *tire)
{
    double diff = cal_diff(x, f), iner_pro = 0, a, b, c, y = cal_y(x, f);
    Vecter normal = cal_normal(diff); // 보라색 벡터
    Vecter green, red_brown, circle1, circle2, point;
    point.x = x;
    point.y = y;
    if (diff == 0)
    {
        red_brown.x = x;
        red_brown.y = cal_circle_y(x, tire);
    }
    else
    {
        a = pow(normal.x, 2) + 1;
        b = -2 * tire->pose.x - 2 * pow(normal.x, 2) * x + 2 * normal.x * y - 2 * normal.x * tire->pose.y;
        c = pow(tire->pose.x, 2) + normal.x * pow(x, 2) + 2 * normal.x * x * y - 2 * normal.x * tire->pose.y * x - 2 * tire->pose.y * y + pow(tire->pose.y, 2) - pow(tire->radius, 2) + pow(y, 2);
        SDL_assert(b * b - 4 * a * c > 0);
        // printf("a : %f b: %f c: %f sqrt : %f \n", a, b, c, sqrt(b * b - 4 * a * c));
        circle1.x = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
        circle1.y = cal_circle_y(circle1.x, tire);
        circle2.x = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
        circle2.y = cal_circle_y(circle2.x, tire);
        // printf("1:  %f %f 2:   %f %f\n", circle1.x, circle1.y, circle2.x, circle2.y);

        red_brown = (distance(&circle1, &point) < distance(&circle2, &point)) ? circle1 : circle2;
    }

    green.x = red_brown.x - x;
    green.y = red_brown.y - y;
    // printf("g x %f, g y %f rb y %f y %f\n", green.x, green.y,red_brown.y,y);
    iner_pro = normal.x * green.x + normal.y * green.y;
    if (iner_pro < 0)
    {
        crashs[collapse].iner = iner_pro;
        crashs[collapse].normal = normal;
        crashs[collapse].pose = red_brown;
        crashs[collapse].part = tire->part;
        collapse++;
    }
    return iner_pro;
}

double cal_y(double x, const Feature *f)
{
    SDL_assert(f->dim <= 3);
    double y = 0;
    for (int i = f->dim; i > -1; i--)
    {
        y += pow(x - f->limit[0], i) * f->value[i];
    }
    return y;
}

double cal_diff(double x, const Feature *f) //기울기 계산
{
    SDL_assert(f->dim <= 3);
    double y = 0;
    for (int i = f->dim - 1; i > -1; i--)
    {
        y += pow(x - f->limit[0], i) * f->value[i] * (i + 1);
    }
    return y;
}

Vecter cal_normal(double diff)
{
    Vecter n;
    if (diff < 0.00000000001)
    {
        n.y = -1;
        n.x = 0;
        return n;
    }
    n.y = -1 / sqrt(1 + diff * diff);    //y는 무조건 음의 방향(y축방향 반대)
    n.x = -diff / sqrt(1 + diff * diff); //x는 음의방향일 수도
    return n;
}

double cal_circle_y(double x, const Tire *tire)
{
    return tire->pose.y + sqrt(tire->radius * tire->radius + pow((tire->pose.x - x), 2)); //아래를 선택
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
    update_Tire(&bike);
    update_Body(&bike);
}
