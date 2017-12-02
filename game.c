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
#define MAX_BONUS 3

/* Structures */

typedef struct bonus{
	int pos_x;
	int pos_y;
	int activ;
}Bonus;

/* Variables globales */

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
	int add_bonus = rand()%20;
	char str[12];

	if(add_bonus == 1 && nb_bonus < MAX_BONUS)
	{
		rand_x = rand()%max_x;
		rand_y = rand()%max_y;
		
		if(rand_y <= 1){rand_y = 2;}
		
		for(i=0; i<MAX_BONUS; i++)
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
	for(i=0; i<MAX_BONUS; i++)
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
	for(i=0; i<MAX_BONUS; i++)
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
	for(i=0; i<MAX_BONUS; i++)
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

/* Programme principale */


int main(int argc, char *argv[]) 
{
	int ch;
	int speed;
	srand(time(NULL));
	initscr();
	noecho();
	cbreak();
	curs_set(FALSE);
	init_bonus();
	
	while(1) {
		speed = 50-score*2;
		if(speed < 0){speed = 0;}
		timeout(speed);
		getmaxyx(stdscr, max_y, max_x);
		ch = getch();
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


