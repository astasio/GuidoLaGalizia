#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_thread.h"
#include "SDL/SDL_ttf.h"
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>
#include "cassone.xpm"
#include "galizia.xpm"
#include "galizia1.xpm"
#include "logo.xpm"
#include "lettera.xpm"
#include "tubi.xpm"
#include "filo.xpm"
#include "scrivania.xpm"
//#include<X11/Xlib.h>

SDL_Surface *galizia;
SDL_Surface *galizia1;
SDL_Surface *cassone;
SDL_Surface *screen;
SDL_Surface *logo;
SDL_Surface *lettera;
SDL_Surface *tubi;
SDL_Surface *filo;
SDL_Surface *scrivania;

SDL_Surface *txt;
TTF_Font *font;
SDL_Color textColor = { 0, 0, 0 };
    
SDL_Thread *thread;

SDL_Event event;

SDL_Rect offset,offset2;
SDL_Rect base,terra,striscia,cielo;

Mix_Music *music;
Mix_Chunk *collision;
Mix_Chunk *salto;

int quit=1;
bool run;

void apply_surface(int x, int y, SDL_Surface *source,SDL_Surface *destination);
void esci();

void draw_bg()
{
base.x=0;
base.y=450;
base.w=1080;
base.h=45;
terra.x = 0;
terra.y = 275;
terra.w = 1080;
terra.h = 220;
striscia.x=0;
striscia.y=265;
striscia.w=1080;
striscia.h=10;
cielo.x=0;
cielo.y=0;
cielo.w=1080;
cielo.h=265;
SDL_FillRect(screen,&terra,0x006611);
SDL_FillRect(screen,&striscia,0x00AA44);
SDL_FillRect(screen,&cielo,0xAAFFDD);
SDL_FillRect(screen, &base,0xAAFFDD);
}

void apply_surface(int x, int y, SDL_Surface *source,SDL_Surface *destination)
{
offset.x=x;
offset.y=y;
SDL_BlitSurface(source,NULL,destination, &offset);
}
void apply_surface2(int x, int y, SDL_Surface *source,SDL_Surface *destination)
{
offset2.x=x;
offset2.y=y;
SDL_BlitSurface(source,NULL,destination, &offset2);
}

void falling_sky()
{
        int score=0;
        char str[12];
	int crash=0;
	int yc=340;
	int xc=1080;
        int xt=2080;
        int xf=1580;
        int xs=2580;
        while(quit==1)
        {	
            cassone=IMG_ReadXPMFromArray(cassone_xpm);
            tubi=IMG_ReadXPMFromArray(tubi_xpm);
            filo=IMG_ReadXPMFromArray(filo_xpm);
            scrivania=IMG_ReadXPMFromArray(scrivania_xpm);
  sprintf(str, "%d", score);          
txt=TTF_RenderText_Solid(font, str, textColor);
            draw_bg();		
            apply_surface2(xf,0,filo,screen);
           apply_surface2(xs,200,scrivania,screen);
            apply_surface(offset.x,offset.y,galizia,screen);
            apply_surface2(xc,yc,cassone,screen);
            apply_surface2(xt,0,tubi,screen); 
            
            apply_surface2(800,20,txt,screen);
            SDL_Flip(screen); 
		SDL_Delay(10);
            draw_bg();
            apply_surface2(xf,0,filo,screen);
            apply_surface2(xs,200,scrivania,screen);
            apply_surface(offset.x,offset.y,galizia1,screen);
            apply_surface2(xc,yc,cassone,screen);
            apply_surface2(xt,0,tubi,screen);  
            
            apply_surface2(800,20,txt,screen);
            SDL_Flip(screen); 
		SDL_Delay(10);
		xc -=25;
                xt-=30;
                xf-=12;
                xs-=12;
                if(yc>=offset.y-150 && yc<=offset.y+150 && xc>=offset.x-150 && xc<=offset.x+150)
			{
				Mix_PlayChannel( -1, collision, 0);
				SDL_Delay(200);
				xc=1080;
				crash++;
				if(crash==3)
				{
					draw_bg();
					SDL_Delay(1800);
                                    run=false;
                                    esci();
				}
			}
		if(xc<0)
		{
			xc=1080;
                        score++;
		}
		if(xt<0)
                {
                        xt=2080;
                }
                if(xf<0){xf=1580;}
                if(xs<0){xs=2580;}
    }    
}
void active()
{
SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_EVENTMASK(SDL_MOUSEBUTTONDOWN));
	run=true;
}
void jump()
{
	run=false;
	Mix_PlayChannel( -1, salto, 0);
    int j=1;
    while(j<25)
    {
    	apply_surface(offset.x,offset.y-j,galizia,screen);
        SDL_Flip(screen);
        SDL_Delay(2*j);
         j+=1;
    }
apply_surface(offset.x,offset.y+10,galizia,screen);
SDL_Flip(screen);
SDL_Delay(150);
apply_surface(offset.x,offset.y+40,galizia,screen);
SDL_Flip(screen);
SDL_Delay(80);
apply_surface(offset.x,offset.y+80,galizia,screen);
SDL_Flip(screen);
SDL_Delay(20);
apply_surface(offset.x,offset.y+150,galizia,screen);
SDL_Flip(screen);
active();
}

void my_thread()
{
    falling_sky();
}

void esci()
{
    run=false;
    SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_EVENTMASK(SDL_MOUSEBUTTONDOWN));
    draw_bg();			 lettera=IMG_ReadXPMFromArray(lettera_xpm);
    apply_surface2(400,100,lettera,screen);
    SDL_Flip(screen);
    SDL_Delay(5000);
   
    Mix_FreeMusic( music ); 
    SDL_FreeSurface(lettera);
    SDL_FreeSurface(screen);
    SDL_KillThread(thread);
    Mix_FreeChunk( collision ); 
    SDL_FreeSurface(galizia);
    SDL_FreeSurface(galizia1);
    SDL_FreeSurface(cassone);
    SDL_FreeSurface(logo);
    SDL_FreeSurface(txt);
    TTF_CloseFont(font);
    SDL_Quit();
   quit=0;
}

int main()
{
    //XInitThreads();
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    font=TTF_OpenFont("resources/font.ttf", 30);
    screen = SDL_SetVideoMode(1080,540,0,SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
    SDL_WM_SetCaption("GuidoLaGalizia",NULL);

    Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 );
    music = Mix_LoadMUS("resources/bg_music.wav");
    Mix_VolumeMusic(20);
    collision = Mix_LoadWAV("resources/collision.wav");
    salto=Mix_LoadWAV("resources/salto.wav");
    if( Mix_PlayingMusic() == 0 ) 
    { 
	if( Mix_PlayMusic( music, -1 ) == -1 ) 
	{ 
            return 1; 
	}
    }

    draw_bg();
    logo=IMG_ReadXPMFromArray(logo_xpm);
    apply_surface(150,150,logo,screen);
    SDL_Flip(screen);
    SDL_Delay(3000);

    draw_bg();
    galizia=IMG_ReadXPMFromArray(galizia_xpm);
    galizia1=IMG_ReadXPMFromArray(galizia1_xpm);
    apply_surface(50,280,galizia,screen);
    SDL_Flip(screen);
    thread = SDL_CreateThread( my_thread, NULL );

    active();
    
    while(quit==1)
    {
    	while(run==false)
    	{
    		SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_EVENTMASK(SDL_MOUSEBUTTONDOWN));
    	}
     SDL_WaitEvent(&event);
		switch(event.type)
		{
		case SDL_QUIT:
                        quit=0;
		case SDL_MOUSEBUTTONDOWN:
		if(run==true)
		{
			jump();
		}
    }
}
return 0;
}