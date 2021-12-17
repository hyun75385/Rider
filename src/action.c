
#include "action.h"

bool contact_state;

void ActGame(void) {
    contact_state = false;
    // physics (Collapse & gravity) & update speed acc omega alpa
    ActBike(); //update speed, omega
    Updatepose();

    return;
}

void physics(void){
    bike.acc.y = -GRAVITY;
}

void ActBike(void) {
    if (app.key_space) {
        if( size(&bike.vel)  < PLAYER_MAXSPEED){
            bike.vel.x+= PLAYER_SPEED * cos(bike.theta);
            bike.vel.y+= PLAYER_SPEED * sin(bike.theta);
        }
        // else {
        //     bike.vel.x+= PLAYER_SPEED * cos(bike.theta);
        //     bike.vel.y+= PLAYER_SPEED * sin(bike.theta);
        // }
    }
    if (!app.key_space){
        if(size(&bike.vel) - PLAYER_SPEED > 0){
            bike.vel.x-= PLAYER_SPEED * cos(bike.theta);
            bike.vel.y-= PLAYER_SPEED * sin(bike.theta);
        }
        else {
            bike.vel.x=0;
            bike.vel.y=0;
        }
    }
    return;
}

void Updatepose(void){
    bike.theta = -1* bike.theta2;
    bike.pose.x += bike.vel.x;
    bike.pose.y += bike.vel.y; 
    update_Tire(&bike);
    update_Body(&bike);
}
