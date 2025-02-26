#include <stdio.h>
#include <conio.h>

#define HLINE 205
#define V_LINE 186
#define LTC 201
#define LBC 200
#define RTC 187
#define RBC 188

void box(int,int,int,int);
void reversevideo(int);
void firstscr();
main()
{
	int i;

	clrscr();
	firstscr();
	i=choice();
	getch();
}

void box(int  X1,int Y1,int X2,int Y2)
{
	int i;

	gotoxy(X1,Y1);
	putch(LTC);
	for(i=1;i<=(X2-X1-1);i++) {
		gotoxy(X1+i,Y1);
		putch(HLINE);
        gotoxy(X1+i,Y2);
		putch(HLINE);
	}
	gotoxy(X2,Y1);
	putch(RTC);
	for(i=0;i<=(Y2-Y1-1);i++){
		gotoxy(X2,Y1+1+i);
		putch(V_LINE);
		  gotoxy(X1,Y1+1+i);
		putch(V_LINE);
	}
	gotoxy(X2,Y2);
	putch(RBC);
	gotoxy(X1,Y2);
	putch(LBC);
}

void reversevideo(int mode)
{
	if (mode) {
		textcolor(0);
		textbackground(15);
	}
	else {
		textcolor(15);
		textbackground(0);
		lowvideo();
	}
}

void firstscr()
{
	int i;

	reversevideo(1);
	box(21,8,60,17);
	gotoxy(22,9);
	cprintf("         MS-DOS VERSION 5.0           ");
	gotoxy(21,10);cprintf("Ç");
	for(i=1;i<=38;i++) cprintf("Ä");cprintf("¶");
	gotoxy(22,11);cprintf("                                      ");
	gotoxy(22,12);cprintf("   Use arrow keys to select option    ");
	gotoxy(22,13);cprintf("          Then press <Enter>          ");
	gotoxy(22,14);cprintf("                                      ");
	gotoxy(22,15);cprintf("  ");
	reversevideo(0);cprintf(" DOS    ");reversevideo(1);
	cprintf("     PATH         RECORD    ");
	gotoxy(22,16);cprintf("                                      ");
	reversevideo(0);
}

int choice()
{
	int i=0,x[3]={24,36,49};
	char ch,*name[3]={" DOS    "," PATH   "," RECORD "};

	do {
		ch=getch();
		if (ch==000) ch=getch();
		switch (ch) {
			case 75:gotoxy(x[i],15);reversevideo(1);
					cprintf("%s",name[i]);
					if (i==0) i=2;
					else i--;
					gotoxy(x[i],15);reversevideo(0);
					cprintf("%s",name[i]);
					break;
			case 77:gotoxy(x[i],15);reversevideo(1);
					cprintf("%s",name[i]);
					if (i==2) i=0;
					else i++;
					gotoxy(x[i],15);reversevideo(0);
					cprintf("%s",name[i]);
					break;
		}
	}while (ch !=13);
	return i;
}