
#include "utils.h"

void update_Tire(BIKE *bike)
{
    bike->front.pose.x = bike->pose.x + cos(bike->theta) * (bike->front.offset.x) - sin(bike->theta) * (bike->front.offset.y);
    bike->front.pose.y = bike->pose.y + sin(bike->theta) * (bike->front.offset.x) + cos(bike->theta) * (bike->front.offset.y);
    bike->back.pose.x  = bike->pose.x + cos(bike->theta) * (bike->back.offset.x)  - sin(bike->theta) * (bike->back.offset.y);
    bike->back.pose.y  = bike->pose.y + sin(bike->theta) * (bike->back.offset.x)  + cos(bike->theta) * (bike->back.offset.y);
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

double distance(const Vecter *p1, const Vecter *p2)
{
    return (sqrt((p1->x - p2->x) * (p1->x - p2->x) + (p1->y - p2->y) * (p1->y - p2->y)));
}

double size(const Vecter* v){
    return (sqrt(v->x* v->x + v->y * v->y));
}
