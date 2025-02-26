#include <stdio.h>
#include <ctype.h>
#include <graphics.h>

enum status {BLK,WHT};
void *cursor,*filler;
int size;
void *font[256];              /* Total 256 ASCII */
enum status figure[9][14];    /* Each Character Bit Map is 9 x 14 */

void init(void);
void drawsheet(void);
char picture(void);
void set_cursor(void);
void fill(void);
void clear(void);
void store(void);
void load(void);

main()
{
	FILE *fp;
	char ch;
	int i;

    init();
	size = imagesize(51,95,59,108);
	fp=fopen("Masud.fnt","rb");
	if (fp)
		for (i=0;i<256;i++) {
			font[i] =(void *)malloc(size);
			fread(font[i],size,1,fp);
		}
	fclose(fp);
	do {
		drawsheet();
		ch=picture();
	} while(ch != 26);
	closegraph();
	for (i=0;i<256;i++) free(font[i]);
	fclose(fp);
}

void init(void)
{
	int driver,mode;
	register int i;

	driver = DETECT;
	registerbgidriver(Herc_driver);
	registerbgifont(tscr_font);
	registerbgifont(triplex_font);
	initgraph(&driver,&mode,"");
	for (i=0;i<256;i++) font[i] = NULL;
}

void drawsheet(void)
{
	register int i;

	cleardevice();
	for (i=0;i<=9;i++) line(400+15*i,150,400+15*i,290);
	for (i=0;i<=14;i++) line(400,150+10*i,535,150+10*i);
	for (i=0;i<=16;i++) line(0,0+21*i,240,0+21*i);
	for (i=0;i<=16;i++) line(0+15*i,0,0+15*i,336);
	outtextxy(100,340,"F2 - Store  F3 - Load   F4 - Clear   F5 - File   ^Z - Exit");
	settextstyle(1,0,4);
	outtextxy(380,20,"FontoGrapher");
	settextstyle(7,0,1);
	outtextxy(420,60,"Version 1.0");
	for (i=0;i<256;i++)
		putimage(15*(i / 16)+3,21*(i % 16)+1,font[i],COPY_PUT);
}

char picture(void)
{
	int x,y;
	char ch;
	FILE *fp;
	register int i;

	for (y=0;y<14;y++)
		for (x=0;x<9;x++)
			figure[x][y]=BLK;
	x=y=0;
	set_cursor();fill();
	do {
		putimage(400+x*15+6,150+y*10+3,cursor,XOR_PUT);
		ch=getch();
		if (!ch) ch=getch();
		putimage(400+x*15+6,150+y*10+3,cursor,XOR_PUT);
		switch (ch) {
			case 60 :store();break;
			case 61 :load();break;
			case 62 :clear();break;
			case 63 :fp=fopen("Masud.fnt","wb");
					for (i=0;i<256;i++) fwrite(font[i],size,1,fp);
					fclose(fp);break;
			case 72 :y--;if (y < 0) y = 13;break;
			case 80 :y++;if (y > 13) y = 0;break;
			case 75 :x--;if (x < 0) x = 8;break;
			case 77 :x++;if (x > 8) x = 0;break;
			case 13 :figure[x][y] = !figure[x][y];
					 putimage(400+x*15+1,150+y*10+1,filler,XOR_PUT);
					 if (figure[x][y]) putpixel(x+300,y+100,WHITE);
					 else putpixel(x+300,y+100,BLACK);
					 break;
		}
	}while (ch !=26);
	return ch;
}

void set_cursor(void)
{
	bar(500,320,503,323);
	cursor=(void *)malloc(imagesize(500,320,503,323));
	getimage(500,320,503,323,cursor);
	putimage(500,320,cursor,XOR_PUT);
}

void fill(void)
{
	bar(501,321,514,329);
	filler=(void *)malloc(imagesize(501,321,514,329));
	getimage(501,321,514,329,filler);
	putimage(501,321,filler,XOR_PUT);
}

void store(void)
{
	void *img;
	int number,size;

	number = getcode();
	setfillstyle(BLACK,SOLID_FILL);
	bar(400,125,560,140);
	setfillstyle(WHITE,SOLID_FILL);
	if (number == 27) return;
	size=imagesize(300,100,309,119);
	img=(void *)malloc(size);
	getimage(300,100,308,113,img);
	putimage(15*(number / 16)+3,21*(number % 16)+1,img,COPY_PUT);
	font[number]=img;
}

void load(void)
{
	int number,size;
	register int i,j;

	number = getcode();
	setfillstyle(BLACK,SOLID_FILL);
	bar(400,125,560,140);
	setfillstyle(WHITE,SOLID_FILL);
	if (number == 27) return;
    clear();
	putimage(300,100,font[number],COPY_PUT);
	for (i=0;i<9;i++)
		for (j=0;j<14;j++)
			if (getpixel(300+i,100+j)) {
				figure[i][j] = WHT;
				putimage(400+i*15+1,150+j*10+1,filler,XOR_PUT);
			}
}

void clear(void)
{
	int i,y;

	for (y=0;y<14;y++)
		for (i=0;i<9;i++)
			figure[i][y]=BLK;
	i=y=0;
	setfillstyle(BLACK,SOLID_FILL);
	bar(300,100,380,140);
	bar(400,150,600,320);
	setfillstyle(WHITE,SOLID_FILL);
	for (i=0;i<=9;i++) line(400+15*i,150,400+15*i,290);
	for (i=0;i<=14;i++) line(400,150+10*i,535,150+10*i);
}

int getcode()
{
	char line[5],ch,disp[2];
	int number,i=0;

	rectangle(400,125,560,140);
	settextstyle(0,0,1);
	outtextxy(405,128,"Code Number :");
	disp[1] = 0;
	number = textwidth("A");
	do {
		ch=getch();
		if (isdigit(ch)) {
			line[i++] = ch;
			disp[0] = ch;
			outtextxy(520+(i-1)*number,128,disp);
			}
		if (ch == 8 && i) {
			i--;
            setfillstyle(BLACK,SOLID_FILL);
			bar(520+i*number,128,520+(i+1)*number,138);
			setfillstyle(WHITE,SOLID_FILL);
		}
	} while (ch !=27 && ch !=13);
	if (ch == 27) return 27;
	line[i] = 0;
	number = atoi(line);
	return number;
}