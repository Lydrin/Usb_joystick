#include <ncurses.h>
#include <unistd.h>
#include <time.h>

#define DELAY 30000
#define UP 1
#define DOWN 2
#define RIGHT 3
#define LEFT 4

int main(int argc, char *argv[]) {

	int x = 10, y = 10;
	int max_y = 0, max_x = 0;
	int next_x = 0;
	int next_y = 0;
	int dir;
	int ch;

	initscr();
	noecho();
	cbreak();
	curs_set(FALSE);
	timeout(30);

	while(1) {
		ch = getch();
		if(ch == 'q'){dir=LEFT;}
		if(ch == 'd'){dir=RIGHT;}
		if(ch == 'z'){dir=UP;}
		if(ch == 's'){dir=DOWN;}

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

		//getmaxyx(stdscr, max_y, max_x);
		
		clear();
		mvprintw(y, x, "o");
		refresh();

		usleep(DELAY);

	}

	endwin();
}


