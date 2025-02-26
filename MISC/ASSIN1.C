#include <stdio.h>
#include <graphics.h>

#define XSIZE 30
#define YSIZE 45

void draw_pic(int x,int y);
long int power(int);

long int array[]={0,0,1056964608,-1,262143,79691776,-1,262144,874512384,-1,393215,
	337641472,-1,344064,872939520,-1,360447,872873984,-1,360447,872472576,-1,360447,
	872423424,-1,360447,872423424,-1,360447,872423424,-1,360447,872423424,-1,360447,
	872423424,-1,360447,872423424,-1,360447,604010480,-1,327679,67207384,-1,262144,

	1,-1,536870912, 1,-1,536870912, 1,-1,536870912, 1,-1,536870912, 1,-1,536870912,
	1,-1,536870912, 1,-1,536870912, 1,-1,536870912, 1,-1,536870912, 1,-1,536870912,
	1,-1,536870912, 1073741823,-1,1073741823, 0,0,0,0,0,0,0,0};

main()
{
	int driver,mode;

	driver=DETECT;
	initgraph(&driver,&mode,"");
	draw_pic(250,100);
	getch();
	closegraph();
}

void draw_pic(int x,int y)
{
	int loop,pointer,tx,ty,i;
	long int sum;

	ty=y;
	for (loop=0,pointer=0;loop < YSIZE;loop++) {
		tx=x;
		do {
			if (array[pointer]==-1) pointer++;
			sum=array[pointer];
			for(i=XSIZE-1;i >= 0;i--)
				if (power(i) <=sum) {
					putpixel(tx+i,ty,WHITE);
					sum-=power(i);
				}
			tx+=XSIZE;pointer++;
		} while (array[pointer]==-1);
		ty++;
	}
}

long int power(int i)
{
	long int answer=1;

	while (i--)	answer *=2;
	return answer;
}