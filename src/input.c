
#include "input.h"

void GetInput(void) {
    SDL_Event event;

    /*SDL_PollEvent() 함수는 이벤트 큐에 남아 있는 이벤트를 가져옴*/
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            /*창닫기 버튼을 누른 경우*/
            case SDL_QUIT:
                QuitSDL(0);
                break;
            /*키보드 버튼을 뗐을 경우*/
            case SDL_KEYUP:
                ResponseKeyUp(&event.key);
                break;
            /*키보드 버튼을 눌렀을 경우*/
            case SDL_KEYDOWN:
                ResponseKeyDown(&event.key);
                break;
            default:
                break;
        }
    }
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

void ResponseKeyUp(SDL_KeyboardEvent *event) {
    if (event->repeat == 0) {
        SDL_Scancode key_input = event->keysym.scancode;
        switch (key_input) {
            case SDL_SCANCODE_SPACE:
                app.key_space = 0;
                break;
        }
    }

    return;
}

void ResponseKeyDown(SDL_KeyboardEvent *event) {
    if (event->repeat == 0) {
        SDL_Scancode key_input = event->keysym.scancode;
        switch (key_input) {
            case SDL_SCANCODE_SPACE:
                app.key_space = 1;
                break;
        }
    }

    return;
}