#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define DELAY 50000
#define UP 1
#define DOWN 2
#define RIGHT 3
#define LEFT 4

int dir = RIGHT;
int x = 0;
int y = 2;
char snake_shape[] = "o";
char title[] = "Simple Snake v1.0";

void set_dir(char c)
{
	switch(c) {
		case 'q':
			dir=LEFT;
			break;
		case 'd':
			dir=RIGHT;
			break;
		case 'z':
			dir=UP;
			break;
		case 's':
			dir=DOWN;
			break;
	}
}

void move_snake(int dir)
{
	int max_y = 0, max_x = 0;
	getmaxyx(stdscr, max_y, max_x);
	switch(dir) {
		case UP:
			y--;
			if(y == 1){y=max_y;}
			break;
		case DOWN:
			y++;
			if(y == max_y+1){y=2;}
			break;
		case RIGHT:
			x++;
			if(x == max_x+1){x=0;}
			break;
		case LEFT:
			x--;
			if(x == -1){x=max_x;}
			break;
	}
}

void title_draw(void)
{
	int max_y = 0, max_x = 0, i = 0;
	getmaxyx(stdscr, max_y, max_x);
	for(i=0;i<=max_x;i++)
	{
		mvprintw(1, i, "#");
	}
	
	int middle = (max_x/2)-(strlen(title)/2);
	mvprintw(0, middle, title);
}

int main(int argc, char *argv[]) 
{
	int ch;
	
	initscr();
	noecho();
	cbreak();
	curs_set(FALSE);
	timeout(30);
	
	mvprintw(0, 0, "Coucou");
	
	while(1) {
		ch = getch();
		set_dir(ch);
		move_snake(dir);
		
		clear();
		title_draw();
		mvprintw(y, x, snake_shape);
	
		refresh();

		usleep(DELAY);

	}

	endwin();
}


