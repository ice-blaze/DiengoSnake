#include "SDL.h"
#include "SDL_image.h"

#include <string>
#include <iostream>
#include <sstream>


#include "constantes.h"
#include "Classe_Snake.h"

Snake::Snake()
{
		SDL_Surface *surface;
		SDL_Rect clipsEnemy[ CLIP_SNAKE_LENGTH ];
		int iPart=-1;
		int iSens=-1;
		//Initialisation des coordonnees
		x = 0;
		y = 0;

		//Initialisation de la vitesse
		xVel = 0;
		yVel = 0;
}

void Snake::initialisation(int test,int part)
{
    //variables
    if (test == 1)
    {
		int iSens=0;
		int iPart=part;
    }
    //position
    else if (test == 3)
    {
        //position.x = 100;
        //position.y = 100;
    }
}
void Snake::initialisationImage()
{
	//On coupe la feuille de sprite
		for(int i=0;i<CLIP_SNAKE_LENGTH;i++)
		{
			clipsEnemy[ i ].x = SPRITE_WIDTH * i;
			clipsEnemy[ i ].y = 0;
			clipsEnemy[ i ].w = SPRITE_WIDTH;
			clipsEnemy[ i ].h = SPRITE_WIDTH;
		}

		switch(iPart)
		{
			case 0:
				surface = IMG_Load("Sprites/snake0.png");
				break;
			case 1:
				surface = IMG_Load("Sprites/snake1.png");
				break;
			case 2:
				surface = IMG_Load("Sprites/snake2.png");
				break;
			case 3:
				surface = IMG_Load("Sprites/snake3.png");
				break;
			case 4:
				surface = IMG_Load("Sprites/snake4.png");
				break;
			case 5:
				surface = IMG_Load("Sprites/snake5.png");
				break;
		}
}

//Recupere la touche pressee et ajuste la vitesse du point
void Snake::handle_input()
{
	//Si une touche a ete pressee
    if( event.type == SDL_KEYDOWN )
    {
        //ajustement de la vitesse
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: yVel -= SPRITE_WIDTH / 2; break;
            case SDLK_DOWN: yVel += SPRITE_WIDTH / 2; break;
            case SDLK_LEFT: xVel -= SPRITE_WIDTH / 2; break;
            case SDLK_RIGHT: xVel += SPRITE_WIDTH / 2; break;
            default: break;
        }
    }
    //Si une touche a ete relachee
    else if( event.type == SDL_KEYUP )
    {
        //ajustement de la vitesse
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: yVel += SPRITE_WIDTH / 2; break;
            case SDLK_DOWN: yVel -= SPRITE_WIDTH / 2; break;
            case SDLK_LEFT: xVel += SPRITE_WIDTH / 2; break;
            case SDLK_RIGHT: xVel -= SPRITE_WIDTH / 2; break;
            default: break;
        }
    }
}

//Montre le point sur l'ecran
void Snake::show()
{
	//Bouge le point à gauche ou à droite
    x += xVel;

    //Si le point se rapproche trop des limites(gauche ou droite) de l'ecran
    if( ( x < 0 ) || ( x + SPRITE_WIDTH > SCREEN_WIDTH ) )
    {
        //On revient
        x -= xVel;
    }

    //Bouge le point en haut ou en bas
    y += yVel;

    //Si le point se rapproche trop des limites(haute ou basse) de l'ecran
    if( ( y < 0 ) || ( y + SPRITE_WIDTH > SCREEN_HEIGHT ) )
    {
        //On revient
        y -= yVel;
    }

    //Affiche le point
    //apply_surface( x, y, surface, screen );
}

void Snake::move()
{
    /*//Test collision avec l'écran
    if(position.x<= 0)
        iSensHori=1;
    if(position.y <= 0)
        iSensVerti=0;
    if(position.x+SPRITE_WIDTH>=wEcran)
        iSensHori=0;
    if(position.y+SPRITE_WIDTH>=hEcran)
        iSensVerti=1;

    //Bouger
    if(iSensVerti==0&&iSensHori==0)
    {
        position.y++;
        position.x--;
    }
    else if(iSensVerti==1&&iSensHori==0)
    {
        position.y--;
        position.x--;
    }
    else if(iSensVerti==0&&iSensHori==1)
    {
        position.y++;
        position.x++;
    }
    else if(iSensVerti==1&&iSensHori==1)
    {
        position.y--;
        position.x++;
    }*/
}
