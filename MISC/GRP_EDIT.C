#include <stdio.h>
#include <graphics.h>
#include <math.h>

#define MAX_X 639
#define MAX_Y 299

int x=0,y=0,ins=0;

void screen(void);
void left_arrow(void);
void right_arrow(void);
void up_arrow(void);
void ins_pressed(void);
void down_arrow(void);
void left(void);
void right(void);
void up(void);
void down(void);
void rev_pixel(void);
void indicator(void);
void draw_aria(int);
void cursor(void);
int arrows(void);

main()
{
	int driver,mode,temp;
	char ch;

	driver=DETECT;
	initgraph(&driver,&mode,"");
	screen();
	do {
		indicator();
		cursor();
		ch=getch();
		if (!ch) ch=getch();
		switch (ch) {
			case 75 :left_arrow();break;
			case 77 :right_arrow();break;
			case 72 :up_arrow();break;
			case 80 :down_arrow();break;
			case -101:left();break;
			case -99:right();break;
			case -104:up();break;
			case -96:down();break;
			case 82 :ins_pressed();break;
			case 13 :rev_pixel();break;
			case 59 :draw_aria(0);break;
			case 60 :draw_aria(2);break;
			case 61 :draw_aria(1);break;
			case 62 :draw_aria(3);break;
		}

	} while (ch !=26);
	closegraph();
}

void screen(void)
{
	setfillstyle(SOLID_FILL,WHITE);
	floodfill(0,0,1);
	setviewport(40,312,679,325,1);
	clearviewport();
	settextstyle(2,HORIZ_DIR,4);
	outtextxy(20,3,"X Location = 1            Y Location = 1");
	setviewport(35,4,684,307,1);
	clearviewport();
    setviewport(40,6,679,305,1);
}

void left_arrow(void)
{
	if (ins) rev_pixel();
	if (x) x--;
	else x=MAX_X;
}

void right_arrow(void)
{
	if (ins) rev_pixel();
	if (x !=MAX_X) x++;
	else x=0;
}

void up_arrow(void)
{
	if (ins) rev_pixel();
	if (y) y--;
	else y=MAX_Y;
}

void down_arrow(void)
{
	if (ins) rev_pixel();
	if (y !=MAX_Y) y++;
	else y=0;
}

void up(void)
{
	if ((ins) && (y >= 9)) lineto(x,y-9);
	if (y >= 9) y -=9;
}

void down(void)
{
	if ((ins) && (y <= (MAX_Y - 9))) lineto(x,y+9);
	if (y <= (MAX_Y - 9)) y +=9;
}

void left(void)
{
	if ((ins) && (x >=14)) lineto(x-14,y);
	if (x >=14) x -=14;
}

void right(void)
{
	if ((ins) && (x <= (MAX_X - 14))) lineto(x+14,y);
	if (x <= (MAX_X - 14)) x +=14;
}

void rev_pixel(void)
{
	int color;

	color=getpixel(x,y);
	if (color) putpixel(x,y,BLACK);
	else putpixel(x,y,WHITE);
}

void ins_pressed(void)
{
	ins = !ins;
	setviewport(640,312,679,325,1);
	clearviewport();
	if (ins) outtextxy(5,3,"Insert");
	setviewport(40,6,679,305,1);
}

void indicator(void)
{
	char a[3];

	setviewport(135,312,160,325,1);
	clearviewport();
	itoa(x+1,a,10);
	outtextxy(1,3,a);
	setviewport(285,312,310,325,1);
	clearviewport();
	itoa(y+1,a,10);
	outtextxy(1,3,a);
	setviewport(40,6,679,305,1);
}

void draw_aria(int mode)
{
	void *ptr;
	int tx1,tx2,ty1,ty2,tx,ty,tr,temp,ch;

	temp=ins;ins=0;
    setviewport(0,0,getmaxx(),getmaxy(),1);
	ptr=(void *)malloc(imagesize(40,330,679,345));
	getimage(40,330,679,345,ptr);
	setviewport(40,330,679,345,1);
	clearviewport();
	if (!mode) outtextxy(50,4,"Go to any end of the diagonal,then press <Enter>");
	else outtextxy(50,4,"Go to any end of the diameter,then press <Enter> ");
    setviewport(40,6,679,305,1);
	do {
		ch = arrows();
	}while(ch !=13);
	tx1=x;ty1=y;
    setviewport(40,330,679,345,1);
	clearviewport();
	if (!mode) outtextxy(50,4,"Go to opposite end of the diagonal,then press <Enter>");
	else outtextxy(50,4,"Go to opposite end of the diameter,then press <Enter> ");
    setviewport(40,6,679,305,1);
	do {
		ch = arrows();
	} while (ch !=13);
	tx2=x;ty2=y;
	if (!mode) rectangle(tx1,ty1,tx2,ty2);
	else if (mode==2) ellipse((tx1+tx2)/2,ty1,0,360,abs(tx2-(tx1+tx2)/2),abs(tx2-(tx1+tx2)/2)/3);
	else {
		tx=(tx1+tx2)/2;ty=(ty1+ty2)/2;
		tr=(int) sqrt((tx1-x)*(tx1-x)+(ty1-y)*(ty1-y));
		if (mode==3) {
			arc(tx,ty,0,180,tr);
			x = tx; y = ty;
			setwritemode(XOR_PUT);
			do {
				ch = arrows();
                arc(tx,ty,0,180,tr);
			} while (ch !=13);
		}
		else circle(tx,ty,tr);
	}
	setviewport(0,0,getmaxx(),getmaxy(),1);
	putimage(40,330,ptr,COPY_PUT);
	setviewport(40,6,679,305,1);
	free(ptr);
	ins=temp;
}

void cursor(void)
{
	char temp;

	moveto(x,y);
	temp=getpixel(x,y);
	while( ! kbhit()) {
		putpixel(x,y,WHITE);delay(200);
		putpixel(x,y,BLACK);delay(100);
	}
	putpixel(x,y,temp);
}

int arrows(void)
{
	char ch;

	indicator();
	cursor();
	ch=getch();
	if (!ch) ch=getch();
	switch (ch) {
		case 75 :left_arrow();break;
		case 77 :right_arrow();break;
		case 72 :up_arrow();break;
		case 80 :down_arrow();break;
	}
	return ch;
}

