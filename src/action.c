
#include "action.h"

bool contact_state;

void ActGame(void)
{
    contact_state = true;
    // physics (Collapse & gravity) & update speed acc omega alpa
    //physics();
    ActBike(); //update speed, omega
    Updatepose();
    printf("vel %f\n",bike.vel.x);

    return;
}

void physics(void)
{
    bike.acc.y = -GRAVITY;

    //change vel
    bike.vel.y += bike.acc.y;
}

void ActBike(void)
{
    if (contact_state)
    {
        if (app.key_space)
        {
            if (bike.vel.x * cos(bike.theta) >= 0 && size(&bike.vel) < PLAYER_MAXSPEED)
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
            else if (bike.vel.x * cos(bike.theta) <= 0 && size(&bike.vel) - PLAYER_SPEED > 0){
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
