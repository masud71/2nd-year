#include<stdio.h>
#include<stdlib.h>
#include<c:\turboc\200\g_menu.c>
#include<math.h>
#include<alloc.h>
#include<graphics.h>

#define MAX_X 639
#define MAX_Y 299
#define LNTH   12
#define UP_ARROW 83
#define DOWN_ARROW 81



void triangle(int,int,int,int);
void rect_angle(int,int,int,int);
void select_y_axis();
void process();
void clear();
void Save();
void corsor(int,int);
int select_x_axis();
int new_x(int,int,int,int);

extern int set_graph_area(),store_image();
extern int gline, row, col,maxx, maxy, currline,lx,rx, ty, by ;

void *scrn,*image,*primary,*cor;
int position,xx1,yy1,xx2,yy2,sv,pos ;

static struct y_position
   {
	int y_up;
	int y_dn;
   }info;

										/*extern int set_graph_area(),store_image();*/
int size,i;


/**************************************************************************/
rect()
{
 char c,choice;
 char *pointer[]={"   X - AXIS   ","   Y - AXIS   ","    CURVES    ","     SAVE     ","     CLEAR    "};

 info.y_up=1;
 info.y_dn=145;
 position=10;

 setviewport(0,0,719,347,1);
 scrn=(void*)malloc(imagesize(100,39,666,200));
 getimage(101,40,665,199,scrn);
 if(!set_graph_area())return;
 setwritemode(COPY_PUT);
 setviewport(101,40,665,199,1);
 clearviewport();
 setviewport(102,52,664,198,1);
 cor=(void*)malloc(imagesize(0,0,8,8));
 line(21,5,25,5);
 line(23,3,23,7);
 getimage(21,3,25,7,cor);
 clearviewport();
 line(0,73,561,73);
 line(10,0,10,145);
 image=(void*)malloc(imagesize(101,51,666,200));
 getimage(0,0,560,145,image);
 primary=(void*)malloc(imagesize(0,0,6,6));
 setviewport(0,0,719,347,0);
 rectangle(101,51,665,199);
 do {
 	setviewport(0,0,719,347,0);
	choice = graph_menu(101,40,pointer,5);
	setviewport(102,52,664,198,1);
	switch(choice)
	{
		case 1:position=select_x_axis();break;
		case 2:select_y_axis();break;
		case 3:process();break;
		case 4:Save();break;
		case 5:clear();break;
	}
 }while (choice !=4);
}


/*******************************************************************************/

void Save()
{char c;
 static int x1=231,x2=331,x3=331,x4=231,y1=43,y2=43,y3=103,y4=103;
 int flag;

 xx1=x1;yy1=y1;
 setviewport(102,52,664,198,1);
 getimage(xx1-2,yy1-2,xx1+2,yy1+2,primary);
 rectangle(x1,y1,x3,y3);
 do
 {
	while(kbhit())
	{
		c=getch();
		switch(c)
		{
			case 54:if(x2<11)break;
				x2--;x3--;flag=1;
				corsor(x2,(y2+y3)/2);break;
			case 50:if(y4<3)break;
				y4--;y3--;flag=1;
				corsor((x4+x3)/2,y3);break;
			case 52:if(x1>560)break;
				x1++;x4++;flag=1;
				corsor(x1,(y1+y4)/2);break;
			case 56:if(y1>144)break;
				y1++;y2++;flag=1;
				corsor((x1+x2)/2,y1);break;
			case 57:if(x2<11 || y2>144)break;
				x2--;x3--;y1++;y2++;
				corsor(x2,y2);flag=1;break;
			case 51:if(x3<11 || y3<3)break;
				x2--;x3--;y3--;y4--;
				corsor(x3,y3);flag=1;break;
			case 49:if(x4>560 || y4<3)break;
				x1++;x4++;y3--;y4--;
				corsor(x4,y4);flag=1;break;
			case 55:if(x1>560 || y1>144)break;
				x1++;x4++;y1++;y2++;
				corsor(x1,y1);flag=1;break;
		};
		if(c==000)
		{
			switch(c=getch())
			{
				case 77:if(x2>560)break;
					x2++;x3++;flag=1;
					corsor(x2,(y2+y3)/2);break;
				case 80:if(y4>144)break;
					y4++;y3++;flag=1;
					corsor((x4+x3)/2,y3);break;
				case 75:if(x1<11)break;
					x1--;x4--;flag=1;
					corsor(x1,(y1+y4)/2);break;
				case 72:if(y1<3)break;
					y1--;y2--;flag=1;
					corsor((x1+x2)/2,y1);break;
				case 73:if(x2>560 || y2<3)break;
					x2++;x3++;y1--;y2--;
					corsor(x2,y2);flag=1;break;
				case 81:if(x3>560 ||y3>144)break;
					x2++;x3++;y3++;y4++;
					corsor(x3,y3);flag=1;break;
				case 79:if(x4<11 || y4>144)break;
					x1--;x4--;y3++;y4++;
					corsor(x4,y4);flag=1;break;
				case 71:if(x1<11 || y1<3)break;
					x1--;x4--;y1--;y2--;
					corsor(x1,y1);flag=1;break;
			};
		};
	delay(90);
	};

	if(flag)
	{
		flag=0;
		putimage(0,0,image,COPY_PUT);
		rectangle(x1,y1,x3,y3);
		xx1=x1+102;xx2=x3+102;
		yy1=y1+52;yy2=y3+52;
	};

}while(c!=13);
getimage(x1+1,y1+1,x3-1,y3-1,image);
setviewport(0,0,719,347,1);
setwritemode(XOR_PUT);
putimage(101,40,scrn,COPY_PUT);
putimage(lx,ty,image,COPY_PUT);
free(primary);free(cor);free(image);
free(scrn);
store_image();

}

/****************************************************************************/
void corsor(int x,int y)
{
 setviewport(100+xx1,50+yy1,104+xx1,54+yy1,1);
 putimage(0,0,primary,COPY_PUT);
 setviewport(102,52,664,198,1);
 getimage(x-2,y-2,x+2,y+2,primary);
 setviewport(100+x,50+y,104+x,54+y,1);
 putimage(0,0,cor,COPY_PUT);
 xx1=x;yy1=y;
 setviewport(102,52,664,198,1);
}
/***************************************************************************/
void clear()
{
  xx1=0;xx2=0;yy1=0;yy2=0;
  setviewport(102,52,664,198,1);
  clearviewport();
  line(0,73,565,73);
  line(10,0,10,198);
  getimage(0,0,554,146,image);
  sv=0;
}
/**************************************************************************/
void process()
{int mode,cycle,length,hight;
 char c;

 setviewport(102,52,664,198,1);
 getimage(0,0,564,146,image);
 mode=1;
 cycle=1;
 for(;;)
 {
	length=50;
	hight=30;
	do{
		if(cycle<=4)
		triangle(length,hight,mode,position);else
		rect_angle(length,hight,mode,position);
		c=getch();



		if(c==000)
		{
			switch(c=getch())
			{
				case 77:length++;break;
				case 75:length--;break;
				case 80:hight--;break;
				case 72:hight++;break;
			  };
			  putimage(0,0,image,COPY_PUT);
		};

		if(c==27)
		{
                        putimage(0,0,image,COPY_PUT);
			if(cycle==7)
			{
				cycle=1;
				mode=1;
			}
			else
			{
				cycle++;
				mode++;
			};
		};
  }while(c!=13);
  getimage(0,0,564,146,image);
  c = getch();
  if(c==10){putimage(0,0,image,COPY_PUT);return;};
  mode=1;
  cycle=1;
 };
}
/***************************************************************************/
void select_y_axis()
{
int p_up;
void *pointer;
char c;

 sv=0;
 p_up=1;
 setviewport(102,52,111,198,1);
 pointer=(void*)malloc(imagesize(0,0,11,150));
 setcolor(BLACK);
 line(0,info.y_up,9,info.y_up);
 line(0,info.y_dn,9,info.y_dn);
 getimage(0,0,10,148,pointer);
 setcolor(WHITE);
 do
 {
	line(0,info.y_up,9,info.y_up);
	line(0,info.y_dn,9,info.y_dn);
	c=getch();
	if(c==000)
	{
		switch(c=getch())
		{
			case 72:if(p_up && info.y_up!=1)info.y_up--;
				else if(!p_up && info.y_dn!=73) info.y_dn--;
				else printf("\a");
				break;
			case 80:if(p_up && info.y_up!=73) info.y_up++;
				else if(!p_up && info.y_dn!=146) info.y_dn++;
				else printf("\a");
				break;
			case 73:p_up=1;break;
			case 81:p_up=0;break;
		  };
		  putimage(0,0,pointer,COPY_PUT);
	   };
 }while(c!=13);
 free(pointer);
 setviewport(102,52,664,198,1);
}
/***************************************************************************/
int select_x_axis()
{
 int x_pos;
 char c;
 void *ptr;

 sv=0;
 x_pos=0;
 ptr=(void*)malloc(imagesize(0,0,550,9));
 setviewport(112,121,664,128,1);
 getimage(0,0,530,8,ptr);
 do
 {
	 line(x_pos,1,x_pos,7);
	 c=getch();
	 if(c==000)
	 {
		switch(c=getch())
		{
			case 77:x_pos++;break;
			case 75:if(x_pos==0)break;
				x_pos--;break;
		};
		putimage(0,0,ptr,COPY_PUT);
	 };
 }while(c!=13);
 free(ptr);
 putimage(0,0,ptr,COPY_PUT);
 setviewport(102,52,664,198,1);
 return x_pos+10;
}
/***************************************************************************/
void triangle(int length,int hight,int mode,int position)
{int y,x1,b,x;

 x1=position;
 setviewport(102,52+info.y_up,664,51+info.y_dn,1);
 y=73-info.y_up;
 b=0;
 do
 {
	if(mode==4)
	{
		b=length;
		goto b;
	};
	if(info.y_up>73-hight)
	{
		x=new_x(x1,x1+length,y,hight-y);
		line(x,0,x1+length,0);
	};
	line(x1,y,x1+length,y-hight);
	line(x1+length,y,x1+length,y-hight);
	if(mode<=2)goto a;
	b:if(info.y_dn-73<hight)
	{
		x=new_x(x1+length-b,x1+2*length-b,info.y_dn-73,hight-info.y_dn+73);
		line(x,info.y_dn-info.y_up-1,x1+2*length-b,info.y_dn-info.y_up-1);
	};
	line(x1+length-b,y,x1+2*length-b,y+hight);
	line(x1+2*length-b,y+hight,x1+2*length-b,y);
	x1=x1+2*length;
	a:if(!(mode>=3))x1=x1+mode*length;
 }while(x1<600);
setviewport(102,52,664,198,1);
}
/***************************************************************************/
void rect_angle(int length,int hight,int mode,int position)
{int x1;
 line(0,73,545,73);
 x1=position;
 do
 {
	if(mode==7)
	{
	  line(x1,73,x1,73+hight);
	  line(x1,73+hight,x1+length,73+hight);
	  line(x1+length,73+hight,x1+length,73);
	  goto a;
	};
	line(x1,73,x1,73-hight);
	line(x1,73-hight,x1+length,73-hight);
	line(x1+length,73-hight,x1+length,73);
	if(mode==6)goto a;
	line(x1+length,73,x1+length,73+hight);
	line(x1+length,73+hight,x1+2*length,73+hight);
	line(x1+2*length,73+hight,x1+2*length,73);
	a:x1=x1+2*length;
 }while(x1<700);
}
/***************************************************************************/
int new_x(int x1,int x2,int m1,int m2)
{float numerator,denomenator;

 numerator=(float)(x1*m2 + x2*m1);
 denomenator=(float)m1+m2;
 numerator=numerator/denomenator;
 return (int)numerator;
}