#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define DELAY 50000
#define UP 1
#define DOWN 2
#define RIGHT 3
#define LEFT 4
#define LOW 120
#define NORMAL 80
#define FAST 50
#define INSANE 5

/* Game parameters */

#define MAX_BONUS 100

/* Structures */

typedef struct bonus{
	int pos_x;
	int pos_y;
	int activ;
}Bonus;

/* Variables globales */

int max_bonus;
int spawning_rate;
int score = 0;
int nb_bonus = 0;
int max_x, max_y;
Bonus bonus_list[MAX_BONUS];
int dir = RIGHT;
int x = 0;
int y = 2;
char snake_shape[] = "o";
char title[] = "Simple Snake v1.0";

/* Fonctions */

void set_dir(int c)
{
	switch(c) {
		case 'q':
		case KEY_LEFT:
			dir=LEFT;
			break;
		case 'd':
		case KEY_RIGHT:
			dir=RIGHT;
			break;
		case 'z':
		case KEY_UP:
			dir=UP;
			break;
		case 's':
		case KEY_DOWN:
			dir=DOWN;
			break;
	}
}

void move_snake(int dir)
{
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
	int i = 0;
	for(i=0;i<=max_x;i++)
	{
		mvprintw(1, i, "#");
	}
	
	int middle = (max_x/2)-(strlen(title)/2);
	mvprintw(0, middle, title);
}

void bonus_create(void)
{
	int i;
	int rand_x;
	int rand_y;
	int add_bonus = rand()%spawning_rate;
	char str[12];

	if(add_bonus == 1 && nb_bonus < max_bonus)
	{
		rand_x = rand()%max_x;
		rand_y = rand()%max_y;
		
		if(rand_y <= 1){rand_y = 2;}
		
		for(i=0; i<max_bonus; i++)
		{
			if(bonus_list[i].activ == 0)
			{
				bonus_list[i].pos_x = rand_x;
				bonus_list[i].pos_y = rand_y;
				bonus_list[i].activ = 1;
				break;

			}
		}

		nb_bonus++;
	}	 	
}

void init_bonus(void)
{
	int i;
	for(i=0; i<max_bonus; i++)
	{
		bonus_list[i].pos_x = 0;
		bonus_list[i].pos_y = 0;
		bonus_list[i].activ = 0;	
	}	
}


void bonus_disp(void)
{

	int i;
	char str[12];
	for(i=0; i<max_bonus; i++)
	{
		if(bonus_list[i].activ)
		{
			mvprintw(bonus_list[i].pos_y, bonus_list[i].pos_x, "+");
		}
	}

}

void score_disp(void)
{
	char score_txt[16] = "Score : ";
	char score_nb[8];
	sprintf(score_nb, "%d", score);
	strcat(score_txt, score_nb);
	mvprintw(0, 0, score_txt);
}


void bonus_check(void)
{
	int i;
	for(i=0; i<max_bonus; i++)
	{
		if(x == bonus_list[i].pos_x && y == bonus_list[i].pos_y)
		{
			score++;
			nb_bonus--;
			bonus_list[i].activ = 0;
			break;
		}
	}	
	
}


void menu_disp(void)
{
	getmaxyx(stdscr, max_y, max_x);
	WINDOW * menu = newwin(8,20,(max_y/2)-4,(max_x/2)-10);
	box(menu,0,0);
	refresh();
	wrefresh(menu);
	keypad(menu, true);

	char* choices[4];
	choices[0] = "LOW";
	choices[1] = "NORMAL";
	choices[2] = "FAST";
	choices[3] = "INSANE";
	int choice;
	int highlight = 0;
	int selected = 0;
	int i; 

	while(1)
	{
		mvwprintw(menu, 1, 1, "Spawning rate");
		mvwprintw(menu, 2, 1, "------------------");
		for(i=0; i<4; i++)
		{
			if(i == highlight)
			{
				wattron(menu, A_REVERSE);
			}	
			mvwprintw(menu, i+3, 1, choices[i]);
			wattroff(menu, A_REVERSE);
		}
		choice = wgetch(menu);
		switch(choice)
		{

			case KEY_UP:
				highlight--;
				if(highlight == -1){highlight = 3;}
				break;
			case KEY_DOWN:
				highlight++;
				if(highlight == 4){highlight = 0;}
				break;
			default:
				break;
		}

		if(choice == 10){ //Enter
			switch(highlight)
			{
				case 0:
					spawning_rate = LOW;
					break;
				case 1:
					spawning_rate = NORMAL;
					break;
				case 2:
					spawning_rate = FAST;
					break;
				case 3:
					spawning_rate = INSANE;
					break;
			}
			break;
		}
	
	}
	
}

int quit_disp(void)
{
	getmaxyx(stdscr, max_y, max_x);
	WINDOW * quit = newwin(6,20,(max_y/2)-3,(max_x/2)-10);
	box(quit,0,0);
	refresh();
	wrefresh(quit);
	keypad(quit, TRUE);

	char* choices[2];
	choices[0] = "RESUME";
	choices[1] = "QUITE";
	int choice;
	int highlight = 0;
	int selected = 0;
	int i; 

	while(1)
	{
		mvwprintw(quit, 1, 1, "PAUSE");
		mvwprintw(quit, 2, 1, "------------------");
		for(i=0; i<2; i++)
		{
			if(i == highlight)
			{
				wattron(quit, A_REVERSE);
			}	
			mvwprintw(quit, i+3, 1, choices[i]);
			wattroff(quit, A_REVERSE);
		}
		choice = wgetch(quit);
		switch(choice)
		{

			case KEY_UP:
				highlight--;
				if(highlight == -1){highlight = 1;}
				break;
			case KEY_DOWN:
				highlight++;
				if(highlight == 2){highlight = 0;}
				break;
			default:
				break;
		}

		if(choice == 10){ //Enter
			switch(highlight)
			{
				case 0:
					return 0;	
				case 1:
					return 1;
			}

		}
	
	}

}

void menu2_disp(void)
{
	getmaxyx(stdscr, max_y, max_x);
	WINDOW * menu2 = newwin(5,30,(max_y/2)-3,(max_x/2)-15);
	box(menu2,0,0);
	refresh();
	wrefresh(menu2);
	char amount[8];

	mvwprintw(menu2, 1, 1, "Bonus (max 100)");
	mvwprintw(menu2, 2, 1, "----------------------------");
	mvwprintw(menu2, 3, 1, "Amount : ");
	mvwgetstr(menu2, 3, 10, amount);
	//wgetch(menu2);
	max_bonus = atoi(amount);
}

/* Programme principale */


int main(int argc, char *argv[]) 
{
	int ch;
	int speed;
	int quit = 0;
	srand(time(NULL));
	initscr();
	//noecho();
	cbreak();
	curs_set(FALSE);
	keypad(stdscr, TRUE);

	menu_disp();
	erase();
	menu2_disp();	

	init_bonus();

	while(!quit) {
		speed = 50-score*2;
		if(speed < 0){speed = 0;}
		timeout(speed);
		getmaxyx(stdscr, max_y, max_x);
		ch = getch();
		if(ch == 27){quit = quit_disp();}
		if(quit){break;}
		set_dir(ch);
		move_snake(dir);
		
		clear();
		title_draw();
		score_disp();

		bonus_create();

		bonus_disp();
		mvprintw(y, x, snake_shape);

		bonus_check();
	
		refresh();

		usleep(DELAY);

	}

	endwin();
}
