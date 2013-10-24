//Les fichiers d'entête
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include <string>
#include <sstream>

//Classe
#include "constantes.h"
#include "Classe_Timer.h"

//Le point que nous allons bouger sur l'ecran
class Point
{
	public:
	SDL_Surface *surface;
	SDL_Rect clipsSnake[ CLIP_SNAKE_LENGTH ];
	int iSens; // 0=haut  1=droite 2=bas 3=gauche
	int iPart;
	int previousPosX;
	int previousPosY;
	int previousSens;
    //Les coordonnées x et y du point
	int x, y;

	//public:
    //Initialisation des variables
    Point();

    //Recupere la touche pressee et ajuste la vitesse du point
    void handle_input();

    //Montre le point sur l'ecran
    void show();
};

class BitmapFont
{
    private:
    //La surface du font
    SDL_Surface *bitmap;

    //Les caractères individuels dans le font
    SDL_Rect chars[ 256 ];

    public:
    //Le constructeur par defaut
    BitmapFont();

    //Le constructeur avec un argument, genere le font quand l'objet est construit
    BitmapFont( SDL_Surface *surface );

    //Genere le font
    void build_font( SDL_Surface *surface );

    //Affiche le texte
    void show_text( int x, int y, std::string text, SDL_Surface *surface );
};

int collision (int pos1X,int pos1Y, int pos2X, int pos2Y);
int MyRand(int a, int b);

//Le nombre de frame par seconde
int FRAMES_PER_SECOND = 2;
int iScore = 0;
bool bGameOver = false;
bool bCredit = true;
Point myPoints[SNAKE_LENGTH];

//Les surfaces
SDL_Surface *screen = NULL, *target = NULL, *arrows = NULL;
SDL_Surface *bitmapFont = NULL;

int targetX, targetY;

//La musique qui sera joué
Mix_Music *music = NULL;

//La structure d'événements que nous allons utiliser
SDL_Event event;

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL)
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    //On blitte la surface
    SDL_BlitSurface( source, clip, destination, &offset );
}

SDL_Surface *load_image( std::string filename )
{
    //L'image qui est chargée
    SDL_Surface* loadedImage = NULL;

    //L'image optimisée qu'on va utiliser
    SDL_Surface* optimizedImage = NULL;

    //Chargement de l'image
    loadedImage = IMG_Load( filename.c_str() );

    //Si l'image a bien chargée
    if( loadedImage != NULL )
    {
        //Création de l'image optimisée
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Libération de l'ancienne image
        SDL_FreeSurface( loadedImage );

        //Si la surface a bien été optimisée
        if( optimizedImage != NULL )
        {
            SDL_SetColorKey( optimizedImage, SDL_RLEACCEL | SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    //On retourne l'image optimisée
    return optimizedImage;
}

Uint32 get_pixel32( int x, int y, SDL_Surface *surface )
{
    //Convertie les pixels en 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;

    //Recupere le pixel demande
    return pixels[ ( y * surface->w ) + x ];
}

bool init()
{
    //Initialisation de tous les sous-système de SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Mise en place de l'écran
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //S'il y a eu une erreur lors de la mise en place de l'écran
    if( screen == NULL )
    {
        return false;
    }

	//Initialisation de SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return false;
    }

	SDL_Surface *ico = NULL;
	ico = load_image( "sprites/ico.png" );
	SDL_WM_SetIcon(ico,NULL);

    //Mise en place de la barre caption
    SDL_WM_SetCaption( "Snake in your face", NULL );
	FRAMES_PER_SECOND = 2;
	iScore = 0;
	bGameOver = false;
	bCredit = true;
	for(int i = 0;i<SNAKE_LENGTH;i++)
	{
		myPoints[i].iSens=0;
		myPoints[i].previousPosX=0;
		myPoints[i].previousPosY=0;
	}
	myPoints[0].x=SPRITE_WIDTH*3;
	myPoints[0].y=SPRITE_WIDTH*10;
	targetX=MyRand(0, SCREEN_WIDTH)+25;
	targetY=MyRand(0, SCREEN_HEIGHT)+25;

    //Si tout c'est bien passé
    return true;
}

bool load_files()
{
	for (int u=0;u<SNAKE_LENGTH;u++)
	{
		for(int i=0;i<CLIP_SNAKE_LENGTH;i++)
		{
			myPoints[u].clipsSnake[ i ].x = SPRITE_WIDTH * i;
			myPoints[u].clipsSnake[ i ].y = 0;
			myPoints[u].clipsSnake[ i ].w = SPRITE_WIDTH;
			myPoints[u].clipsSnake[ i ].h = SPRITE_WIDTH;
		}
	}
    //Chargement du point
	myPoints[0].surface = load_image( "sprites/snake0.bmp" );
	myPoints[1].surface = load_image( "sprites/snake1.bmp" );
	myPoints[2].surface = load_image( "sprites/snake2.bmp" );
	myPoints[3].surface = load_image( "sprites/snake3.bmp" );
	myPoints[4].surface = load_image( "sprites/snake4.bmp" );
	myPoints[5].surface = load_image( "sprites/snake5.bmp" );
	target = load_image("sprites/apple.bmp");
    bitmapFont = load_image( "sprites/font.png" );
	arrows = load_image("sprites/arrows.png");


    //S'il y a eu un problème au chargement du point
    if( myPoints[0].surface == NULL||
		myPoints[1].surface == NULL||
		myPoints[2].surface == NULL||
		myPoints[3].surface == NULL||
		myPoints[4].surface == NULL||
		myPoints[5].surface == NULL||
		target == NULL||
		bitmapFont == NULL||
		arrows == NULL)
    {
        return false;
    }

	//Chargement de la musique
    music = Mix_LoadMUS( "Sounds/8bitDiego.mp3" );

    //S'il y a eu une erreur au chargement de la musique
    if( music == NULL )
    {
        return false;
    }

    //Si tout s'est bien passé
    return true;
}

void clean_up()
{
	SDL_FreeSurface(screen);
    //Libèration des surfaces
	for(int i=0;i<SNAKE_LENGTH;i++)
	{
		SDL_FreeSurface( myPoints[i].surface );
	}
	SDL_FreeSurface(target);
	SDL_FreeSurface(bitmapFont);
	SDL_FreeSurface(arrows);

	//Libération de la musique
    Mix_FreeMusic( music );

	//On quitte SDL_mixer
    Mix_CloseAudio();

    //On quitte SDL
    SDL_Quit();
}

Point::Point()
{
    //Initialisation des coordonnees

	surface = NULL;

	for(int i = 0;i<SNAKE_LENGTH;i++)
	{
		myPoints[i].iPart = i;
		x = 700;
		y = 700;
		if(iPart==0)
		{
			x = SPRITE_WIDTH*3;
			y = SPRITE_WIDTH*10;
		}
	}

	iSens = 0; // 0=haut  1=droite 2=bas 3=gauche
}

void Point::handle_input()
{
    //Si une touche a ete pressee
    if( event.type == SDL_KEYDOWN )
    {
		switch(event.key.keysym.sym)
		{
			case SDLK_UP:
				if(previousSens!=2)
					iSens=0;
				break;

			case SDLK_RIGHT:
				if(previousSens!=3)
					iSens=1;
				break;

			case SDLK_DOWN:
				if(previousSens!=0)
					iSens=2;
				break;

			case SDLK_LEFT:
				if(previousSens!=1)
					iSens=3;
				break;

            default:
                break;
		}

    }
}

void Point::show()
{
	if(iPart==0)
	{
		switch(iSens)
		{
			case 0:
				y -= SPRITE_WIDTH ;
				break;
			case 1:
				x += SPRITE_WIDTH ;
				break;
			case 2:
				y += SPRITE_WIDTH ;
				break;
			case 3:
				x -= SPRITE_WIDTH ;
				break;
		}

		if(  x<0 || y<0 || x + SPRITE_WIDTH > SCREEN_WIDTH || y + SPRITE_WIDTH > SCREEN_HEIGHT)
			bGameOver=true;

		//collision snake
		for(int i=1; i<SNAKE_LENGTH;i++)
		{
			if(collision(x,y,myPoints[i].x,myPoints[i].y))
				bGameOver=true;
		}

		//collision cible
		if(collision(x,y,targetX,targetY))
		{
			targetX=MyRand(0, 14)*SPRITE_WIDTH+25/2;
			targetY=MyRand(0, 11)*SPRITE_WIDTH+25/2;
			iScore++;
		}
	}
	else if (iPart>0)
	{
		x = myPoints[iPart-1].previousPosX;
		y = myPoints[iPart-1].previousPosY;
		iSens = myPoints[iPart-1].previousSens;
	}
	//Affiche le point
	apply_surface( x, y,surface, screen, &clipsSnake[ iSens ] );

	previousPosX = x;
	previousPosY = y;
	previousSens = iSens;
}

BitmapFont::BitmapFont()
{
    //Met bitmap a NULL
    bitmap = NULL;
}

BitmapFont::BitmapFont( SDL_Surface *surface )
{
    //Construit le font
    build_font( surface );
}

void BitmapFont::build_font( SDL_Surface *surface )
{
    //Si la surface est NULL
    if( surface == NULL )
    {
        return;
    }

    //Recupere le bitmap
    bitmap = surface;

    //Mise en place de la couleur du background (fond)
    Uint32 bgColor = SDL_MapRGB( bitmap->format, 0, 0xFF, 0xFF );

    //Dimension des cellules
    int cellW = bitmap->w / 16;
    int cellH = bitmap->h / 16;

    //Le caractere courant
    int currentChar = 0;

    //On parcours les lignes des cellules
    for( int rows = 0; rows < 16; rows++ )
    {
        //On parcours les colonnes des cellules
        for( int cols = 0; cols < 16; cols++ )
        {
            //caractere courant
            chars[ currentChar ].x = cellW * cols;
            chars[ currentChar ].y = cellH * rows;

            //Mise en place des dimensions du caractere
            chars[ currentChar ].w = cellW;
            chars[ currentChar ].h = cellH;

            //On parcours les colonnes des pixels
            for( int pCol = 0; pCol < cellW; pCol++ )
            {
                //On parcours les lignes des pixels
                for( int pRow = 0; pRow < cellH; pRow++ )
                {
                    //Recupere les coordonnees du pixel
                    int pX = ( cellW * cols ) + pCol;
                    int pY = ( cellH * rows ) + pRow;

                    //Si un pixel non "colorkey" est trouve
                    if( get_pixel32( pX, pY, bitmap ) != bgColor )
                    {
                        //Coordonnee x
                        chars[ currentChar ].x = pX;

                        //On arrete la boucle
                        pCol = cellW;
                        pRow = cellH;
                    }
                }
            }

            //On parcours les colonnes des pixels
            for( int pCol_w = cellW - 1; pCol_w >= 0; pCol_w-- )
            {
                //On parcours les lignes des pixels
                for( int pRow_w = 0; pRow_w < cellH; pRow_w++ )
                {
                    //Recupere les coordonnees du pixel
                    int pX = ( cellW * cols ) + pCol_w;
                    int pY = ( cellH * rows ) + pRow_w;

                    //Si un pixel non "colorkey" est trouve
                    if( get_pixel32( pX, pY, bitmap ) != bgColor )
                    {
                        //longueur du caractere courant
                        chars[ currentChar ].w = ( pX - chars[ currentChar ].x ) + 1;

                        //On arrete la boucle
                        pCol_w = -1;
                        pRow_w = cellH;
                    }
                }
            }

            //caractere suivant
            currentChar++;
        }
    }
}

void BitmapFont::show_text( int x, int y, std::string text, SDL_Surface *surface )
{
    //coordonnees temporaires
    int X = x, Y = y;

    //verification que le font a bien ete construit
    if( bitmap != NULL )
    {
        //On parcours le texte
        for( int show = 0; text[ show ] != '\0'; show++ )
        {
            //Si le caractere courant est un espace
            if( text[ show ] == ' ' )
            {
                //On bouge de la taille d'un caractere
                X += bitmap->w / 32;
            }
            //Si le caractere courant est un "newline"
            else if( text[ show ] == '\n' )
            {
                //On descent
                Y += bitmap->h / 16;

                //On revient en arriere
                X = x;
            }
            else
            {
                //recupere la valeur ASCII du caractere
                int ascii = (int)text[ show ];

                //Affiche le caractere
                apply_surface( X, Y, bitmap, surface, &chars[ ascii ] );

                //On bouge de la longueur du caractere + un pas de un pixel
                X += chars[ ascii ].w + 1;
            }
        }
    }
}

int main( int argc, char* args[] )
{
    //Ce qui va nous permettre de quitter
    bool quit = false;

    //Le regulateur
    Timer fps;


    //Initialisation
    if( init() == false )
    {
        return 1;
    }

    //Chargement des fichiers
    if( load_files() == false )
    {
        return 1;
    }
	BitmapFont font( bitmapFont );

	while(bCredit == true)
	{
		SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x00, 0xCD, 0xFF ) );
		font.show_text( 150, 100, "Created by Ice-Blaze", screen );
		font.show_text( 50, 150, "music, sprites and all what you can see", screen );
		font.show_text( 160, 200, "press a key to start", screen );
		font.show_text( 170, 300, "use arrows to play !", screen );
		apply_surface( 250, 350, arrows, screen,NULL);


		//Mise à jour de l'écran
		if( SDL_Flip( screen ) == -1 )
			return 1;

		while( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_KEYDOWN )
                bCredit = false;

			if( event.type == SDL_QUIT )
			{
				bCredit = false;
                quit = true;
			}
		}
	}

	//On lance la musique
    if( Mix_PlayMusic( music, -1 ) == -1 )
    {
        return 1;
    }

    //Tant que l'utilisateur n'a pas quitter
    while( quit == false )
    {
        //On demarre le timer fps
        fps.start();

        //Tant qu'il y a un événement
        while( SDL_PollEvent( &event ) )
        {
			if( event.type == SDL_KEYDOWN )
			{
				if(bGameOver == true)
				{
					if( init() == false )
					{
						return 1;
					}
				}

				if(event.key.keysym.sym==SDLK_ESCAPE){quit = true;}
			}

            //On recupere l'evenement pour le point
            myPoints[0].handle_input();

            //Si l'utilisateur a cliqué sur le X de la fenêtre
            if( event.type == SDL_QUIT )
            {
                //On quitte the programme
                quit = true;
            }
        }

        //On remplit l'ecran de noir
        SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ) );

		std::ostringstream out;
		out << iScore;
		font.show_text( 0, 0, "Score : "+out.str(), screen );

		if(iScore>=10)
		{
			if(FRAMES_PER_SECOND<7)
				FRAMES_PER_SECOND++;
			myPoints[5].show();
		}
		if(iScore>=8)
		{
			if(FRAMES_PER_SECOND<6)
				FRAMES_PER_SECOND++;
			myPoints[4].show();
		}
		if(iScore>=6)
		{
			if(FRAMES_PER_SECOND<5)
				FRAMES_PER_SECOND++;
			myPoints[3].show();
		}
		if(iScore>=4)
		{
			if(FRAMES_PER_SECOND<4)
				FRAMES_PER_SECOND++;
			myPoints[2].show();
		}
		if(iScore>=2)
		{
			if(FRAMES_PER_SECOND<3)
				FRAMES_PER_SECOND++;
			myPoints[1].show();
		}
		myPoints[0].show();

		apply_surface( targetX, targetY, target, screen,NULL);

		if(bGameOver == true)
		{
			SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0x00, 0x00 ) );
			font.show_text( 270, 150, "You loose!", screen );
			font.show_text( 260, 200, "try again !", screen );
			font.show_text( 220, 250, "and press a key", screen );
		}
		if(iScore == 12)
		{
			SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xC2, 0x00 ) );
			font.show_text( 250, 150, "You win!", screen );
			font.show_text( 100, 200, "You are a real diego now !", screen );
			font.show_text( 200, 250, "and press a key", screen );
			SDL_Flip( screen );
			while (quit!=true)
			{
				while( SDL_PollEvent( &event ) )
				{
					if( event.type == SDL_KEYDOWN )
					{
					    quit=true;
					}
				}
			}
		}

        //Mise à jour de l'écran
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }
        //Tant que le timer fps n'est pas assez haut
        while( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
		{
            //On attend...
        }
    }

    //Nettoyage
    clean_up();

    return 0;
}

int MyRand(int a, int b){
    return rand()%(b-a) +a;
}

int collision (int pos1X, int pos1Y, int pos2X, int pos2Y)
{
	//Nous savons que les sufraces font 45 sur 45
	//On devrait beaucoup changer la fonction si on voulait des objets avec des tailles différentes
    int result = NULL;
    if(pos1X > pos2X+SPRITE_WIDTH-1
       || pos1X+SPRITE_WIDTH-1 < pos2X
       || pos1Y > pos2Y+SPRITE_WIDTH-1
       || pos1Y+SPRITE_WIDTH-1 < pos2Y)

        result = 0;
    else
    {
        result = 1;
    }


    return result;
}
