#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "usb_host.h"

#define DELAY 50000
#define UP 1
#define DOWN 2
#define RIGHT 3
#define LEFT 4
#define LOW 120
#define NORMAL 80
#define FAST 50
#define INSANE 5
#define INACTIV 0

/* USB KEY */

#define USBKEY_LEFT 48
#define USBKEY_RIGHT 34
#define USBKEY_UP 36
#define USBKEY_DOWN 40
#define USBKEY_FCT 33

/* Game parameters */

#define MAX_BONUS 100

/* Structures */

typedef struct bonus{
	int pos_x;
	int pos_y;
	int activ;
}Bonus;

typedef struct pos{
	int x;
	int y;
	int dir;
}Pos;

typedef struct Cell
{
    Pos pos;
    struct Cell* next;
}Cell;

typedef Cell* Snake;

typedef Cell* ptr_cell;


/* Variables globales */

Snake snake;
Cell snake_end;
int max_bonus;
int spawning_rate;
int score = 0;
int nb_bonus = 0;
int max_x, max_y;
Bonus bonus_list[MAX_BONUS];
int dir = RIGHT;
char snake_shape[] = "o";
char title[] = "Simple Snake v2.0";

/* Fonctions */

void set_dir(int c)
{
	switch(c) {
		case USBKEY_LEFT:
			dir=LEFT;
			break;
		case USBKEY_RIGHT:
			dir=RIGHT;
			break;
		case USBKEY_UP:
			dir=UP;
			break;
		case USBKEY_DOWN:
			dir=DOWN;
			break;
	}
}

void title_draw(void)
{
	int i = 0;
	for(i=0;i<=max_x;i++)
	{
		mvprintw(1, i, "#");
		mvprintw(max_y-1, i, "#");
	}

	for(i=0;i<=max_y;i++)
	{
		mvprintw(i+2, 0, "#");
		mvprintw(i+2, max_x-1, "#");
	}
	
	int middle = (max_x/2)-(strlen(title)/2);
	attron(A_BOLD);
	attron(COLOR_PAIR(2));
	mvprintw(0, middle, title);
	attroff(A_BOLD);
	attron(COLOR_PAIR(2));
}

void bonus_create(void)
{
	int i;
	int rand_x;
	int rand_y;
	int add_bonus = rand()%spawning_rate;

	if(add_bonus == 1 && nb_bonus < max_bonus)
	{
		rand_x = rand()%max_x-1;
		rand_y = rand()%max_y-1;
		
		if(rand_y <= 1){rand_y = 2;}
		if(rand_x == 0){rand_x = 1;}
		
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

void head_add(Snake* snake, Pos pos) //début
{
    ptr_cell temp = *snake;
    *snake = malloc(sizeof(Cell));
    (*(*snake)).pos = pos;
    (*(*snake)).next = temp;
}

void snake_grow(Snake* snake)
{
	int first_pos_x = (*snake)->pos.x;
	int first_pos_y = (*snake)->pos.y;
	int first_dir = (*snake)->pos.dir;
	Pos new_pos;

	switch(first_dir)
	{
		case UP:
			new_pos.x = first_pos_x+1;
			new_pos.y = first_pos_y;
			new_pos.dir = UP;
			head_add(snake, new_pos);			
			break;
		case DOWN:
			new_pos.x = first_pos_x-1;
			new_pos.y = first_pos_y;
			new_pos.dir = DOWN;
			head_add(snake, new_pos);
			break;
		case RIGHT:
			new_pos.x = first_pos_x;
			new_pos.y = first_pos_y-1;
			new_pos.dir = RIGHT;
			head_add(snake, new_pos);
			break;
		case LEFT:
			new_pos.x = first_pos_x;
			new_pos.y = first_pos_y+1;
			new_pos.dir = LEFT;
			head_add(snake, new_pos);
			break;
	}	
}

void bonus_check(Snake* snake)
{
	int i;
	for(i=0; i<max_bonus; i++)
	{
		if(snake_end.pos.x == bonus_list[i].pos_y && snake_end.pos.y == bonus_list[i].pos_x)
		{
			score++;
			nb_bonus--;
			bonus_list[i].activ = 0;
			snake_grow(snake);
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

	char* choices[4];
	choices[0] = "LOW";
	choices[1] = "NORMAL";
	choices[2] = "FAST";
	choices[3] = "INSANE";
	int choice;
	int highlight = 0;
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
		
		choice = key_pressed();
		
		switch(choice)
		{
			case USBKEY_UP:
				highlight--;
				if(highlight == -1){highlight = 3;}
				break;
			case USBKEY_DOWN:
				highlight++;
				if(highlight == 4){highlight = 0;}
				break;
			default:
				break;
		}

		if(choice == USBKEY_FCT){ //Enter
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
		
		wrefresh(menu);
	
	}
	flushinp();
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
	choices[1] = "QUIT";
	int choice;
	int highlight = 0;
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
		choice = key_pressed();
		switch(choice)
		{

			case USBKEY_UP:
				highlight--;
				if(highlight == -1){highlight = 1;}
				break;
			case USBKEY_DOWN:
				highlight++;
				if(highlight == 2){highlight = 0;}
				break;
			default:
				break;
		}

		if(choice == USBKEY_FCT){ //Enter
			switch(highlight)
			{
				case 0:
					flushinp();
					return 0;	
				case 1:
					flushinp();
					return 1;
			}

		}
		wrefresh(quit);
	
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
	flushinp();
}

void head_del(Snake* snake)
{
    ptr_cell temp = (*(*snake)).next;
    free(*snake);
    *snake = temp;
}

void tail_add(Snake* snake, Pos pos) //fin
{
    ptr_cell temp = *snake;
    while((*temp).next != NULL)
    {
        temp = (*temp).next;
    }
    (*temp).next = malloc(sizeof(Cell));
    (*((*temp).next)).pos = pos;
    (*((*temp).next)).next = NULL;
	snake_end.pos = pos;
	snake_end.next = NULL;
}

Cell get_tail(Snake* snake)
{
	ptr_cell temp = *snake;
    while( (*temp).next != NULL )
    {
        temp = (*temp).next;
    }
	
	return *temp;
}

void init_snake(Snake* snake)
{
	*snake = NULL;
	Pos sk1 = {2,1,RIGHT};
	Pos sk2 = {2,2,RIGHT};
	Pos sk3 = {2,3,RIGHT};
	head_add(snake, sk3);
	head_add(snake, sk2);
	head_add(snake, sk1);
	snake_end = get_tail(snake);
}


void snake_move(Snake* snake, int dir)
{
	int pos_x = snake_end.pos.x;
	int pos_y = snake_end.pos.y;
	int dir_l = snake_end.pos.dir;
	Pos new_pos;

	switch(dir) {
		case UP:
			if(dir_l == DOWN){
				new_pos.x = pos_x+1;
				new_pos.y = pos_y;
				new_pos.dir = DOWN;
				tail_add(snake, new_pos);
				head_del(snake);
				break;							
			}
			new_pos.x = pos_x-1;
			new_pos.y = pos_y;
			new_pos.dir = UP;
			tail_add(snake, new_pos);
			head_del(snake);
			break;
		case DOWN:
			if(dir_l == UP){
				new_pos.x = pos_x-1;
				new_pos.y = pos_y;
				new_pos.dir = UP;
				tail_add(snake, new_pos);
				head_del(snake);
				break;
			}
			new_pos.x = pos_x+1;
			new_pos.y = pos_y;
			new_pos.dir = DOWN;
			tail_add(snake, new_pos);
			head_del(snake);	
			break;
		case RIGHT:
			if(dir_l == LEFT){
				new_pos.x = pos_x;
				new_pos.y = pos_y-1;
				new_pos.dir = LEFT;
				tail_add(snake, new_pos);
				head_del(snake);
				break;
			}
			new_pos.x = pos_x;
			new_pos.y = pos_y+1;
			new_pos.dir = RIGHT;
			tail_add(snake, new_pos);
			head_del(snake);	
			break;
		case LEFT:
			if(dir_l == RIGHT){
				new_pos.x = pos_x;
				new_pos.y = pos_y+1;
				new_pos.dir = RIGHT;
				tail_add(snake, new_pos);
				head_del(snake);
				break;
			}
			new_pos.x = pos_x;
			new_pos.y = pos_y-1;
			new_pos.dir = LEFT;
			tail_add(snake, new_pos);
			head_del(snake);
			break;
	}	

}

void snake_disp(Snake* snake)
{
    ptr_cell temp = *snake;
    while(temp != NULL)
    {
		mvprintw(((*temp).pos.x), ((*temp).pos.y), snake_shape);
        temp = (*temp).next;
    }
}


int wall_check(void)
{
	if(snake_end.pos.x <= 1){return 1;} //En haut
	if(snake_end.pos.x >= max_y-1){return 1;} //En bas
	if(snake_end.pos.y <= 0){return 1;} //Gauche
	if(snake_end.pos.y >= max_x-1){return 1;} //Droite
	return 0;
}

int crash_check(Snake* snake)
{
	int pos_x, pos_y;
    ptr_cell temp = *snake;
	ptr_cell temp2;
    while(temp != NULL)
    {
		pos_x = (*temp).pos.x;
		pos_y = (*temp).pos.y;
        temp2 = (*temp).next;
		while(temp2 != NULL)
		{
			if((pos_x == (*temp2).pos.x) && (pos_y == (*temp2).pos.y)){return 1;}
			temp2 = (*temp2).next; 
		}
		temp = (*temp).next;
    }
	return 0;	
}

/* Programme principale */


int main(void) 
{
	if(init_usb()){printf("Unable to open the USB device\n");exit(-1);}
	Snake snake;
	int ch;
	int speed;
	int start = 1;
	srand(time(NULL));
	initscr();
	cbreak();
	curs_set(FALSE);
	keypad(stdscr, FALSE);
	
	menu_disp();
	erase();
	menu2_disp();	
	erase();
	
	init_snake(&snake);
	init_bonus();

	while(1) {

		speed = 50-score*2;
		if(speed < 0){speed = 0;}
		timeout(speed);

		getmaxyx(stdscr, max_y, max_x);

		ch = key_pressed();
		if(ch == USBKEY_FCT && !start){if(quit_disp()){break;}}
		start = 0; /* Eviter d'afficher le menu pause en cas d'appuie sur le bouton du joystick dans le menu 2 */
		set_dir(ch);
				
		clear();
		title_draw();
		score_disp();

		bonus_create();
		bonus_disp();

		snake_move(&snake,dir);
		snake_disp(&snake);

		if(wall_check()){break;}
		if(crash_check(&snake)){break;}
		bonus_check(&snake);
	
		refresh();
		usleep(DELAY);
		
	}

	endwin();
}
