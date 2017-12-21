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

/* USB Keys */

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
Cell snake_end_b;
int max_bonus;
int spawning_rate;
int score = 0;
int score_b = 0;
int nb_bonus = 0;
int max_x, max_y;
Bonus bonus_list[MAX_BONUS];
int dir = RIGHT;
int dir_b = RIGHT;
char snake_shape[] = "o";
char title[] = "Simple Snake v3.0";

/* Fonctions */

void set_dir(int c, int id)
{
	if(id == 1){
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
	
	else if(id == 2)
	{
		switch(c) {
			case KEY_LEFT:
				dir_b=LEFT;
				break;
			case KEY_RIGHT:
				dir_b=RIGHT;
				break;
			case KEY_UP:
				dir_b=UP;
				break;
			case KEY_DOWN:
				dir_b=DOWN;
				break;
		}
	}
}

void UI_draw(void)
{
	int i = 0;
	for(i=0;i<=max_x;i++)
	{
		mvprintw(1, i, "#");
		mvprintw(max_y-2, i, "#");
	}

	for(i=0;i<=max_y-5;i++)
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
		rand_y = rand()%max_y-2;
		
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

void score_disp(int id)
{
	char score_txt[16] = "Score : ";
	char score_nb[8];
	
	if(id == 1)
	{
		sprintf(score_nb, "%d", score);
		strcat(score_txt, score_nb);
		mvprintw(0, 0, score_txt);
	}
	
	else if(id == 2)
	{
		sprintf(score_nb, "%d", score_b);
		strcat(score_txt, score_nb);
		mvprintw(max_y-1, 0, score_txt);
	}
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

void bonus_check(Snake* snake, int id)
{
	int i;
	Cell snake_end_temp;
	if(id == 1){snake_end_temp = snake_end;}
	else if(id == 2){snake_end_temp = snake_end_b;}
	
	for(i=0; i<max_bonus; i++)
	{
		if(snake_end_temp.pos.x == bonus_list[i].pos_y && snake_end_temp.pos.y == bonus_list[i].pos_x)
		{
			if(id == 1){score++;}
			else if(id == 2){score_b++;}
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
	noecho();
	WINDOW * menu = newwin(8,20,(max_y/2)-4,(max_x/2)-10);
	box(menu,0,0);
	refresh();
	wrefresh(menu);
	//keypad(menu, true);

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

		if(choice == USBKEY_FCT)
		{
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

		if(choice == USBKEY_FCT)
		{
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
	char amount[8];
	echo();
	getmaxyx(stdscr, max_y, max_x);
	WINDOW * menu2 = newwin(5,30,(max_y/2)-3,(max_x/2)-15);
	box(menu2,0,0);
	refresh();
	wrefresh(menu2);

	mvwprintw(menu2, 1, 1, "Bonus (max 100)");
	mvwprintw(menu2, 2, 1, "----------------------------");
	mvwprintw(menu2, 3, 1, "Amount : ");
	mvwgetstr(menu2, 3, 10, amount);
	max_bonus = atoi(amount);
	if(max_bonus > 100){max_bonus = 100;}
	flushinp();
}

void head_del(Snake* snake)
{
    ptr_cell temp = (*(*snake)).next;
    free(*snake);
    *snake = temp;
}

void tail_add(Snake* snake, Pos pos, int id) //fin
{
    ptr_cell temp = *snake;
    while((*temp).next != NULL)
    {
    	temp = (*temp).next;
    }
    (*temp).next = malloc(sizeof(Cell));
    (*((*temp).next)).pos = pos;
    (*((*temp).next)).next = NULL;
    if(id == 1)
    {
			snake_end.pos = pos;
			snake_end.next = NULL;
    }
    else if(id == 2)
    {
			snake_end_b.pos = pos;
			snake_end_b.next = NULL;
    }
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

void init_snake(Snake* snake, int id)
{
	*snake = NULL;
	Pos sk1;
	Pos sk2;
	Pos sk3;
	if(id == 1)
	{
		sk1.x = 2;
		sk1.y = 1;
		sk1.dir = RIGHT;

		sk2.x = 2;
		sk2.y = 2;
		sk2.dir = RIGHT;
		
		sk3.x = 2;
		sk3.y = 3;
		sk3.dir = RIGHT;		
	}
	else if(id == 2)
	{
		sk1.x = max_y-3;
		sk1.y = 1;
		sk1.dir = RIGHT;

		sk2.x = max_y-3;
		sk2.y = 2;
		sk2.dir = RIGHT;
		
		sk3.x = max_y-3;
		sk3.y = 3;
		sk3.dir = RIGHT;	
	}
	head_add(snake, sk3);
	head_add(snake, sk2);
	head_add(snake, sk1);
	if(id == 1) snake_end = get_tail(snake);
	else if(id == 2) snake_end_b = get_tail(snake);
}


void snake_move(Snake* snake, int dir, int id)
{
	Pos new_pos;
	int pos_x;
	int pos_y;
	int dir_l;
	if(id == 1)
	{
		pos_x = snake_end.pos.x;
		pos_y = snake_end.pos.y;
		dir_l = snake_end.pos.dir;
	}
	else if(id == 2)
	{
		pos_x = snake_end_b.pos.x;
		pos_y = snake_end_b.pos.y;
		dir_l = snake_end_b.pos.dir;
	}	
	switch(dir) {
		case UP:
			if(dir_l == DOWN){
				new_pos.x = pos_x+1;
				new_pos.y = pos_y;
				new_pos.dir = DOWN;
				tail_add(snake, new_pos, id);
				head_del(snake);
				break;							
			}
			new_pos.x = pos_x-1;
			new_pos.y = pos_y;
			new_pos.dir = UP;
			tail_add(snake, new_pos, id);
			head_del(snake);
			break;
		case DOWN:
			if(dir_l == UP){
				new_pos.x = pos_x-1;
				new_pos.y = pos_y;
				new_pos.dir = UP;
				tail_add(snake, new_pos, id);
				head_del(snake);
				break;
			}
			new_pos.x = pos_x+1;
			new_pos.y = pos_y;
			new_pos.dir = DOWN;
			tail_add(snake, new_pos, id);
			head_del(snake);	
			break;
		case RIGHT:
			if(dir_l == LEFT){
				new_pos.x = pos_x;
				new_pos.y = pos_y-1;
				new_pos.dir = LEFT;
				tail_add(snake, new_pos, id);
				head_del(snake);
				break;
			}
			new_pos.x = pos_x;
			new_pos.y = pos_y+1;
			new_pos.dir = RIGHT;
			tail_add(snake, new_pos, id);
			head_del(snake);	
			break;
		case LEFT:
			if(dir_l == RIGHT){
				new_pos.x = pos_x;
				new_pos.y = pos_y+1;
				new_pos.dir = RIGHT;
				tail_add(snake, new_pos, id);
				head_del(snake);
				break;
			}
			new_pos.x = pos_x;
			new_pos.y = pos_y-1;
			new_pos.dir = LEFT;
			tail_add(snake, new_pos, id);
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


int wall_check(int id)
{
	if(id == 2)
	{
		if(snake_end_b.pos.x <= 1){return 1;} //En haut
		if(snake_end_b.pos.x >= max_y-2){return 1;} //En bas
		if(snake_end_b.pos.y <= 0){return 1;} //Gauche
		if(snake_end_b.pos.y >= max_x-1){return 1;} //Droite
	}
	else if(id == 1)
	{
		if(snake_end.pos.x <= 1){return 1;} //En haut
		if(snake_end.pos.x >= max_y-2){return 1;} //En bas
		if(snake_end.pos.y <= 0){return 1;} //Gauche
		if(snake_end.pos.y >= max_x-1){return 1;} //Droite
	}
	
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

int min(int a, int b)
{
	if(a<b){return a;}
	else {return b;}
}

int crash_snake_check(Snake* snake, int id)
{
	Cell snake_end_temp;
	if(id == 1)
	{
		snake_end_temp = snake_end;
	}
	
	else if(id == 2)
	{
		snake_end_temp = snake_end_b;
	}
		
  ptr_cell temp = *snake;
  while(temp != NULL)
  {
		if( ((*temp).pos.x == snake_end_temp.pos.x) && ((*temp).pos.y == snake_end_temp.pos.y) ){return 1;}
    temp = (*temp).next;
  }
	return 0;
}

/* Programme principale */

int main(int argc, char *argv[]) 
{
	if(argc != 2){printf("Too many arguments, please use the play.sh script to play\n");exit(-1);}
	if(init_usb()){printf("Unable to open the USB device\n");exit(-1);}
	
	int nb_player;
	nb_player = atoi(argv[1]);
	if(nb_player != 1 && nb_player != 2)
	{
		nb_player = 1;
	}
		
	Snake snake;
	Snake snake_b;
	
	int ch, ch2, speed;
	int start = 1;
	srand(time(NULL));
	initscr();
	cbreak();
	curs_set(FALSE);
	keypad(stdscr, TRUE);
	
	menu_disp();
	erase();
	menu2_disp();	

	getmaxyx(stdscr, max_y, max_x);
	init_snake(&snake,1);
	if(nb_player == 2){init_snake(&snake_b,2);}
	init_bonus();

	while(1) {

		if(nb_player == 2){speed = 50-min(score,score_b)*2;}
		else{speed = 50-score*2;}
		if(speed < 0){speed = 0;}
		timeout(speed);

		getmaxyx(stdscr, max_y, max_x);

		ch = key_pressed();
		
		if(nb_player == 2){ch2 = getch();}
		flushinp();
		
		if((ch == USBKEY_FCT || ch2 == 27) && !start)
		{
			usb_send('1');
			usb_send('1');
			if(quit_disp()){break;}
			usb_send('0');
			usb_send('0');
		}
		start = 0; /* Eviter d'afficher le menu pause en cas d'appuie sur le bouton du joystick dans le menu 2 */

		if((ch == USBKEY_UP) || (ch == USBKEY_DOWN) || (ch == USBKEY_LEFT) || (ch == USBKEY_RIGHT)){set_dir(ch,1);}
		if(nb_player == 2)
		{
			if((ch2 == KEY_UP) || (ch2 == KEY_DOWN) || (ch2 == KEY_LEFT) || (ch2 == KEY_RIGHT)){set_dir(ch2,2);}
		}
				
		clear();
		UI_draw();
		score_disp(1);
		if(nb_player == 2){score_disp(2);}
		
		bonus_create();
		bonus_disp();

		snake_move(&snake,dir,1);
		if(nb_player == 2){snake_move(&snake_b,dir_b,2);}
		snake_disp(&snake);
		if(nb_player == 2){snake_disp(&snake_b);}

		if(wall_check(1)){break;}		
		if(crash_check(&snake)){break;}
		bonus_check(&snake,1);
		
		if(nb_player == 2)
		{
			if(wall_check(2)){break;}
			if(crash_check(&snake_b)){break;}
			bonus_check(&snake_b,2);
			if(crash_snake_check(&snake,2)){break;}
			if(crash_snake_check(&snake_b,1)){break;}
		}
		
		refresh();
		usleep(DELAY);

	}
	usb_send('0');
	usb_send('0');
	endwin();
}
