/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_thread.h"
#include <string>
#include "X11/Xlib.h"
//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The surfaces
SDL_Surface *image = NULL;
SDL_Surface *screen = NULL;

//The event structure
SDL_Event event;

//The thread that will be used
SDL_Thread *thread = NULL;

//Quit flag
bool quit = false;
SDL_Surface * personaje_up[2];
SDL_Surface * personaje_down[2];
SDL_Surface * personaje_left[2];
SDL_Surface * personaje_rigth[2];


SDL_Surface * personaje1_up[2];
SDL_Surface * personaje1_down[2];
SDL_Surface * personaje1_left[2];
SDL_Surface * personaje1_rigth[2];
int image_x = 0;
int image_y = 0;
int image1_x = 100;
int image1_y = 100;
char estado = 'd';
char estado1 = 'r';
int frame = 0;
int frame_monja = 0;

SDL_Surface *load_image( std::string filename )
{
    //Load the image
    return IMG_Load( filename.c_str() );

    //If the image loade
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //XInitThreads();
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Thread test", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
    //Load the image
    image = load_image( "image.png" );
    personaje_down[0] = load_image("down1.png");
    personaje_down[1] = load_image("down2.png");

    personaje_up[0] = load_image("up1.png");
    personaje_up[1] = load_image("up2.png");

    personaje_left[0] = load_image("left1.png");
    personaje_left[1] = load_image("left2.png");

    personaje_rigth[0] = load_image("right1.png");
    personaje_rigth[1] = load_image("right2.png");

    personaje1_left[0] = load_image("left11.png");
    personaje1_left[1] = load_image("left22.png");

    personaje1_rigth[0] = load_image("right11.png");
    personaje1_rigth[1] = load_image("right22.png");
    //If there was an error in loading the image
    if( image == NULL )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}

void clean_up()
{
    //Stop the thread
    SDL_KillThread( thread );

    //Free the surface
    SDL_FreeSurface( image );

    //Quit SDL
    SDL_Quit();
}

int my_thread( void *data )
{
    //While the program is not over
    while( quit == false )
    {
        //Do the caption animation
        SDL_Delay( 15 );

        if (estado1 == 'r')
        {
            image1_x++;
        }
        if (estado1 == 'l')
        {
            image1_x--;
        }

        if (estado1 == 'r' && image1_x > 300)
        {
            estado1 = 'l';
        }
        if (estado1 == 'l' && image1_x < 0)
        {
            estado1 = 'r';
        }

        frame++;
        if (frame % 15 == 0)
            frame_monja++;
        while( SDL_PollEvent( &event ) )
        {
            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }


        }
        Uint8 *keystates = SDL_GetKeyState( NULL );

        if (keystates [ SDLK_UP])
        {
            image_y-=3;
            estado = 'u';
        }
        if (keystates [SDLK_DOWN])
        {
            image_y+=3;
            estado = 'd';
        }
        if (keystates [SDLK_LEFT])
        {
            image_x-=3;
            estado = 'l';
        }
        if (keystates [SDLK_RIGHT])
        {
            image_x+=3;
            estado = 'r';
        }
    }



    return 0;
}

int main( int argc, char* args[] )
{
    //Initialize
    if( init() == false )
    {
        return 1;
    }

    //Load the files
    if( load_files() == false )
    {
        return 1;
    }

    //Create and run the thread
    thread = SDL_CreateThread( my_thread, NULL );

    //Apply the image to the screen


    //While the user hasn't quit
    while( quit == false )
    {
        //While there's events to handle
        if(estado == 'd')
        {
            apply_surface( image_x, image_y, personaje_down[frame_monja%2], screen );
        }

        if (estado == 'u')
        {
            apply_surface( image_x, image_y, personaje_up[frame_monja%2], screen );
        }

        if (estado == 'l')
        {
            apply_surface( image_x, image_y, personaje_left[frame_monja%2], screen );
        }

        if (estado == 'r')
        {
            apply_surface( image_x, image_y, personaje_rigth[frame_monja%2], screen );
        }

        if (estado1 == 'r')
        apply_surface(image1_x, image1_y, personaje1_rigth[frame_monja%2], screen);
        if (estado1 == 'l')
        apply_surface(image1_x, image1_y, personaje1_left[frame_monja%2], screen);

        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }
    }

    //Clean up
    clean_up();

    return 0;
}
