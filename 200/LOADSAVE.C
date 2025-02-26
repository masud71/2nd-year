/**************************************************************
  This part of the program is done by Masud Karim Khan.


  */

#include <stdio.h>
#include <ctype.h>
#include <string.h>     /*FOR strcpy(),strcat(),strlen() functions.*/
#include <graphics.h>
#include <stdlib.h>

extern void mycursor();
char *inputf(char stringss[],int );
char *getstring(int);
char find(char,char);
int decodefile();

		/*	GLOBAL VARIABLE DECLARATIONS	  */

extern unsigned char *BUFFER[200],*minibuf[100];
extern maxline;
extern int  status[2][200];
extern FILE *fp1;
extern maxx, maxy;

int codefile()
/*
	Function : codefile
	Purpose  : To store the question file in the disk.The function generates
				a random number and store it in the top of the file.The number
				is subtracted from each character of the file & the converted
				characters are stored.After the random number a password is
				stored which is taken as a keyboard input.Then the status of
				the examination is stored & after that the questions are
				stored with their status.
	Programmed By : Masud Karim Khan
*/

{
  unsigned char ch,key;
  char *filename ,tstring[100], *password;
  int line = 0,i;


  filename = inputf("Enter File Name",1);  /* Gets file Name  */
  fp1 = fopen (filename,"wb");

  if (fp1==NULL)
  {
    error_msg("Error Opening File");
	setviewport(12,58,maxx-13,maxy-26,1);
    return 0;
  }
  if (fp1!=NULL)
  {
	randomize();
	password = inputf("Enter Password",0); /* Gets Password from keybord*/
	key = random(100);
	fputs("QuizTaker question file.",fp1);
	putc(26,fp1);
	putc(key,fp1);
	for(i=0;i<4;i++) putc(password[i]-key,fp1);
	for (i=0;minibuf[0][i];i++) putc(minibuf[0][i]-key,fp1);
	putc(13-key,fp1);
	putc(10-key,fp1);
	while(line <=maxline) {
		if (status[0][line] > 0) {
			putc(254-key,fp1);
			putc('G'-key,fp1);
			putw(status[0][line]-key,fp1);
			fwrite(BUFFER[line],status[0][line],1,fp1);
		}
		else {
			if (status[1][line])
				for (i=0;minibuf[status[1][line]][i];i++)
					putc(minibuf[status[1][line]][i]-key,fp1);
			strcpy(tstring,BUFFER[line]);
			for(i=0;tstring[i];i++) putc(tstring[i]-key,fp1);
			putc(13-key,fp1);
			putc(10-key,fp1);
		}
		line++;
	}
	putc(26-key,fp1);
	fclose(fp1);
  }
  setviewport(12,58,maxx-13,maxy-26,1);
  return 1;
}

int decodefile()
/*
	Function :decodefile
	Purpose  :Loads a file from disk.Working principle is just reverse of
				decodefile() function
	Programmed By : Masud Karim Khan
*/
{
  unsigned char ch,key;
  char *filename ,tstring[100], *password;
  int line = 0,i,i_size,minind=0;


  filename = inputf("Enter File Name",1);  /* Gets file Name  */
  fp1 = fopen (filename,"rb");

  if (fp1==NULL)
  {
	error_msg("File Not Found");
	setviewport(12,58,maxx-13,maxy-26,1);
    return 0;
  }
  if (fp1)
  {
	fgets(tstring,25,fp1);
	getc(fp1);
	password = inputf("Enter Password",0); /* Gets Password from keybord*/
	key=getc(fp1);
	for(i=0;i<4;i++) tstring[i]=getc(fp1) + key;
	for(i=0;i<4 && tstring[i]==password[i];i++);
	if (i != 4) {
		error_msg("Incorrect Password");
		setviewport(12,58,maxx-13,maxy-26,1);
		return 0;
	}
	minibuf[0] = (char *)malloc(13);
	for (i=0;i<12;i++) minibuf[0][i] = getc(fp1)+key;
	minibuf[0][12] = 0;
	getc(fp1);getc(fp1);
	do {
		if ((ch=getc(fp1)+key) == 254) {
			if ((ch=getc(fp1)+key)=='G') {
				i_size = getw(fp1) + key;
				BUFFER[line] = (void *)malloc(i_size);
				fread(BUFFER[line],i_size,1,fp1);
				status[0][line] = i_size;
				line++;
			}
			else {
				minibuf[++minind] = (char *)malloc(16);
				minibuf[minind][0]=254;
				minibuf[minind][1] = ch;
				for (i=2;i<8;i++) minibuf[minind][i]=getc(fp1)+key;
				ch = minibuf[minind][7];
				if (ch=='T'|| ch=='M') {
					for (i=8;i<11;i++) minibuf[minind][i]=getc(fp1)+key;
					if (ch=='M') {
						for (i=11;i<14;i++) minibuf[minind][i]=getc(fp1)+key;
						minibuf[minind][14] = 0;
					}
					else minibuf[minind][11] = 0;
				}
				else minibuf[minind][8] = 0;
				status[1][line] = minind;
			}
		}
		else {
			if (ch == 26) break;
			if (ch !=13) {
			tstring[0] = ch;
			i=1;
			do {
				ch = getc(fp1)+key;
				if (ch != 13) tstring[i++] = ch;
			}while (ch != 13);
			tstring[i] = 0;
			}
			else tstring[0]=0;
			BUFFER[line] =(char *)malloc(strlen(tstring)+1);
			strcpy(BUFFER[line],tstring);
			status[0][line] = -1;
			getc(fp1);
			line++;
		}
	}while(ch !=26);
	fclose(fp1);
	maxline=line;
  }
  setviewport(12,58,maxx-13,maxy-26,1);
  return 1;
}


/**********************************************************/


char *inputf(char stringss[],int mode)
{
   char *fname;
   char  *box;
   unsigned size;

 setviewport(0,0,maxx,maxy,0);
 size = imagesize( 213, 55, 506, 127);
 /* If enough memory cannot be found then only make a beep sound */
 if( (box = (char  *) malloc(size) ) != NULL)
 {
   getimage( 213, 55, 506, 127, box);
   /* Save the image of the area where the message will appear */
   setcolor(1);
   dialog_box(213,55,506,127,stringss);
   if (mode)
   {
     setfillstyle(0,0);
     bar(240,80,480,100);
   }
   moveto(245,82);
   fname = getstring(mode);
   putimage(213,55,box,COPY_PUT);
   free(box);
   return fname ;
 }
  else
 putch(7);

 return NULL;
}

char *getstring(int mode)
{
	char ch[2]=" ",dump[15],*tstr;
	int x,y,i=0;

	x=getx();
	y=gety();
	do {
		moveto(x,y);
		mycursor();
		ch[0]=getch();
		if (!ch[0]) ch[0]=getch();
		mycursor();
		if (ch[0]>=32 &&ch[0]<=127) {
			if (mode) outtext(ch);
			dump[i++]=ch[0];
			x += 8;
		}
		else if (ch[0]==8 && i > 0) {
			x -= 8;
			setfillstyle(SOLID_FILL,BLACK);
			bar(x,y,x+7,y+7);
			setfillstyle(SOLID_FILL,WHITE);
			i--;
		}
	}while(ch[0] !=13 && ch[0] !=27);
	dump[i]='\0';
	if (ch[0]==13) {
		i=strlen(dump);
		tstr=(char *)malloc(i+1);
		strcpy(tstr,dump);
		return tstr;
	}
	else return NULL;
}