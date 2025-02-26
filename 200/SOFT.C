#include <stdio.h>
#include<ctype.h>
#include<graphics.h>
#include<alloc.h>

#define MAX  	77
#define DES      4
#define MULT 	 1
#define FILL 	 3
#define TF 	     2

/********************************************************************/
int  MAX_WORD;
char buffers[3][MAX],word[3];
char far *image,far *bits,cursor_pos,lines,far *subject;

/*DEFINING EXTERNAL VARIABLES IN OTHER MODULE*/
extern unsigned max_question, cur_question;
extern int TOPX,TOPY,max_x,max_y,A_X1,A_Y1,A_X2,A_Y2;
extern char LINE_HEIGHT,CHAR_WIDTH;
extern char far *identies[10],data[10],len[10], far *font[256];
extern char far *mostofa, far *mamun;
extern struct que_type{
	unsigned  line_no;
	unsigned char q_type, ans_set,marks_set, answer, marks,obtained;
	char far *ptr_ans,far *ans;
	} questions[30];

/********************************************************************/
/*FUNCTION DECLARATION BEFORE CALLING*/
int save_identy(void);
extern void dialog_box( int, int, int, int, char * );
extern void error_msg(char * s, char *t);
extern void g_printf(char row,char col,char *p,int op);
extern void g_outtextxy(int row,int col,char *p,int op);
void ans_tf(char ch,int i);
void clear_view(void);
void show_tf(char far *ptr);
void ans_mult(char ch,int i);
void erase(void);
void show_answer(unsigned char i);
void cursor(void);
void show_fill(char far *ptr);
void show_mult(char far *ptr);
void str_cpy(char far *ch,char *p);
/*******************************************************************/
/*NAME:moveimage()
  VARABLE USED:cursor_pos,MAX_WORD,TOPX,TOPY,LINE_HEIGHT,CHAR_WIDTH,
  image;
  MACRO USED:COPY_PUT,XOR_PUT
  FUNCTION:This function moves the image of the line or descriptive
  editor by one character at atime.
  ARGUMENT:no
  RETURNING VARABLE:no
  */
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
  /*determining the starting position*/
  x1=cursor_pos * CHAR_WIDTH + TOPX;
  y1=lines * LINE_HEIGHT + TOPY;
  /*determining the ending position*/
  x2=TOPX + (MAX_WORD-1) * CHAR_WIDTH-1;
  y2=y1 + LINE_HEIGHT-1;
  image=(char far *)farmalloc(imagesize(x1,y1,x2,y2));
  /*storing and erasing the image*/
  getimage(x1,y1,x2,y2,image);
  putimage(x1,y1,image,XOR_PUT);
  cursor_pos=MAX_WORD-1;
  erase(); /*erasing the charcter at last position of the line*/
  x1=x1+CHAR_WIDTH;
  /*putting the image one character after*/
  putimage(x1,y1,image,COPY_PUT);
  cursor_pos=count;
  farfree(image);
}
/********************************************************************/
/*NAME:make_cursor()
  VARABLE USED:CHAR_WIDTH.
  MACRO USED:NOT_PUT
  FUNCTION:This function stores the neccessary image in pointer bits
  and this must be called at the starting of executing the program
  ARGUMENT:no.
  RETURNING VARABLE:no.
  */
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
/*NAME:cursor()
  VARABLE USED:cursor_pos,TOPX,TOPY,LINE_HEIGHT,CHAR_WIDTH,bits,line
  MACRO USED:XOR_PUT
  FUNCTION:This function creates cursor at cursor_pos if there is no
  cursor other wise moves the cursor.
  ARGUMENT:no.
  RETURNING VARABLE:no.
  */
void cursor(void)
{
putimage(cursor_pos*CHAR_WIDTH+TOPX,(lines+1)*LINE_HEIGHT+TOPY-3,bits,
  XOR_PUT);
}
/********************************************************************/
/*NAME:erase()
  VARABLE USED:cursor_pos,TOPX,TOPY,LINE_HEIGHT,CHAR_WIDTH,lines.
  MACRO USED:XOR_PUT.
  FUNCTION:This function removes the existing character in the current
  line and at current cursor_pos
  ARGUMENT:no.
  RETURNING VARABLE:no.
  */
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
/*NAME:get()
  VARABLE USED:cursor_pos,TOPX,TOPY,LINE_HEIGHT,CHAR_WIDTH,lines.
  MACRO USED:XOR_PUT
  FUNCTION:This function removes the charcters of any line from cursor_pos
  and stores the image in "image".
  ARGUMENT:no.
  RETURNING VARABLE:no.
  */
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
/*NAME:put()
  VARABLE USED:cursor_pos,TOPX,TOPY,LINE_HEIGHT,CHAR_WIDTH,lines,image.
  MACRO USED:COPY_PUT
  FUNCTION:This function puts the "image" at ne cursor_pos and new line.
  ARGUMENT:no.
  RETURNING VARABLE:no.
  */
void put(void)
{
  int x1,y1;

  x1=cursor_pos * CHAR_WIDTH + TOPX;
  y1=lines * LINE_HEIGHT + TOPY;
  putimage(x1,y1,image,COPY_PUT);
}
/*********************************************************************/
/*NAME:display()
  VARABLE USED:cursor_pos,TOPX,TOPY,LINE_HEIGHT,CHAR_WIDTH,lines,font[].
  MACRO USED:COPY_PUT.
  FUNCTION:This function displays a character from the font library.
  ARGUMENT:A charcter "ch"
  RETURNING VARABLE:no.
  */
void display(char ch)
{
  int x1,y1;

  x1=((int)cursor_pos *(int) CHAR_WIDTH) + TOPX;
  y1=((int)lines * (int)LINE_HEIGHT) + TOPY;
  if(font[ch]!=NULL) putimage(x1,y1,font[ch],COPY_PUT);
}
/*********************************************************************/
/*NAME:descriptive()
  VARABLE USED:cursor_pos,TOPX,TOPY,LINE_HEIGHT,CHAR_WIDTH,lines,
  buffer[][],word[].
  MACRO USED:no.
  FUNCTION:This function makes a editor of limited three lines for
  answering the descriptive answers,the buffer of the editor is saved
  in a field ptr_ans which is a pointer to far char.
  ARGUMENT:An unsigned in "serial" which indicates the serial no of
  the current question.
  RETURNING VARABLE:no.
  FUNCTION USED:get(),put(),display,erase(),moveimage(),cursor()
  */
void descriptive(unsigned int serial)
{
  char j,y,k=0,temp,l,s[3],far *ptr;
  int i=0;

  MAX_WORD=MAX;
  cursor_pos=0;
  lines=0;
  cursor();
  y=getch();/*taking a keyboard input*/
  while(k==0){
    switch(y){
	  case 0:y=getch();/*if hotkey is pressed*/
		 /*if the character is uparrow*/
		 if (y==72 && lines!=0){
	       cursor();
	       lines--;
	       cursor();
		 }
		 /*if the character is downarrow*/
	     else if(y==80 && lines!=2){
	       cursor();
	       lines++;
	       cursor();
		 }
		 /*if the character is leftarrow*/
	     else if(y==75 && cursor_pos!=0){
		 cursor();
		 cursor_pos--;
		 cursor();
		 }
		 /*if the character is rightarrow*/
	     else if(y==77 && cursor_pos!=MAX_WORD){
	       cursor();
	       cursor_pos++;
	       cursor();
		 }
		 /*if the character is HOME key*/
	     else if(y==71){
	       cursor();
	       cursor_pos=0;
	       cursor();
		 }
		 /*if the character is END key*/
	     else if(y==79){
	       cursor();
	       cursor_pos=word[lines];
	       cursor();
		 }
		 /*if DEL button is pressed*/
	     else if(y==83 && MAX_WORD!=cursor_pos && cursor_pos<word[lines]){
	       cursor();
	       for(j=cursor_pos;j<word[lines]-1;j++)
		   buffers[lines][j]=buffers[lines][j+1];
	       cursor_pos++;
	       get();
	       cursor_pos--;
	       put();
	       farfree(image);
	       cursor();
	       word[lines]--;
	     }
	     break;
	  case 26:/*CRTL-Z to escape from descriptive editor*/
		   cursor();
		   cursor_pos=0;
		   ptr=questions[serial].ptr_ans;
		   i=0;
		   /*saving the buffer in pointer ptr_ans*/
		   for(lines=0;lines<3;lines++){
			 for(j=0;j<word[lines];j++){
			   ptr[i]=buffers[lines][j];
			   i++;
			 }
			 ptr[i]='\n';
			 i++;
		   }
		   ptr[i]='\0';
		   return ; /*returning from the function*/
      case  7:break;
	  case  8:/*when backspace is pressed*/
		   /*at the begining of the line backspace will not do*/
		   if (cursor_pos==0)  break;
		   /*when backspace is pressed at the end of the line*/
		   if(cursor_pos==word[lines]){
			 word[lines]--;
			 buffers[lines][cursor_pos-1]=32;
			 cursor();
			 cursor_pos--;
			 erase();
			 cursor();
		   }
		   /*when backspace is pressed at the middle of the line*/
		   else if(cursor_pos<word[lines]){
			 for(i=cursor_pos-1;i<word[lines]-1;i++)
			 buffers[lines][i]=buffers[lines][i+1];
			 get();
			 cursor_pos--;
			 put();
			 farfree(image);
			 word[lines]--;
		   }
		   /*when backspace is pressed after the last character of the line*/
		   else if(cursor_pos>word[lines]){
		   cursor();
		   cursor_pos--;
		   cursor();
		 }
		 break;
	  case 13:/*when <enter> is pressed */
		  /*carriage return in last line will not do*/
		  if (lines==2) break;
		  /*if the cursor at 2nd line but 3rd line is not empty then it
		  will not do*/
	      if (lines==1 && word[2]!=0) break;
		  /*if cursor at 2nd line and 3rd line is empty*/
		  else if (lines==1 && word[2]==0) {
			cursor();
			/*modifying the buffer*/
			for(i=cursor_pos;i<word[1];i++)
			buffers[2][i-cursor_pos]=buffers[1][i];
			if(cursor_pos<=word[1]){
			  word[2]=word[1]-cursor_pos;
			  word[1]=cursor_pos;
			}
			else word[2]=0;
			/*transfering the image by get() and put()*/
			get();
			cursor_pos=0;
			lines=2;
			put();
			farfree(image);
			cursor();
		  }
		  /*if the cursor at 1st line*/
		  else if(lines==0){
			/*if the 3rd line is empty*/
			if(word[2]==0){
			cursor();
			/*transferring the buffer*/
			for(i=0;i<MAX_WORD;i++) buffers[2][i]=buffers[1][i];
			word[2]=word[1];
			for(i=cursor_pos;i<word[0];i++)
			buffers[1][i-cursor_pos]=buffers[0][i];
			if(cursor_pos<=word[0]){
			  word[1]=word[0]-cursor_pos;
			  word[0]=cursor_pos;
			}
			else word[1]=0;
			lines=1;
			cursor_pos=0;
			/*transferring the image by get() and put()*/
			get();
			lines=2;
			put();
			farfree(image);
			lines=0;
			cursor_pos=word[0];
			get();
			lines=1;
			cursor_pos=0;
			put();
			farfree(image);
			cursor();
		  }
		}
		break;
		/*if any other key of for typing is pressed*/
		default:
		if(lines==2 && word[2]==MAX_WORD) break;
		if(lines==1 && (word[1]+word[2]==2*MAX_WORD)) break;
		if (cursor_pos==MAX_WORD) break;
		if (cursor_pos>=word[lines]){
		  buffers[lines][cursor_pos]=y;
		  cursor();
		  display(y);
		  cursor_pos++;
		  cursor();
		  for(i=word[lines];i<cursor_pos-1;i++) buffers[lines][i]=32;
		  word[lines]=cursor_pos;
		}
		else if (cursor_pos<word[lines]){
		if (word[0] + word[1] + word[2] == MAX_WORD*3) break;
		if (word[lines] < MAX_WORD){
		  for(i=word[lines];i>cursor_pos;i--)
		  buffers[lines][i]=buffers[lines][i-1];
		  buffers[lines][cursor_pos]=y;
		  word[lines]++;
		  moveimage();
		  display(y);
		  cursor_pos++;
		}
		else if(word[lines]==MAX_WORD){
		  temp=lines;
		  while(word[lines]==MAX_WORD && lines<2) lines++;
		  for(i=lines;i>temp;i--){
		    for(j=word[i];j>0;j--){
		      buffers[i][j]=buffers[i][j-1];
		    }
		    buffers[i][0]=buffers[i-1][MAX_WORD-1];
		  }
		  word[lines]++;
		  for(i=MAX_WORD-1;i>cursor_pos;i--){
		    buffers[temp][i]=buffers[temp][i-1];
		  }
		  buffers[temp][cursor_pos]=y;
		  l=lines;
		  lines=temp;
		  moveimage();
		  display(y);
		  cursor_pos++;
		  j=cursor_pos;
		  for(i=temp+1;i<l+1;i++){
		    cursor_pos=0;
		    lines=i;
		    moveimage();
		    display(buffers[lines][0]);
		  }
		  lines=temp;
		  cursor_pos=j;
		}
	  }
	  break;
    }
	y=getch();/*asking for any key again*/
  }
}
/*********************************************************************/
/*NAME:show_descriptive()
  VARABLE USED:ptr_ans,cursor_pos(),buffer[][],,lines.
  MACRO USED:no.
  FUNCTION:This function displays the current buffer of the question
  in 3 line descriptive editor.
  ARGUMENT:far pointer to character
  RETURNING VARABLE:no.
  FUNCTION USED:display()
  */
void show_descriptive(char far *ptr)
{
  int i,j;
  cursor_pos=0;
  lines=0;
  /*transferring the answer buffer into descriptive buffer*/
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
  /*displaying the buffer*/
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
/*NAME:init_identy()
  VARABLE USED:identies[],far pointer to character.
  MACRO USED:no.
  FUNCTION:This function allocates the memory for getting the identy
  of the student and for line editor buffer.it also initialize the lengths
  and maximum length.This is called at the begining
  of the program.
  ARGUMENT:no.
  RETURNING VARABLE:no.
  */
void init_identy(void)
{
  char j;

  identies[0]=(char far *)farmalloc(31);
  data[0]=30;
  for(j=1;j<4;j++){
    identies[j]=(char far *)farmalloc(4);
    data[j]=3;
  }
  identies[4]=(char far *)farmalloc(10);
  data[4]=9;
  identies[5]=(char far *)farmalloc(6);
  data[5]=5;
  identies[6]=(char far *)farmalloc(7);
  data[6]=6;
  for(j=7;j<9;j++){
    identies[j]=(char far *)farmalloc(6);
    data[j]=5;
  }
  for(j=0;j<10;j++) len[j]=0;
  identies[9]=(char far *)farmalloc(78);
  data[9]=77;
}
/********************************************************************/
/*NAME:line_edit()
  VARABLE USED:cursor_pos,MAX_WORD,identies[],line,len[].
  MACRO USED:no.
  FUNCTION:This function make line editor of specified length mentioned by
  data[i].This editor is used by function identy to take different string.
  ARGUMENT:integer:i.
  RETURNING VARABLE:no
  FUNCTION_USED:display(),erase(),cursor(),get(),put()
  */
void line_edit(int i)
{
  char j,ch,k=0;

  /*Taking the maximum length of the line*/
  MAX_WORD=data[i];
  lines=0;
  /*Displaying the content of the buffer*/
  for(cursor_pos=0;cursor_pos<len[i];cursor_pos++)
  display(identies[i][cursor_pos]);
  cursor_pos=0;
  cursor();
  ch=getch();/*Taking the first character*/
  while(k==0){
    switch(ch){
	  case 0:ch=getch();/*if the key is hot key*/
		 /*if left arrow is pressed*/
		 if(ch==75 && cursor_pos!=0){
		   cursor();
		   cursor_pos--;
		   cursor();
		 }
		 /*if right arrow is pressed*/
	     else if(ch==77 && cursor_pos!=MAX_WORD){
	       cursor();
	       cursor_pos++;
	       cursor();
		 }
		 /*if HOME key is pressed*/
	     else if(ch==71){
	       cursor();
	       cursor_pos=0;
	       cursor();
		 }
		 /*if END key is pressed*/
	     else if(ch==79){
	       cursor();
	       cursor_pos=len[i];
	       cursor();
		 }
		 /*if DEL key is pressed*/
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
	  case 8:/*if backspace is pressed*/
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
	 case 13:/*the line editor is terminated by pressing <enter>*/
		 /*saving the buffer*/
		 for (j=len[i];j<MAX_WORD;j++) identies[i][j]=32;
	     identies[i][len[i]]='\0';
	     cursor();
	     cursor_pos=0;
	     return;
	 default:/*if othe than hot key is pressed*/
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
	  ch=getch();/*taking the next character*/
    }
  }
/********************************************************************/
/*NAME:identy()
  VARABLE USED:CHAR_WIDTH,LINE_HEIGHT,
  MACRO USED:SOLID_FILL,BLACK,XOR_PUT.
  FUNCTION:This function creates a identy card where the student will
  give their different identy before begining the examination.
  ARGUMENT:no.
  RETURNING VARABLE:no.
  */
void identy(void)
{
  int i,x1,x2,y1,y2, initial;
  char far *bitmap;

  x1 = 9*CHAR_WIDTH;
  x2 = 77 * CHAR_WIDTH;
  y1 = 10;
  y2 = 23 + (8 + LINE_HEIGHT) * 10 ;
  clearviewport();
  dialog_box( x1,y1,x2,y2,"                         IDENTITY CARD ");
  initial = y1 = 20 + LINE_HEIGHT;
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
	}
  setfillstyle( SOLID_FILL, WHITE );
  setcolor(WHITE);
  do{
    y1 = initial;
    TOPX = 32*CHAR_WIDTH;
    x2 = 63 * CHAR_WIDTH + 1;
    for(i=0;i<8;i++){
	TOPY = y1 ;
	rectangle( TOPX - 4, y1 - 3, x2 , y1 + LINE_HEIGHT + 2);
	line_edit(i);
	setcolor( BLACK );
	rectangle( TOPX - 4, y1 - 3, x2 , y1 + LINE_HEIGHT + 2);
	setcolor( WHITE );
        y1 += LINE_HEIGHT + 8;
	}
    x1 = 15 * CHAR_WIDTH - 6;
    x2 = 55 * CHAR_WIDTH + 6;
    y1 = 10 * LINE_HEIGHT - 4;
    y2 = y1 + LINE_HEIGHT + 6;
    bitmap=(char far *)farmalloc(imagesize(x1,y1,x2,y2));
    getimage(x1,y1,x2,y2,bitmap);
    putimage(x1,y1,bitmap,XOR_PUT);
    rectangle(x1+2,y1+2,x2-2,y2-2);
    g_printf( 11, 16, "Do You Want To Reset Your Identies(Y/N)", OR_PUT);
    i=getch();
    putimage(x1,y1,bitmap,COPY_PUT);
    farfree(bitmap);
  }while(i=='Y' || i=='y');
  clearviewport();
  subject=(char far *)farmalloc(7);
  subject=identies[6];/*subject stores the subject name of the exam.*/
}
/********************************************************************/
/*NAME:showanswer()
  VARABLE USED:HAR_WIDTH,LINE_HEIGHT,questions[],
  MACRO USED:XOR_PUT,COPY_PUT
  FUNCTION:This function detects the type of question and shows the
  question in their own manner.
  ARGUMENT:i,the question number
  RETURNING VARABLE:no.
  */
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
		g_printf(24,12,"Press F10 to edit answer                 ",COPY_PUT);
		g_printf( 2, 50, "DESCRIPTIVE", OR_PUT);
		show_descriptive(questions[i].ptr_ans);
		break;
	case FILL:
		g_printf(24,12,"Press F10 to edit answer                 ",COPY_PUT);
		g_printf( 2, 50, "FILL GAPS", OR_PUT);
		MAX_WORD=77;
		show_fill(questions[i].ptr_ans);
		break;
	case TF:
		g_printf(24,12,"Press the letter inside the box to select",COPY_PUT);
		g_printf( 2, 50, "TRUE-FALSE", OR_PUT);
		show_tf(questions[i].ptr_ans);
		break;
	case MULT:
		g_printf(24,12,"Press the letter inside the box to select",COPY_PUT);
		g_printf( 2, 50, "MULTIPLE CHOICE", OR_PUT);
		show_mult(questions[i].ptr_ans);
		break;
	}
}
/*********************************************************************/
/*NAME:show_fill()
  VARABLE USED:cursor_pos,identies[9].
  MACRO USED:no.
  FUNCTION:This function shows the content of fill gaps question in
  a line editor.
  ARGUMENT:far pointer to char,*ptr.
  RETURNING VARABLE:no.
  FUNCTION USED:display().
  */
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
/*NAME:reverse()
  VARABLE USED:TOPX,TOPY.
  MACRO USED:NOT_PUT.
  FUNCTION:This function reverses boxes at the position specified in i.
  ARGUMENT:i,the position of box
  RETURNING VARABLE:no.
  */
void reverse(int i)
{
  char far *bitmap;
  int x1,x2,y1,y2;
  x1=TOPX + (2*i +1)*15;
  x2=x1+15;
  y1=TOPY+5;
  y2=TOPY+20;
  bitmap=(char far *)farmalloc(imagesize(x1+1,y1+1,x2-1,y2-1));
  /*Taking the image*/
  getimage(x1+1,y1+1,x2-1,y2-1,bitmap);
  /*reversing the image*/
  putimage(x1+1,y1+1,bitmap,NOT_PUT);
  farfree(bitmap);
}
/********************************************************************/
/*NAME:show_tf()
  VARABLE USED:TOPY,TOPY.
  MACRO USED:no.
  FUNCTION:shows the answer stored in *ptr graphically by two boxes.
  the reverse box indicates the answer.
  ARGUMENT:far pointer to character ptr which stores the answer.
  RETURNING VARABLE:no.
  */
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
/*NAME:show_mult()
  VARABLE USED:TOPX,TOPY.
  MACRO USED:no.
  FUNCTION:shows the answer stored in *ptr graphically by two boxes.
  the reverse box indicates the answer.
  ARGUMENT:far pointer to character ptr which stores the answer.
  RETURNING VARABLE:no.
  */
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
/*NAME:clear_view()
  VARABLE USED:A_X1,A_Y1,A_X2,A_Y2
  MACRO USED: XOR_PUT.
  FUNCTION:Clears the places for answer.
  ARGUMENT:no.
  RETURNING VARABLE:no.
  */
void clear_view(void)
{
  char far *bitmap;
  bitmap=(char far *)farmalloc(imagesize(A_X1,A_Y1,A_X2,A_Y2));
  getimage(A_X1,A_Y1,A_X2,A_Y2,bitmap);
  putimage(A_X1,A_Y1,bitmap,XOR_PUT);
  farfree(bitmap);
}
/********************************************************************/
/*NAME:ans_set().
  VARABLE USED:questions[].
  MACRO USED:no.
  FUNCTION:This function sets a answer after taking input from
  the keyboard.
  ARGUMENT:ch,character which indicates the key pressed and i,which
  indicates the question number.
  RETURNING VARABLE:no.
  FUNCTION USED:ans_tf(),ans_mult(),decriptive();
  */
void ans_set(char ch,int i)
{

  switch(questions[i].q_type){
    case DES:
	     if (ch==4) {
	       g_printf(24,12,"Press CRTL-Z to terminate the answer                ",COPY_PUT);
	       descriptive(i);
	       questions[i].ans_set=1;
	     }
	     break;
   case MULT:
	     ans_mult(ch,i);
	     break;
   case FILL:
		if(ch==0){
		  g_printf(24,12,"Press Enter to terminate                 ",COPY_PUT);
		  line_edit(9);
		  g_printf(24,12,"PRESS  F10 TO EDIT                            ",COPY_PUT);
		  for(ch=0;ch<len[9];ch++)
		  questions[i].ptr_ans[ch]=identies[9][ch];
		  questions[i].ptr_ans[ch]='\0';
		  questions[i].ans_set=1;
		}
		break;
    case TF:
	     ans_tf(ch,i);
	     break;
    default: break;
  }
}
/********************************************************************/
/*NAME:ans_mult()
  VARABLE USED:questions[]
  MACRO USED:no.
  FUNCTION:this function is made for answering multiple choice question.
  ARGUMENT:ch,character which indicates the key pressed and i,which
  indicates the question number.
  RETURNING VARABLE:no.
  */
void ans_mult(char ch,int i)
{
  char far *ptr;
  int j;

  ptr=questions[i].ptr_ans;
  ch=toupper(ch);
  for(j=0;j<5;j++) {
	if (ptr[j]!=32 && ptr[j]!=ch) {
	  reverse(ptr[j]-'A');
	  ptr[j]=32;
	}
  }
  if(ch>='A' && ch<='E'){
    reverse(ch-'A');
    if (ptr[ch-'A']==32) ptr[ch-'A']=ch;
    else ptr[ch-'A']=32;
    questions[i].ans_set=1;
  }
}
/*******************************************************************/
/*NAME:ans_tf
  VARABLE USED:questions[].
  FUNCTION USED:reverse()
  MACRO USED:no.
  FUNCTION:this function is made for answering true false question.
  ARGUMENT:ch,character which indicates the key pressed and i,which
  indicates the question number.
  RETURNING VARABLE:no.
  */
void ans_tf(char ch,int i)
{
  char far *ptr;

  ptr=questions[i].ptr_ans;
  ch=toupper(ch);
  if(ch=='T'||ch=='F'||ch=='C')  questions[i].ans_set=1;
  switch(ptr[0]){
    case 'T':if(ch=='T') break;
	     if(ch=='F'){
	       reverse(0);
	       reverse(1);
	       ptr[0]='F';
	     }
	     break;
    case 'F':if(ch=='F') break;
	     if(ch=='T'){
	       reverse(0);
	       reverse(1);
	       ptr[0]='T';
	     }
	     break;
    case 32: if(ch=='T'){
	       reverse(0);
	       ptr[0]='T';
	     }
	     else if(ch=='F'){
	       reverse(1);
	       ptr[0]='F';
	     }
	     break;
     default: break;
   }
   if(ch=='C'){
     if(ptr[0]=='T') reverse(0);
     if(ptr[0]=='F') reverse(1);
     ptr[0]=32;
   }
 }
 /*****************************************************************/
/*NAME:str_cpy()
  VARABLE USED: no global variable used.
  MACRO USED:no.
  FUNCTION:This function make copy of character string to a far character
  string.
  ARGUMENT:ch,far character pointer and p,character pointer
  RETURNING VARABLE:no.
  */
void str_cpy(char far *ch,char *p)
{
  int i,j;
  i=strlen(p);
  for(j=0;j<i;j++) ch[j]=p[j];
  ch[j]='\0';
}
/*******************************************************************/
/*NAME:free_ans()
  MACRO USED:no.
  FUNCTION:This function make all alocated memory pointed by
  mostofa,bits,subject at the end of the program.
  ARGUMENT:no.
  RETURNING VARABLE:no.
  */
void free_ans(void)
{
  int i;
  for(i=0;i<10;i++) farfree(identies[i]);
  farfree(mostofa);
  farfree(bits);
  farfree(subject);
}
/******************************************************************/
