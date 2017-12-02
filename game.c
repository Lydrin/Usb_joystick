#include <ncurses.h>
#include <unistd.h>
#include <time.h>

#define DELAY 30000
#define UP 1
#define DOWN 2
#define RIGHT 3
#define LEFT 4

int dir = RIGHT;
int x = 0;
int y = 0;

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
	switch(dir) {
		case UP:
			y--;
			break;
		case DOWN:
			y++;
			break;
		case RIGHT:
			x++;
			break;
		case LEFT:
			x--;
			break;
	}
}

int main(int argc, char *argv[]) {

	int max_y = 0, max_x = 0;
	int next_x = 0;
	int next_y = 0;
	int ch;

	initscr();
	noecho();
	cbreak();
	curs_set(FALSE);
	timeout(30);

	while(1) {
		ch = getch();
		set_dir(ch);
		move_snake(dir);

		//getmaxyx(stdscr, max_y, max_x);
		
		clear();
		mvprintw(y, x, "o");
		refresh();

		usleep(DELAY);

	}

	endwin();
}


