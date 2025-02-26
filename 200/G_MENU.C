#include<graphics.h>
#include<stdio.h>
#include<alloc.h>
int graph_menu(int x,int y,char *pr[],int number)
{int i,size,x1;
 char c;
 void *small,*memory;

 size=strlen(pr[1]);
 small=(void*)malloc(imagesize(0,0,10*size,12));
 memory=(void*)malloc(imagesize(0,0,number*size*10,12));
 setviewport(0,0,719,347,1);

 for(i=0;i<number;i++)outtextxy(i*size*8+x,y,pr[i]);
 getimage(x,y-2,x+number*8*size,9+y,memory);
 setviewport(x,y-2,x+8*size,y+9,1);
 getimage(0,0,8*size,10,small);
 putimage(0,0,small,NOT_PUT);
 i=1;
 x1=x;
 do
 {
	c=getch();
	if(c==000)
	{
		switch(c=getch())
		{
			case 75:if(x1==x)break;
				i--;
				x1=x1-8*size;
				setviewport(x,y-2,x+number*8*size,9+y,1);
				putimage(0,0,memory,COPY_PUT);
				setviewport(x1,y-2,x1+8*size,y+9,1);
				getimage(0,0,8*size,10,small);
				putimage(0,0,small,NOT_PUT);
				break;
			case 77:if(x1==x+(number-1)*8*size)break;
				i++;
				setviewport(x,y-2,x+number*8*size,9+y,1);
				putimage(0,0,memory,COPY_PUT);
				x1=x1+8*size;
				setviewport(x1,y-2,x1+8*size,y+9,1);
				getimage(0,0,8*size,10,small);
				putimage(0,0,small,NOT_PUT);
				break;
		 };
   };
}while(c!=13);
setviewport(x,y-2,x+number*8*size,9+y,1);
putimage(0,0,memory,COPY_PUT);
free(small);
free(memory);
return i;
}