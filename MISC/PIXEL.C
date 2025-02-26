#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>

void main()
{
	int driver,mode,x1,y1,x2,y2;
	unsigned color;
	register int i;
	driver = DETECT;
	initgraph(&driver,&mode,"");
	setwritemode(XOR_PUT);
	do {
		x1 = random(720);
		y1 = random(348);
        x1 = random(720);
		y1 = random(348);
		line(x1,y1,x2,y2);
	}while(!kbhit());
	closegraph();
}