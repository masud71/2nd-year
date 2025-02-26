					 /*NAME :M.D MOSTOFA AKBAR
					   DEPT :CSE
					   ROLL :16
					   YEAR :II
					   COURSE NO.:CSE 200
					   NAME OF THE COURSE:SOFTWARE DEVELOPMENT
					   NAME OF THE FUNCTION :sine_wave   */

/*INCLUDING NECCESSARY HEADER FILE*/

#include<math.h>
#include<graphics.h>
#include<stdio.h>
#include<alloc.h>

/*DEFINING DIFFERENT ASCII CONSTANT*/
#define UP 72   /*UPARROW*/
#define DOWN 80  /*DOWN ARROW*/
#define LEFT 75  /*LEFT ARROW*/
#define RIGHT 77 /*RIGHT ARROW*/
#define ESC 27  /*ESC KEY*/
#define F10 18  /*ASCII OF CRTL-R*/
#define F3 19   /*ASCII OF CRTL-S*/
#define IN 0
#define FIRST 1


/*THIS STRUCTURE DEFINES THE CO-ORDINATES  OF THE SINE-GRAPH*/
typedef struct{
		  int u1,v1,u2,v2;
		}memories;
/*THE STRUCTURE SAVES PARAMETERES OF THE CURVE*/
typedef struct{
		  int len,loops,amplitude,cutting,rectified,full,angle;
		}option;

void scursor(int x,int y,int i);
int select_message(int l);
void shift_cursor(int x,int y);
void sreverse(int x,int y,char i);
option default_option(int length);
memories draw_sine(option option1);
void draw_wave( option option1);
option option_message(option option1);
void print_wave(memories memories1,int x,int y);
void border(void);
void scale(void);

extern void error_msg(char *s);
extern int set_graph_area(void);
extern int store_image(void);
extern void cursor();
extern int row,col,currline,maxline,lx,rx,ty,by,rx,lx;

/*NAME :sine_wave()
  FUNCTION_USED:scale,cursor,select_massage,default_option,draw_sine
  FUNCTION OF THE SUBROUTINE:Master function to draw a sine wave
  ARGUMENTS:topleft & bottomdown coordinates
  RETURNING VARIABLES:intiger
  MACRO USED:No*/

int sine_wave()
{
  int k,k1,k2,k3,k4,length,position1,position2;
  int x1=0,x2=300;
  option option1;
  memories memories1;
  char *bitmap,*bitmap1;

  if (set_graph_area()==0) return 0;
  setwritemode(COPY_PUT);
  setviewport(0,0,getmaxx(),getmaxy(),0);
  length=x2-x1;
  bitmap1=malloc(imagesize(20,80,600,220));
  if (bitmap1==NULL) error_msg("MEMORY IS OVERLOADED");
  getimage(20,80,600,220,bitmap1);
  setviewport(20,80,600,220,0);
  do{
  clearviewport();
  rectangle(0,0,579,139);
  scale();
  if(length==0){
	scursor(30,40,1);
	line(29,0,29,80);
	position1=29;
	position2=select_message(x1);
	if(position2==30 || position2==-1) {
	  memories1.u1=0;
	  setviewport(0,0,getmaxx(),getmaxy(),0);
	  putimage(20,80,bitmap1,COPY_PUT);
      free(bitmap1);
	  setviewport(12,58,getmaxx()-13,getmaxy()-26,0);
	  setwritemode(XOR_PUT);
	  return 0;
	}
	scursor(position2,40,0);
	length=position2-position1;
	if (length==0) {
	   memories1.u1=0;
	   setviewport(0,0,getmaxx(),getmaxy(),0);
	   putimage(20,80,bitmap1,COPY_PUT);
	   free(bitmap1);
	   setviewport(12,58,getmaxx()-13,getmaxy()-26,0);
	   setwritemode(XOR_PUT);
	   return 0;
	}
	else if(length<0){
	  length=length*(-1);
	  position1=position2;
	}
  }
  option1=default_option(length);
  memories1=draw_sine(option1);
  if(memories1.u1==-1) length=0;
  } while(memories1.u1==-1);
  setviewport(0,0,getmaxx(),getmaxy(),0);
  k1=memories1.u1;
  k2=memories1.v1;
  k3=memories1.u2;
  k4=memories1.v2;
  if(k1>0 && k2>0 && k3>0 && k4>0){
	if(((k4-k2)>(by-ty)) || ((k3-k1)>(rx-lx))){
	  error_msg("NOT ENOUGH AREA");
	  putimage(20,80,bitmap1,COPY_PUT);
	  setwritemode(XOR_PUT);
	  free(bitmap1);
	  return 0;
	}
	bitmap=malloc(imagesize(k1,k2,k3,k4));
	if (bitmap==NULL) error_msg("MEMORY IS OVERLOADED");
	getimage(k1,k2,k3,k4,bitmap);
	putimage(20,80,bitmap1,COPY_PUT);
	free(bitmap1);
	putimage(lx,ty,bitmap,OR_PUT);
	free(bitmap);
	store_image();
	}

  setviewport(12,58,getmaxx()-13,getmaxy()-26,0);
  setwritemode(XOR_PUT);
}

/*NAME :sclear()
  FUNCTION_USED:None
  FUNCTION OF THE SUBROUTINE:To clear current shapes from the screen
  ARGUMENTS:x y position & number of parts
  RETURNING VARIABLES:Void
  MACRO USED:No*/
void sclear(int x,int y,int i)
{
  char *bitmap;
  bitmap=malloc(imagesize(x,y-10,x+8*i,y+10));
  if (bitmap==NULL) error_msg("MEMORY IS OVERLOADED");
  getimage(x,y-10,x+8*i,y+10,bitmap);
  putimage(x,y-10,bitmap,XOR_PUT);
  getimage(x,y-10,x+8*i,y+10,bitmap);
  putimage(x,y-10,bitmap,NOT_PUT);
  free(bitmap);
}

/*NAME :cursor()
  FUNCTION_USED:No
  FUNCTION OF THE SUBROUTINE:To create a cursor to draw sine wave
  ARGUMENTS:x y position of the cursor and mode
  RETURNING VARIABLES:Void
  MACRO USED:No*/
void scursor(int x,int y,int i)
{
  int k,j;
  if(getpixel(x,y)==1 && i==1) j=0;
  else if(getpixel(x,y)==1 && i==0) j=0;
  else if(getpixel(x,y)==0 && i==1) j=1;
  else if(getpixel(x,y)==0 && i==0) j=1;
  for(k=0;k<5;k++) {
	putpixel(x+k,y,j);
	putpixel(x+k,y-1,j);
  }
  moveto(x,y);
  x=getx();
  y=gety();
}

/*NAME :select_message()
  FUNCTION_USED:cursor(),shift_cursor().
  FUNCTION OF THE SUBROUTINE:selects a point from the sinwave window.
  ARGUMENTS:length of the window.
  RETURNING VARIABLES:integer
  MACRO USED:No*/
int select_message(int l)
{
  unsigned int keystroke;
  char *bitmap;
  int x1,x,y,y1;
  unsigned char upper,lower;
  int position;

  x1=getx();
  y1=gety();
  bitmap=malloc(imagesize(200,10,400,30));
  if (bitmap==NULL) error_msg("MEMORY IS OVERLOADED");
  getimage(200,10,400,30,bitmap);
  rectangle(200,10,400,30);
  outtextxy(200,20,"SELECT ANOTHER POINT ");
  moveto(x1,y1);
  keystroke=bioskey(0);
  upper=keystroke>>8;
  lower=keystroke;
  while (lower!=13 && lower!=27){
	if(lower==0){
	  if(upper==RIGHT && x1<520-l){
		shift_cursor(x1+5,y1);
		x1=x1+5;
	  }
	  else if(upper==LEFT && x1>30){
		shift_cursor(x1-5,y1);
		x1=x1-5;
	  }
	}
	keystroke=bioskey(0);
	upper=keystroke>>8;
	lower=keystroke;
  }
  if(lower==13){
	position=x1;
  }
  else if(lower==27){
	position=-1;
  }
  putimage(200,10,bitmap,COPY_PUT);
  free(bitmap);
  return position;
}
/*NAME :shift_cursor()
  FUNCTION_USED:No function prepared by me is used.
  FUNCTION OF THE SUBROUTINE:This function erase the cursor
							 at the specified pixel.
  ARGUMENTS: position of the cursor.
  RETURNING VARIABLES: void.
  MACRO USED:No*/
void shift_cursor(int x,int y)
{
  int i1,i;
  i1=getx();
  for(i=0;i<5;i++) putpixel(i1+i,y,0);
  for(i=0;i<5;i++) putpixel(i1+i,y-1,0);
  for(i=0;i<5;i++) putpixel(x+i,y,1);
  for(i=0;i<5;i++) putpixel(x+i,y-1,1);
  moveto(x,y);
}

/*NAME :default_option()
  FUNCTION_USED:No function prepared by me is used.
  FUNCTION OF THE SUBROUTINE:sets the default option of the curve.
  ARGUMENTS:The length of the existing curve in the graphics memory
  RETURNING VARIABLES:option .
  MACRO USED:No macro is used.*/

option default_option(int length)
{
  option option1;

  option1.len=length;
  option1.loops=2;
  option1.amplitude=9;
  option1.cutting=90;
  option1.rectified=0;
  option1.full=0;
  option1.angle=0;
  return option1;
}

/*NAME :draw_sine()
  FUNCTION_USED:draw_wave(),option_message().
  FUNCTION OF THE SUBROUTINE:This function draws wave save the wave
							 saves the wave.
  ARGUMENTS:The default option of waveshape.
  RETURNING VARIABLES:the co-ordinates of the wave.
  MACRO USED:No macro used.*/
memories draw_sine(option option1)
{
  int graphmode,y;
  option option2;
  memories memories1;

  option2=option1;
  do{
	setviewport(51,81,598,200,0);
	clearviewport();
	setviewport(20,80,600,220,0);
	rectangle(0,0,579,139);
	line(29,0,29,80);
    draw_wave(option2);
	outtextxy(32,90,"ESC- ESCAPE");
	outtextxy(350,90,"^S- SAVE THE CURVE ");
	outtextxy(32,110,"^R- CHANGE THE CURVE");
	outtextxy(350,110,"^A- RESELECT THE LENGTH");
	y=getch();
	while(y!=ESC && y!=F10 && y!=F3 && y!=1) y=getch();
	if(y==ESC){
	  memories1.u1=0;
	  memories1.u2=0;
	  memories1.v2=0;
	  memories1.v1=0;
	  return memories1;
	}
	if(y==F10){
	  option2=option_message(option2);
	}

	if(y==F3){
	  float cut;
	  memories1.u1=50;
	  cut=((40.0*option1.amplitude)/10.0)*sin((option1.cutting*3.14)/180.0);
	  memories1.v1=120-cut-1;
	  memories1.u2=memories1.u1+option2.len;
	  if (option2.rectified==0) memories1.v2=120+cut+1;
	  else memories1.v2=120;
	  return memories1;
	}
	if(y==1){
	  memories1.u1=-1;
	  return memories1;
	}
  }while(y!=ESC);
}

/*NAME :draw_wave()
  FUNCTION_USED:No function prepared by me is used.
  FUNCTION OF THE SUBROUTINE:It draws awave shape of the specified
							 option of the wave.
  ARGUMENTS:Option of the waveshape.
  RETURNING VARIABLES:returns void.
  MACRO USED:No macro is used.*/
void draw_wave( option option1)
{
  float cut,diff,angle,y;
  int i,k;

  cut=sin((option1.cutting*3.14)/180.0);
  diff=(360.0*option1.loops*3.14)/(option1.len*180.0);
  angle=(option1.angle*3.14)/180.0;
  for(i=0; i<option1.len;i++){
	y=sin(diff*i+angle);
	if(option1.cutting!=0){
	  if(y>0){
		if(y>cut) y=cut;
	  }
	  else if(y<0){
		if(y<-cut) y=-cut;
	  }
	}
	y=(y*40)*(option1.amplitude/10.0);
	if(option1.rectified==1){
	  if(option1.full==1){
		if(y<0) y=-y;
	  }
	  else if(option1.full==0){
		if(y<0) y=0.0;
	  }
	}
	y=40-y;
	k=(int)y;
	putpixel(i+30,k,1);
	putpixel(i+31,k,1);
	putpixel(i+30,k-1,1);
	putpixel(i+31,k-1,1);
  }
  line(0,40,option1.len+30,40);
  line(30,0,30,80);
}
/*NAME :reverse()
  FUNCTION_USED:No function prepared by me is used.
  FUNCTION OF THE SUBROUTINE:it reverses the pixels of specified area.
  ARGUMENTS:The co-ordinate of the point and number
			of reversing characters.
  RETURNING VARIABLES:returns void.
  MACRO USED:NOT_PUT*/
void sreverse(int x,int y,char i)
{
  char *bitmap;
  int j,k;

  j=getx();
  k=gety();
  bitmap=malloc(imagesize(x,y-10,x+i*8,y+10));
  if (bitmap==NULL) error_msg("MEMORY IS OVERLOADED");
  getimage(x,y-10,x+i*8,y+10,bitmap);
  putimage(x,y-10,bitmap,NOT_PUT);
  free(bitmap);
  moveto(j,k);
}
/*NAME :intoa()
  FUNCTION_USED:No function prepared by me is used.
  FUNCTION OF THE SUBROUTINE:It makes an integer into a character array.
  ARGUMENTS:An integer vaiable.
  RETURNING VARIABLES:a character pointer.
  MACRO USED:No macro used*/
char *intoa(int x)
{
  char *s,p[4],i;
  s=(char *)malloc(4);
  if (s==NULL) error_msg("MEMORY IS OVERLOADED");
  s[3]='\0';
  for(i=0;i<3;i++) s[i]=32;
  itoa(x,p,10);
  x=strlen(p);
  for(i=0;i<x;i++)  s[i]=p[i];
  return s;
}

/*NAME :arrow()
  FUNCTION_USED:reverse(),cursor()
  FUNCTION OF THE SUBROUTINE:this function acts when uparrow
							 is pressed in the option choicing utilities.
  ARGUMENTS:The current position of the cursor.
  RETURNING VARIABLES:The position of the new cursor
  MACRO USED: No macro is used*/
int sarrow(char lower,int i)
{
  int j,k;
  j=getx();
  k=gety();
  if(i<4) sreverse(500,k-10,4);
  else sreverse(500,k-10,1);
  scursor(j,k,0);
  if(lower==DOWN){ i++;k=k+30;}
  else if(lower==UP){ i--;k=k-30;}
  if (i<4) sreverse(500,k-10,4);
  else sreverse(500,k-10,1);
  scursor(500,k,1);
  j=i;
  return j;
}
/*NAME :option_message()
  FUNCTION_USED:intoa(),clear(),reverse().
  FUNCTION OF THE SUBROUTINE:At first it gives a selecting
							 utility window.from this window we
							 can choose options.
  ARGUMENTS: the existing option.
  RETURNING VARIABLES:The selected option after executing.
  MACRO USED:No macro used*/
option option_message(option option1)
{
  char *s[6],state,*bitmap1,i;
  unsigned int keystroke,j,k,l;
  unsigned char lower,upper;
  option option2;

  s[0]=intoa(option1.loops);
  s[1]=intoa(option1.amplitude);
  s[2]=intoa(option1.cutting);
  s[3]=intoa(option1.angle);
  s[4]=malloc(2);
  if (s[4]==NULL) error_msg("MEMORY IS OVERLOADED");
  s[5]=malloc(2);
  if (s[5]==NULL) error_msg("MEMORY IS OVERLOADED");
  if (option1.rectified==1) strcpy(s[4],"Y");
  else if(option1.rectified==0) strcpy(s[4],"N");
  if (option1.full==1) strcpy(s[5],"Y");
  else if(option1.full==0) strcpy(s[5],"N");
  setviewport(100,50,651,301,0);
  bitmap1=malloc(imagesize(100,50,651,301));
  if(bitmap1==NULL){
    return;
  }
  getimage(0,0,550,250,bitmap1);
  clearviewport();
  rectangle(0,0,550,250);
  outtextxy(20,20,"THE NUMBER OF LOOPS");
  outtextxy(20,50,"THE DEGREE OF AMPLITUDE");
  outtextxy(20,80,"THE CUTTING ANGLE");
  outtextxy(20,110,"THE PHASE ANGLE");
  outtextxy(20,140,"IS IT RECTIFIED (Y/N)");
  outtextxy(20,170,"FULL WAVE (Y) HALF WAVE(N)");
  outtextxy(20,200,"PRESS <ESC> TO ACCEPT THIS OPTIONS");
  for(i=0;i<6;i++) outtextxy(500,20+i*30,s[i]);
  scursor(500,30,1);
  sreverse(500,20,4);
  i=0;
  state=FIRST;
  keystroke=bioskey(0);
  upper=keystroke>>8;
  lower=keystroke;
  while(lower!=ESC){
	if(lower==0){
	  switch(upper){
		case UP  : if(i!=0) i=sarrow(upper,i);
				   state=FIRST;
				   break;
		case DOWN: if(i!=5) i=sarrow(upper,i);
                   state=FIRST;
				   break;
		default: break;
	  }
	}
	else if(lower!=0){
	  switch(lower){
	 case '0':
	 case '1':
	 case '2':
	 case '3':
	 case '4':
	 case '5':
	 case '6':
	 case '7':
	 case '8':
	 case '9':       if(i<4 && getx()>=500 && getx()<=516){
					   j=getx();
					   l=(j-500)/8;
					   s[i][l]=lower;
					   for(k=l+1;k<3;k++) s[i][k]=32;
                       k=gety();
					   if(state==FIRST){
						 sclear(j,k-10,4);
						 sreverse(j,k-10,4);
						 outtextxy(j,k-10,s[i]);
						 sreverse(j,k-10,4);
						 scursor(j+8,k,1);
					   }
					   else if(state==IN){
						 char *p;
						 p = (char *)malloc(10);
						 sclear(j,k-10,1);
						 sreverse(j,k-10,1);
						 itoa(lower-'0',p,10);
						 outtextxy(j,k-10,p);
						 free(p);
						 sreverse(j,k-10,1);
						 scursor(j+8,k,1);
					   }
					 }
					 state=IN;
					 break;
		case 8:        if(i<4 && getx()!=500){
						 s[i][((getx()-500)/8)-1]=32;
						 j=getx();
						 k=gety();
						 sclear(j-8,k-10,2);
						 scursor(j-8,k,1);
						 moveto(j-8,k);
					   }
					   break;
		case 'y':
		case 'Y':
		case 'n':
		case 'N':      if(i>3){
						 j=getx();
						 k=gety();
						 s[i][0]=toupper(lower);
						 sclear(j,k-10,1);
						 sreverse(j,k-10,1);
						 outtextxy(j,k-10,s[i]);
						 scursor(j,k,1);
						 sreverse(j,k-10,1);
					   }
					   break;
		case '-':      if (i==3 && getx()==500){
						  s[i][0]='-';
						  s[i][1]=32;
						  s[i][2]=32;
						  j=getx();
						  k=gety();
						  sclear(j,k-10,4);
						  sreverse(j,k-10,4);
						  outtextxy(j,k-10,s[i]);
						  sreverse(j,k-10,4);
						  scursor(j+8,k,1);
					   }
					   state=IN;
					   break;
		default :      break;
	  }
	}
	keystroke=bioskey(0);
	lower=keystroke;
	upper=keystroke>>8;

  }
  if(atoi(s[0])>5) option1.loops=5;
  else if(atoi(s[0])<1) option1.loops=1;
  else option1.loops=atoi(s[0]);
  if(atoi(s[1])>9) option1.amplitude=9;
  else if(atoi(s[1])<1) option1.amplitude=1;
  else option1.amplitude=atoi(s[1]);
  if(atoi(s[2])>90) option1.cutting=90;
  else option1.cutting=atoi(s[2]);
  if(atoi(s[3])>180) option1.angle=180;
  else if(atoi(s[3])<-180) option1.angle=-180;
  else option1.angle=atoi(s[3]);
  if( s[4][0]=='y'|| s[4][0]=='Y') option1.rectified=1;
  else option1.rectified=0;
  if( s[5][0]=='y'|| s[5][0]=='Y') option1.full=1;
  else option1.full=0;
  option2=option1;
  setviewport(0,0,getmaxx(),getmaxy(),0);
  putimage(100,50,bitmap1,COPY_PUT);
  setviewport(20,80,600,220,0);
  free(bitmap1);
  for (i=0;i<6;i++) free(s[i]);
  return option2;
}
/*NAME :scale()
  FUNCTION_USED:sprintf(char*s,int i)
  FUNCTION OF THE SUBROUTINE:It gives the scales in the wave shape
							 making utility window.
  ARGUMENTS:No argument taking.
  RETURNING VARIABLES:No variable retyrns.
  MACRO USED:No macro used*/
void scale(void)
{
  char *s;
  int i,k;
  s=malloc(3);
  for(i=-4;i<=4;i=i+1){
	if(i!=0){
	  k=40-8*i;
	  line(0,k,5,k);
	  sprintf(s,"%2d",i*2);
	  outtextxy(9,k-3,s);
	}
  }
  settextstyle(0,0,0);
  for(i=30;i<521;i=i+70){
	line(i,139,i,135);
	line(i-1,139,i-1,135);
	itoa((i-30)/70,s,10);
	outtextxy(i-2,127,s);
  }
  free(s);
}
