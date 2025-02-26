/********************************************************************/
/* INCLUDING NECCESSARY HEADER FILES AND DECLARING GLOBAL VARIABLES*/
#include <stdio.h>
#include<ctype.h>
#include<graphics.h>
#include<alloc.h>

#define MAX  	77
#define DES     4
#define MULT 	1
#define FILL 	3
#define TF 	2

int  MAX_WORD;

extern unsigned max_question, cur_question;
extern int TOPX,TOPY,max_x,max_y,A_X1,A_Y1,A_X2,A_Y2, Q_X1, Q_X2, Q_Y1, Q_Y2;
extern char LINE_HEIGHT,CHAR_WIDTH;
char buffers[3][MAX],word[3];
extern char identies[10][30],data[10],len[10], far *font[256];
extern char far *mostofa, far *mamun;
extern struct que_type{
	unsigned  line_no;
	unsigned char q_type, ans_set, marks_set, answer, marks, obtained;
	char far *ptr_ans, far *ans;
	} questions[30];
char far *image,far *bits,cursor_pos,lines,far *subject;

/*void descriptive(unsigned int serial);*/
extern void dialog_box( int, int, int, int, char * );
extern void error_msg(char * s, char *t);
extern void g_printf(char row,char col,char *p,int op);
extern void g_outtextxy(int row,int col,char *p,int op);
extern void viewclear( int ,int, int, int );
extern void show_page( int );

void clear_view(void);
void show_tf(char far *ptr);
void erase(void);
void show_answer(unsigned char i);
void cursor(void);
void show_fill(char far *ptr);
void show_mult(char far *ptr);
void str_cpy(char far *ch,char *p);
/*******************************************************************/
void moveimage(void)
{
  int count,x1,x2,y1,y2;

  count=cursor_pos;
  if(count==MAX_WORD-1){
    erase();
    cursor_pos++;
    cursor();
    cursor_pos--;
    return;
  }
  x1=cursor_pos * CHAR_WIDTH + TOPX;
  y1=lines * LINE_HEIGHT + TOPY;
  x2=TOPX + (MAX_WORD-1) * CHAR_WIDTH-1;
  y2=y1 + LINE_HEIGHT-1;
  image=(char far *)farmalloc(imagesize(x1,y1,x2,y2));
  getimage(x1,y1,x2,y2,image);
  putimage(x1,y1,image,XOR_PUT);
  cursor_pos=MAX_WORD-1;
  erase();
  x1=x1+CHAR_WIDTH;
  putimage(x1,y1,image,COPY_PUT);
  cursor_pos=count;
  farfree(image);
}
/********************************************************************/
void make_cursor(void)
{
  int i;
  bits=(char far *)farmalloc(imagesize(2,1,CHAR_WIDTH,2));
  getimage(2,1,CHAR_WIDTH,2,bits);
  putimage(2,1,bits,NOT_PUT);
  getimage(2,1,CHAR_WIDTH,2,bits);
  clearviewport();
}
/********************************************************************/
void cursor(void)
{
putimage(cursor_pos*CHAR_WIDTH+TOPX,(lines+1)*LINE_HEIGHT+TOPY-3,bits,XOR_PUT);
}
/********************************************************************/
void erase(void)
{
  char far *ch;
  int x1,x2,y1,y2;
  x1=cursor_pos*CHAR_WIDTH +TOPX;
  y1=lines * LINE_HEIGHT + TOPY;
  x2=x1 + CHAR_WIDTH-1;
  y2=y1 + LINE_HEIGHT-1;
  ch=(char far *)farmalloc(imagesize(x1,y1,x2,y2));
  getimage(x1,y1,x2,y2,ch);
  putimage(x1,y1,ch,XOR_PUT);
  farfree(ch);
}
/********************************************************************/
void get (void)
{
  int x1,x2,y1,y2;

  x1=cursor_pos * CHAR_WIDTH + TOPX;
  y1=lines * LINE_HEIGHT + TOPY;
  x2=TOPX + (MAX_WORD+1) * CHAR_WIDTH-1;
  y2=y1 + LINE_HEIGHT-1;
  image=(char far *)farmalloc(imagesize(x1,y1,x2,y2));
  getimage(x1,y1,x2,y2,image);
  putimage(x1,y1,image,XOR_PUT);
}
/********************************************************************/
void put(void)
{
  int x1,y1;

  x1=cursor_pos * CHAR_WIDTH + TOPX;
  y1=lines * LINE_HEIGHT + TOPY;
  putimage(x1,y1,image,COPY_PUT);
}
/*********************************************************************/
void display(char ch)
{
  int x1,y1;

  x1=((int)cursor_pos *(int) CHAR_WIDTH) + TOPX;
  y1=((int)lines * (int)LINE_HEIGHT) + TOPY;
  if(font[ch]!=NULL) putimage(x1,y1,font[ch],COPY_PUT);
}
/*********************************************************************/
/*********************************************************************/
void show_descriptive(char far *ptr)
{
  int i,j;
  cursor_pos=0;
  lines=0;
  for(i=0;ptr[i]!='\0';i++){
    if(ptr[i]!='\n') {
      buffers[lines][cursor_pos]=ptr[i];
      cursor_pos++;
    }
    else if(ptr[i]=='\n'){
      word[lines]=cursor_pos;
      cursor_pos=0;
      lines++;
    }
  }
  cursor_pos=0;
  lines=0;
  for(i=0;i<3;i++){
    for(j=0;j<word[i];j++){
      display(buffers[i][j]);
      cursor_pos++;
    }
    cursor_pos=0;
    lines++;
  }
}
/********************************************************************/
/********************************************************************/
void line_edit(int i)
{
  char j,ch,k=0;

  MAX_WORD=data[i];
  lines=0;
  for(cursor_pos=0;cursor_pos<len[i];cursor_pos++)
  display(identies[i][cursor_pos]);
  cursor_pos=0;
  cursor();
  ch=getch();
  while(k==0){
    switch(ch){
      case 0:ch=getch();
	     if(ch==75 && cursor_pos!=0){
		 cursor();
		 cursor_pos--;
		 cursor();
	     }
	     else if(ch==77 && cursor_pos!=MAX_WORD){
	       cursor();
	       cursor_pos++;
	       cursor();
	     }
	     else if(ch==71){
	       cursor();
	       cursor_pos=0;
	       cursor();
	     }
	     else if(ch==79){
	       cursor();
	       cursor_pos=len[i];
	       cursor();
	     }
	     else if(ch==83 && MAX_WORD!=cursor_pos && cursor_pos<len[i]){
	       cursor();
	       for(j=cursor_pos;j<len[i]-1;j++)
		 identies[i][j]=identies[i][j+1];
	       cursor_pos++;
	       get();
	       cursor_pos--;
	       put();
	       farfree(image);
	       cursor();
	       len[i]--;
	     }
	     break;
      case 7:break;
      case 8:
	     if(cursor_pos==0) break;
	     if(cursor_pos==len[i]){
	       cursor();
	       cursor_pos--;
	       erase();
	       cursor();
	       len[i]--;
	     }
	     else if(cursor_pos<len[i]){
	       for(j=cursor_pos-1;j<len[i]-1;j++)
	       identies[i][j]=identies[i][j+1];
	       cursor();
	       get();
	       cursor_pos--;
	       put();
	       farfree(image);
	       cursor();
	       len[i]--;
	     }
	     else if(cursor_pos>len[i]){
	       cursor();
	       cursor_pos--;
	       cursor();
	     }
	     break;
     case 13:
	     for (j=len[i];j<MAX_WORD;j++) identies[i][j]=32;
	     identies[i][len[i]]='\0';
	     cursor();
	     cursor_pos=0;
	     return;
     default:
	     if(cursor_pos==MAX_WORD || len[i]==MAX_WORD) break;
	     if(cursor_pos>=len[i]){
	       for(j=len[i];j<cursor_pos;j++) identies[i][j]=32;
	       identies[i][cursor_pos]=ch;
	       cursor();
	       display(ch);
	       cursor_pos++;
	       cursor();
	       len[i]=cursor_pos;
	     }
	     else if(cursor_pos<len[i]){
	       for(j=len[i];j>cursor_pos;j--) identies[i][j]=identies[i][j-1];
	       identies[i][cursor_pos]=ch;
	       cursor();
	       moveimage();
	       display(ch);
	       cursor_pos++;
	       cursor();
	       len[i]++;
	     }
	     break;
      }
      ch=getch();
    }
  }
/********************************************************************/
void identy(void)
{
  int i,x1,x2,y1,y2, initial;

  x1 = 9*CHAR_WIDTH;
  x2 = 77 * CHAR_WIDTH;
  y1 = 2 * LINE_HEIGHT;
  y2 = 23 + (8 + LINE_HEIGHT) * 10 ;
  dialog_box( x1,y1,x2,y2,"                         IDENTITY CARD ");
  initial = y1 = y1 + LINE_HEIGHT + 10;
  x1 += CHAR_WIDTH * 8;
  g_outtextxy( x1, y1, "Name", XOR_PUT);
  y1 += LINE_HEIGHT + 8;
  g_outtextxy( x1, y1 ,"Roll Number", XOR_PUT);
  y1 += LINE_HEIGHT + 8;
  g_outtextxy( x1, y1 ,"Dept. Name", XOR_PUT);
  y1 += LINE_HEIGHT + 8;
  g_outtextxy( x1, y1 ,"Class Year", XOR_PUT);
  y1 += LINE_HEIGHT + 8;
  g_outtextxy( x1, y1 ,"Session", XOR_PUT);
  y1 += LINE_HEIGHT + 8;
  g_outtextxy( x1, y1 ,"Registration #", XOR_PUT);
  y1 += LINE_HEIGHT + 8;
  g_outtextxy( x1, y1 ,"Subject Name", XOR_PUT);
  y1 += LINE_HEIGHT + 8;
  g_outtextxy( x1, y1 , "Student Code", XOR_PUT);
  setfillstyle( SOLID_FILL, BLACK );
  setcolor(BLACK);
  x2 = 63 * CHAR_WIDTH - 1;
  for( i = 0, y1 = initial - 1, x1 = 32*CHAR_WIDTH - 2; i < 8; i++, y1 += LINE_HEIGHT + 8) {
	bar( x1 , y1, x2, y1 + LINE_HEIGHT + 1);
	rectangle( x1 - 2, y1 - 2, x2 + 2, y1 + LINE_HEIGHT + 3);
	g_outtextxy( x1+2, y1+1, identies[i], COPY_PUT);
	}
  setfillstyle( SOLID_FILL, WHITE );
  setcolor(WHITE);
  getch();
  viewclear( Q_X1, Q_Y1, Q_X2, Q_Y2);
  show_page( cur_question );
}
/********************************************************************/
void show_answer(unsigned char i)
{
  char s[]="  ";

  s[0] = (i+1)/10+48;
  s[1] = (i+1)%10+48;
  putimage(49*CHAR_WIDTH,LINE_HEIGHT,mostofa,COPY_PUT);
  putimage( 13 * CHAR_WIDTH, 18 * LINE_HEIGHT, mamun, COPY_PUT);
  g_printf(19,14,s,XOR_PUT);
  clear_view();
  switch(questions[i].q_type){
	case DES:
		g_printf( 2, 50, "DESCRIPTIVE", OR_PUT);
		show_descriptive(questions[i].ptr_ans);
		break;
	case FILL:
		g_printf( 2, 50, "FILL GAPS", OR_PUT);
		MAX_WORD=77;
		show_fill(questions[i].ptr_ans);
		break;
	case TF:
		g_printf( 2, 50, "TRUE-FALSE", OR_PUT);
		show_tf(questions[i].ptr_ans);
		break;
	case MULT:
		g_printf( 2, 50, "MULTIPLE CHOICE", OR_PUT);
		show_mult(questions[i].ptr_ans);
		break;
	}
}
/*********************************************************************/
void show_fill(char far *ptr)
{
  lines=0;
  for(cursor_pos=0;ptr[cursor_pos]!='\0';cursor_pos++){
    display(ptr[cursor_pos]);
    identies[9][cursor_pos]=ptr[cursor_pos];
  }
  len[9]=cursor_pos;
}
/********************************************************************/
void reverse(int i)
{
  char far *bitmap;
  int x1,x2,y1,y2;
  x1=TOPX + (2*i +1)*15;
  x2=x1+15;
  y1=TOPY+5;
  y2=TOPY+20;
  bitmap=(char far *)farmalloc(imagesize(x1+1,y1+1,x2-1,y2-1));
  getimage(x1+1,y1+1,x2-1,y2-1,bitmap);
  putimage(x1+1,y1+1,bitmap,NOT_PUT);
  farfree(bitmap);
}
/********************************************************************/
void show_tf(char far *ptr)
{
  int x1,x2,y1,y2,i;
  x1=TOPX;
  x2=TOPX;
  for(i=0;i<2;i++){
    x1=x2+15;
    y1=TOPY+5;
    x2=x1+15;
    y2=TOPY+20;
    rectangle(x1,y1,x2,y2);
    if (i==0) outtextxy(x1+5,y1+5,"T");
    else outtextxy(x1+5,y1+5,"F");
  }
  if (ptr[0]=='T') reverse(0);
  else if (ptr[0]=='F') reverse(1);
}
/********************************************************************/
void show_mult(char far *ptr)
{
  char s[2];
  int x1,x2,y1,y2,i;

  str_cpy(s,"A");
  x1=TOPX;
  x2=TOPX;
  for(i=0;i<5;i++){
    x1=x2+15;
    y1=TOPY+5;
    x2=x1+15;
    y2=TOPY+20;
    rectangle(x1,y1,x2,y2);
    outtextxy(x1+5,y1+5,s);
    s[0]++;
  }
  for(i=0;ptr[i]!='\0';i++) reverse(ptr[i]-'A');
}
/********************************************************************/
void clear_view(void)
{
  char far *bitmap;
  bitmap=(char far *)farmalloc(imagesize(A_X1,A_Y1,A_X2,A_Y2));
  getimage(A_X1,A_Y1,A_X2,A_Y2,bitmap);
  putimage(A_X1,A_Y1,bitmap,XOR_PUT);
  farfree(bitmap);
}
/********************************************************************/
/********************************************************************/
void str_cpy(char far *ch,char *p)
{
  int i,j;
  i=strlen(p);
  for(j=0;j<i;j++) ch[j]=p[j];
  ch[j]='\0';
}