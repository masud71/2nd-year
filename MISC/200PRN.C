/*  Functions for free hand drawing  */

/*------------------------------------------------------------------------*/
/*  Global Variables from others :-										  */
/*         x -- Current x coordinate in pixel							  */
/*		   y -- Current y coordinate in pixel                             */
/*         line_no -- Current line number (Index of buffer)				  */
/*         g_mode -- Indicator of current mode is graphic 				  */
/*  Global Variables created by own :-                                    */
/*         ins -- (Charecter Type) indecator of active drawing mode       */
/*         arrow -- (void pointer) Store the bitmap of the drawing pen    */
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
#define ERASE     59

#define MAX_X 	  719   /*------------------------------------------------*/
#define MAX_Y 	  347   /*    These three values will be found from setup */
#define LNHT	  12    /*------------------------------------------------*/

char ins=0;             /*   Global Variables only requared during free   */
void *arrow,*rubber;    /*               hand drawing                     */

void free_graph(void);
void left_arrow(void);
void right_arrow(void);
void up_arrow(void);
void down_arrow(void);
void left(void);
void right(void);
void up(void);
void down(void);
void rev_pixel(void);
void cursor(void);
void create_cursor(void);
void rub(void);
void create_rubber(void);
void store_image(int,int,int,int);

void free_graph(void)
/*
	NAME 	: free_graph
	FUNCTION: Master function to control free hand drawing.Call from sub menu
	ROUTINES: stdio.h,graphics.h
*/
{
	int lx = MAX_X,rx = 0,ty = MAX_Y,by = 0;
	/* Initialize left,right,top & bottom by opposite end */
	int size;
	char ch,temp,delmode=0;

	g_mode = 1      /* Set graphic mode 1  */
	activedraw();  /* Show an indecator that drawing mode is active or not */
	create_cursor();    /* Create the Pen for drawing purpose    */
	create_rubber();    /* Create an eraser for deleting purpose */
	do {
		if (!delmode) cursor();
			else rub();
		ch = getch();
		if (!ch) ch = getch();
		if (!delmode) cursor();                /* Show the pen or the */
		else putimage(x,y,rubber,XOR_PUT);     /* eraser according to mode */
		switch (ch) {
			case L_ARROW :left_arrow();break;
			case R_ARROW :right_arrow();break;
			case U_ARROW :up_arrow();break;
			case D_ARROW :down_arrow();break;
			case ALT_LEFT:left();break;
			case ALT_RIGHT:right();break;
			case ALT_UP:up();break;
			case ALT_DOWN:down();break;
			case ACTIVE :if (!delmode) {
						ins = !ins;
						activedraw();break;
			case ENTER :if (!delmode) rev_pixel();break;
			case ERASE :delmode = !delmode;
					 d_pressed(delmode);/* Show an indicator if delete mode */
					 if (delmode) {
						temp = ins;  /* If delete mode then temporaryly */
						ins = 0;     /* stop the drawing mode           */
					 } else {
						ins = temp;
					 }
					 break;
		}                         /* End of switch */
/* If drawing mode then calculate the current aria of the art wark  */
		if (ins) {
			if (x < lx) lx = x;
			if (x > rx) rx = x;
			if (y < ty) ty = y;
			if (y > by) by = y;
		}
	} while (ch != 26);        /* Exit from the wark by ^Z */
    free(arrow);
	free(rubber);
	store_image(lx,ty,rx,by);   /* Store the image in buffer  */
	g_mode = 0;        /*  Set graphic mode 0 */
}

void left_arrow(void)
/*
	NAME 	: left_arrow
	FUNCTION: To move the pen one pixel left & put a pixel in previous point
			  in active mode
	ROUTINES: graphics.h
*/
{
	if (ins) putpixel(x,y,1);
	if (x) x--;
	else x = MAX_X;   /* If left border crossed then go to right border */
}

void right_arrow(void)
/*
	NAME 	: right_arrow
	FUNCTION: To move the pen one pixel right & put a pixel in previous point
			  in active mode
	ROUTINES: graphics.h
*/
{
	if (ins) putpixel(x,y,1);
	if (x != MAX_X) x++;
	else x = 0;       /* If right border crossed then go to left border */
}

void up_arrow(void)
/*
	NAME 	: up_arrow
	FUNCTION: To move the pen one pixel up & put a pixel in previous point
			  in active mode
	ROUTINES: graphics.h
*/
{
	if (ins) putpixel(x,y,1);
	if (y) y--;
	else y = MAX_Y;    /* If top border crossed then go to bottom border */
}

void down_arrow(void)
/*
	NAME 	: down_arrow
	FUNCTION: To move the pen one pixel down & put a pixel in previous point
			  in active mode
	ROUTINES: graphics.h
*/
{
	if (ins) putpixel(x,y,1);
	if (y != MAX_Y) y++;
	else y = 0;        /* If bottom border crossed then go to top border */
}

void up(void)
/*
	NAME 	: up
	FUNCTION: To move the pen nine pixel up & put a line between the points
			  in active mode
	ROUTINES: graphics.h
*/
{
	if ((ins) && (y >= 9)) lineto(x,y-9);
	if (y >= 9) y -= 9;
}

void down(void)
/*
	NAME 	: down
	FUNCTION: To move the pen nine pixel down & put a line between the points
			  in active mode
	ROUTINES: graphics.h
*/
{
	if ((ins) && (y <= (MAX_Y - 9))) lineto(x,y+9);
	if (y <= (MAX_Y - 9)) y += 9;
}

void left(void)
/*
	NAME 	: left
	FUNCTION: To move the pen 14 pixel left & put a line between the points
			  in active mode
	ROUTINES: graphics.h
*/
{
	if ((ins) && (x >= 14)) lineto(x-14,y);
	if (x >= 14) x -= 14;
}

void right(void)
/*
	NAME 	: right
	FUNCTION: To move the pen 14 pixel right & put a line between the points
			  in active mode
	ROUTINES: graphics.h
*/
{
	if ((ins) && (x <= (MAX_X - 14))) lineto(x+14,y);
	if (x <= (MAX_X - 14)) x += 14;
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

	color = getpixel(x,y);
	if (color) putpixel(x,y,BLACK);
	else putpixel(x,y,WHITE);
}

void cursor(void)
/*
	NAME 	: cursor
	FUNCTION: To move the pen in the screen to draw picture
	ROUTINES: graphics.h
*/
{
	moveto(x,y);
	putimage(x,y,arrow,XOR_PUT);
}

void rub(void)
/*
	NAME 	: rub
	FUNCTION: To move the eraser in the screen to erase the picture
	ROUTINES: graphics.h
*/
{
	moveto(x,y);
	putimage(x,y,rubber,COPY_PUT);
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

	drawpoly(9,poly);
	floodfill(301,151,1);
	line(304,154,310,160);
	arrow = (void *)malloc(imagesize(300,150,311,162));
	getimage(300,150,311,162,arrow);
	putimage(300,150,arrow,XOR_PUT);
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

	rectangle(306,152,324,162);
	setfillpattern(pattern,1);
	floodfill(315,157,1);
	rubber = (void *)malloc(imagesize(306,152,324,162));
	getimage(306,152,324,162,rubber);
	putimage(306,152,rubber,XOR_PUT);
}

void store_image(int lx,int ty,int rx,int by)
/*
	NAME 	: store_image
	FUNCTION: To store an image in main buffer
	VARIABLE: i -- incriments line numbers of buffer
			  size -- store the size of one line image
			  img  -- pointer to store one line image
	ROUTINES: graphics.h
*/
{
	int i,size;
	void *img;

	i = line_no;    /* line_no is a global variable to indicate current line */
	while (ty <= by) {
		status[i] = lx; /* Setting left column of image in graph status array */
		size = imagesize(lx,ty,rx,ty+LNHT-1);
		img = (void *)malloc(size);
		getimage(lx,ty,rx,ty+LNHT-1,img);
		buffer[i++] = img;      /* Storing 1 line image in buffer  */
		ty += LNHT;
	}
	line_no = i;      /* update line_no before exiting graphic mode */
}

/*-----------------Masud Karim Khan. Roll 17 , Group no. 7-----------------*/