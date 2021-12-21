
#include "action.h"

bool contact_state = false; //뒷바퀴 지면 닿는지 여부
int inter = 0;              //index

int collapse = 0;
int fcrash = 0;
Crash crashs[10];
Crash finalcrash[10];
int last_feature = 0;
int turnscore = 0;
double dtheta=0, last_theta = 0;

void ActGame(void)
{
    contact_state = false;
    physics();
    ActBike(); //update speed, omega
    Updatepose();
    ActScoreBoard();
    return;
}

void physics(void)
{
    bool flag;
    Vecter adjust;
    fcrash = 0;
    collapse = 0;
    contact_state = false;
    inter = closestFeature();
    if(inter>last_feature){
        last_feature = inter;
    }
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
            if (i == j)
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
    adjust = adjust_pose();
    //change vel

    Force();
    // printf("vel  %f %f %f %f\n", bike.front.vel.x, bike.front.vel.y, bike.back.vel.x, bike.back.vel.y);

    // bike.front.pose.x += bike.front.vel.x;
    // bike.front.pose.y += bike.front.vel.y;
    // bike.back.pose.x += bike.back.vel.x;
    // bike.back.pose.y += bike.back.vel.y;
    // bike.front.pose = vecter_plus(&bike.front.pose, &bike.front.vel);
    // bike.back.pose = vecter_plus(&bike.back.pose, &bike.back.vel);
    // printf("pose %f %f %f %f\n", bike.front.pose.x, bike.front.pose.y, bike.back.pose.x, bike.back.pose.y);
    // printf("\n");
}

Vecter Force(void){
    part_num count[4]={0};
    Vecter tmp;
    // if(fcrash==0){ 
    //     // printf("in air\n");
    //     bike.front.vel.y -= GRAVITY;
    //     bike.back.vel.y -= GRAVITY;
    // }
    for (int i = 0; i < fcrash; i++){
        if(count[finalcrash[i].part]==0){
            count[finalcrash[i].part]=1;
            if(finalcrash[i].part==front){
                bike.front.vel = bounce(&bike.front.vel, &finalcrash[i].normal);
                // printf("normal %f %f\n",finalcrash[i].normal.x,finalcrash[i].normal.y);
                bike.front.vel.x += finalcrash[i].normal.x * finalcrash[i].normal.x * GRAVITY ;
                bike.front.vel.y -= finalcrash[i].normal.x * finalcrash[i].normal.y * GRAVITY ;
                printf("front gravity: %f %f \n",finalcrash[i].normal.x * finalcrash[i].normal.y * GRAVITY,finalcrash[i].normal.x * finalcrash[i].normal.x * GRAVITY);
            }
            else if(finalcrash[i].part==back){
                bike.back.vel = bounce(&bike.back.vel, &finalcrash[i].normal);
                bike.back.vel.x += finalcrash[i].normal.x * finalcrash[i].normal.x * GRAVITY ;
                bike.back.vel.y -= finalcrash[i].normal.x * finalcrash[i].normal.y * GRAVITY ;
                printf("back gravity: %f %f \n",finalcrash[i].normal.x * finalcrash[i].normal.y * GRAVITY,finalcrash[i].normal.x * finalcrash[i].normal.x * GRAVITY);

            }
        }
        else{
            // printf("2secter detected\n");
            if(finalcrash[i].part==front){
                tmp = bounce(&bike.front.vel, &finalcrash[i].normal);
                bike.front.vel.x /= 2;
                bike.front.vel.y /= 2;
                bike.front.vel.x += tmp.x/2;
                bike.front.vel.y += tmp.y/2;
            }
            else if(finalcrash[i].part==back){
                tmp = bounce(&bike.back.vel, &finalcrash[i].normal);
                bike.back.vel.x /= 2;
                bike.back.vel.y /= 2;
                bike.back.vel.x += tmp.x/2;
                bike.back.vel.y += tmp.y/2;
            }
        }
    }
    if(count[front]==0){
        bike.front.vel.y -= GRAVITY;
    }
    if(count[back]==0){
        bike.back.vel.y -= GRAVITY;

    }
}

Vecter adjust_pose(void)
{
    Vecter max = {0, 0};
    for (int i = 0; i < fcrash; i++)
    {   
        if(finalcrash[i].part==front){
            bike.front.pose.x += (-finalcrash[i].iner - GAP) * finalcrash[i].normal.x;
            bike.front.pose.y += (-finalcrash[i].iner - GAP) * finalcrash[i].normal.y;
            // printf("adjust front : %f %f\n",(-finalcrash[i].iner - GAP) * finalcrash[i].normal.x,(-finalcrash[i].iner - GAP) * finalcrash[i].normal.y);
        }
        if(finalcrash[i].part==back){ 
            bike.back.pose.x += (-finalcrash[i].iner - GAP) * finalcrash[i].normal.x;
            bike.back.pose.y += (-finalcrash[i].iner - GAP) * finalcrash[i].normal.y;
            // printf("adjust back : %f %f\n",(-finalcrash[i].iner - GAP) * finalcrash[i].normal.x,(-finalcrash[i].iner - GAP) * finalcrash[i].normal.y);

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
    Vecter P,max,min;
    int i;
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
        // printf("iner1 %f mins x %f mind %f \n", iner1, min_x, min_d);
        iner1 = cal_crash_tire(min_x, f, &bike.front);
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

    i=0;
    min.x = bike.body.pose[i].x;
    min.y = bike.body.pose[i].y;
    max.x = bike.body.pose[i].x;
    max.y = bike.body.pose[i].y;
    for(i=1;i<4;i++){
        if(bike.body.pose[i].x<min.x)
            min.x = bike.body.pose[i].x;
        if(bike.body.pose[i].y<min.y)
            min.y = bike.body.pose[i].y;
        if(bike.body.pose[i].x>max.x)
            max.x = bike.body.pose[i].x;
        if(bike.body.pose[i].y>max.y)
            max.y = bike.body.pose[i].y;
    }
    for (P.x = f->limit[0]; P.x < f->limit[1]; P.x += 1)
    {
        P.y = cal_y(P.x, f);
        if(min.x <= P.x && min.y <= P.y && P.x <= max.x && P.y <= max.y){
            GAME_END=true;
            printf("GAMEEND %d\n\n",GAME_END);
            break;
        }
    }


}

double cal_crash_tire(double x, const Feature *f, const Tire *tire)
{
    double diff = cal_diff(x, f), inv_diff, iner_pro = 0, a, b, c, y = cal_y(x, f);
    Vecter normal = cal_normal(diff); // 보라색 벡터
    // printf("normal %f %f diff %f\n",normal.x,normal.y,diff);
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
        inv_diff = -1/diff;
        a = pow(inv_diff, 2) + 1;
        b = -2 * tire->pose.x - 2 * pow(inv_diff, 2) * x + 2 * inv_diff * y - 2 * inv_diff * tire->pose.y;
        c = pow(tire->pose.x, 2) + pow(inv_diff,2) * pow(x, 2) - 2 * inv_diff * x * y + 2 * inv_diff * tire->pose.y * x -2*y* tire->pose.y  + pow(tire->pose.y, 2) - pow(tire->radius, 2) + pow(y, 2);

        // printf("\na : %f b: %f c: %f  x : %f y : %f\n", a, b, c,x,y);
        // printf("tire-> pose x %f y : %f inv_diff %f  \n",tire->pose.x,tire->pose.y, inv_diff);


        SDL_assert(b * b - 4 * a * c >= 0);
        circle1.x = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
        circle1.y = cal_circle_y(circle1.x, tire);
        circle2.x = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
        circle2.y = cal_circle_y(circle2.x, tire);
        printf("1:  %f %f 2:   %f %f\n", circle1.x, circle1.y, circle2.x, circle2.y);

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
        y += pow( x - f->limit[0], i) * f->value[i];
    }

    return y;
}

double cal_diff(double x, const Feature *f) //기울기 계산
{
    SDL_assert(f->dim <= 3);
    double y = 0;
    if(x < f->limit[0])
        x = f->limit[0];
    for (int i = f->dim - 1; i > -1; i--)
    {
        y += pow(x - f->limit[0], i) * f->value[i+1] * (i + 1);
    }
    return y;
}

Vecter cal_normal(double diff)
{
    Vecter n;
    if (diff == 0)
    {
        n.y = -1;
        n.x = 0;
        return n;
    }
    n.y = -1 / sqrt(1 + diff * diff);    //y는 무조건 음의 방향(y축방향 반대)
    n.x = diff / sqrt(1 + diff * diff); //x는 음의방향일 수도
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
        bike.omega = 0;
        bike.alpa = 0;
        dtheta = 2*PI;
        last_theta=0;
        if (app.key_space)
        {
            // printf("act bike %f\n",size(&bike.back.vel));
            if (bike.back.vel.x * cos(bike.theta) >= 0 && size(&bike.back.vel) < PLAYER_MAXSPEED) //방향이 다를때
            {   
                bike.back.vel.x += PLAYER_SPEED * cos(bike.theta);
                bike.back.vel.y += PLAYER_SPEED * sin(bike.theta);
            }
            else if (bike.back.vel.x * cos(bike.theta) < 0)
            {
                bike.back.vel.x += PLAYER_SPEED * cos(bike.theta);
                bike.back.vel.y += PLAYER_SPEED * sin(bike.theta);
            }
        }
        if (!app.key_space)
        {
            if (bike.back.vel.x * cos(bike.theta) >= 0 && size(&bike.back.vel) - PLAYER_BRAKE > 0)
            {
                bike.back.vel.x -= PLAYER_BRAKE * cos(bike.theta);
                bike.back.vel.y -= PLAYER_BRAKE * sin(bike.theta);
            }
            else if (bike.back.vel.x * cos(bike.theta) <= 0 && size(&bike.back.vel) - PLAYER_BRAKE > 0)
            { //방향이 다를때
                bike.back.vel.x += PLAYER_BRAKE * cos(bike.theta);
                bike.back.vel.y += PLAYER_BRAKE * sin(bike.theta);
            }
            else
            {
                bike.back.vel.x = 0;
                bike.back.vel.y = 0;
            }
        }
    }
    else
    {
        if(dtheta + last_theta - bike.theta < 0 && dtheta > 0){
            dtheta=2*PI;
            turnscore++;
        }
        // if(last_theta>PI)
        if(bike.theta -last_theta > PI)
            dtheta += 2*PI -(bike.theta -last_theta);
        else
            dtheta += bike.theta -last_theta;
        printf("last_theat : %f d %f\n",(bike.theta-last_theta)*180/PI,dtheta*180/PI);
        last_theta = bike.theta;
        
        if (app.key_space)
        {
            if (bike.omega < PLAYER_MAXROT)
            {   
                bike.alpa = PLAYER_ROT;
                bike.omega += bike.alpa;
            }
        }
        if (!app.key_space)
        {
            if (bike.omega > 0 && bike.omega - PLAYER_ROT > 0)
            {
                bike.alpa = -PLAYER_ROT;
                bike.omega+=bike.alpa;

            }
            else if (bike.omega < 0 && bike.omega + PLAYER_ROT < 0)
            {
                bike.alpa = PLAYER_ROT;
                bike.omega += bike.alpa;
            }
            else
            {   
                bike.alpa = 0;
            }
        }
    }
    return;
}


void Updatepose(void)
{
    update_tire_vel(&bike);
    bike.front.pose.x += bike.front.vel.x;
    bike.front.pose.y += bike.front.vel.y;
    bike.back.pose.x += bike.back.vel.x;
    bike.back.pose.y += bike.back.vel.y;
    update_Center_theta(&bike);

    bike.theta = -1 * bike.theta2;
    update_Center(&bike);
    update_Tire(&bike); //Tire위치 재조정(center -> boyd)
    update_Body(&bike);
    SCORE = turnscore + last_feature;

    printf("SCORE %d\n",SCORE);
    if(bike.pose.y > 1000){
        GAME_END = true;
    }
    printf("\n");
}

void ActScoreBoard(void) {
    char tmp[36];
    strcpy(score_text, "Score: ");
    SDL_itoa(SCORE, tmp, 10);
    strcat(score_text, tmp);
    score_board.surface =TTF_RenderText_Solid(app.font, score_text, score_board.color);
    score_board.texture =SDL_CreateTextureFromSurface(app.renderer, score_board.surface);
    return;
}