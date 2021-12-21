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
    bike.texture = IMG_LoadTexture(app.renderer, "./gfx/64.png");
    bike.size.x = 100-32;
    bike.size.y = 100-32;
    bike.size.w = 64;
    bike.size.h = 64;

    bike.pose.x = 100.0;
    bike.pose.y = 100.0;
    bike.vel =zero;
    bike.acc = zero;

    bike.theta2 = 0;
    bike.theta = -1 * bike.theta2;
    bike.omega = 0.0;
    bike.alpa = 0.0;

    bike.body.offset.x = 0;
    bike.body.offset.y = -15;
    bike.body.min_d = 24.27447;
    bike.body.r[0].x = -13;
    bike.body.r[0].y = -15.5;
    bike.body.r[1].x = 13;
    bike.body.r[1].y = -15.5;
    bike.body.r[2].x = 13;
    bike.body.r[2].y = 5.5;
    bike.body.r[3].x = -13;
    bike.body.r[3].y = 5.5;
    update_Body(&bike);

    bike.front.offset.x = 23;
    bike.front.offset.y = 5;
    bike.front.radius = 12;
    bike.front.min_d = 11;
    bike.front.part = front;
    bike.front.vel = zero;

    

    bike.back.offset.x = -23;
    bike.back.offset.y = 5;
    bike.back.radius = 12;
    bike.back.min_d = 11;
    bike.back.part = back;
    bike.back.vel = zero;

    
    update_Tire(&bike);
    return;
}

void InitFeature(void)
{
    int i = 0, last, checkf[FEATURENUM]={0};
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
    /*장애물 리스트*/
    second(3,checkf,-0.003,0);
    second(9,checkf,-0.006,0);

    first(6,checkf,-0.3);



    for (i = 0; i < FEATURENUM; i++)
    {
        if(checkf[i]==0)
            zero(i,checkf);
    }
    
}


void initgamestate(void){
    GAME_END = false;
}