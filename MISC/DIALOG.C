#include <stdio.h>
#include <graphics.h>
#include <alloc.h>

int Color[9];
	
int dbox(int,int,int,int,char *,char);

void main(void)
{
	int dr,mo;

/*	Color[0] = EGA_YELLOW;
	Color[1] = EGA_LIGHTBLUE;
	Color[2] = EGA_CYAN;
	Color[3] = EGA_MAGENTA;
	Color[4] = EGA_DARKGRAY;
	Color[5] = EGA_LIGHTGREEN;
	Color[6] = EGA_LIGHTGRAY;
	Color[7] = EGA_WHITE; */

	Color[0] = Color[3] = Color[4] = BLACK;
	Color[1] = Color[2] = Color[5] = Color[6] = Color[7] = WHITE;

	dr = DETECT;
	initgraph(&dr,&mo,"");
	dbox(150,150,450,300,"Test Dialog 1",7);
	getch();
	closegraph();
}

int dbox(int lx,int ty,int rx,int by,char *title,char button)
/*
		Dialog box creator for the DFD Editor.Input parameters are dialog
	box top left & bottom right co ordinates,a string containing the title
	of the dialog box and a character type variable containing codes for
	buttons.The codes are :-
		4 : OK
		2 : Cancel
		1 : Help
		Any combination between 0 to 7 can be used as code. Button Positions
	in Co ordinates are :-
		OK : (rx-10*Ch_wd,ty+2*Ln_ht-1);(rx-2*Ch_wd,ty+3*Ln_ht+1)
		Cancel : (rx-10*Ch_wd,ty+4*Ln_ht-1);(rx-2*Ch_wd,ty+5*Ln_ht+1)
		Help : (rx-10*Ch_wd,ty+6*Ln_ht-1);(rx-2*Ch_wd,ty+7*Ln_ht+1)
		Toolbox :(lx+3,ty+2);(lx+3*Ch_wd+3,ty_Ln_ht+2)

		As the dialog box may overlap any existing design, so the user of
	the routine must save appropriate amount of screen aria before using it.
	He should clear , free & restore the aria after completion of use of the
	dialog box.
*/

{
	char my_pat[8] = {0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA};
	int Ln_ht=12,Ch_wd=9,color;

/*	setfillstyle(SOLID_FILL,Color[2]);
	bar(0,0,639,479);*/
	color = getcolor();
	setcolor(BLACK);
	setfillpattern(my_pat,1);
	bar(lx,ty,rx,by);
    rectangle(lx,ty,rx,by);
	setfillstyle(SOLID_FILL,Color[7]);
	lx+=3;rx-=3;ty+=2;by-=2;
	bar(lx,ty,rx,by);
    rectangle(lx,ty,rx,by);
	setfillstyle(SOLID_FILL,Color[1]);
	bar(lx+3*Ch_wd+1,ty+1,rx-1,ty+Ln_ht);
	setfillstyle(SOLID_FILL,Color[6]);
	bar(lx+1,ty+1,lx+3*Ch_wd,ty+Ln_ht);
	outtextxy(lx/2+3/2*Ch_wd+(rx-textwidth(title))/2,ty+3,title);
	setcolor(BLACK);
	line(lx,ty+Ln_ht,rx,ty+Ln_ht);
	line(lx+3*Ch_wd,ty,lx+3*Ch_wd,ty+Ln_ht);

	line(lx+Ch_wd/2+1,ty+Ln_ht/2-2,lx+5*Ch_wd/2-1,ty+Ln_ht/2-2);
	line(lx+Ch_wd/2+1,ty+Ln_ht/2+1,lx+5*Ch_wd/2-1,ty+Ln_ht/2+1);
	line(lx+Ch_wd/2+2,ty+Ln_ht/2+2,lx+5*Ch_wd/2-1,ty+Ln_ht/2+2);
	line(lx+Ch_wd/2+1,ty+Ln_ht/2-1,lx+Ch_wd/2+1,ty+Ln_ht/2);
	line(lx+5*Ch_wd/2-1,ty+Ln_ht/2-1,lx+5*Ch_wd/2-1,ty+Ln_ht/2);
	line(lx+5*Ch_wd/2,ty+Ln_ht/2-1,lx+5*Ch_wd/2,ty+Ln_ht/2+2);

	lx-=3;rx+=3;ty-=2;by+=2;
	if (button >= 4) {
		button-=4;
		setfillstyle(SOLID_FILL,Color[6]);
        setcolor(BLACK);
		rectangle(rx-10*Ch_wd-1,ty+3*Ln_ht-2,rx-2*Ch_wd+1,ty+4*Ln_ht+2);
		bar(rx-10*Ch_wd,ty+3*Ln_ht-1,rx-2*Ch_wd,ty+4*Ln_ht+1);
		outtextxy(rx-7*Ch_wd,ty+3*Ln_ht+3,"OK");
        setcolor(Color[4]);
		line(rx-10*Ch_wd+1,ty+4*Ln_ht,rx-2*Ch_wd-1,ty+4*Ln_ht);
		line(rx-2*Ch_wd-1,ty+3*Ln_ht,rx-2*Ch_wd-1,ty+4*Ln_ht);
		setcolor(Color[7]);
		line(rx-10*Ch_wd+1,ty+3*Ln_ht,rx-2*Ch_wd-2,ty+3*Ln_ht);
		line(rx-10*Ch_wd+1,ty+3*Ln_ht,rx-10*Ch_wd+1,ty+4*Ln_ht-1);
	}
	if (button >= 2) {
		button-=2;
		setfillstyle(SOLID_FILL,Color[6]);
        setcolor(BLACK);
		rectangle(rx-10*Ch_wd-1,ty+6*Ln_ht-2,rx-2*Ch_wd+1,ty+7*Ln_ht+2);
		bar(rx-10*Ch_wd,ty+6*Ln_ht-1,rx-2*Ch_wd,ty+7*Ln_ht+1);
		outtextxy(rx-9*Ch_wd,ty+6*Ln_ht+3,"Cancel");
        setcolor(Color[4]);
		line(rx-10*Ch_wd+1,ty+7*Ln_ht,rx-2*Ch_wd-1,ty+7*Ln_ht);
		line(rx-2*Ch_wd-1,ty+6*Ln_ht,rx-2*Ch_wd-1,ty+7*Ln_ht);
		setcolor(Color[7]);
		line(rx-10*Ch_wd+1,ty+6*Ln_ht,rx-2*Ch_wd-2,ty+6*Ln_ht);
		line(rx-10*Ch_wd+1,ty+6*Ln_ht,rx-10*Ch_wd+1,ty+7*Ln_ht-1);
	}
	if (button >= 1) {
		setfillstyle(SOLID_FILL,Color[6]);
		setcolor(BLACK);
		rectangle(rx-10*Ch_wd-1,ty+9*Ln_ht-2,rx-2*Ch_wd+1,ty+10*Ln_ht+2);
		bar(rx-10*Ch_wd,ty+9*Ln_ht-1,rx-2*Ch_wd,ty+10*Ln_ht+1);
		outtextxy(rx-8*Ch_wd,ty+9*Ln_ht+3,"Help");
		setcolor(Color[7]);
		line(rx-10*Ch_wd+1,ty+10*Ln_ht,rx-2*Ch_wd-1,ty+10*Ln_ht);
		line(rx-2*Ch_wd-1,ty+9*Ln_ht,rx-2*Ch_wd-1,ty+10*Ln_ht);
		setcolor(Color[4]);
		line(rx-10*Ch_wd+1,ty+9*Ln_ht,rx-2*Ch_wd-2,ty+9*Ln_ht);
		line(rx-10*Ch_wd+1,ty+9*Ln_ht,rx-10*Ch_wd+1,ty+10*Ln_ht-1);
	}
	setcolor(color);
}