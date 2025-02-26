/* Popup menu	*/

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

int popup(int,int,int,int,char *element[]);
void uparrow(int,int *,int,int *,int,char *element[]);
void dnarrow(int,int *,int,int *,int,char *element[]);
void reversevideo();
void normalvideo();
int getcursor();
void setcursor();

int popup(int x,int y,int i,int max,char *element[])
{
	int tempy,fill;
	char ch;

	reversevideo();
	gotoxy(x-1,y);cprintf(" %s ",element[0]);
	fill=max-strlen(element[0]);
	while (fill--) cprintf(" ");
	normalvideo();
	for (tempy=y+1;tempy<y+i;tempy++) {
		gotoxy(x,tempy);
		printf("%s",element[tempy-y]);
	}
	tempy=1;
	gotoxy(x,y);
	do {
		ch=getch();
		if (ch==000) ch=getch();
		if (ch==72) uparrow(x,&y,i,&tempy,max,element);
		else if (ch==80) dnarrow(x,&y,i,&tempy,max,element);
	} while (ch !=13);
	return tempy;
}

void uparrow(int x,int *y,int i,int *tempy,int max,char *element[])
{
	int fill;

	gotoxy(x-1,*y);
	cprintf(" %s ",element[*tempy-1]);
    fill=max-strlen(element[*tempy-1]);
	while (fill--) cprintf(" ");
	if (*tempy==1) {*tempy=i;*y=*y+i-1;}
	else {(*tempy)--;(*y)--;}
	gotoxy(x-1,*y);
	reversevideo();
	cprintf(" %s ",element[*tempy-1]);
	fill=max-strlen(element[*tempy-1]);
	while (fill--) cprintf(" ");
	normalvideo();
}

void dnarrow(int x,int *y,int i,int *tempy,int max,char *element[])
{
	int fill;

	gotoxy(x-1,*y);
	cprintf(" %s ",element[*tempy-1]);
    fill=max-strlen(element[*tempy-1]);
	while (fill--) cprintf(" ");
	if (*tempy==i) {*tempy=1;*y=*y-i+1;}
	else {(*tempy)++;(*y)++;}
	gotoxy(x-1,*y);
	reversevideo();
	cprintf(" %s ",element[*tempy-1]);
	fill=max-strlen(element[*tempy-1]);
	while (fill--) cprintf(" ");
	normalvideo();
}

void reversevideo()
{
	textcolor(0);
	textbackground(15);
}

void normalvideo()
{
	textcolor(15);
	textbackground(0);
	lowvideo();
}

unsigned int getcursor(void)
{
	union REGS reg;

	reg.h.ah=3;
	reg.h.bh=0;
	int86(0x10,&reg,&reg);
	return (reg.x.cx);
}

void setcursor(unsigned int shape)
{
	union REGS reg;

	reg.h.ah=1;
	reg.x.cx=shape;
	int86(0x10,&reg,&reg);
}