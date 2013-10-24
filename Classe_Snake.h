#include "SDL.h"
#include "SDL_image.h"

#include "constantes.h"

class Snake
{
	public:
		SDL_Event event;
		int iSens; // 0=haut  1=droite 2=bas 3=gauche
		int iPart;
		//Les coordonnées x et y du point
		int x, y;
		//La vitesse du point
		int xVel, yVel;


		//Initialisation des variables
		Snake();

		//Recupere la touche pressee et ajuste la vitesse du point
		void handle_input();

		//Montre le point sur l'ecran
		void show();

		SDL_Surface *surface;
		SDL_Rect clipsEnemy[ CLIP_SNAKE_LENGTH ];
		void initialisation(int test, int part);
		void initialisationImage();
		void move();
};
