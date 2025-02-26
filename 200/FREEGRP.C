/*  Functions for free hand drawing  */

/*------------------------------------------------------------------------*/
/*  Global Variables from others :-										  */
/*         lx     -- Left x Coordinate of Graph Aria                      */
/*         rx     -- Right x Coordinate of Graph Aria                     */
/*         ty     -- Top y Coordinate of Graph Aria                       */
/*         by     -- Bottom y Coordinate of Graph Aria                    */
/*  Global Variables created by own :-                                    */
/*         gx -- Current x coordinate in pixel							  */
/*		   gy -- Current y coordinate in pixel                            */
/*         insrt -- (Charecter Type) indecator of active drawing mode     */
/*         curarr -- (void pointer) Store the bitmap of the drawing pen    */
/*         rubber --(void pointer) Store the bitmap of the eraser         */
/*     																	  */
/*	  The bit map of the graph drawn is stored in the global buffer,line  */
/*	by line & its starting column number is stored in another global array*/
/*  status;in parralal index to buffer which is simultaniously indicats   */
/*  whether the indexed line is graphic or not. The status array must be  */
/*  initialized by -1 during starting of the program in the setup function*/
/*                                                                        */
/*		Calling of free_grp function :--                                  */
/*					Main menu -->Graphics -->Free Hand Drawing            */
/*------------------------------------------------------------------------*/
#include <graphics.h>
#include <alloc.h>

#define L_ARROW   75
#define R_ARROW   77
#define U_ARROW   72
#define D_ARROW   80
#define ALT_LEFT  -101
#define ALT_RIGHT -99
#define ALT_UP    -104
#define ALT_DOWN  -96
#define ENTER     13
#define ACTIVE    82
#define ERASE     4

#define MAX_X 	  719   /*------------------------------------------------*/
#define MAX_Y 	  347   /*    These three values will be found from setup */
#define LNHT	  12    /*------------------------------------------------*/

extern int set_graph_aria(void);
extern int store_image(void);

extern int lx,rx,ty,by,maxx,maxy;
int insrt,gx,gy;
char far *curarr,far *rubber;    /*               hand drawing                     */

void gleft_arrow(void)
/*
	NAME 	: left_arrow
	FUNCTION: To move the pen one pixel left & put a pixel in previous point
			  in active mode
	ROUTINES: graphics.h
*/
{
	if (insrt) putpixel(gx,gy,1);
	if (gx > lx) gx--;
	else gx = rx;   /* If left border crossed then go to right border */
}

void gright_arrow(void)
/*
	NAME 	: right_arrow
	FUNCTION: To move the pen one pixel right & put a pixel in previous point
			  in active mode
	ROUTINES: graphics.h
*/
{
	if (insrt) putpixel(gx,gy,1);
	if (gx < rx) gx++;
	else gx = lx;       /* If right border crossed then go to left border */
}

void gup_arrow(void)
/*
	NAME 	: up_arrow
	FUNCTION: To move the pen one pixel up & put a pixel in previous point
			  in active mode
	ROUTINES: graphics.h
*/
{
	if (insrt) putpixel(gx,gy,1);
	if (gy > ty) gy--;
	else gy = by;    /* If top border crossed then go to bottom border */
}

void gdown_arrow(void)
/*
	NAME 	: down_arrow
	FUNCTION: To move the pen one pixel down & put a pixel in previous point
			  in active mode
	ROUTINES: graphics.h
*/
{
	if (insrt) putpixel(gx,gy,1);
	if (gy < by) gy++;
	else gy = ty;        /* If bottom border crossed then go to top border */
}
void up(void)
/*
	NAME 	: up
	FUNCTION: To move the pen nine pixel up & put a line between the points
			  in active mode
	ROUTINES: graphics.h
*/
{
	if ((insrt) && (gy >= (ty + 9))) lineto(gx,gy-9);
	if (gy >= (ty + 9)) gy -= 9;
}

void down(void)
/*
	NAME 	: down
	FUNCTION: To move the pen nine pixel down & put a line between the points
			  in active mode
	ROUTINES: graphics.h
*/
{
	if ((insrt) && (gy <= (by - 9))) lineto(gx,gy+9);
	if (gy <= (by - 9)) gy +=9;
}

void left(void)
/*
	NAME 	: left
	FUNCTION: To move the pen 14 pixel left & put a line between the points
			  in active mode
	ROUTINES: graphics.h
*/
{
	if ((insrt) && (gx >= (lx + 14))) lineto(gx-14,gy);
	if (gx >=(lx+14)) gx -=14;
}

void right(void)
/*
	NAME 	: right
	FUNCTION: To move the pen 14 pixel right & put a line between the points
			  in active mode
	ROUTINES: graphics.h
*/
{
	if ((insrt) && (gx <= (rx - 14))) lineto(gx+14,gy);
	if (gx <= (rx - 14)) gx +=14;
}

void gcursor(void)
/*
	NAME 	: cursor
	FUNCTION: To move the pen in the screen to draw picture
	ROUTINES: graphics.h
*/
{
	moveto(gx,gy);
	putimage(gx,gy,curarr,XOR_PUT);
}

void rev_pixel(void)
/*
	NAME 	: rev_pixel
	FUNCTION: To reverse the color of the pixel of current position
	VARIABLE: color -- store the current color of pixel at current location
	ROUTINES: graphics.h
*/
{
	int color;

	color = getpixel(gx,gy);
	if (color) putpixel(gx,gy,BLACK);
	else putpixel(gx,gy,WHITE);
}

void rub(void)
/*
	NAME 	: rub
	FUNCTION: To move the eraser in the screen to erase the picture
	ROUTINES: graphics.h
*/
{
	moveto(gx,gy);
	putimage(gx,gy,rubber,COPY_PUT);
}

void create_cursor(void)
/*
	NAME 	: create_cursor
	FUNCTION: To create a pen for drawing purpose
	VARIABLE: poly -- store some points to create the structure of the pen
	ROUTINES: graphics.h
*/
{
	int poly[] = {300,150, 300,156, 301,156, 303,154, 310,161, 311,160, 305,154, 308,153,300,150};

	fillpoly(9,poly);
	curarr = (char far *) farmalloc(imagesize(300,150,311,162));
	getimage(300,150,311,162,curarr);
	putimage(300,150,curarr,XOR_PUT);
}

void create_rubber(void)
/*
	NAME 	: create_rubber
	FUNCTION: To create a eraser for erasing picture purpose
	VARIABLE: pattern -- store the fill style of the eraser
	ROUTINES: graphics.h
*/
{
	char pattern[8] = {0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55};
	int poly[] = {306,152,324,152,324,162,306,162,306,152};

	setfillpattern(pattern,1);
	fillpoly(5,poly);
	rubber = (char far *) farmalloc(imagesize(306,152,324,162));
	getimage(306,152,324,162,rubber);
	putimage(306,152,rubber,XOR_PUT);
}

void activedraw(int stats)
{
	setviewport(620,maxy-16,710,maxy,0);
	clearviewport();
	if (stats) {
		rectangle(2,1,72,12);
		floodfill(10,4,1);
		setcolor(0);
		if (insrt) outtextxy(5,3,"Active");
		else outtextxy(5,3,"Inactive");
		setcolor(7);
		setviewport(0,0,maxx,maxy,1);
	}
}

void d_pressed(int stats)
{
	setviewport(500,maxy-16,600,maxy,1);
	clearviewport();
	if (stats) {
		rectangle(2,1,94,12);
		floodfill(10,4,1);
		setcolor(0);
		outtextxy(5,3,"Delete Mode");
		setcolor(7);
	}
	setviewport(0,0,maxx,maxy,1);
}


void free_graph(void)
/*
	NAME 	: free_graph
	FUNCTION: Master function to control free hand drawing.Call from sub menu
	ROUTINES: stdio.h,graphics.h
*/
{
	/* Initialize left,right,top & bottom by opposite end */
	int size;
	char temp,delmode=0;
	union {
		int i;
		char ch[2];
	}input;

	if (!set_graph_area()) return;
	gy = ty;
	gx = lx;
	setwritemode(COPY_PUT);
	setviewport(20,maxy-16,500,maxy,1);
	clearviewport();
	outtextxy(5,1,"Ins - Active/Inactive   ^D - Rubber   ^Z - Exit");
	setviewport(0,0,maxx,maxy,1);
	activedraw(1);   /* Show an indecator that drawing mode is active or not */
	do {
		if (!delmode) gcursor();
			else rub();
		input.i = bioskey(0);
		if (!delmode) gcursor();                /* Show the pen or the */
		else putimage(gx,gy,rubber,XOR_PUT);     /* eraser according to mode */
		if (input.ch[0]) {
		switch (input.ch[0]) {
			case ENTER :if (!delmode) rev_pixel();break;
            case ERASE :delmode = !delmode;
					 d_pressed(delmode); /*Show an indicator if delete mode */
					 if (delmode) {
						temp = insrt;  /* If delete mode then temporaryly */
						insrt = 0;     /* stop the drawing mode           */
					 } else {
						insrt = temp;
					 }
					 break;
            default : if (input.ch[0] >=32 && input.ch[0] <=126) {
						if (rx-gx > 8 && by-gy > 8) {
							input.ch[1] = '\0';
							outtext(input.ch);
							gx +=8;
						}
					  }
					  break;
			}
		}
		else {
		switch (input.ch[1]) {
			case L_ARROW :gleft_arrow();break;
			case R_ARROW :gright_arrow();break;
			case U_ARROW :gup_arrow();break;
			case D_ARROW :gdown_arrow();break;
			case ALT_LEFT:left();break;
			case ALT_RIGHT:right();break;
			case ALT_UP:up();break;
			case ALT_DOWN:down();break;
			case ACTIVE :if (!delmode) {
							insrt = !insrt;
							activedraw(1);
						}
						break;
			case 71 :temp=insrt;gleft_arrow();insrt=0;
					gup_arrow();insrt=temp;break;
			case 73 :temp=insrt;gright_arrow();insrt=0;
					gup_arrow();insrt=temp;break;
			case 79 :temp=insrt;gleft_arrow();insrt=0;
					gdown_arrow();insrt=temp;break;
			case 81 :temp=insrt;gright_arrow();insrt=0;
					gdown_arrow();insrt=temp;break;

		}                         /* End of switch */
		}
	} while (input.ch[0] != 26);        /* Exit from the wark by ^Z */
	d_pressed(0);
	activedraw(0);
	setviewport(20,maxy-16,520,maxy,1);
	clearviewport();
	setviewport(0,0,maxx,maxy,1);
	outtextxy(300,maxy-8,"F10-MAIN MENU");
	store_image();
	setwritemode(XOR_PUT);
}
/*----------------Masud Karim Khan. Roll 17 , Group no. 7-----------------*/