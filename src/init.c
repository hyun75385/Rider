#include "init.h"

void InitSDL(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("[ERROR] in InitSDL(): %s", SDL_GetError());
        exit(1);
    }

    app.window = SDL_CreateWindow("Rider!", SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                  SCREEN_HEIGHT, 0);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    return;
}


void InitMemorySet(void)
{
    memset(&app, 0, sizeof(App));
    memset(&bike, 0, sizeof(BIKE));
    memset(&flist, 0, sizeof(Feature **));
    return;
}

void InitBike(void)
{
    Vecter zero = {0, 0};
    bike.pose.x = 100.0;
    bike.pose.y = 100.0;
    bike.vel =zero;
    bike.acc = zero;

    bike.theta2 = 0;
    bike.theta = -1 * bike.theta2;
    bike.omega = 0.0;
    bike.alpa = 0.0;

    bike.body.offset.x = 0;
    bike.body.offset.y = -10;
    bike.body.min_d = 24.27447;
    bike.body.r[0].x = -13;
    bike.body.r[0].y = -20.5;
    bike.body.r[1].x = 13;
    bike.body.r[1].y = -20.5;
    bike.body.r[2].x = 13;
    bike.body.r[2].y = 9.5;
    bike.body.r[3].x = -13;
    bike.body.r[3].y = 9.5;
    update_Body(&bike);

    bike.front.offset.x = 23;
    bike.front.offset.y = 9.5;
    bike.front.radius = 12;
    bike.front.min_d = 11;
    bike.front.part = front;
    bike.front.vel = zero;

    

    bike.back.offset.x = -23;
    bike.back.offset.y = 9.5;
    bike.back.radius = 12;
    bike.back.min_d = 11;
    bike.back.part = back;
    bike.back.vel = zero;

    
    update_Tire(&bike);
    return;
}

void InitFeature(void)
{
    int i = 0, last;
    Feature *tmp;
    tmp = malloc(sizeof(Feature *) * FEATURENUM);
    if (tmp != NULL)
        flist = (Feature **) tmp;
    for (i = 0; i < FEATURENUM; i++)
    {
        tmp = malloc(sizeof(Feature));
        if (tmp != NULL)
            flist[i] = (Feature *)tmp;
        else
            exit(0);
    }
    for (i = 0; i < 2; i++)
    {   
        printf("%d ",i);
        flist[i]->dim=0;
        flist[i]->value[0] = 800;
        flist[i]->limit[0] = 200*i;
        flist[i]->limit[1] = 200*(i+1);
        flist[i]->pose.x = (flist[i]->limit[0] +flist[i]->limit[1])/2;
        flist[i]->pose.y = 1000;
    }
    for (i = 2; i < 3; i++)
    {   
        printf("%d ",i);
        flist[i]->dim=1;
        flist[i]->value[0] = 800;
        flist[i]->value[1] = -0.8;
        flist[i]->limit[0] = 200*i;
        flist[i]->limit[1] = 200*(i+1);
        flist[i]->pose.x = (flist[i]->limit[0] +flist[i]->limit[1])/2;
        flist[i]->pose.y = 1000;
    }
    for (i = 3; i < 5; i++)
    {   
        printf("%d ",i);
        flist[i]->dim=0;
        flist[i]->value[0] = 800;
        flist[i]->limit[0] = 200*i;
        flist[i]->limit[1] = 200*(i+1);
        flist[i]->pose.x = (flist[i]->limit[0] +flist[i]->limit[1])/2;
        flist[i]->pose.y = 1000;
    }
    for (i = 5; i < 6; i++)
    {   
        printf("%d ",i);
        flist[i]->dim=2;
        flist[i]->value[0] = 800;
        flist[i]->value[1] = 0;
        flist[i]->value[2] = -0.003;
        flist[i]->limit[0] = 200*i;
        flist[i]->limit[1] = 200*(i)+200;
        flist[i]->pose.x = (flist[i]->limit[0] +flist[i]->limit[1])/2;
        flist[i]->pose.y = 1000;
    }
    for (i = 6; i < FEATURENUM; i++)
    {   
        printf("%d ",i);
        flist[i]->dim=0;
        flist[i]->value[0] = 800;
        flist[i]->limit[0] = 200*(i+1);
        flist[i]->limit[1] = 200*(i+2);
        flist[i]->pose.x = (flist[i]->limit[0] +flist[i]->limit[1])/2;
        flist[i]->pose.y = 1000;
    }

    printf("\n");
}