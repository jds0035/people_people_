#include <iostream>
#include <fstream>
#include "windows.h"
#include "SDL.h"
#include <SDL_mixer.h>
#include <string>
#include "variables.h"
#include "classes.h"
#include <time.h>

/*


ctrl+m+h	

add the images to resource files and load from there.

*/


int triangle_a[number_of_teams] = {0, 0};
int triangle_z[number_of_teams] = {number_of_triangles[0]-1,number_of_triangles[1]-1};
int square_a[number_of_teams] = {triangle_z[0]+1, triangle_z[1]+1}; //for lists and stuff
int square_z[number_of_teams] = {triangle_z[0]+number_of_squares[0], triangle_z[1]+number_of_squares[1]};
int circle_a[number_of_teams] = {square_z[0]+1,square_z[1]+1};
int circle_z[number_of_teams] = {square_z[0]+number_of_circles[0], square_z[1]+number_of_circles[1]};
int octagon_a[number_of_teams] = {circle_z[0]+1,circle_z[1]+1};
int octagon_z[number_of_teams] = {circle_z[0]+number_of_octagons[0], circle_z[1]+number_of_octagons[1]};
int xclick=0;
int yclick=0;
SDL_Event sdl_event;

void renderTexture(SDL_Texture *texx, SDL_Renderer *renn, int x, int y){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = (x+1)*50; //x,y are in index of which board position it's in.. -95 because we need the top left corner, and they're 90 pixel squares... this could be done better
	dst.y = (y+1)*50;  ///////////////////////////////////////things to change when board changes
	//Query the texture to get its width and height to use
	SDL_QueryTexture(texx, NULL, NULL, &dst.w, &dst.h);
	//SDL_RenderCopy(renn, texx, NULL, &dst);

	double angle = GetTickCount();
	angle = angle/(8+std::abs((x-8)+(y-8)));
	int angle_i = angle;
	angle_i = angle_i % 360;

	SDL_RenderCopyEx(renn,texx,NULL,&dst,angle_i,NULL,SDL_FLIP_NONE);


} //definition of function that renders textures

void renderTexture_pixel(SDL_Texture *texx, SDL_Renderer *renn, int x, int y){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x; 
	dst.y = y;  
	//Query the texture to get its width and height to use
	SDL_QueryTexture(texx, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(renn, texx, NULL, &dst);

	
} //definition of function that renders textures

void bigrenderTexture_pixel(SDL_Texture *texx, SDL_Renderer *renn, int x, int y){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x; 
	dst.y = y;  
	//Query the texture to get its width and height to use
	SDL_QueryTexture(texx, NULL, NULL, &dst.w, &dst.h);
	dst.w *=3;
	dst.h *=3;
	
	SDL_RenderCopy(renn, texx, NULL, &dst);
} //definition of function that renders textures



 
 

int linecheck(int x1, int y1, int x2, int y2, int spaces[][y_boxes+1])
{


			int x1n = (x1+2)*50-25;
			int y1n = (y1+2)*50-25; //things to change when board changes
	 		int x2n = (x2+2)*50-25;
	 		int y2n = (y2+2)*50-25;	
			double x1_p = x1n;
			double y1_p = y1n;
			double x2_p = x2n;
			double y2_p = y2n;
			double slope = (y2_p-y1_p)/(x2_p-x1_p);
			


	 

			
			int x_test;
			int y_test;
			
			int temp;
			if (x1n>x2n)
			{temp = x2n;
			x2n=x1n;
			x1n=temp;
			temp = y2n;
			y2n=y1n;
			y1n=temp;}
			
			int xcounter = 0;
			for (int i = x1n;i<x2n;i++)
			{ y_test = slope*(i-x1n)+y1n;
			double x_raw = i;
			double y_raw = y_test;
			 int x_location = x_raw/50-1;
			 int y_location = y_raw/50-1; //things to change when board ;
			 if (spaces[x_location][y_location]==1)

			 {xcounter = xcounter+1;
			  if (xcounter>2)
				  {return 0;}
			 
			}
			 else
			 {xcounter=0;} // so if 3 pixels in a row are inside a black square, it returns 0.




			
			}

						if (y1n>y2n)
			{temp = x2n;
			x2n=x1n;
			x1n=temp;
			temp = y2n;
			y2n=y1n;
			y1n=temp;}

			int ycounter=0;
			for (int i = y1n;i<y2n;i++)
			{x_test = (1/slope)*(i-y1n)+x1n;
			double x_raw = x_test;
			double y_raw = i;
			 int x_location = x_raw/50-1;
			 int y_location = y_raw/50-1; //things to change when board }
			 //std::cout << "\n  x/y = " << x_location << "  "<< y_location<< "\n ";
			 if (spaces[x_location][y_location]==1)
			 {ycounter=ycounter+1;
			 if (ycounter>2)
			 {return 0;}
			 }
			 else
			 {ycounter=0;}
			 
			}
			
			//std::cout << xcounter << "         " <<ycounter ; 
			return 1;

}

int dig[] = {0,0,0,0,0,0}; 
int digits;

void get_digits(int k, int dig[])
{
	if (k<0)
	{k = -k;}
	int g = 5;
	for (int j=10000;j>0.5;j -= 0.9*j)
	{
		g=g-1;
		for (int i=0;i<11;i++)
		{
			if (k >= j)
			{
				k = k-j;
				dig[g] = dig[g]+1;
			}
		}
	}
}

void clear_digits(int dig[6])
{	for (int i=0;i<6;i++)
{		dig[i]=0;
}
}

void renderstats(SDL_Renderer *ren, SDL_Texture *texx[], SDL_Texture *minus_t, Triangle t1[][number_of_teams],int clicked, int turn, int moves_remaining)
{
	
	int g = 0; //this could be more efficiently done in a loop with an array of g's later

	g = moves_remaining;
		get_digits(g,dig);
		if (moves_remaining <0)
		{
	renderTexture_pixel(minus_t,ren,945,615);
		}
	renderTexture_pixel(texx[dig[1]],ren,970,600);
	renderTexture_pixel(texx[dig[0]],ren,1010,600);
	clear_digits(dig);
	int not_turn;
	if (turn==0)
	{not_turn=1;}
	else if (turn ==1)
	{not_turn=0;}

	int turny;
	int clicky;
	int anything;
	if (clicked >=0 || clicked < number_of_shapes[turn])
	{turny=turn;
	clicky=clicked;}
	if (clicked >= number_of_shapes[turn])
	{turny=not_turn;
	clicky=clicked-number_of_shapes[turn];
	}
	if (clicked>=0)
	{
		g = t1[clicky][turny].power;
	get_digits(g,dig);
	renderTexture_pixel(texx[dig[0]],ren,950,365);
	clear_digits(dig);

	g = t1[clicky][turny].maxrange;
		
	get_digits(g,dig);
	renderTexture_pixel(texx[dig[0]],ren,950,450);
	clear_digits(dig);

	g = t1[clicky][turny].d_max;

	get_digits(g,dig);
	renderTexture_pixel(texx[dig[0]],ren,950,410);
	clear_digits(dig);

	g = t1[clicky][turny].health;

	get_digits(g,dig);
	renderTexture_pixel(texx[dig[1]],ren,950,322);
	renderTexture_pixel(texx[dig[0]],ren,970,322);
	clear_digits(dig);

	g = maxhealth[t1[clicky][turny].shape_type];

	get_digits(g,dig);
	renderTexture_pixel(texx[dig[1]],ren,990,332);
	renderTexture_pixel(texx[dig[0]],ren,1010,332);
	clear_digits(dig);
	}
}




		int Status(int x, int y, Triangle t1[][number_of_teams], int number_of_shapes[number_of_teams], int spaces[][y_boxes+1],int mode, int turn) 
			{//(x,y)???? this needs work
				
				
			
				if (mode == 2&&spaces[x][y]<7)
				{
					return 7;
				}
				
			if (spaces[x][y]==1)
			{
				return 5;
			}
			else
			{
				int counter = 0;
					{for (int j=0;j< number_of_teams;j++)
					{
				for (int i=0;i < number_of_shapes[j] ;i++)
						{if (t1[i][j].x_f == x && t1[i][j].y_f == y)
						{counter += 1;}
						}
					}
				}
			return counter;
			}
				
		}

void SDL_fire(SDL_Renderer *renn, int x1, int y1, int x2, int y2, SDL_Texture *explosion_t)
		{
		//			SDL_RenderClear(renn);
					

			int x1n = (x1+2)*50-25;
			int y1n = (y1+2)*50-25; //things to change when board changes
	 		int x2n = (x2+2)*50-25;
	 		int y2n = (y2+2)*50-25;
			 
			 SDL_SetRenderDrawColor(renn, 255, 0, 0, 255);
						SDL_RenderDrawLine(renn,x1n+1,y1n+1,x2n+1,y2n+1); 
						SDL_RenderDrawLine(renn,x1n-1,y1n-1,x2n-1,y2n-1); 
						SDL_RenderDrawLine(renn,x1n,y1n,x2n,y2n); 
						SDL_RenderDrawLine(renn,x2n+20,y2n,x2n-20,y2n);
						SDL_RenderDrawLine(renn,x2n,y2n+20,x2n,y2n-20);
						SDL_RenderDrawLine(renn,x2n-15,y2n-15,x2n+15,y2n+15);
						SDL_RenderDrawLine(renn,x2n-15,y2n+15,x2n+15,y2n-15);
						renderTexture(explosion_t,renn,x2,y2);
						SDL_RenderPresent(renn);
						SDL_Delay(600);

		}






void renderHealth(SDL_Texture *green_t, SDL_Texture *red_t, SDL_Renderer *renn, Triangle t1[][number_of_teams])
{
	const int width = 35; // things to change when board changes
	SDL_Rect rect[width];
	int green_w;
	int green_h;
	SDL_QueryTexture(green_t, NULL, NULL, &green_w, &green_h);
	for (int j = 0;j<number_of_teams;j++)
	{
	for (int i=0;i<number_of_shapes[j];i++)
	{
	for (int p=0; p<width;p++)
	{
		double width_d = width;
			double p_d = p;
		rect[p].x=(t1[i][j].x_f+1)*50+(50-width)/2+p;
		rect[p].y=(t1[i][j].y_f-1+2)*50+45; //things to change when board changes
		rect[p].w=green_w;
		rect[p].h=green_h;
		if (t1[i][j].health_fraction>p_d/width_d)
		{
	SDL_RenderCopy(renn, green_t, NULL, &rect[p]);
		}
		else
		{
	SDL_RenderCopy(renn, red_t, NULL, &rect[p]);

		}
	}
	}
	
			

		////////////to finish!!!!!
	}

}




	int clickindex(SDL_Event sdl_event)

	{		double xclick_raw = sdl_event.button.x;
			double yclick_raw = sdl_event.button.y; ////for conversion of mouseclick to integer location of square
			 xclick = xclick_raw/50-1;
			 yclick= yclick_raw/50-1; //things to change when board 
			return (0); //this still needs to be checked if it works
	} 

	




	int DrawEverything(SDL_Renderer *ren, SDL_Texture *board_t, SDL_Texture *bad_box_t, SDL_Texture *triangle_t[], SDL_Texture *square_t[], SDL_Texture *circle_t[], SDL_Texture *octagon_t[], Triangle t1[][number_of_teams],SDL_Texture *green_t,SDL_Texture *red_t, SDL_Texture *player_1_t, SDL_Texture *player_2_t, SDL_Texture *numeral_t[], SDL_Texture *fired_t, SDL_Texture *minus_t, int turn, SDL_Texture *setup_t, SDL_Texture *move_t, SDL_Texture *attack_t, int mode,int clicked, int moves_remaining, int spaces[][y_boxes+1])
	{   
					SDL_RenderClear(ren);
    SDL_RenderCopy(ren, board_t, NULL, NULL);
	int not_turn;
	if(turn==0)
	{not_turn=1;}
	else if(turn==1)
	{not_turn=0;}

	for (int j=0; j<number_of_teams;j++)
	{
		
		for (int i=triangle_a[j];i<=triangle_z[j];i++)
		{
			renderTexture(triangle_t[j], ren, t1[i][j].x_f, t1[i][j].y_f);
			if ((clicked==i && turn == j)||(clicked==i+number_of_shapes[j]&&not_turn==j))
			{
			bigrenderTexture_pixel(triangle_t[j], ren, 900,150);} ////it doesn't get the color right, but besides that, works

		}
		for (int i=square_a[j];i<=square_z[j];i++)
		{
			renderTexture(square_t[j], ren, t1[i][j].x_f, t1[i][j].y_f);
			if ((clicked==i && turn == j)||(clicked==i+number_of_shapes[j]&&not_turn==j))
			{bigrenderTexture_pixel(square_t[j], ren, 900,150);}
		}
		for (int i = circle_a[j]; i<=circle_z[j]; i++)
		{
			renderTexture(circle_t[j], ren, t1[i][j].x_f, t1[i][j].y_f);
			if ((clicked==i && turn == j)||(clicked==i+number_of_shapes[j]&&not_turn==j))
			{bigrenderTexture_pixel(circle_t[j], ren, 900,150);}
		}
		for (int i = octagon_a[j]; i<=octagon_z[j]; i++)
		{
			renderTexture(octagon_t[j], ren, t1[i][j].x_f, t1[i][j].y_f);
			if ((clicked==i && turn == j)||(clicked==i+number_of_shapes[j]&&not_turn==j))
			{bigrenderTexture_pixel(octagon_t[j], ren, 900,150);}
		}

		for (int i=0; i<number_of_shapes[j];i++)
		{
			if (t1[i][j].fired==1)
			{renderTexture(fired_t,ren,t1[i][j].x_f,t1[i][j].y_f);
			}
		}


	}

	



	for (int i=1;i<=x_boxes;i++)
	{
		for(int j=1;j<=y_boxes;j++)////////////////////////
		{
			if (spaces[i][j]==1)
			{
				//std::cout<<spaces[i][j]<< " ";
			renderTexture_pixel(bad_box_t,ren,50*(i+1)+1,50*(j+1)+1);
			}
		}
			//std::cout<< "\n";
	}
	

	if (turn==0)
	{renderTexture_pixel(player_1_t,ren,580,70);}
	else
	{renderTexture_pixel(player_2_t,ren,580,70);}



	if (mode==0) //should have initial positioning picture
	{renderTexture_pixel(move_t,ren,760,70);}
	if (mode==1)
	{renderTexture_pixel(attack_t,ren,760,70);}
	if (mode ==2)
	{renderTexture_pixel(setup_t,ren,760,70);}

	
	renderstats(ren, numeral_t, minus_t, t1, clicked, turn,moves_remaining);
	renderHealth(green_t, red_t, ren, t1);
	
	

	//std::cout << "whoa... \n";

	SDL_RenderPresent(ren); //draws the board and all the pieces


	return 0;
	}

	/*
		  int status_check() //this doesn't work, needs work
	  {
			if (Status(t1[clicked][turn].x_f,t1[clicked][turn].y_f,t1[][number_of_teams],number_of_triangles[]) ==1)
			{return 1;}
			else
			{return 0;}

	  } 
				*/
		
	
	Triangle t1[][number_of_teams] = { {Triangle(1,12), Triangle(15,1)}, 
									   {Triangle(1,13), Triangle(15,2)}, 
									   {Triangle(1,14), Triangle(15,3)}, 
									   {Triangle(1,15), Triangle(15,4)}, 
									   {Triangle(2,12), Triangle(14,1)}, 
									   {Triangle(2,13), Triangle(14,2)}, 
									   {Triangle(2,14), Triangle(14,3)},  
									   {Triangle(2,15), Triangle(14,4)},
									   {Triangle(3,13), Triangle(13,1)}, 
									   {Triangle(3,14), Triangle(13,2)},
									   {Triangle(3,15), Triangle(13,3)}, };




 //so there are 8 triangles, 8 squares
	//// 0 = triangle, 1 = square, etc.
	

int clicked = -1; //tells which triangle has been selected
int smicked = -1; // if a triangle of the other team is selected
int m = 1;       //basically tells if the code is still running
int mode = 2;  //mode (0 is moving, 1 is shooting)
int death = 0; //1 means somethin died
int turn = 0; //0 is triangle, 1 is square
int turn_counter;
int attack = 0; //whether or not attack is about to happen
int moves_remaining = 0; //obvious
int random_map=0;
int percentage=10;
int timesaver=0;
int music_index=0;


int main(int argc, char** argv) ///where the code starts
{
	
	std::string laura = "You are now playing people_people_!";
	LPCSTR craig = laura.c_str();
	int msgbox = MessageBox(NULL,craig,NULL,MB_OKCANCEL);


		int spaces[x_boxes+1][y_boxes+1]; //this is a weird thing, because i want to call the boxes starting with 1, but this array starts with 0. so, the spaces with [0] still exist, but the t1[][].inside() function keeps anyone from getting there.
	
	for (int i = 0;i<=x_boxes;i++)
		{for (int j = 0; j<=y_boxes;j++)
	{
	spaces[i][j] = 0;
	
	}
	}

		std::string line;
    std::ifstream file;
    file.open("map.txt");  //open a file

	for (int i=1;i<=x_boxes;i++)
	{
		for (int j=1;j<=y_boxes;j++)
		{
	getline(file,line, ' ');
	spaces[j][i] = atoi(line.c_str()); ///////////////////////////////////this j/i thing flips the board accross y=x
	std::cout<<spaces[j][i];//<<" " << "( " << i << ", " << j << " )";
		}
		std::cout<<"\n";
	}
	file.close(); 


	std::cout<<"random map? 0 for no, 1 for yes \n";
	std::cin>>random_map;


	if (random_map==1)
	{
	srand (time(NULL));
	std::cout<<"percentage of black squares? 0-100 \n";
	std::cin>>percentage;
	if (std::cin.fail())
	{std::cout << "u a nig \n";
	percentage=15;
	}

	for (int i=1;i<=x_boxes;i++)
	{
		for (int j=1;j<=y_boxes;j++)
		{
			if (spaces[i][j]==0||spaces[i][j]==1 || Status(i,j,t1,number_of_shapes,spaces,mode,turn)==0)
			{
				double random_number = rand() % 100; 
				if (random_number < percentage)
				{
				spaces[i][j]=1;
				}
				else if (spaces[i][j]>7)
				{
				spaces[i][j]=spaces[i][j];
				}
				else
				{
					spaces[i][j]=0;
				}
			}
		}
	}
	}


	
			for (int j=0;j<number_of_teams;j++)
			{
				for (int i=0;i<number_of_shapes[j];i++)
				{
					int type = -1000;
					if (i <= triangle_z[j] && i >= triangle_a[j])
					{type=0;}
					if (i <= square_z[j] && i >= square_a[j])
					{type=1;}	
					if (i <= circle_z[j] && i >= circle_a[j])
					{type=2;}
					if (i <= octagon_z[j] && i >= octagon_a[j])
					{type=3;}

					t1[i][j].set_values(type);
				}
			}
			
	if (random_map==666)
	{total_moves=500;
	for (int j = 0; j<number_of_teams;j++)
	{	
	for (int i=0;i<number_of_shapes[j];i++)
	{
		t1[i][j].d_max=500;
		t1[i][j].maxrange = 30;
		mode=0;
	}
	}
	}



	
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1){
  //      std::cout << SDL_GetError() << std::endl;
        return 1;
    }

	Mix_Music *menu_music = NULL;
	Mix_Music *theme_music_1 = NULL;
	Mix_Music *theme_music_2 = NULL;
	  if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    
                }
	  
	  
		   menu_music = Mix_LoadMUS( "People People Menu.mp3" );
		   theme_music_1 = Mix_LoadMUS( "People People Theme 1.mp3" );
		   theme_music_2 = Mix_LoadMUS( "People People Theme 2.mp3" );


	   SDL_Window *win = nullptr;
    win = SDL_CreateWindow("people_people_", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	
	   SDL_Renderer *ren = nullptr;
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


	  SDL_Surface *board = nullptr;
    board = SDL_LoadBMP("bond-o.bmp");


	    SDL_Texture *board_t = nullptr;
    board_t = SDL_CreateTextureFromSurface(ren, board);
    SDL_FreeSurface(board);
	
	  SDL_Surface *bad_box = nullptr;
    bad_box = SDL_LoadBMP("bad_box.bmp");


	    SDL_Texture *bad_box_t = nullptr;
    bad_box_t = SDL_CreateTextureFromSurface(ren, bad_box);
    SDL_FreeSurface(bad_box);




	SDL_Surface *triangle[number_of_teams] = {nullptr, nullptr};
    triangle[0] = SDL_LoadBMP("triangle[0].bmp");
	triangle[1] = SDL_LoadBMP("triangle[1].bmp");
	   
	SDL_Texture *triangle_t[number_of_teams] = {nullptr, nullptr};
	for (int i=0;i<number_of_teams;i++)
	{
	SDL_SetColorKey(triangle[i], SDL_TRUE, 0x000000);
    triangle_t[i] = SDL_CreateTextureFromSurface(ren, triangle[i]);
    SDL_FreeSurface(triangle[i]);
SDL_SetTextureBlendMode(triangle_t[i],SDL_BLENDMODE_BLEND);
	}

	
	SDL_Surface *square[number_of_teams] = {nullptr,nullptr};
    square[0] = SDL_LoadBMP("square[0].bmp");
    square[1] = SDL_LoadBMP("square[1].bmp");


	   
	SDL_Texture *square_t[number_of_teams] = {nullptr,nullptr};
	for (int i=0;i<number_of_teams;i++)
	{
	SDL_SetColorKey(square[i], SDL_TRUE, 0x000000);
    square_t[i] = SDL_CreateTextureFromSurface(ren, square[i]);
    SDL_FreeSurface(square[i]);
	SDL_SetTextureBlendMode(square_t[i],SDL_BLENDMODE_BLEND);
	}

		SDL_Surface *circle[number_of_teams] = {nullptr,nullptr};
    circle[0] = SDL_LoadBMP("circle[0].bmp");
    circle[1] = SDL_LoadBMP("circle[1].bmp");

	   
	SDL_Texture *circle_t[number_of_teams] = {nullptr,nullptr};
	for (int i=0;i<number_of_teams;i++)
	{
	SDL_SetColorKey(circle[i], SDL_TRUE, 0x000000);
    circle_t[i] = SDL_CreateTextureFromSurface(ren, circle[i]);
    SDL_FreeSurface(circle[i]);
	SDL_SetTextureBlendMode(circle_t[i],SDL_BLENDMODE_BLEND);
	}		
	
	SDL_Surface *octagon[number_of_teams] = {nullptr,nullptr};
    octagon[0] = SDL_LoadBMP("octagon[0].bmp");
    octagon[1] = SDL_LoadBMP("octagon[1].bmp");

	   
	SDL_Texture *octagon_t[number_of_teams] = {nullptr,nullptr};
	for (int i=0;i<number_of_teams;i++)
	{
	SDL_SetColorKey(octagon[i], SDL_TRUE, 0x000000);
    octagon_t[i] = SDL_CreateTextureFromSurface(ren, octagon[i]);
    SDL_FreeSurface(octagon[i]);
	SDL_SetTextureBlendMode(octagon_t[i],SDL_BLENDMODE_BLEND);
	}

	SDL_Surface *red = nullptr;
	red = SDL_LoadBMP("red_pixel.bmp");
	SDL_Texture *red_t = nullptr;
	red_t = SDL_CreateTextureFromSurface(ren,red);
	SDL_FreeSurface(red);

	SDL_Surface *green = nullptr;
	green = SDL_LoadBMP("green_pixel.bmp");
	SDL_Texture *green_t = nullptr;
	green_t = SDL_CreateTextureFromSurface(ren,green);
	SDL_FreeSurface(green);

		SDL_Surface *player_1 = nullptr;
	player_1 = SDL_LoadBMP("player_1.bmp");
	SDL_SetColorKey(player_1, SDL_TRUE, 0x000000);
	SDL_Texture *player_1_t = nullptr;
	player_1_t = SDL_CreateTextureFromSurface(ren,player_1);
	SDL_SetTextureBlendMode(player_1_t,SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(player_1);

		SDL_Surface *player_2 = nullptr;
	player_2 = SDL_LoadBMP("player_2.bmp");
	SDL_SetColorKey(player_2, SDL_TRUE, 0x000000);
	SDL_Texture *player_2_t = nullptr;
	player_2_t = SDL_CreateTextureFromSurface(ren,player_2);
	SDL_SetTextureBlendMode(player_2_t,SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(player_2);

	
		SDL_Surface *move = nullptr;
	move = SDL_LoadBMP("move.bmp");
	SDL_SetColorKey(move, SDL_TRUE, 0x000000);

	SDL_Texture *move_t= nullptr;
	move_t = SDL_CreateTextureFromSurface(ren,move);
	SDL_SetTextureBlendMode(move_t,SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(move);
	
		SDL_Surface *attack_s = nullptr;
	attack_s = SDL_LoadBMP("attack.bmp");
	SDL_SetColorKey(attack_s, SDL_TRUE, 0x000000);
	SDL_Texture *attack_t= nullptr;
	attack_t = SDL_CreateTextureFromSurface(ren,attack_s);
	SDL_SetTextureBlendMode(attack_t,SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(attack_s);

		SDL_Surface *setup_s = nullptr;
	setup_s = SDL_LoadBMP("setup.bmp");
	SDL_SetColorKey(setup_s, SDL_TRUE, 0x000000);
	SDL_Texture *setup_t= nullptr;
	setup_t = SDL_CreateTextureFromSurface(ren,setup_s);
	SDL_SetTextureBlendMode(setup_t,SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(setup_s);

	SDL_Surface *numeral[10] = {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
    numeral[0] = SDL_LoadBMP("zero.bmp");
    numeral[1] = SDL_LoadBMP("one.bmp");
    numeral[2] = SDL_LoadBMP("two.bmp");
    numeral[3] = SDL_LoadBMP("three.bmp");
    numeral[4] = SDL_LoadBMP("four.bmp");
    numeral[5] = SDL_LoadBMP("five.bmp");
    numeral[6] = SDL_LoadBMP("six.bmp");
    numeral[7] = SDL_LoadBMP("seven.bmp");
    numeral[8] = SDL_LoadBMP("eight.bmp");
    numeral[9] = SDL_LoadBMP("nine.bmp");

	   
	SDL_Texture *numeral_t[10] = {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
	for (int i=0;i<10;i++)
	{
	SDL_SetColorKey(numeral[i], SDL_TRUE, 0x000000);
    numeral_t[i] = SDL_CreateTextureFromSurface(ren, numeral[i]);
    SDL_FreeSurface(numeral[i]);
	SDL_SetTextureBlendMode(numeral_t[i],SDL_BLENDMODE_BLEND);
	}


		  SDL_Surface *fired = nullptr;
    fired = SDL_LoadBMP("fired.bmp");
	SDL_SetColorKey(fired, SDL_TRUE, 0x000000);
	SDL_Texture *fired_t = nullptr;
    fired_t = SDL_CreateTextureFromSurface(ren, fired);
    SDL_FreeSurface(fired);
	
SDL_SetTextureBlendMode(fired_t,SDL_BLENDMODE_BLEND);

		  SDL_Surface *explosion = nullptr;
    explosion = SDL_LoadBMP("explosion.bmp");
	SDL_SetColorKey(explosion, SDL_TRUE, 0x000000);
	SDL_Texture *explosion_t = nullptr;
    explosion_t = SDL_CreateTextureFromSurface(ren, explosion);
    SDL_FreeSurface(explosion);
	
SDL_SetTextureBlendMode(explosion_t,SDL_BLENDMODE_BLEND);


			  SDL_Surface *minus = nullptr;
		minus = SDL_LoadBMP("minus.bmp");
		SDL_SetColorKey(minus, SDL_TRUE, 0x000000);
		SDL_Texture *minus_t = nullptr;
		minus_t = SDL_CreateTextureFromSurface(ren, minus);
		SDL_FreeSurface(minus);
		SDL_SetTextureBlendMode(minus_t,SDL_BLENDMODE_BLEND);

 //loading stuff in, making textures

  
	   SDL_RenderClear(ren);
    SDL_RenderCopy(ren, board_t, NULL, NULL);
    SDL_RenderPresent(ren); //drawing the board

	
	  Mix_PlayMusic( menu_music, -1 );
	
	while(m==1) //this is the main loop... if m == 0, then the game is over
	
{	
	


	if (turn_counter>2)
	{
	total_moves=13;
	}


	if (mode == 1)
		{moves_remaining=0;}
	if (mode == 0)
	{
		int g = 0;
		for (int i = 0; i < number_of_shapes[turn];i++)
		{g = g+t1[i][turn].d;
		}

		moves_remaining = total_moves - g;
	}
	if (mode==2)
	{moves_remaining=total_moves;}
	

	DrawEverything(ren, board_t, bad_box_t, triangle_t, square_t, circle_t, octagon_t, t1, green_t, red_t,player_1_t,player_2_t,numeral_t,fired_t,minus_t,turn,setup_t,move_t,attack_t,mode,clicked,moves_remaining,spaces); //draws everything
//	std::cout << "mode = " << mode << "attack = " << attack << "clicked = " << clicked << "turn = " << turn << "status = " << Status(xclick,yclick,t1,number_of_shapes, spaces,mode,turn) <<	"\n"; 
	
	//std::cout << Status(t1[clicked][turn].x_f,t1[clicked][turn].y_f,t1[][number_of_teams],number_of_triangles[]); 
	 
	
while(SDL_PollEvent(&sdl_event) > 0)
{
	 switch(sdl_event.type)
    {

		 case(SDL_KEYDOWN):
		switch(sdl_event.key.keysym.sym)
        {
        case(SDLK_UP):

			if (clicked == -1|| clicked >= number_of_shapes[turn])//gotta make sure we clicked one of our own guys
			{break;}
			if ((mode == 2 ||mode == 0) && Status (t1[clicked][turn].x_f,t1[clicked][turn].y_f-1,t1,number_of_shapes,spaces,mode,turn)==0)
			{
			t1[clicked][turn].move_up(mode);
			//std::cout << "check = " <<t1[clicked][turn].check() << "inside = " << t1[clicked][turn].inside() << "distance = " << t1[clicked][turn].d;
			}
            break;
		
        case(SDLK_LEFT):
			if (clicked == -1|| clicked >= number_of_shapes[turn])
			{break;}
			if ((mode == 2 ||mode == 0) && Status (t1[clicked][turn].x_f-1,t1[clicked][turn].y_f,t1,number_of_shapes,spaces,mode,turn)==0)
			{
			t1[clicked][turn].move_left(mode);
			}
            break;
	 
        case(SDLK_DOWN):
			if (clicked == -1|| clicked >= number_of_shapes[turn])
			{break;}
			if ((mode == 2 ||mode == 0) && Status (t1[clicked][turn].x_f,t1[clicked][turn].y_f+1,t1,number_of_shapes,spaces,mode,turn)==0)
			{
			t1[clicked][turn].move_down(mode);
			}
			break;

        case(SDLK_RIGHT):
			if (clicked == -1|| clicked >= number_of_shapes[turn])
			{break;}
			if ((mode == 2 ||mode == 0) && Status (t1[clicked][turn].x_f+1,t1[clicked][turn].y_f,t1,number_of_shapes,spaces,mode,turn)==0)
			{
			t1[clicked][turn].move_right(mode);
			}
			break;

		case(SDLK_SPACE):
                            //If there is no music playing
                            if( Mix_PlayingMusic() == 0 )
                            {
                                //Play the music
                                Mix_PlayMusic( theme_music_1, -1 );
                            }
                            //If music is being played
                            else
                            {
                                //If the music is paused
                                if( Mix_PausedMusic() == 1 )
                                {
                                    //Resume the music
                                    Mix_ResumeMusic();
                                }
                                //If the music is playing
                                else
                                {
                                    //Pause the music
                                    Mix_PauseMusic();
                                }
                            }
                            break;
			
		case(SDLK_RETURN):
			
									

			if (mode==0 || mode == 2)
			{

			if (moves_remaining <0)
			{	std::string mosha = "you gotta take a few of those moves back";
	LPCSTR clyde = mosha.c_str();
	int msgbox = MessageBox(NULL,clyde,NULL,MB_OKCANCEL);
			break;
			}

			for (int i=0;i<number_of_shapes[turn];i++)
			{t1[i][turn].fixation();}
			if (mode == 2)
			{
			clicked = -1; //so that nothin's selected
			}
			if (mode == 0)
			{
			mode = 1;
			attack = 1; //so you can attack without reclick
			
                                Mix_PlayMusic( theme_music_2, -1 );
			}
			if (mode ==2)
			{
				{turn +=1;}
				if (turn == number_of_teams)
				{mode = 0;
				turn = 0;
				Mix_PlayMusic( theme_music_1, -1 );}
			}
			break;
			}


			else if (mode ==1)
			{
				
				for (int i = 0; i<number_of_shapes[turn]; i++)
				{t1[i][turn].unfire();}

				if (turn == 0)
				{turn = 1;}
				else if (turn = 1)
				{turn = 0;
				turn_counter += 1;}	
				
                      Mix_PlayMusic( theme_music_1, -1 );
			clicked = -1;
			mode = 0;
				attack = 0;
			break;
			}
				
	 
	  }

		case(SDL_MOUSEBUTTONDOWN):


			 if(sdl_event.button.button == SDL_BUTTON_RIGHT)
			 {
				 clickindex(sdl_event);

				 if (mode == 0)
				 {
					 for (int i=0;i<number_of_shapes[turn];i++)
					 {
					 if(xclick==t1[i][turn].x_f && yclick==t1[i][turn].y_f)
						{
							t1[i][turn].reset();
						}
					 }
				 }
				else if (attack ==1)
			{
				std::cout << "???? \n";
				int not_turn = -1;
				int attacked = -1;
				if (turn == 0)
				{not_turn = 1;}
				else if (turn = 1)
				{not_turn = 0;}

				//std::cout<<not_turn<<"   " <<turn;
				for (int i=0;i<number_of_shapes[not_turn];i++)
				{
				if(xclick==t1[i][not_turn].x_f && yclick == t1[i][not_turn].y_f) 
				{attacked = i;
				int max = t1[clicked][turn].maxrange;
				int one = t1[attacked][not_turn].x_f;
				int two = t1[clicked][turn].x_f;
				int three = t1[attacked][not_turn].y_f;
				int four = t1[clicked][turn].y_f;
				//std::cout << "attacked = " << attacked << "max = "<< max << "shit = " << one <<" " << two << " "<< three << " "<< four;
				if ((std::abs(one-two)+std::abs(three-four))<= max && linecheck(two,four,one,three,spaces)==1)
				{	
					std::cout << "!!!!! \n";
					if (t1[clicked][turn].fired == 0)
					{
						t1[clicked][turn].fire();
						SDL_fire(ren,two,four,one,three,explosion_t);

						//!!!
						int g = t1[clicked][turn].power;
					t1[attacked][not_turn].take_damage(g); 
					//number_of_triangles[not_turn] += -1;
					}
				}
				}
			}
				
			attack = 0;
			break;
		}

			 }


        if(sdl_event.button.button == SDL_BUTTON_LEFT)
		{
		clickindex(sdl_event); 
		int not_turn;
		if (turn == 0)
		{not_turn = 1;}
		else if (turn = 1)
		{not_turn = 0;}

		for (int i=0;i<number_of_shapes[turn];i++)
		{
			if (xclick==t1[i][turn].x_f && yclick==t1[i][turn].y_f)
			{
				clicked=i;
				if (mode ==1)
					{attack =1;}
			}
				
		}

		for (int i=0;i<number_of_shapes[not_turn];i++)
		{	
			if (xclick==t1[i][not_turn].x_f && yclick==t1[i][not_turn].y_f)
			{
			clicked=i+number_of_shapes[turn];		
			}
		}
		break;
		}
			
		}

		

	  
	 
	 
	 }
}	

	 
	
	 SDL_Delay(2000);
	 
	    SDL_DestroyTexture(board_t);
	    SDL_DestroyTexture(bad_box_t);
		SDL_DestroyTexture(triangle_t[number_of_teams]);
		SDL_DestroyTexture(square_t[number_of_teams]);
		SDL_DestroyTexture(circle_t[number_of_teams]);
		SDL_DestroyTexture(octagon_t[number_of_teams]);
		SDL_DestroyTexture(green_t);
		SDL_DestroyTexture(red_t);
		SDL_DestroyTexture(player_1_t);
		SDL_DestroyTexture(player_2_t);
		SDL_DestroyTexture(setup_t);
		SDL_DestroyTexture(move_t);
		SDL_DestroyTexture(attack_t);
		SDL_DestroyTexture(fired_t);
		SDL_DestroyTexture(minus_t);
		SDL_DestroyTexture(explosion_t);
for (int i=0;i<10;i++)
{SDL_DestroyTexture(numeral_t[i]);
}
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);

	
	Mix_FreeMusic(menu_music);
    menu_music = NULL;
	Mix_FreeMusic(theme_music_1);
    theme_music_1 = NULL;
	Mix_FreeMusic(theme_music_2);
    theme_music_2 = NULL;

	    SDL_Quit();
    return 0;
}



