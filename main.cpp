#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>

int main ( int argc, char** argv )
{
   
    
    //initiating the wiring PI 
    if (wiringPiSetup() == -1){
        printf("Error while setting up wiring lib ! \n");
        return 0;
    }


    //config E/S des GPIO
   for (int i = 0 ; i<8 ; i++){
        pinMode( i , OUTPUT);
        digitalWrite( i , LOW);
   }


    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }


    if(!IMG_Init(IMG_INIT_PNG))
    {
        printf( "Unable to init SDL_image: %s\n", IMG_GetError() );
        return 1;
    }

    if(TTF_Init() == -1)
    {
        printf( "Unable to init SDL_ttf: %s\n", TTF_GetError() );
        return 1;
    }

    // create a new window
    SDL_Surface* screen = SDL_SetVideoMode(960, 600, 32,
                                           SDL_HWSURFACE|SDL_DOUBLEBUF); // SDL_FULLSCREEN
    if ( !screen )
    {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Surface * background = IMG_Load("wallpaper.jpg");
    if ( !background )
    {
        printf("Unable to load the background image\n", SDL_GetError());
        return 1;
    }


    SDL_Rect center;


    center.x = (screen->w - background->w) / 2;
    center.y = (screen->h - background->h) / 2;
    SDL_BlitSurface(background, 0, screen, &center);
    SDL_Flip(screen);

    TTF_Font * police = TTF_OpenFont("From Cartoon Blocks.ttf", 300);
    if ( !police )
    {
        printf("Unable to load font\n");
        return 1;
    }
    SDL_Color couleurBlanche = {255, 255, 255};
    SDL_Color couleurRouge = {161, 80, 0};

    SDL_Surface * text = NULL;
    char numberString[4] = "";
    int number = 0;
    SDLKey key;


    // program main loop
    bool done = false;

    // message processing loop
    SDL_Event event;
    //while (SDL_PollEvent(&event))
    while (!done && SDL_WaitEvent(&event))
    {
        // check for messages
        switch (event.type)
        {
            // exit if the window is closed
        case SDL_QUIT:
            done = true;
            break;

            // check for keypresses
        case SDL_KEYDOWN:
            {
                key = event.key.keysym.sym;
                switch(key)
                {
                case SDLK_ESCAPE:
                    done = true;
                    break;


                case SDLK_BACKSPACE:

                    if(strlen(numberString) == 0) break;

                    center.x = (screen->w - background->w) / 2;
                    center.y = (screen->h - background->h) / 2;
                    SDL_BlitSurface(background, 0, screen, &center);
                    SDL_Flip(screen);

                    number /= 10;
                    if(number == 0) break;

                    if(text) SDL_FreeSurface(text);

                    sprintf(numberString, "%d", number);
                    text = TTF_RenderText_Blended(police, numberString, couleurBlanche);

                    center.x = (screen->w - text->w) / 2;
                    center.y = (screen->h - text->h) / 2;
                    SDL_BlitSurface(text, 0, screen, &center);
                    SDL_Flip(screen);

                    break;

                // Afficher le Numero sur l'ecran !

                case SDLK_0:
                case SDLK_1:
                case SDLK_2:
                case SDLK_3:
                case SDLK_4:
                case SDLK_5:
                case SDLK_6:
                case SDLK_7:
                case SDLK_8:
                case SDLK_9:
                case SDLK_KP0:
                case SDLK_KP1:
                case SDLK_KP2:
                case SDLK_KP3:
                case SDLK_KP4:
                case SDLK_KP5:
                case SDLK_KP6:
                case SDLK_KP7:
                case SDLK_KP8:
                case SDLK_KP9:

                    if(key >= SDLK_0 && key <= SDLK_9)
                        number = number*10 + key - SDLK_0;
                    else
                        number = number*10 + key - SDLK_KP0;

                    center.x = (screen->w - background->w) / 2;
                    center.y = (screen->h - background->h) / 2;
                    SDL_BlitSurface(background, 0, screen, &center);

                    if(number > 255)
                    {
                        number = 0;
                        sprintf(numberString, "");
                    }
                    else
                    {
                        // show Number
                        if(text) SDL_FreeSurface(text);

                        sprintf(numberString, "%d", number);
                        text = TTF_RenderText_Blended(police, numberString, couleurBlanche);

                        center.x = (screen->w - text->w) / 2;
                        center.y = (screen->h - text->h) / 2;
                        SDL_BlitSurface(text, 0, screen, &center);
                    }

                    SDL_Flip(screen);

                    break;

                case SDLK_RETURN: // Afficher le code binaire !

                    if(strlen(numberString) == 0) break;

                    center.x = (screen->w - background->w) / 2;
                    center.y = (screen->h - background->h) / 2;
                    SDL_BlitSurface(background, 0, screen, &center);

                    if(text) SDL_FreeSurface(text);
                    text = TTF_RenderText_Blended(police, numberString, couleurRouge);

                    center.x = (screen->w - text->w) / 2;
                    center.y = (screen->h - text->h) / 2;
                    SDL_BlitSurface(text, 0, screen, &center);
                    SDL_Flip(screen);

                    // Reset number
                    number = 0;
                    sprintf(numberString, "");



                    // Executer le code des GPIOs de la raspberry pi

                    //execute :)
			   digitalWrite( 0 , number & 0x01);
			   digitalWrite( 1 , number & 0x02);
			   digitalWrite( 2 , number & 0x04);
			   digitalWrite( 3 , number & 0x08);
			   digitalWrite( 4 , number & 0x10);
			   digitalWrite( 5 , number & 0x20);
			   digitalWrite( 6 , number & 0x40);
   			   digitalWrite( 7 , number & 0x80);

                    break;
                }

                break;
            }
        }
    }



    SDL_FreeSurface(background);
    if(text) SDL_FreeSurface(text);

    TTF_CloseFont(police);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();


    return 0;
}
