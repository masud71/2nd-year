/* Graphics menu	*/

#include <stdio.h>
#include <string.h>
#include <graphics.h>
#include <stdlib.h>

int popup(int,int,int,int,char *element[]);
void uparrow(int,int *,int,int *,int,char *element[]);
void dnarrow(int,int *,int,int *,int,char *element[]);
void hilight(int,int,int,int);

main()
{
	int i=-1,x,y,max=0,length;
	char *element[15];

	clrscr();
	printf("Enter menu elements one after another & type END to stop : \n");
	do {
		element[++i]=(char *)malloc(40*sizeof(char));
		gets(element[i]);
		max=(textwidth(element[i]) > max) ? textwidth(element[i]) : max;
	}while (stricmp(element[i],"END"));
	printf("Enter starting location :(X & Y) ");
	scanf("%d%d",&x,&y);
	clrscr();
	popup(x,y,i,max,element);
}

int popup(int x,int y,int i,int max,char *element[])
{
	int tempy,driver,mode;
	char ch;

	driver=DETECT;
	initgraph(&driver,&mode,"");
	for (tempy=y;tempy<y+i;tempy++)
		outtextxy(x,y+12*(tempy-y),element[tempy-y]);
	hilight(x,y,max,1);
	outtextxy(x,y,element[0]);
	tempy=1;
	do {
		ch=getch();
		if (ch==000) ch=getch();
		if (ch==72) uparrow(x,&y,i,&tempy,max,element);
		else if (ch==80) dnarrow(x,&y,i,&tempy,max,element);
	} while (ch !=13);
	closegraph();
	return tempy;
}

void uparrow(int x,int *y,int i,int *tempy,int max,char *element[])
{
	hilight(x,*y,max,0);
	outtextxy(x,*y,element[*tempy-1]);
	if (*tempy==1) {*tempy=i;*y=*y+(i-1)*12;}
	else {(*tempy)--;(*y)-=12;}
	hilight(x,*y,max,1);
	outtextxy(x,*y,element[*tempy-1]);
}

void dnarrow(int x,int *y,int i,int *tempy,int max,char *element[])
{

	hilight(x,*y,max,0);
	outtextxy(x,*y,element[*tempy-1]);
	if (*tempy==i) {*tempy=1;*y=*y-(i-1)*12;}
	else {(*tempy)++;(*y)+=12;}
	hilight(x,*y,max,1);
	outtextxy(x,*y,element[*tempy-1]);
}

void hilight(int x,int y,int max,int mode)
{
	if (mode) {
		setfillstyle(SOLID_FILL,WHITE);
		setcolor(BLACK);
	}
	else {
		setfillstyle(EMPTY_FILL,WHITE);
		setcolor(WHITE);
	}
	bar(x-8,y-3,x+max+8,y+9);
}