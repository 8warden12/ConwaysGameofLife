#include "curses.h"
#undef getch
#undef bool
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>

#define WORLD_X 100
#define WORLD_Y 100

#define VIEW_X 60
#define VIEW_Y 20

int validate_x(int x)
{
	if(x < 0)
	return WORLD_X + x%WORLD_X;
	
	if(x >= WORLD_X)
	return x%WORLD_X;
	
	return x;
}

int validate_y(int y)
{
	if(y < 0)
	return WORLD_Y + y%WORLD_Y;
	
	if(y >= WORLD_Y)
	return y%WORLD_Y;
	
	return y;
}
int view_y_start = 0;
int view_x_start = 0;

struct loc{
	bool state;
	bool next_state;
};

struct loc world[WORLD_Y][WORLD_X];
bool view[VIEW_Y][VIEW_X];

void get_view()
{
	view_y_start = validate_y(view_y_start);
	view_x_start = validate_x(view_x_start);
	for(int i = view_y_start; i<view_y_start+VIEW_Y; ++i)
	{
		for(int x = view_x_start; x<view_x_start+VIEW_X; ++x)
		{
			view[i-view_y_start][x-view_x_start] = world[validate_y(i)][validate_x(x)].state;
		}
	}
	
	for(int i = 0; i<VIEW_Y; ++i)
	{
		for(int x = 0; x<VIEW_X; ++x)
		{
			printw("%c",view[i][x] ? '#' : ' ');
		}
		printw("\n");
	}
}

char get_live(int X, int Y)
{
	int live = 0;
	for(int i = Y-1; i<=Y+1; ++i)
	{
		for(int x = X-1; x<=X+1; ++x)
		{
			if(i != Y && x != X)
			{
				if(world[validate_y(i)][validate_x(x)].state)
				++live;
			}
		}
	}
	
	return live;
}

void logic_update()
{
	for(int i = 0; i<WORLD_Y; ++i)
	{
		for(int x = 0; x<WORLD_X; ++x)
		{
			world[i][x].next_state = world[i][x].state;
		}
	}
	
	char live = 0;
	for(int i = 0; i<WORLD_Y; ++i)
	{
		for(int x = 0; x<WORLD_X; ++x)
		{
			live = get_live(x,i);
			if(world[i][x].state)
			{
				if(live < 2 || live > 3)
				{
					world[i][x].next_state = 0;
				}
			}else{
				if(live == 5)
				world[i][x].next_state = 1;
			}
			
		}
	}
	
	for(int i = 0; i<WORLD_Y; ++i)
	{
		for(int x = 0; x<WORLD_X; ++x)
		{
			world[i][x].state = world[i][x].next_state;
		}
	}
}

void init()
{
	for(int i = 0; i<WORLD_Y; ++i)
	{
		for(int x = 0; x<WORLD_X; ++x)
		{
			world[i][x].state = rand()%2;
		}
	}
}

void key()
{
	switch(getch())
	{
		case 'w':
			--view_y_start;
		break;
		
		case 'a':
			--view_x_start;
		break;
		
		case 's':
			++view_y_start;
		break;
		
		case 'd':
			++view_x_start;
		break;
		
		case '.':
			logic_update();
		break;
	}
}





int main()
{
	srand(time(0));
	init();
	printf("%d\n",get_live(7,1));
	initscr();
	for(;;)
	{
		
		get_view();
		refresh();
		key();
		clear();
		
	}
}
