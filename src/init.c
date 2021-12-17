
#include "init.h"

void InitSDL(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
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

void InitTTF(void) {
    if (TTF_Init() < 0) {
        printf("[ERROR] in InitTTF(): %s", SDL_GetError());
        exit(1);
    }

    app.font = TTF_OpenFont("./ttf/LiberationSans-Regular.ttf", 20);

    return;
}

void QuitSDL(int flag) {
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    QuitTTF();
    SDL_Quit();
    exit(flag);

    return;
}

void QuitTTF(void) {
    TTF_CloseFont(app.font);
    TTF_Quit();

    return;
}

void InitMemorySet(void) {
    memset(&app, 0, sizeof(App));
    memset(&bike,0, sizeof(BIKE));
    return;
}


void InitBike(void) {
    Vecter zero ={0,0};
    bike.pose.x = 100.0;
    bike.pose.y = 800.0;
    bike.vel = zero;
    bike.acc = zero;

    bike.theta2 = 0 ;
    bike.theta = -1 * bike.theta2;
    bike.omega =0.0;
    bike.alpa = 0.0;

    bike.body.offset = zero;
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
    bike.front.radius = 10;
    bike.front.min_d = 11;
    bike.back.offset.x = -23;
    bike.back.offset.y = 9.5;
    bike.back.radius = 10;
    bike.back.min_d = 11;
    update_Tire(&bike);



    return;
}

