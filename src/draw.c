
#include "draw.h"

int X;

void ClearWindow(void)
{
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 0);
    SDL_RenderClear(app.renderer);

    return;
}

void ShowWindow(void)
{
    SDL_RenderPresent(app.renderer);
    return;
}

void DrawGame(void)
{
#if (DEBUG_MODE == 1)
    RenderBikeBody(&bike);
    RenderTire(&bike);
    for (int i = 0; i < FEATURENUM; i++)
    {
        if (flist[i]->dim == 0)
            RenderFeature_0(flist[i]);
        if (flist[i]->dim == 1)
            RenderFeature_1(flist[i]);
        if (flist[i]->dim == 2)
            RenderFeature_2(flist[i]);
    }
    return;
#endif
#if (DEBUG_MODE == 0)
    if (bike.pose.x > 300)
        X = (int)bike.pose.x - 300;
    else
        X = 0;
    RenderBikeBody(&bike);
    RenderTire(&bike);
    for (int i = 0; i < FEATURENUM; i++)
    {
        if (flist[i]->dim == 0)
            RenderFeature_0(flist[i]);
        if (flist[i]->dim == 1)
            RenderFeature_1(flist[i]);
        if (flist[i]->dim == 2)
            RenderFeature_2(flist[i]);
    }
    return;

#endif
}

void RenderBikeBody(BIKE *bike)
{
    SDL_SetRenderDrawColor(app.renderer, 100, 255, 255, 255);
#if (DEBUG_MODE == 1)
    for (int i = 0; i < 3; i++)
    {
        SDL_RenderDrawLine(app.renderer, (int)bike->body.pose[i].x, (int)bike->body.pose[i].y, (int)bike->body.pose[i + 1].x, (int)bike->body.pose[i + 1].y);
    }
    SDL_RenderDrawLine(app.renderer, (int)bike->body.pose[3].x, (int)bike->body.pose[3].y, (int)bike->body.pose[0].x, (int)bike->body.pose[0].y);
#endif

#if (DEBUG_MODE == 0)
    bike->size.x -= X;
    SDL_QueryTexture(bike->texture, NULL, NULL, &(bike->size.w),&(bike->size.h));
    SDL_RenderCopy(app.renderer, bike->texture, NULL, &(bike->size));
    for (int i = 0; i < 3; i++)
    {
        SDL_RenderDrawLine(app.renderer, (int)bike->body.pose[i].x - X, (int)bike->body.pose[i].y, (int)bike->body.pose[i + 1].x - X, (int)bike->body.pose[i + 1].y);
    }
    SDL_RenderDrawLine(app.renderer, (int)bike->body.pose[3].x - X, (int)bike->body.pose[3].y, (int)bike->body.pose[0].x - X, (int)bike->body.pose[0].y);
    // printf("pose %d %d %d %d\n",(int)bike->size.x - X, (int)bike->size.y,(int)bike->size.w , (int)bike->size.h);
#endif
    return;

}

void RenderTire(const BIKE *bike)
{
#if (DEBUG_MODE == 1)
    int old_x = bike->front.pose.x + bike->front.radius;
    int old_y = bike->front.pose.y + 0;
    double x2_d, y2_d;
    int x1, y1, x2, y2;
    // printf("%d %d %d %d\n",old_x,old_y,x2,y2);
    SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
    for (int i = 0; i < 100; i++)
    {
        x1 = old_x;
        y1 = old_y;
        x2_d = bike->front.pose.x + bike->front.radius * cos(3.14 / 50 * (i + 1));
        y2_d = bike->front.pose.y + bike->front.radius * sin(3.14 / 50 * (i + 1));
        x2 = (int)x2_d;
        y2 = (int)y2_d;
        SDL_RenderDrawLine(app.renderer, x1, y1, x2, y2);
        // printf("front %d %d %d %d\n",x1,y2,x2,y2);

        old_x = x2;
        old_y = y2;
    }

    old_x = bike->back.pose.x + bike->back.radius;
    old_y = bike->back.pose.y + 0;
    for (int i = 0; i < 100; i++)
    {
        x1 = old_x;
        y1 = old_y;
        x2_d = bike->back.pose.x + bike->back.radius * cos(3.14 / 50 * (i + 1));
        y2_d = bike->back.pose.y + bike->back.radius * sin(3.14 / 50 * (i + 1));
        x2 = (int)x2_d;
        y2 = (int)y2_d;
        SDL_RenderDrawLine(app.renderer, x1, y1, x2, y2);
        // printf("back  %d %d %d %d\n",x1,y1,x2,y2);
        old_x = x2;
        old_y = y2;
    }
#endif
#if (DEBUG_MODE == 0)
    int old_x = bike->front.pose.x + bike->front.radius;
    int old_y = bike->front.pose.y + 0;
    double x2_d, y2_d;
    int x1, y1, x2, y2;
    // printf("%d %d %d %d\n",old_x,old_y,x2,y2);
    SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
    for (int i = 0; i < 100; i++)
    {
        x1 = old_x;
        y1 = old_y;
        x2_d = bike->front.pose.x + bike->front.radius * cos(3.14 / 50 * (i + 1));
        y2_d = bike->front.pose.y + bike->front.radius * sin(3.14 / 50 * (i + 1));
        x2 = (int)x2_d;
        y2 = (int)y2_d;
        SDL_RenderDrawLine(app.renderer, x1 - X, y1, x2 - X, y2);
        // printf("front %d %d %d %d\n",x1,y2,x2,y2);

        old_x = x2;
        old_y = y2;
    }

    old_x = bike->back.pose.x + bike->back.radius;
    old_y = bike->back.pose.y + 0;
    for (int i = 0; i < 100; i++)
    {
        x1 = old_x;
        y1 = old_y;
        x2_d = bike->back.pose.x + bike->back.radius * cos(3.14 / 50 * (i + 1));
        y2_d = bike->back.pose.y + bike->back.radius * sin(3.14 / 50 * (i + 1));
        x2 = (int)x2_d;
        y2 = (int)y2_d;
        SDL_RenderDrawLine(app.renderer, x1 - X, y1, x2 - X, y2);
        // printf("back  %d %d %d %d\n",x1,y1,x2,y2);
        old_x = x2;
        old_y = y2;
    }
#endif
}

void RenderFeature_0(const Feature *f)
{ // 하나만 그림
#if (DEBUG_MODE == 1)
    SDL_RenderDrawLine(app.renderer, f->limit[0], f->value[0], f->limit[1], f->value[0]);
#endif
#if (DEBUG_MODE == 0)
    SDL_RenderDrawLine(app.renderer, f->limit[0] - X, f->value[0], f->limit[1] - X, f->value[0]);
    SDL_RenderDrawLine(app.renderer, f->limit[0] - X, f->value[0]+1, f->limit[1] - X, f->value[0]+1);
    SDL_RenderDrawLine(app.renderer, f->limit[0] - X, f->value[0]+2, f->limit[1] - X, f->value[0]+2);
    SDL_RenderDrawLine(app.renderer, f->limit[0] - X, f->value[0]+3, f->limit[1] - X, f->value[0]+3);
#endif
}

void RenderFeature_1(const Feature *f)
{ // 하나만 그림
#if (DEBUG_MODE == 1)
    SDL_RenderDrawLine(app.renderer, f->limit[0], f->value[0], f->limit[1], f->value[1] * (f->limit[1] - f->limit[0]) + f->value[0]);
#endif
#if (DEBUG_MODE == 0)
    SDL_RenderDrawLine(app.renderer, f->limit[0] - X, f->value[0], f->limit[1] - X, f->value[1] * (f->limit[1] - f->limit[0]) + f->value[0]);
    SDL_RenderDrawLine(app.renderer, f->limit[0] - X, f->value[0]+1, f->limit[1] - X, f->value[1] * (f->limit[1] - f->limit[0]) + f->value[0]+1);
    SDL_RenderDrawLine(app.renderer, f->limit[0] - X, f->value[0]+2, f->limit[1] - X, f->value[1] * (f->limit[1] - f->limit[0]) + f->value[0]+2);
    SDL_RenderDrawLine(app.renderer, f->limit[0] - X, f->value[0]+3, f->limit[1] - X, f->value[1] * (f->limit[1] - f->limit[0]) + f->value[0]+3);

#endif
}

void RenderFeature_2(const Feature *f)
{ // 하나만 그림
#if (DEBUG_MODE == 1)
    int oldx = f->limit[0], oldy = f->value[0];
    int y = 0;
    for (int x = f->limit[0] + 1; x < f->limit[1]; x++)
    {
        y = f->value[2] * pow(x - f->limit[0], 2) + f->value[1] * pow(x - f->limit[0], 1) + f->value[0];
        SDL_RenderDrawLine(app.renderer, oldx, oldy, x, y);
        oldx = x;
        oldy = y;
    }
#endif
#if (DEBUG_MODE == 0)
    int oldx = f->limit[0], oldy = f->value[0];
    int y = 0;
    for (int x = f->limit[0] + 1; x < f->limit[1]; x++)
    {
        y = f->value[2] * pow(x - f->limit[0], 2) + f->value[1] * pow(x - f->limit[0], 1) + f->value[0];
        SDL_RenderDrawLine(app.renderer, oldx-X, oldy, x-X, y);
        SDL_RenderDrawLine(app.renderer, oldx-X, oldy+1, x-X, y+1);
        SDL_RenderDrawLine(app.renderer, oldx-X, oldy+2, x-X, y+2);
        SDL_RenderDrawLine(app.renderer, oldx-X, oldy+3, x-X, y+3);

        oldx = x;
        oldy = y;
    }
#endif
}