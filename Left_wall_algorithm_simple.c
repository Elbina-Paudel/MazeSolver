#include <stdio.h>
#include <graphics.h>
#include <conio.h>

static int radius = 5;
static int startx = 20;
static int starty = 20;
static int endx = 420;
static int endy = 420;
static int unit_cell;
static int count = 0;

struct coordinate{
	int x;
	int y;
};

struct coordinate node[200];

int determine_orientation(int old_x, int old_y, int x, int y){
	int orientation;
		/*
		(1)facing right, <default>
		(2)facing up,
		(-1)facing left,
		(-2)facing down.
		*/
	if(old_x == x && old_y == y){
		//denotes start position.
		orientation = 1; // setting default value or starting orientation.
	}else{
		if(old_y == y){
			orientation = (x > old_x)? (1): (-1);
		}else{ //old_x == x
			orientation = (y > old_y)? (-2): (2);
		}
	}
	return orientation;
}

void maze(){
	int i = 0;

	//making the maze border
	setcolor(RED);
	setlinestyle(3, 0, 3);
	rectangle(startx - 10, starty - 10, endx + 10, endy + 10); // encloses a square of dimension 400 x 400

	//making maze path
	setcolor(LIGHTGRAY); // code : 7
	moveto(startx, starty);

	for(i = 0; i < count+1 ; i ++){
		setlinestyle(0, 0, 1);
		lineto(node[i].x, node[i].y);
	}
}

int check(int x, int y){
	if(x < startx || y < starty || x > endx || y > endy)
		return 0;
	else
		return 1;
}

void draw_maze(){
	char key;
	int n, u, loop = 1;
	int x, y, i;

	cleardevice();
	delay(10);
	printf("\nMaze Size: <1> 2x2 <2> 4x4 <3> 5x5 <4> 8x8 <5> 10x10 <6> 16x16 ");
	while(loop == 1){
		key = getch();
		switch(key){
			case '1':
				n = 2;
				loop = 0;
			break;

			case '2':
				n = 4;
				loop = 0;
			break;

			case '3':
				n = 5;
				loop = 0;
			break;

			case '4':
				n = 8;
				loop = 0;
			break;

			case '5':
				n = 10;
				loop = 0;
			break;

			case '6':
				n = 16;
				loop = 0;
			break;

			default:
				loop =1;
		}
	}
	cleardevice();
	unit_cell = (endx - startx)/n;

	loop = 1;

	circle(startx, starty, radius);
	circle(endx, endy, radius);

	moveto(startx, starty);
	x = startx;
	y = starty;
	node[count].x = x;
	node[count].y = y;

	while(loop == 1){
		key = getch();
		switch(key){
			case 'a':
				if(check(x-unit_cell, y) == 1){
					x = x-unit_cell;
					y = y;
					count++;
					node[count].x = x;
					node[count].y = y;
				}
			break;

			case 'w':
				if(check(x, y-unit_cell) == 1){
					x = x;
					y = y-unit_cell;
					count++;
					node[count].x = x;
					node[count].y = y;
				}
			break;

			case 'd':
				if(check(x+unit_cell, y) == 1){
					x = x+unit_cell;
					y = y;
					count++;
					node[count].x = x;
					node[count].y = y;
				}
			break;

			case 's':
				if(check(x, y+unit_cell) == 1){
					x = x;
					y = y+unit_cell;
					count++;
					node[count].x = x;
					node[count].y = y;
				}
			break;
			case 27:
				cleardevice();
				circle(startx, starty, radius);
				circle(endx, endy, radius);
			break;

			default:;
		}
		lineto(x, y);
		if(x == endx && y == endy){
			loop =0;
		}
	}
}

int bot_color(int x, int y){

	int status;
	circle(x, y, radius);

	if(x == startx && y == starty){
		setfillstyle(SOLID_FILL, YELLOW);
		status = 1; // start
	}
	else if(x == endx && y == endy){
		setfillstyle(SOLID_FILL, LIGHTGREEN);
		status = 2; // end
	}
	else if(x > endx || y > endy || x < startx || y < starty){
		setfillstyle(SOLID_FILL, RED);
		status = 13; // out of bounds
	}
	else{
		setfillstyle(SOLID_FILL, DARKGRAY);
		status = 0; // on maze
	}
	floodfill(x+1, y+1, LIGHTGRAY);
	return status;
}

int refresh(int x, int y){
	int status;

	cleardevice();

	status = bot_color(x, y);

	maze();

	delay(20);

	return status;
}

void left_wall_algorithm(){
	int find = 1; //controlls the loop below
	int status; //holds the bot status
	int orientation;

	int x = startx; // the start coordinates.
	int y = starty;

	int old_x = x, old_y = y;

	int maze_color = getpixel(x, y); //detect maze color.

	while(find == 1)
	{
		//Traversal of Maze

		orientation = determine_orientation(old_x, old_y, x, y);
		/*
		(1)facing right,
		(2)facing up,
		(-1)facing left,
		(-2)facing down.
		*/
		old_x = x;
		old_y = y;
		switch (orientation)
		{
			case 1: //facing right.
				if(getpixel(x, y - 1) == maze_color){
					y--;
				}else if(getpixel(x + 1, y) == maze_color){
					x++;
				}else if(getpixel(x, y + 1) == maze_color){
					y++;
				}else{ // turn around i.e. dead end found.
					x--;
				}
			break;

			case -1: // facing left.
				if(getpixel(x, y + 1) == maze_color){
					y++;
				}else if(getpixel(x - 1, y) == maze_color){
					x--;
				}else if(getpixel(x, y - 1) == maze_color){
					y--;
				}else{ // turn around i.e. dead end found.
					x++;
				}
			break;

			case 2: // facing up.
				if(getpixel(x-1, y) == maze_color){
					x--;
				}else if(getpixel(x, y-1) == maze_color){
					y--;
				}else if(getpixel(x+1, y) == maze_color){
					x++;
				}else{ // turn around i.e. dead end found.
					y++;
				}
			break;

			case -2: // facing down.
				if(getpixel(x+1, y) == maze_color){
					x++;
				}else if(getpixel(x, y+1) == maze_color){
					y++;
				}else if(getpixel(x-1, y) == maze_color){
					x--;
				}else{ // turn around i.e. dead end found.
					y--;
				}
			break;

			default:;
		}

		status = refresh(x, y);
		if(status == 2 || status == 13)
		{
			break;
		}
	}
}

int main()
{
	int gd = DETECT, gm, wait_for_key = 1, n;
	char key;

	initgraph(&gd, &gm, "c:\\turboc3\\bgi");

	// Darwing a maze.
	//maze();
	draw_maze();
	maze();
	refresh(startx, starty);

	while (wait_for_key == 1) {
		key = getch();
		switch(key)
		{
			case 13: //enter
				wait_for_key = 0;
				left_wall_algorithm();
			break;
			case 27: //esc
				wait_for_key = 0;
			break;
			default:
				wait_for_key = 1;
		}
	}

	getch();
	cleardevice();
	closegraph();
	return 0;
}