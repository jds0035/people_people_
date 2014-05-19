#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include "windows.h"
#include "SDL.h"
#include <SDL_mixer.h>
#include <string>
#include "variables.h"
#include "classes.h"
#include <time.h>
#include <cstdlib>
#include <cstring>




//ctrl+m+h	
	
//add the images to resource files and load from there.

//sound effects

//black squares go with music

//upgrade stuff

//change mouse cursor

//stats->corners

//lines match team colors

//random piece placement should come with first to move getting less moves or something like that.

//screen size stuff



int triangle_a[number_of_teams] = {0, 0, 0, 0};
int triangle_z[number_of_teams] = {number_of_triangles[0]-1,number_of_triangles[1]-1,number_of_triangles[2]-1,number_of_triangles[3]-1};
int square_a[number_of_teams] = {triangle_z[0]+1, triangle_z[1]+1,triangle_z[2]+1,triangle_z[3]+1}; //for lists and stuff
int square_z[number_of_teams] = {triangle_z[0]+number_of_squares[0], triangle_z[1]+number_of_squares[1], triangle_z[2]+number_of_squares[2], triangle_z[3]+number_of_squares[3]};
int circle_a[number_of_teams] = {square_z[0]+1,square_z[1]+1,square_z[2]+1,square_z[3]+1};
int circle_z[number_of_teams] = {square_z[0]+number_of_circles[0], square_z[1]+number_of_circles[1], square_z[2]+number_of_circles[2], square_z[3]+number_of_circles[3]};
int octagon_a[number_of_teams] = {circle_z[0]+1,circle_z[1]+1,circle_z[2]+1,circle_z[3]+1};
int octagon_z[number_of_teams] = {circle_z[0]+number_of_octagons[0], circle_z[1]+number_of_octagons[1],circle_z[2]+number_of_octagons[2],circle_z[3]+number_of_octagons[3]};
int xclick=0;
int yclick=0;
SDL_Event sdl_event;
int horizontal=1800;
int vertical=900;
int delta_x=0;
int delta_y=0;

void GetDesktopResolution(int& horizontal, int& vertical)
{
   RECT desktop;
   // Get a handle to the desktop window
   const HWND hDesktop = GetDesktopWindow();
   // Get the size of screen to the variable desktop
   GetWindowRect(hDesktop, &desktop);
   // The top left corner will have coordinates (0,0)
   // and the bottom right corner will have coordinates
   // (horizontal, vertical)
   horizontal = desktop.right;
   vertical = desktop.bottom;
}

void renderTexture_rotate(SDL_Texture *texx, SDL_Renderer *renn, int x, int y,int delta_x,int delta_y){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = (x+1)*50+delta_x; //x,y are in index of which board position it's in.. -95 because we need the top left corner, and they're 90 pixel squares... this could be done better
	dst.y = (y+1)*50+delta_y ;  ///////////////////////////////////////things to change when board changes
	//Query the texture to get its width and height to use
	SDL_QueryTexture(texx, NULL, NULL, &dst.w, &dst.h);
	//SDL_RenderCopy(renn, texx, NULL, &dst);

	double angle = GetTickCount();
	angle = angle/(8+std::abs(x-8)+std::abs(y-8));
	int angle_i = angle;
	angle_i = angle_i % 360;

	SDL_RenderCopyEx(renn,texx,NULL,&dst,angle_i,NULL,SDL_FLIP_NONE);


} //definition of function that renders textures

void renderTexture(SDL_Texture *texx, SDL_Renderer *renn, int x, int y,int delta_x,int delta_y){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = (x+1)*50+delta_x; //x,y are in index of which board position it's in.. -95 because we need the top left corner, and they're 90 pixel squares... this could be done better
	dst.y = (y+1)*50+delta_y;  ///////////////////////////////////////things to change when board changes
	SDL_QueryTexture(texx, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(renn, texx, NULL, &dst);
}




void renderTexture_pixel(SDL_Texture *texx, SDL_Renderer *renn, int x, int y,int delta_x,int delta_y){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x+delta_x; 
	dst.y = y+delta_y;  
	//Query the texture to get its width and height to use
	SDL_QueryTexture(texx, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(renn, texx, NULL, &dst);

	
} //definition of function that renders textures

void bigrenderTexture_pixel(SDL_Texture *texx, SDL_Renderer *renn, int x, int y,int delta_x,int delta_y){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x+delta_x; 
	dst.y = y+delta_y;  
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
			 //////////////////need to adjust for double-corners




			
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

void get_digits(int k, int dig[]) //allow for 10 digit numbers
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

void renderstats(SDL_Renderer *ren, SDL_Texture *texx[], SDL_Texture *minus_t, Triangle t1[][number_of_teams],int clicked, int turn, int clicked_turn, int moves_remaining, int real_number_of_teams,int delta_x,int delta_y)
{
	
	int g = 0; //this could be more efficiently done in a loop with an array of g's later

	g = moves_remaining;
		get_digits(g,dig);
		if (moves_remaining <0)
		{
	renderTexture_pixel(minus_t,ren,945,615,delta_x,delta_y);
		}
	renderTexture_pixel(texx[dig[1]],ren,970,600,delta_x,delta_y);
	renderTexture_pixel(texx[dig[0]],ren,1010,600,delta_x,delta_y);
	clear_digits(dig);


	int turny;
	int clicky;
	int anything;
	if (clicked >=0 || clicked < number_of_shapes[turn])
	{turny=turn;
	clicky=clicked;}

	if (clicked >= number_of_shapes[turn])
	{turny=clicked_turn;
	clicky=clicked-number_of_shapes[turn];
	}
	if (clicked>=0)
	{
		g = t1[clicky][turny].power;
	get_digits(g,dig);
	renderTexture_pixel(texx[dig[0]],ren,950,365,delta_x,delta_y);
	clear_digits(dig);

	g = t1[clicky][turny].maxrange;
		
	get_digits(g,dig);
	renderTexture_pixel(texx[dig[0]],ren,950,450,delta_x,delta_y);
	clear_digits(dig);

	g = t1[clicky][turny].d_max;

	get_digits(g,dig);
	renderTexture_pixel(texx[dig[0]],ren,950,410,delta_x,delta_y);
	clear_digits(dig);

	g = t1[clicky][turny].health;

	get_digits(g,dig);
	renderTexture_pixel(texx[dig[1]],ren,950,322,delta_x,delta_y);
	renderTexture_pixel(texx[dig[0]],ren,970,322,delta_x,delta_y);
	clear_digits(dig);

	g = t1[clicky][turny].maximum_health;

	get_digits(g,dig);
	renderTexture_pixel(texx[dig[1]],ren,990,332,delta_x,delta_y);
	renderTexture_pixel(texx[dig[0]],ren,1010,332,delta_x,delta_y);
	clear_digits(dig);
	}
}




		int Status(int x, int y, Triangle t1[][number_of_teams], int number_of_shapes[number_of_teams], int spaces[][y_boxes+1],int mode, int turn, int real_number_of_teams) 
			{
				
				
			
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
					{for (int j=0;j< real_number_of_teams;j++)
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

void SDL_draw_fire(SDL_Renderer *renn, int attack_coordinates[4], SDL_Texture *explosion_t,int delta_x,int delta_y)
{
	
			int x1n = (attack_coordinates[0]+2)*50-25+delta_x;
			int y1n = (attack_coordinates[1]+2)*50-25+delta_y; //things to change when board changes
	 		int x2n = (attack_coordinates[2]+2)*50-25+delta_x;
	 		int y2n = (attack_coordinates[3]+2)*50-25+delta_y;
			 
			 SDL_SetRenderDrawColor(renn, 224, 0, 192, 255);
						SDL_RenderDrawLine(renn,x1n+1,y1n+1,x2n,y2n); 
						SDL_RenderDrawLine(renn,x1n-1,y1n-1,x2n,y2n);
						SDL_RenderDrawLine(renn,x1n+1,y1n-1,x2n,y2n); 
						SDL_RenderDrawLine(renn,x1n-1,y1n+1,x2n,y2n); 
						SDL_RenderDrawLine(renn,x1n,y1n,x2n,y2n); 
						/*
						SDL_RenderDrawLine(renn,x2n+20,y2n,x2n-20,y2n);
						SDL_RenderDrawLine(renn,x2n,y2n+20,x2n,y2n-20);
						SDL_RenderDrawLine(renn,x2n-15,y2n-15,x2n+15,y2n+15);
						SDL_RenderDrawLine(renn,x2n-15,y2n+15,x2n+15,y2n-15);
						*/
						renderTexture_rotate(explosion_t,renn,attack_coordinates[2],attack_coordinates[3],delta_x,delta_y);
						
			 SDL_SetRenderDrawColor(renn, 0, 0, 0, 255);
					//	SDL_RenderPresent(renn);

}





void renderHealth(SDL_Texture *green_t, SDL_Texture *red_t, SDL_Renderer *renn, Triangle t1[][number_of_teams], int real_number_of_teams,int j,int delta_x,int delta_y)
{
	const int width = 35; // things to change when board changes
	SDL_Rect rect[width];
	int green_w;
	int green_h;
	SDL_QueryTexture(green_t, NULL, NULL, &green_w, &green_h);
	
	for (int i=0;i<number_of_shapes[j];i++)
	{
	for (int p=0; p<width;p++)
	{
		double width_d = width;
			double p_d = p;
		rect[p].x=(t1[i][j].x_f+1)*50+(50-width)/2+p+delta_x;
		rect[p].y=(t1[i][j].y_f-1+2)*50+45+delta_y; //things to change when board changes
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




	int clickindex(SDL_Event sdl_event,int delta_x,int delta_y)

	{		double xclick_raw = sdl_event.button.x-delta_x;
			double yclick_raw = sdl_event.button.y-delta_y; ////for conversion of mouseclick to integer location of square
			 xclick = xclick_raw/50-1;
			 yclick= yclick_raw/50-1; //things to change when board 
			return (0); //this still needs to be checked if it works
	} 

	


	void DrawMenu(SDL_Renderer *ren, SDL_Texture *menu_t)
	{SDL_RenderClear;
    SDL_RenderCopy(ren, menu_t, NULL, NULL);
	SDL_RenderPresent(ren); 
	}

	int DrawEverything(SDL_Renderer *ren, SDL_Texture *board_t, SDL_Texture *bad_box_t[], SDL_Texture *triangle_t[], SDL_Texture *square_t[], SDL_Texture *circle_t[], SDL_Texture *octagon_t[], Triangle t1[][number_of_teams], SDL_Texture *sound_t, SDL_Texture *green_t[],SDL_Texture *red_t, SDL_Texture *player_1_t, SDL_Texture *player_2_t, SDL_Texture *player_3_t, SDL_Texture *player_4_t, SDL_Texture *numeral_t[], SDL_Texture *fired_t, SDL_Texture *minus_t, int turn, int clicked_turn, SDL_Texture *setup_t, SDL_Texture *move_t, SDL_Texture *attack_t, SDL_Texture *explosion_t, SDL_Texture *select_t, int mode,int clicked, int moves_remaining, int spaces[][y_boxes+1],int real_number_of_teams, int attack_coordinates[4],int attack_time_counter, int select_time_counter, int select_coordinates[2],int delta_x, int delta_y)
	{   
					SDL_RenderClear(ren);
	renderTexture_pixel(board_t,ren,0,0,delta_x,delta_y);

	for (int j=0; j<real_number_of_teams;j++)
	{




		for (int i=triangle_a[j];i<=triangle_z[j];i++)
		{
			if (t1[i][j].inside()==1)
			{renderTexture_rotate(triangle_t[j], ren, t1[i][j].x_f, t1[i][j].y_f,delta_x,delta_y);}
			if ((clicked==i && turn == j)||(clicked==i+number_of_shapes[j]&&clicked_turn==j))
			{
			bigrenderTexture_pixel(triangle_t[j], ren, 900,150,delta_x,delta_y);}

		}
		for (int i=square_a[j];i<=square_z[j];i++)
		{
			if (t1[i][j].inside()==1)
			{renderTexture_rotate(square_t[j], ren, t1[i][j].x_f, t1[i][j].y_f,delta_x,delta_y);}
			if ((clicked==i && turn == j)||(clicked==i+number_of_shapes[j]&&clicked_turn==j))
			{bigrenderTexture_pixel(square_t[j], ren, 900,150,delta_x,delta_y);}
		}
		for (int i = circle_a[j]; i<=circle_z[j]; i++)
		{
			if (t1[i][j].inside()==1)
			{renderTexture_rotate(circle_t[j], ren, t1[i][j].x_f, t1[i][j].y_f,delta_x,delta_y);}
			if ((clicked==i && turn == j)||(clicked==i+number_of_shapes[j]&&clicked_turn==j))
			{bigrenderTexture_pixel(circle_t[j], ren, 900,150,delta_x,delta_y);}
		}
		for (int i = octagon_a[j]; i<=octagon_z[j]; i++)
		{
			if (t1[i][j].inside()==1)
			{renderTexture_rotate(octagon_t[j], ren, t1[i][j].x_f, t1[i][j].y_f,delta_x,delta_y);}
			if ((clicked==i && turn == j)||(clicked==i+number_of_shapes[j]&&clicked_turn==j))
			{bigrenderTexture_pixel(octagon_t[j], ren, 900,150,delta_x,delta_y);}
		}

		for (int i=0; i<number_of_shapes[j];i++)
		{
			if (t1[i][j].fired==1)
			{renderTexture_rotate(fired_t,ren,t1[i][j].x_f,t1[i][j].y_f,delta_x,delta_y);
			}
		}


	}

	
			if (GetTickCount()-attack_time_counter<700)
	{
	SDL_draw_fire(ren,attack_coordinates,explosion_t,delta_x,delta_y);
	}

			int aaa=GetTickCount()-select_time_counter;
			if (aaa<400||(aaa<1200&&aaa>800)||(aaa<2000&&aaa>1600)||(aaa<2800&&aaa>2400))


			{renderTexture(select_t,ren,select_coordinates[0],select_coordinates[1],delta_x,delta_y);}
	

	for (int i=1;i<=x_boxes;i++)
	{
		for(int j=1;j<=y_boxes;j++)////////////////////////
		{
			if (spaces[i][j]==1)
			{
				//std::cout<<spaces[i][j]<< " ";
				int skk=0;
				int skkkk = GetTickCount()%1000;
					if (skkkk>750)
				{skk = 3;}
					else if (skkkk>500)
				{skk = 2;}
					else if (skkkk>250)
				{skk = 1;}

			renderTexture_pixel(bad_box_t[skk],ren,50*(i+1)+1,50*(j+1)+1,delta_x,delta_y);


			}
		}
			//std::cout<< "\n";
	}
	

	if (turn==0)
	{renderTexture_pixel(player_1_t,ren,580,70,delta_x,delta_y);}
	if (turn==1)
	{renderTexture_pixel(player_2_t,ren,580,70,delta_x,delta_y);}
	if (turn==2)
	{renderTexture_pixel(player_3_t,ren,580,70,delta_x,delta_y);}
	if (turn==3)
	{renderTexture_pixel(player_4_t,ren,580,70,delta_x,delta_y);}



	if (mode==0) 
	{renderTexture_pixel(move_t,ren,760,70,delta_x,delta_y);}
	if (mode==1)
	{renderTexture_pixel(attack_t,ren,760,70,delta_x,delta_y);}
	if (mode ==2)
	{renderTexture_pixel(setup_t,ren,760,70,delta_x,delta_y);}
	
	{renderTexture_pixel(sound_t,ren,1100,700,delta_x,delta_y);}
	
	renderstats(ren, numeral_t, minus_t, t1, clicked, turn,clicked_turn,moves_remaining, real_number_of_teams,delta_x,delta_y);


	for(int j=0;j<real_number_of_teams;j++)
	{
	renderHealth(green_t[j], red_t, ren, t1, real_number_of_teams,j,delta_x,delta_y);
	}
	

	//std::cout << "whoa... \n";

	SDL_RenderPresent(ren); //draws the board and all the pieces


	return 0;
	}



	Triangle t1[][number_of_teams] = { {Triangle(1,12), Triangle(15,1), Triangle(1,1),Triangle(15,12)}, 
									   {Triangle(1,13), Triangle(15,2), Triangle(1,2),Triangle(15,13)}, 
									   {Triangle(1,14), Triangle(15,3), Triangle(1,3),Triangle(15,14)}, 
									   {Triangle(1,15), Triangle(15,4), Triangle(1,4),Triangle(15,15)}, 
									   {Triangle(2,12), Triangle(14,1), Triangle(2,1),Triangle(14,12)}, 
									   {Triangle(2,13), Triangle(14,2), Triangle(2,2),Triangle(14,13)}, 
									   {Triangle(2,14), Triangle(14,3), Triangle(2,3),Triangle(14,14)},  
									   {Triangle(2,15), Triangle(14,4), Triangle(2,4),Triangle(14,15)},
									   {Triangle(3,13), Triangle(13,1), Triangle(3,1),Triangle(13,13)}, 
									   {Triangle(3,14), Triangle(13,2), Triangle(3,2),Triangle(13,14)},
									   {Triangle(3,15), Triangle(13,3), Triangle(3,3),Triangle(13,15)}, };




	

int clicked = -1; //tells which triangle has been selected
int clicked_turn = -1; // if a triangle of another team is selected
int m = 1;       //basically tells if the code is still running
int mode = 2;  //mode (0 is moving, 1 is shooting)
int death = 0; //1 means somethin died
int turn = 0; //0 is player 1, etc
int turn_counter;
int attack = 0; //whether or not attack is about to happen
int moves_remaining = 0; //obvious
int random_map=0;
int percentage=10;
int timesaver=0;
int music_index=0;
int real_number_of_teams = 2;
int time_start=0;
int nick_cannon=0;
int attack_coordinates[4]= {0,0,0,0};
int attack_time_counter=0;
int select_time_counter=0;
int select_coordinates[2] = {0,0};
int menu_mode = 1;
int random_placement = 0;
int user_upgrades[4][number_of_teams] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
int moves_upgrade_counter[number_of_teams] = {0,0,0,0};
int player_number = 0;
int turn_switch=1; //0 is my turn, 2 is about to switch, 1 is not my turn
int fire_happened=0;
int quit=0;

int main(int argc, char* argv[]) ///where the code starts
{
	// boost::asio::ip::address_v4 targetIP;
	// boost::system::error_code myError;
	//targetIP = boost::asio::ip::address::from_string("10.1.1.75", myError); 

	//LoadImage(NULL,
	//SetCursor(

	/*
	std::string laura = "You are now playing people_people_!";
	LPCSTR craig = laura.c_str();
	int msgbox = MessageBox(NULL,craig,NULL,MB_OKCANCEL);
	*/

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

		int spaces[x_boxes+1][y_boxes+1]; //this is a weird thing, because i want to call the boxes starting with 1, but this array starts with 0. so, the spaces with [0] still exist, but the t1[][].inside() function keeps anyone from getting there.
	
	for (int i = 0;i<=x_boxes;i++)
	{
		for (int j = 0; j<=y_boxes;j++)
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
	
	std::cout<<"# of players? (2-4) \n";
	std::cin>>real_number_of_teams;

	if(real_number_of_teams!=2&&real_number_of_teams!=3&&real_number_of_teams!=4)
	{std::cout<< "u a nig \n";
	real_number_of_teams=2;}

	int networking=0;
	std::cout<<"networking? 0 for no, 1 for yes";
	std::cin>>networking;
	  std::string network_name;
	  std::string port_name;

	if(networking==1)
	{
	  std::cout<<"network?";
	  std::cin >> network_name;

	  std::cout<<"port?";
	  std::cin >> port_name;

	  std::cout<<"player number? (1-4)";
	  std::cin >> player_number;
	  player_number -=1;
	  if(player_number==0)
	  {turn_switch=0;}
	 
	}


	std::cout<<"random map? 0 for no, 1 for yes \n";
	std::cin>>random_map;
	
	if (random_map==1)
	{
	srand (time(NULL));
	std::cout<<"percentage of black squares? 0-100 \n";
	std::cin>>percentage;
	}


	if (std::cin.fail())
	{std::cout << "u a nig \n";
	percentage=15;
	}

		std::cout<<"random placement? 0 for no, 1 for yes \n";
	std::cin>>random_placement;


	
	int x_placement;
	int y_placement;



	int upgrades=0;
	std::cout<<"upgrades? 0 for no, 1 for yes \n";
	std::cin>>upgrades;
	if (upgrades==1)
	{

	for (int j = 0; j<real_number_of_teams; j++)
	{
	std::cout<<"player " << j+1 << ": choose triangle upgrade: 0=health,1=move,2=damage,3=range";
	std::cin>>user_upgrades[0][j];
	std::cout<<"player " << j+1 << ": choose square upgrade: 0=health,1=move,2=damage,3=range";
	std::cin>>user_upgrades[1][j];
	std::cout<<"player " << j+1 << ": choose circle upgrade: 0=health, 1=move,2=damage,3=range";
	std::cin>>user_upgrades[2][j];
	std::cout<<"player " << j+1 << ": choose pentagon upgrade: 0=health,1=move,2=damage,3=range";
	std::cin>>user_upgrades[3][j];
	}

	}

	if (random_map==1)
	{
	for (int i=1;i<=x_boxes;i++)
	{
		for (int j=1;j<=y_boxes;j++)
		{
			if (spaces[i][j]==0||spaces[i][j]==1 || Status(i,j,t1,number_of_shapes,spaces,mode,turn,real_number_of_teams)==0)
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

		if (random_placement==1)
	{
		mode=0;
		for (int j=0;j<real_number_of_teams;j++)
		{
			for (int i=0;i<number_of_shapes[j];i++)
			{
				x_placement = rand() % 15+1;
				y_placement = rand() % 15+1;
				while (Status(x_placement,y_placement,t1,number_of_shapes,spaces,mode,turn,real_number_of_teams)!=0)
				{
				x_placement = rand() % 15+1;
				y_placement = rand() % 15+1;

				}
				t1[i][j].x_f=x_placement;
				t1[i][j].y_f=y_placement;
				t1[i][j].fixation();
			}
		}

	}
	


	
			for (int j=0;j<real_number_of_teams;j++)
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
					
					if(nick_cannon==1)
					{type=4;}	


						t1[i][j].set_values(type);
					if (upgrades==1)
					{
						moves_upgrade_counter[j] = t1[i][j].upgrade(user_upgrades[type][j],moves_upgrade_counter[j]);
					}
					


				}
			}
			

	if (random_map==666)	
	{total_moves=500;
	for (int j = 0; j<real_number_of_teams;j++)
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



		   GetDesktopResolution(horizontal,vertical);
		   int x_restest=(horizontal-SCREEN_WIDTH);
		   int y_restest=(vertical-SCREEN_HEIGHT);
		   if(x_restest<0||y_restest<0)
		   {
			   SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
			   std::cout<<"your screen resolution is too low to play";
		   SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		   }





	   SDL_Window *win = nullptr;
    win = SDL_CreateWindow("people_people_", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );//!!!!!!!!!!!!!!!!!!!!

	
	   SDL_Renderer *ren = nullptr;
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


	  SDL_Surface *board = nullptr;
    board = SDL_LoadBMP("bond-o.bmp");


	    SDL_Texture *board_t = nullptr;
    board_t = SDL_CreateTextureFromSurface(ren, board);
    SDL_FreeSurface(board);
	
	SDL_Surface *bad_box[4] = {nullptr,nullptr};
	  
    bad_box[0] = SDL_LoadBMP("bad_box[0].bmp");
    bad_box[1] = SDL_LoadBMP("bad_box[1].bmp");
    bad_box[2] = SDL_LoadBMP("bad_box[2].bmp");
    bad_box[3] = SDL_LoadBMP("bad_box[3].bmp");


	SDL_Texture *bad_box_t[4] = {nullptr,nullptr};
	for (int i=0;i<4;i++)
	{
    bad_box_t[i] = SDL_CreateTextureFromSurface(ren, bad_box[i]);
    SDL_FreeSurface(bad_box[i]);
	}




	SDL_Surface *triangle[number_of_teams] = {nullptr, nullptr};
    triangle[0] = SDL_LoadBMP("triangle[0].bmp");
	triangle[1] = SDL_LoadBMP("triangle[1].bmp");
    triangle[2] = SDL_LoadBMP("triangle[2].bmp");
	triangle[3] = SDL_LoadBMP("triangle[3].bmp");
	   
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
    square[2] = SDL_LoadBMP("square[2].bmp");
    square[3] = SDL_LoadBMP("square[3].bmp");


	   
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
    circle[2] = SDL_LoadBMP("circle[2].bmp");
    circle[3] = SDL_LoadBMP("circle[3].bmp");

	   
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
    octagon[2] = SDL_LoadBMP("octagon[2].bmp");
    octagon[3] = SDL_LoadBMP("octagon[3].bmp");

	   
	SDL_Texture *octagon_t[number_of_teams] = {nullptr,nullptr};
	for (int i=0;i<number_of_teams;i++)
	{
	SDL_SetColorKey(octagon[i], SDL_TRUE, 0x000000);
    octagon_t[i] = SDL_CreateTextureFromSurface(ren, octagon[i]);
    SDL_FreeSurface(octagon[i]);
	SDL_SetTextureBlendMode(octagon_t[i],SDL_BLENDMODE_BLEND);
	}


	SDL_Texture *nick_cannon_t=nullptr;
	if(nick_cannon==1)
	{SDL_Surface *nick_cannon_s=nullptr;
	nick_cannon_s=SDL_LoadBMP("nick cannon.bmp");
	SDL_SetColorKey(nick_cannon_s, SDL_TRUE, 0x000000);
	nick_cannon_t=SDL_CreateTextureFromSurface(ren,nick_cannon_s);
	SDL_SetTextureBlendMode(nick_cannon_t,SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(nick_cannon_s);//975 825 1125 875

	for (int i=0;i<number_of_teams;i++)
	{triangle_t[i]=nick_cannon_t;
	square_t[i]=nick_cannon_t;
	circle_t[i]=nick_cannon_t;
	octagon_t[i]=nick_cannon_t;
	}
	nick_cannon=2;
	}

	SDL_Surface *red = nullptr;
	red = SDL_LoadBMP("red_pixel.bmp");
	SDL_Texture *red_t = nullptr;
	red_t = SDL_CreateTextureFromSurface(ren,red);
	SDL_FreeSurface(red);

	SDL_Surface *green[number_of_teams] = {nullptr,nullptr,nullptr,nullptr};
	green[0] = SDL_LoadBMP("green[0].bmp");
	green[1] = SDL_LoadBMP("green[1].bmp");
	green[2] = SDL_LoadBMP("green[2].bmp");
	green[3] = SDL_LoadBMP("green[3].bmp");
	
	SDL_Texture *green_t[number_of_teams] = {nullptr,nullptr,nullptr,nullptr};

	for(int i=0; i<number_of_teams;i++)
	{
	green_t[i] = SDL_CreateTextureFromSurface(ren,green[i]);
	SDL_FreeSurface(green[i]);
	}

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

		SDL_Surface *player_3 = nullptr;
	player_3 = SDL_LoadBMP("player_3.bmp");
	SDL_SetColorKey(player_3, SDL_TRUE, 0x000000);
	SDL_Texture *player_3_t = nullptr;
	player_3_t = SDL_CreateTextureFromSurface(ren,player_3);
	SDL_SetTextureBlendMode(player_3_t,SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(player_3);

		SDL_Surface *player_4 = nullptr;
	player_4 = SDL_LoadBMP("player_4.bmp");
	SDL_SetColorKey(player_4, SDL_TRUE, 0x000000);
	SDL_Texture *player_4_t = nullptr;
	player_4_t = SDL_CreateTextureFromSurface(ren,player_4);
	SDL_SetTextureBlendMode(player_4_t,SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(player_4);

	
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
	
	SDL_Surface *sound = nullptr;
    sound = SDL_LoadBMP("sound_button.bmp");
	SDL_SetColorKey(sound, SDL_TRUE, 0x000000);
	SDL_Texture *sound_t = nullptr;
    sound_t = SDL_CreateTextureFromSurface(ren, sound);
    SDL_FreeSurface(sound);
	
SDL_SetTextureBlendMode(fired_t,SDL_BLENDMODE_BLEND);

		  SDL_Surface *explosion = nullptr;
    explosion = SDL_LoadBMP("explosion.bmp");
	SDL_SetColorKey(explosion, SDL_TRUE, 0x000000);
	SDL_Texture *explosion_t = nullptr;
    explosion_t = SDL_CreateTextureFromSurface(ren, explosion);
    SDL_FreeSurface(explosion);
	
SDL_SetTextureBlendMode(explosion_t,SDL_BLENDMODE_BLEND);


		  SDL_Surface *select = nullptr;
    select = SDL_LoadBMP("dudespot.bmp");
	SDL_SetColorKey(select, SDL_TRUE, 0x000000);
	SDL_Texture *select_t = nullptr;
    select_t = SDL_CreateTextureFromSurface(ren, select);
    SDL_FreeSurface(select);
	
SDL_SetTextureBlendMode(select_t,SDL_BLENDMODE_BLEND);


			  SDL_Surface *minus = nullptr;
		minus = SDL_LoadBMP("minus.bmp");
		SDL_SetColorKey(minus, SDL_TRUE, 0x000000);
		SDL_Texture *minus_t = nullptr;
		minus_t = SDL_CreateTextureFromSurface(ren, minus);
		SDL_FreeSurface(minus);
		SDL_SetTextureBlendMode(minus_t,SDL_BLENDMODE_BLEND);

			SDL_Surface *menu = nullptr;
	menu = SDL_LoadBMP("menu_screen.bmp");
	SDL_Texture *menu_t = nullptr;
	menu_t = SDL_CreateTextureFromSurface(ren,menu);
	SDL_FreeSurface(menu);

 //loading stuff in, making textures

  
	   SDL_RenderClear(ren);
    SDL_RenderCopy(ren, board_t, NULL, NULL);
    SDL_RenderPresent(ren); //drawing the board

	
	  Mix_PlayMusic( menu_music, -1 );
	
	while(quit==0) //this is the main loop... if m == 0, then the game is over
	
{	
	if (turn_switch==2)
	{turn_switch=1;}

	if (fire_happened==1)
	{fire_happened=0;}

	if(turn==player_number)
	{turn_switch=0;}

	int menu_count=0;
	while(menu_mode==1)
	{

	while(SDL_PollEvent(&sdl_event) > 0)
		{
		 switch(sdl_event.type)
			{

			 case(SDL_KEYDOWN):
				 switch(sdl_event.key.keysym.sym)
				 {
						case(SDLK_RETURN):
						menu_mode=0;
						break;
				}
			}
		}
			DrawMenu(ren,menu_t);
			menu_count +=1;
		
		if (menu_count==1000)
		{menu_mode=0;}
	}

		if(nick_cannon==1)
	{SDL_Surface *nick_cannon_s=nullptr;
	nick_cannon_s=SDL_LoadBMP("nick cannon.bmp");
	SDL_SetColorKey(nick_cannon_s, SDL_TRUE, 0x000000);
	nick_cannon_t=SDL_CreateTextureFromSurface(ren,nick_cannon_s);
	SDL_SetTextureBlendMode(nick_cannon_t,SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(nick_cannon_s);//975 825 1125 875

	for (int i=0;i<number_of_teams;i++)
	{triangle_t[i]=nick_cannon_t;
	square_t[i]=nick_cannon_t;
	circle_t[i]=nick_cannon_t;
	octagon_t[i]=nick_cannon_t;
	}
	nick_cannon=2;
	
		for (int j=0;j<real_number_of_teams;j++)
			{
				for (int i=0;i<number_of_shapes[j];i++)
				{
					int type=4;
						t1[i][j].set_values(type);
				}
			}
		}


	if (turn_counter>2 || random_placement==1  || (real_number_of_teams>2&&turn_counter>0))
	{
	total_moves=13 + moves_upgrade_counter[turn];



	}




	if(random_map==666)
	{total_moves=500;}

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
	

	DrawEverything(ren, board_t, bad_box_t, triangle_t, square_t, circle_t, octagon_t, t1, sound_t, green_t, red_t,player_1_t,player_2_t,player_3_t,player_4_t,numeral_t,fired_t,minus_t,turn,clicked_turn,setup_t,move_t,attack_t,explosion_t,select_t,mode,clicked,moves_remaining,spaces, real_number_of_teams,attack_coordinates,attack_time_counter,select_time_counter,select_coordinates,delta_x,delta_y); //draws everything
//	std::cout << "mode = " << mode << "attack = " << attack << "clicked = " << clicked << "turn = " << turn << "status = " << Status(xclick,yclick,t1,number_of_shapes, spaces,mode,turn) <<	"\n"; 
	
	//std::cout << Status(t1[clicked][turn].x_f,t1[clicked][turn].y_f,t1[][number_of_teams],number_of_triangles[]); 
	 
	
while(SDL_PollEvent(&sdl_event) > 0)
{
	 switch(sdl_event.type)
    {

	 case(SDL_QUIT):
		 quit=1;
		 break;

		 case(SDL_KEYDOWN):
		switch(sdl_event.key.keysym.sym)
        {
        case(SDLK_UP):

			if (clicked == -1|| clicked >= number_of_shapes[turn])//gotta make sure we clicked one of our own guys
			{break;}
			if ((mode == 2 ||mode == 0) && Status (t1[clicked][turn].x_f,t1[clicked][turn].y_f-1,t1,number_of_shapes,spaces,mode,turn,real_number_of_teams)==0)
			{
			t1[clicked][turn].move_up(mode);
			//std::cout << "check = " <<t1[clicked][turn].check() << "inside = " << t1[clicked][turn].inside() << "distance = " << t1[clicked][turn].d;
			}
            break;
		
        case(SDLK_LEFT):
			if (clicked == -1|| clicked >= number_of_shapes[turn])
			{break;}
			if ((mode == 2 ||mode == 0) && Status (t1[clicked][turn].x_f-1,t1[clicked][turn].y_f,t1,number_of_shapes,spaces,mode,turn,real_number_of_teams)==0)
			{
			t1[clicked][turn].move_left(mode);
			}
            break;
	 
        case(SDLK_DOWN):
			if (clicked == -1|| clicked >= number_of_shapes[turn])
			{break;}
			if ((mode == 2 ||mode == 0) && Status (t1[clicked][turn].x_f,t1[clicked][turn].y_f+1,t1,number_of_shapes,spaces,mode,turn,real_number_of_teams)==0)
			{
			t1[clicked][turn].move_down(mode);
			}
			break;

        case(SDLK_RIGHT):
			if (clicked == -1|| clicked >= number_of_shapes[turn])
			{break;}
			if ((mode == 2 ||mode == 0) && Status (t1[clicked][turn].x_f+1,t1[clicked][turn].y_f,t1,number_of_shapes,spaces,mode,turn,real_number_of_teams)==0)
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
			
						if(turn_switch==0||networking==0)
						{

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
				{turn +=1;
				turn_switch=2;}
				if (turn == real_number_of_teams) 
				{mode = 0;
				turn = 0;
				turn_switch=2;
				Mix_PlayMusic( theme_music_1, -1 );}
			}
			break;
			}
			



			else if (mode ==1)
			{
				
				for (int i = 0; i<number_of_shapes[turn]; i++)
				{t1[i][turn].unfire();}

				turn += 1;
				turn_switch=2;

				if (turn == real_number_of_teams) 
				{turn = 0;
				turn_switch=2;
				turn_counter += 1;}	
				
                      Mix_PlayMusic( theme_music_1, -1 );
			clicked = -1;
			mode = 0;
				attack = 0;
			break;
			}
						}
						break;
				
	 case(SDLK_BACKSPACE):

		 break;
	  }

	  case(SDL_WINDOWEVENT):
		  switch (sdl_event.window.event)  {
              
              case SDL_WINDOWEVENT_SIZE_CHANGED:  
                delta_x = (sdl_event.window.data1-SCREEN_WIDTH)/2.;
                delta_y = (sdl_event.window.data2-SCREEN_HEIGHT)/2.;
                break;
              
		   }

                break;
	  

		case(SDL_MOUSEBUTTONDOWN):


			 if(sdl_event.button.button == SDL_BUTTON_RIGHT)
			 {
				 if (mode==2)
				 {break;}

				 clickindex(sdl_event,delta_x,delta_y);

				 if (mode == 0)
				 {
					 for (int i=0;i<number_of_shapes[turn];i++)
					 {
					 if(xclick==t1[i][turn].x_f && yclick==t1[i][turn].y_f)
						{
							int collision=0;
							for (int k=0;k<number_of_shapes[turn];k++)
							{
								if (t1[i][turn].reset_testx()==t1[k][turn].x_f&&t1[i][turn].reset_testy()==t1[k][turn].y_f)
								{
									if (k!=i)
									{
									
									double select_time_d = GetTickCount();
									select_time_counter = select_time_d;
									select_coordinates[0]=t1[i][turn].reset_testx();
									select_coordinates[1]=t1[i][turn].reset_testy();
									
									//std::string susie = "there's a dude in your old spot";
									//LPCSTR mark = susie.c_str();
									//int msgbox = MessageBox(NULL,mark,NULL,MB_OKCANCEL);
									collision=1;
									}
								}

							}
							
								if(collision==0)
								{
							t1[i][turn].reset(); 
								}
								collision=0;
						}
					 }
					 break;
				 }
				else if (attack ==1 && clicked !=-1)
			{
				std::cout << "???? \n";
				int attacked = -1;

				//std::cout<<not_turn<<"   " <<turn;
				for (int j=0;j<real_number_of_teams;j++) 
					{
						if(j != turn)
						{
				for (int i=0;i<number_of_shapes[j];i++)
				{
				if(xclick==t1[i][j].x_f && yclick == t1[i][j].y_f) 
				{attacked = i;
				int max = t1[clicked][turn].maxrange;
				 attack_coordinates[2] = t1[attacked][j].x_f;
				 attack_coordinates[0] = t1[clicked][turn].x_f;
				 attack_coordinates[3] = t1[attacked][j].y_f;
				 attack_coordinates[1] = t1[clicked][turn].y_f;
				//std::cout << "attacked = " << attacked << "max = "<< max << "shit = " << one <<" " << two << " "<< three << " "<< four;
				if ((std::abs(attack_coordinates[2]-attack_coordinates[0])+std::abs(attack_coordinates[3]-attack_coordinates[1]))<= max && linecheck(attack_coordinates[0],attack_coordinates[1],attack_coordinates[2],attack_coordinates[3],spaces)==1)
				{	
					std::cout << "!!!!! \n";
					if (t1[clicked][turn].fired == 0)
					{
						t1[clicked][turn].fire();		
						
						fire_happened=1;

						//!!!

						int g = t1[clicked][turn].power;
					/*	if ((std::abs(attack_coordinates[2]-attack_coordinates[0])+std::abs(attack_coordinates[3]-attack_coordinates[1])) == 1)
						{g=g+t1[clicked][turn].power_upgrade_counter;}

						if ((std::abs(attack_coordinates[2]-attack_coordinates[0])+std::abs(attack_coordinates[3]-attack_coordinates[1])) == max)
						{g=g+t1[clicked][turn].maxrange_upgrade_counter;}
						*/
						t1[attacked][j].take_damage(g);


					//int aaa = 2*t1[attacked][j].take_damage(g); 
					//aaa=0;
					//if (aaa >0)
					//{
					//int zzz = rand() %number_of_shapes[j];
					//int while_counter=0;
					//while (t1[zzz][j].inside()==0&&while_counter<10000)
					//{
					//zzz = rand() % number_of_shapes[j];
					//while_counter+=1; //////////////////////should be better than this. game over should be handled better
					//}
					//while_counter=0;


					//t1[zzz][j].addhealth(aaa);


					//}

					}
				}
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
		clickindex(sdl_event,delta_x,delta_y); 

		for (int j=0;j<real_number_of_teams;j++)
			{
			for (int i=0;i<number_of_shapes[j];i++)
			{
				if (xclick==t1[i][j].x_f && yclick==t1[i][j].y_f)
				{
					if(j==turn)
					{
				clicked=i;
				if (mode ==1)
					{attack =1;}
					}
					else
					{clicked=i+number_of_shapes[j];
					clicked_turn=j;
					}
					}
			}
		}
		sdl_event.button.x-=delta_x;
		sdl_event.button.y-=delta_y;
		if(sdl_event.button.x<1125&&sdl_event.button.x>975&&sdl_event.button.y>825&&sdl_event.button.y<875)
		{
		nick_cannon += 2;

		if (nick_cannon>20)
		{
		nick_cannon=1;
		}
		}
		if(sdl_event.button.x<1075&&sdl_event.button.x>975&&sdl_event.button.y>70&&sdl_event.button.y<110)
		{
			menu_mode=1;

		}
		                           
		if(sdl_event.button.x<1130&&sdl_event.button.x>1100&&sdl_event.button.y>700&&sdl_event.button.y<730)
		{
		if( Mix_PlayingMusic() == 0 )
                            {
                                Mix_PlayMusic( theme_music_1, -1 );
                            }
                            else
                            {
                                if( Mix_PausedMusic() == 1 )
                                {
                                    Mix_ResumeMusic();
                                }
                                else
                                {
                                    Mix_PauseMusic();
                                }
                            }
                            break;
		}
		

		
		break;
		}
			
		}

		

	  
	 
	 
	 }


	 ////network stuff
	 
		 if(networking==1)
		 {

		 using boost::asio::ip::tcp;

enum { max_length = 4096 };

  try
  {




    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), network_name, port_name);
    tcp::resolver::iterator iterator = resolver.resolve(query);


    tcp::socket s(io_service);
    boost::asio::connect(s, iterator);

    using namespace std; // For strlen.
    char request[max_length];
	int header_length=16;
    //std::cin.getline(request, max_length);

	const int number_of_stats=8;
	int data_stuff[number_of_stats][20][number_of_teams]; ///number of shapes is array size..

	for (int j=0;j<real_number_of_teams;j++)
	{
		for (int i=0;i<number_of_shapes[j];i++)
		{
				data_stuff[0][i][j]=i;
				data_stuff[1][i][j]=t1[i][j].x_f;
				data_stuff[2][i][j]=t1[i][j].y_f;
				data_stuff[3][i][j]=t1[i][j].health;
				data_stuff[4][i][j]=t1[i][j].power;
				data_stuff[5][i][j]=t1[i][j].d_max;
				data_stuff[6][i][j]=t1[i][j].maxrange;
				data_stuff[7][i][j]=t1[i][j].fired; /////should probs send "fired" info too
		}
	}

	int counter=header_length;

	//header
	request[0]='0'+turn_switch;
	request[1]='0'+turn;
	request[2]='0'+player_number;
	request[3]='0'+mode;

	get_digits(moves_remaining,dig);
	request[4]='0'+dig[1];
	request[5]='0'+dig[0];
	clear_digits(dig);


	request[6]='0'+real_number_of_teams;
	request[7]='0'+fire_happened;

	for (int i=0;i<4;i++)
	{
	request[8+i]='0'+attack_coordinates[i];
	}
	for (int i=0;i<4;i++)
	{
	request[12+i]='0';
	}
	//header


	for (int j=0;j<real_number_of_teams;j++)
	{
		for (int i = 0;i<number_of_shapes[j];i++)
		{
			for (int h = 0; h<number_of_stats;h++)
			{

				get_digits(data_stuff[h][i][j],dig);

				for (int k=3;k>=0;k--)
				{
				
					request[counter]='0' + dig[k];
					//std::cout<<request[counter]<< "     " <<dig[k]<<"\n";
					counter+=1;
				}
			
				clear_digits(dig);
			}
		}
	}

    size_t request_length = strlen(request);

    boost::asio::write(s, boost::asio::buffer(request, request_length));

	          //all variables should equal 0 or something once they are dead, and draw/click_index should only happen if xf!=0


    char reply[max_length];
    size_t reply_length = boost::asio::read(s,
        boost::asio::buffer(reply, request_length)); ////////crucial
   // std::cout << "Reply is: ";
  //  std::cout.write(reply, reply_length);
  //  std::cout << "\n";
	
	int anti_counter=header_length; 
	for (int j=0;j<real_number_of_teams;j++)
	{
		for (int i=0;i<number_of_shapes[j];i++)
		{
			for (int k=0;k<number_of_stats;k++)
			{

				int request_i[4]={reply[anti_counter]-48,reply[anti_counter+1]-48,reply[anti_counter+2]-48,reply[anti_counter+3]-48};
				//std::cout<<"!!!"<<request_i[0]<<" "<<request[anti_counter]<<"!!!";
				data_stuff[k][i][j]=request_i[0]*1000+request_i[1]*100+request_i[2]*10+request_i[3];
				anti_counter +=4;
			//	std::cout<< j<<i<<k<<"\n";
				//std::cout<<"   "<<data_stuff[k][i][j]<<"   ";	
			}
		}
	}
	if(turn_switch==1) ////////////////////////////////////////////so now all i need to do is constantly refresh if it's not your turn and constantly send if it is.. also should send move count, turn, and attack.
	{
		for (int j=0;j<real_number_of_teams;j++)
		{
			for (int i=0;i<number_of_shapes[j];i++)
			{
				
				t1[i][j].x_f	 =  data_stuff[1][i][j];
				t1[i][j].y_f	 =  data_stuff[2][i][j];
				t1[i][j].health  =  data_stuff[3][i][j];
				t1[i][j].power	 =  data_stuff[4][i][j];
				t1[i][j].d_max	 =  data_stuff[5][i][j];
				t1[i][j].maxrange=  data_stuff[6][i][j];
				t1[i][j].fired=  data_stuff[7][i][j];
				t1[i][j].fixation();
				t1[i][j].take_damage(0);

	turn = reply[1]-48;
	mode=reply[3]-48;

	moves_remaining=10*(reply[4]-48)+(reply[5]-48);
	real_number_of_teams=reply[6]-48;

	fire_happened= reply[7]-48; //seems to work for player 2 but not for player 1.
	


	for (int i=0;i<4;i++)
	{
	attack_coordinates[i]=reply[8+i]-48;
	}
	


			}
		}
	}

  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  }


  	if(fire_happened==1)
	{
						double attack_time_d = GetTickCount();
						attack_time_counter = attack_time_d;
						SDL_draw_fire(ren,attack_coordinates,explosion_t,delta_x,delta_y);
	}

}	

	 
	
	 
	    SDL_DestroyTexture(board_t);
		for (int i=0;i<4;i++)
		{

	    SDL_DestroyTexture(bad_box_t[i]);
		SDL_DestroyTexture(triangle_t[i]);
		SDL_DestroyTexture(square_t[i]);
		SDL_DestroyTexture(circle_t[i]);
		SDL_DestroyTexture(octagon_t[i]);
		SDL_DestroyTexture(green_t[i]);
		
		}
		SDL_DestroyTexture(nick_cannon_t);
		SDL_DestroyTexture(red_t);
		SDL_DestroyTexture(player_1_t);
		SDL_DestroyTexture(player_2_t);
		SDL_DestroyTexture(player_3_t);
		SDL_DestroyTexture(player_4_t);
		SDL_DestroyTexture(setup_t);
		SDL_DestroyTexture(move_t);
		SDL_DestroyTexture(attack_t);
		SDL_DestroyTexture(fired_t);
		SDL_DestroyTexture(sound_t);
		SDL_DestroyTexture(minus_t);
		SDL_DestroyTexture(explosion_t);
		SDL_DestroyTexture(menu_t);
		SDL_DestroyTexture(select_t);


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



