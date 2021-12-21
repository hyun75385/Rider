

/**
 @file      main.c
 @brief     dodger 게임 main 함수를 정의한 소스 파일
 @author    이성재 (seongjae.lee.1118@gmail.com)
 */
#include "main.h"

int main(void) {
    int tmpcnt = 0;


    srand(time(NULL));

    /* 1. 초기화 */
    InitMemorySet();
    InitSDL();
    InitBike();
    InitTTF();
    InitFeature();
    initgamestate();
    initbackground();
    InitScoreBoard();



    /* 2. 프로그램 무한 루프 */
    for (;;) {

        /* (1) 빈 화면 만들기 */
        ClearWindow();

        /* (2) 키보드 입력 받기*/
        GetInput();
        /* (3) 메인 로직 수행 후 객체들 화면에 그리기 */
        if(GAME_END == false){
        /* 게임 진행 중일 시 */
            ActGame();
        /* (4) 화면 보여주기 */
        }
        DrawGame();
        
        ShowWindow();
        /* 일정 시간 대기 (60FPS 기준 한 프레임에 16ms) */
        // SDL_Delay(1000);
        SDL_Delay(10);
    }


    return 0;
}