#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#ifdef __APPLE__
    #include <SDL/SDL.h>
#else
    #include <SDL.h>
#endif

#define pi 3.14159265359

#include <ctime>
#include <iostream>




#include "snowflakes/Math.h"
#include "snowflakes/core/Structures.h"
#include "snowflakes/Core.h"
#include "snowflakes/Objects.h"

using namespace std;

SDL_Surface *screen;
int width = 800;
int height = 600;
int fps = 30;

char const* tytul = "Snowflakes by Arkadiusz Kasprzyk";




namespace io{
    Point mousePosition() {
        int x, y;
        SDL_GetMouseState(&x, &y);
        return Point(x, y);
    }
}


void Funkcja1() {

    //s1.draw();
}


void Funkcja2() {

    SDL_Flip(screen);
}


void Funkcja3() {

    SDL_Flip(screen);
}


void Funkcja4() {

    SDL_Flip(screen);
}


int main ( int argc, char** argv ) {
    // console output
    freopen( "CON", "wt", stdout );
    freopen( "CON", "wt", stderr );

    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    screen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if ( !screen ) {
        printf("Unable to set video: %s\n", SDL_GetError());
        return 1;
    }

    // initialize student's engine
    gfx::Engine engine(screen, width, height, fps);

    SDL_WM_SetCaption( tytul , NULL );

    // program main loop
    bool done = false;
    while (!done) {
        engine.tick();

        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // check for messages
            switch (event.type) {
                // exit if the window is closed
                case SDL_QUIT:
                    done = true;
                    break;

                // check for keypresses
                case SDL_KEYDOWN: {
                    // exit if ESCAPE is pressed
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    if (event.key.keysym.sym == SDLK_1)
                        Funkcja1();
                    if (event.key.keysym.sym == SDLK_2)
                        Funkcja2();
                    if (event.key.keysym.sym == SDLK_3)
                        Funkcja3();
                    if (event.key.keysym.sym == SDLK_4)
                        Funkcja4();
                    }
            }
        }
    }

    printf("Exited cleanly\n");
    return 0;
}
