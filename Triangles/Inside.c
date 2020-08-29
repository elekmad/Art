#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <time.h>

#define SCREEN_WIDTH 1920*.8
#define SCREEN_HEIGTH 1080*.8

#define GOLDER_RATIO 1.61803
#define square(x) ((x)*(x))

void point_add_vector(double *out, double *point1, double *point2, int geometric, double distance)
{
    if(geometric)
    {
        out[0] = point1[0] + (point2[0] - point1[0]) * distance / 100;
        out[1] = point1[1] + (point2[1] - point1[1]) * distance / 100;
    }
    else
    {
	double vector_size = sqrt(square(point2[0] - point1[0]) + square(point2[1] - point1[1]));
	printf("vector size %f points %f %f %f %f distance %f", vector_size, point1[0], point1[1], point2[0], point2[1], distance);
        out[0] = point1[0] + ((point2[0] - point1[0]) / vector_size) * distance;
        out[1] = point1[1] + ((point2[1] - point1[1]) / vector_size) * distance;
	printf(" => out %f %f\n", out[0], out[1]);
    }
}

void draw_triangles(SDL_Renderer *renderer, double points[6], int geometric, double distance, int number, int rotation_direction)
{
    SDL_Point triangle[4];
    for(int cmpt = 0; cmpt < 3; cmpt++)
    {
        triangle[cmpt].x = points[cmpt * 2];
        triangle[cmpt].y = points[cmpt * 2 + 1];
    }
    triangle[3] = triangle[0];
    SDL_RenderDrawLines(renderer, triangle, 4);
    if(number > 0)
    {
	double new_triangle[6];
	memcpy(new_triangle, points, 6 * sizeof(double));
	if(rotation_direction == 0)
	{
            point_add_vector(&new_triangle[0], &points[0], &points[2], geometric, distance);
            point_add_vector(&new_triangle[2], &points[2], &points[4], geometric, distance);
            point_add_vector(&new_triangle[4], &points[4], &points[0], geometric, distance);
	}
	else
	{
            point_add_vector(&new_triangle[2], &points[2], &points[0], geometric, distance);
            point_add_vector(&new_triangle[4], &points[4], &points[2], geometric, distance);
            point_add_vector(&new_triangle[0], &points[0], &points[4], geometric, distance);
	}
        draw_triangles(renderer, new_triangle, geometric, distance, --number, rotation_direction);
    }
}

int main( int argc, char *argv[ ] )
{
    int animated = 0, nb_triangles_max = 100, geometric = 0, distance = 3;
    SDL_Window *window;
    if( SDL_Init( SDL_INIT_VIDEO ) == -1 )
    {
        printf( "Can't init SDL:  %s\n", SDL_GetError( ) );
        return EXIT_FAILURE;
    }

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
    if(argc > 1)
	animated = atoi(argv[1]);
    if(argc > 2)
        nb_triangles_max = atoi(argv[2]);
    if(argc > 3)
	geometric = atoi(argv[3]);
    if(argc > 4)
	distance = atoi(argv[4]);

        // Main loop
    const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
    SDL_Event event;
    unsigned int current_time = SDL_GetTicks(), nb_triangles = nb_triangles_max, triangles_speed = 1;;
    double center_x = SCREEN_WIDTH / 2, center_y = SCREEN_HEIGTH / 2, size = (SCREEN_WIDTH < SCREEN_HEIGTH ? SCREEN_WIDTH : SCREEN_HEIGTH) * 0.5;
    for(int num_triangle = 0; num_triangle < 6; num_triangle++)
    {
        double triangle[6] = {center_x, center_y, center_x + size * cos(num_triangle * M_PI / 3), center_y + size * sin(num_triangle * M_PI / 3), center_x + size * cos((num_triangle + 1) * M_PI / 3), center_y + size * sin((num_triangle + 1) * M_PI / 3)};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        draw_triangles(renderer, triangle, geometric, distance, nb_triangles, num_triangle % 2);
    }
/*        double triangle[6] = {center_x, center_y, center_x + size * cos(M_PI / 5), center_y + size * sin(M_PI / 5), center_x - size * cos( M_PI / 5), center_y + size * sin(M_PI / 5)};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        draw_triangles(renderer, triangle, geometric, distance, nb_triangles, 0);*/
    while(1)
    {
	if(animated)
	{
       	    if(SDL_GetTicks() - current_time > 50)
  	    {
  	        current_time = SDL_GetTicks();
  	        nb_triangles += triangles_speed;
  	        if(nb_triangles > nb_triangles_max)
  	    	    triangles_speed = -1;
  	        if(nb_triangles < 1)
  	    	    triangles_speed = 1;
  	           
  	        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  	           pos.x = 0;
  	           pos.y = 0;
  	           pos.w = SCREEN_WIDTH;
  	           pos.h = SCREEN_HEIGTH;
  	           SDL_RenderClear(renderer);
  	           SDL_RenderFillRect(renderer, &pos);
  	    
  	        for(int num_triangle = 0; num_triangle < 6; num_triangle++)
  	           {
  	               double triangle[6] = {center_x, center_y, center_x + size * cos(num_triangle * M_PI / 3), center_y + size * sin(num_triangle * M_PI / 3), center_x + size * cos((num_triangle + 1) * M_PI / 3), center_y + size * sin((num_triangle + 1) * M_PI / 3)};
  	               SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  	               draw_triangles(renderer, triangle, geometric, distance, nb_triangles, num_triangle % 2);
  	           }
  	    
  	    }
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
