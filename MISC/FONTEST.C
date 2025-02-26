#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>

void main(void)
{
	int driver,mode,i;
	void *array[256];
	FILE *fp;

	fp = fopen("MASUD.FNT","rb");
	for (i=0;i<256;i++) {
		array[i] = malloc(40);
		fread(array[i],34,1,fp);
	}
	fclose(fp);
	driver = DETECT;
	initgraph(&driver,&mode,"");
	for (i=33;i<=47;i++)
		putimage(i*15-300,100,array[i],COPY_PUT);
	for (i=48;i<=64;i++)
		putimage(i*15-600,150,array[i],COPY_PUT);
	for (i=65;i<=90;i++)
		putimage(i*15-900,200,array[i],COPY_PUT);
	for (i=91;i<=126;i++)
		putimage(i*15-1200,250,array[i],COPY_PUT);
	getch();
	closegraph();
}