#ifdef __APPLE__
    #include <SDL/SDL.h>
#else
    #include <SDL.h>
#endif
#include <cstdlib>
#include <iostream>
#include "engine/Math.h"
#include "engine/core/Structures.h"
#include "engine/Core.h"
#include "engine/Engine.h"
#include "engine/Objects.h"
#include "snowflakes/snowflakes.h"


using namespace std;

//config
int WIDTH = 800;
int HEIGHT = 600;
int FPS = 30;
int N_SNOWFLAKES = 50;
char const* TITLE = "Snowflakes by Arkadiusz Kasprzyk";

void createObjects(Engine& engine) {
    engine.addTickingPrimitive(new SnowflakeGenerator(engine.canvas(), N_SNOWFLAKES));
}

// increase wind
void Funkcja1(Engine& engine) {
    engine.wind(-25);
}

// decrease wind
void Funkcja2(Engine& engine) {
    engine.wind(25);
}



int main ( int argc, char** argv ) {
    try {
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
        SDL_Surface* screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
        if ( !screen ) {
            printf("Unable to set video: %s\n", SDL_GetError());
            return 1;
        }

        // initialize student's engine
        gfx::Engine engine(screen, WIDTH, HEIGHT, FPS);
        createObjects(engine);

        SDL_WM_SetCaption( TITLE , NULL );

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
                            Funkcja1(engine);
                        if (event.key.keysym.sym == SDLK_2)
                            Funkcja2(engine);
                    }
                }
            }
        }

        printf("Exited cleanly\n");
        return 0;
    } catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 1;
}
