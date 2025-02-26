/* 	Graphic Print Program	*/
#include <stdio.h>
#include <graphics.h>
#include <bios.h>

void resetprinter(void);
void initline(unsigned int);
void print(void);

main()
{
	int driver,mode,i;

	driver=DETECT;
	initgraph(&driver,&mode,"");
	setfillstyle(LTSLASH_FILL,WHITE); /*   Insert the program to draw    */
	circle(340,120,100);         /*   the image which to be printed */
	print();
	closegraph();
}


void resetprinter(void)        /*  Initialize to text mode again   */
{
	biosprint(0,27,0);
	biosprint(0,0x40,0);
}

void initline(unsigned int gcol)	/*  Initialize each line to graphic mode */
{
	biosprint(0,27,0);
	biosprint(0,0x4C,0);          /*  Selecting Double-density mode  */
	biosprint(0,gcol % 256,0);    /*  Total byte in each column =    */
	biosprint(0,gcol / 256,0);    /*         n1 + (n2 * 256)         */
}

void print(void)
{
	char mask[8]={0x80,0x40,0x20,0x10,0x8,0x4,0x2,0x1};
	int hlim,vlim,vext,klim,i,j,k;
	char pins;

	biosprint(1,0,0);
	biosprint(0,27,0);             /*    Selecting line spacing   */
	biosprint(0,0x31,0);           /*         7 / 72  inch        */
	hlim=getmaxx();
	vlim=getmaxy()/8;
	vext=getmaxy()%8;
	klim=7;
	for (i=0;i<=vlim;i++) {
		initline(hlim*2+2);
		for (j=0; j<=hlim;j++) {
			if (i==vlim) klim=vext;
			pins=0;
			for (k=0;k<=klim;k++)
				if (getpixel(j,i*8+k)) pins+=mask[k];
			biosprint(0,pins,0);
		}
	}
	resetprinter();
}
