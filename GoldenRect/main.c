#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <time.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGTH 768

#define GOLDER_RATIO 1.61803
#define NB_RECTS 20

void draw_golden_rect(SDL_Renderer *renderer)
{
    int rect_thickness;
    SDL_SetRenderDrawColor(renderer, rand() % 255, rand() % 255, rand() % 255, 255);
    SDL_Rect pos;
    pos.x = rand() % (SCREEN_WIDTH / 2);
    pos.y = rand() % (SCREEN_HEIGTH / 2);
    pos.w = rand() % (SCREEN_WIDTH - pos.x);
    if(rand()%2 && pos.w * GOLDER_RATIO + pos.y <= SCREEN_HEIGTH)
	    pos.h = pos.w * GOLDER_RATIO;
    else
	    pos.h = pos.w / GOLDER_RATIO;
    rect_thickness = rand() % 10;
    if(rect_thickness > pos.w)
	    rect_thickness = pos.w;
    if(rect_thickness > pos.y)
	    rect_thickness = pos.y;
    for(int cmpt = 0; cmpt < rect_thickness; cmpt++)
    {
        SDL_RenderDrawRect(renderer, &pos);
	pos.x++;
	pos.y++;
	pos.w-=2;
	pos.h-=2;
    }
}

int main( int argc, char *argv[ ] )
{
    SDL_Window *window;
    if( SDL_Init( SDL_INIT_VIDEO ) == -1 )
    {
        printf( "Can't init SDL:  %s\n", SDL_GetError( ) );
        return EXIT_FAILURE;
    }
    int draw_rects = 1;

    srand(time(0));
    atexit( SDL_Quit );
    SDL_Renderer *renderer;
    window = SDL_CreateWindow("Ma fenêtre de jeu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGTH, 0 );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect pos;
    pos.x = 0;
    pos.y = 0;
    pos.w = SCREEN_WIDTH;
    pos.h = SCREEN_HEIGTH;
    SDL_RenderClear(renderer);
    SDL_RenderFillRect(renderer, &pos);

        // Main loop
    const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
    SDL_Event event;
    while(1)
    {
	if(draw_rects)
	{
	    for(int cmpt = 0; cmpt < NB_RECTS; cmpt++)
	        draw_golden_rect(renderer);
	    draw_rects = 0;
	}
        SDL_PumpEvents();
        if(keyboard[SDL_SCANCODE_ESCAPE])
        {
            SDL_Quit();
            return EXIT_SUCCESS;
        }
	if (SDL_PollEvent(&event))
        {
            // Check for the quit message
            switch (event.type)
            {
                case SDL_QUIT:
                    SDL_Quit();
                    return EXIT_SUCCESS;// Quit the program
                    break;
            }
	    if(event.key.keysym.sym == SDLK_SPACE)
	    {
    		//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                draw_rects = 1;
	        SDL_RenderClear(renderer);
                SDL_RenderFillRect(renderer, &pos);
	    }
        }

        //Update the display
        SDL_RenderPresent(renderer);

    }


    SDL_RenderPresent(renderer);	
        
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
