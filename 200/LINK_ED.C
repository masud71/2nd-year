#include <stdio.h>
#include <string.h>     /*FOR strcpy(),strcat(),strlen() functions.*/
#include <alloc.h>      /*FOR calloc() and free() functions. */
#include <bios.h>       /*FOR bioskey() functions. */
#include <ctype.h>
#include <graphics.h>
#include <dir.h>


/* ####################       CONSTANT DEFINING       ####################*/


#define TURBOC
#define MAXINDEX 70
#define PAGE_LINE_NUM 22

/* #############     CONSTANT DEFINING FOR DIFFERENT KEYS     #############*/

#define DEL 	       	83
#define BACKSPACE      '\b'
#define HOME            71
#define END             79
#define PAGE_UP         73
#define PAGE_DOWN       81
#define UP_ARROW        72
#define DOWN_ARROW      80
#define RIGHT_ARROW     77
#define LEFT_ARROW      75
#define CTRL_HOME       077
#define CTRL_END        075
#define ESC             27
#define CTRL_Y          25
#define CTRL_R          '\x12'
#define CTRL_S          '\x13'
#define CTRL_K          '\v'
#define CTRL_E          '\x5'
#define CTRL_C          '\x3'
#define CTRL_D          '\x4'
#define ENTER           13
#define INSERT          82
#define F2 		60
#define F4              62
#define F7              65
#define F8              66
#define F10		68


/*  ################     FUNCTION  DECLARATIONS     ###################  */


void box(int,int,int,int); /*print a box on the screen*/
void screen();
void initialize();
void buf_initialize();
void setup();
int check_status();
void changemode();
void put_in_buffer();
void delete_char();
void back_space();
void del_line();
void undelete();
void write_default();
void enter();
void left_arrow();
void right_arrow();
void up_arrow();
void down_arrow();
void home();
void end();
void ctrl_home();
void ctrl_end();
void pg_up();
void pg_down();
void block();
void insert();
void insert_mode(void);
void push();
void display_a_page(int );
void writechar(unsigned  char ascii_code);
void write_n_str(char  *st);
void cursor();
void delete_line();
void ins_line();
void clear_line();
void test_memory(char *memo);
void write_col_row(void);
void write_col (void);
void write_line (void);
void find_drive_dir_filename(void);
int set_graph_area( void );
int store_image( void );
void mycursor();


extern void symbol(void);
extern void free_graph(void);
extern void sine_wave(void);
extern void rect(void);
extern char *getstring(int);
extern void error_msg(char *);
extern void create_cursor(void);
extern void create_rubber(void);
extern void print();

/**/ void load(), new(), save(), dir(), list(), quit(),
	   edit(),symbol_call(),f1(),f2(), o2(),
	  set_stat(), set_ques(),help();


void menu();
char submenu(int, int, int, char*);
void reverse(int, int, int, int, char *, char);
void (*function[]) (void) = {        /* function name in an function type pointer*/
	 load, new, save, dir, list, quit,/*quit,*/ /* 6*/
	 sine_wave, rect, free_graph, edit, symbol_call,
     f1, f2,   /*13*/
	 print,o2,set_stat, set_ques
};


/*	###############        GLOBAL VARIABLE DECLARATIONS	    #############  */



char g_mode;
int cur_x , cur_y, lx, rx, ty, by, dead_x, dead_y;
unsigned char *p,*BUFFER[200],*minibuf[100];
int status[2][200];
unsigned char *temp1,*tempc,*dline;
unsigned char tempstr[MAXINDEX];
int	j,count,clength,currline,maxline,index,max_index,ins,marker = 0,
	ascii_val,loc,mode,maxgraph,col = 1,row = 1,scr_first_line,
	maxx,maxy,gline = 0;
unsigned char  normal_font[256][12];
FILE *fp1;


/*  	###############       DEFINING A UNION       ################      */


union u
	 {
		 unsigned	int i;
	   char	ch[2];
	 } key;

/*  $$$$$$$$$$$$$$$$$$$$   MAIN PROGRAM BEGINS   $$$$$$$$$$$$$$$$$$$$$$$  */



main(int argc,char *argv[])	/*MAIN PROGRAM BEGINS*/
{
	int i;
	if (argc != 2 || strcmp(argv[1],"Pass1")) {
		printf("This program requares Quiz Taker (QT.EXE)\n");
		exit(0);
	}
	for (i=0;i<200;i++) BUFFER[i] = NULL;
	for (i=0;i<100;i++) minibuf[i] = NULL;
	 setup();
	 screen();
	 write_col_row();
	 initialize();
	 find_drive_dir_filename();
	 for(;;)
		{
			write_col();
			write_line();
			insert_mode();
			#ifdef TURBOC
				key.i = bioskey(0);
			#endif
			#ifdef MICROSOFTC
				key.i = _bios_keybrd(0);
			#endif
			ascii_val = (int)key.ch[0];
			if (key.ch[0])
			{
				switch(key.ch[0])         /*NORMAL KEYS*/
					{
					case  ENTER :
									if(!check_status())	enter();
									else down_arrow();
									break;


					case  BACKSPACE :
									if(!check_status())
									back_space();
									break;

					case  CTRL_Y :
									if(!check_status())
									del_line();
									break;

					case  CTRL_R :
									undelete();
									break;

					case  CTRL_K :
									block();
									break;

					case  CTRL_C :
									block();
									break;

					case  CTRL_D :
									block();
									break;

					case  CTRL_E :
									block();
									break;

					case  CTRL_S :
									if(!check_status())
									changemode();
									break;

					case  ESC : buf_initialize();
								closegraph();
								exit(1);

					default :
								if(!check_status())
								write_default ();
								break;
				}
			}
			else {
					strcpy(BUFFER[currline],tempstr);
					switch(key.ch[1])	/*HOT KEYS*/
					{
						case  LEFT_ARROW :
										if(!check_status())
										left_arrow();
										break;

						case  INSERT :
										if(!check_status())
										insert();
										break;

						case  DEL :
									   if(!check_status())
									   delete_char();
									   break;

						case RIGHT_ARROW :
									   if(!check_status())
									   right_arrow();
									   break;

						case UP_ARROW :
									   up_arrow();
									   break;

						case DOWN_ARROW :
									   down_arrow();
									   break;

					 case PAGE_UP : pg_up();
								 break;

					 case PAGE_DOWN : pg_down();
								   break;

					 case HOME :
		   if(!check_status())
					 home();
							   break;

					 case END	:
		   if(!check_status())
					 end();
							  break;

					 case CTRL_HOME : ctrl_home();
								   break;

					 case CTRL_END : ctrl_end();
								  break;
					 case F10 : cursor();
								menu();
                                setviewport(12,58,maxx-13,maxy-26,1);
								moveto(col,row);
								cursor();
								break;
					 case F2 :  codefile();
								break;
					 case F4 :  decodefile();
								break;
					 case F8 :  cursor();
								set_ques();
								moveto(col,row);
								cursor();
								break;
					 case F7 :  cursor();
								set_stat();
                                moveto(col,row);
								cursor();
								break;
				 }
			}
			if (index > max_index )  max_index = index;
		}
}


/*************************************************************************
			   NAME: initialize();
			   FUNCTION: To initialize global variables.
			   LOCAL VARIABLES USED : None.
			   MACROS USED : None.
			   ROUTINES CALLED : None.
**************************************************************************/

void initialize()
{int i;

     create_cursor();
	 create_rubber();
	 setfillstyle(SOLID_FILL,WHITE);
	 setviewport(12,58,maxx-13,maxy-26,1);
	 setwritemode(XOR_PUT);
	 clearviewport();
	 moveto(col,row);
	 cursor();
	 maxline  = 1;
	 mode = index = max_index = currline = maxline = ins = 0;
}

/*************************************************************************
			   NAME: enter();
			   FUNCTION: To control the memory position when
				     "enter" is pressed.
			   LOCAL VARIABLES USED : i,max,rest.
			   MACROS USED : None.
			   GLOBAL VARIABLES: tempstr,index,currline,p,
					     maxline,max_index .
			   ROUTINES CALLED : down_arrow.
**************************************************************************/

void enter()
{
	int i,max,rest,row1;
	char *q;
	if (index == 0) status[0][currline] = 0;
	if (ins == 0)     /*INSERT ON*/
	{
			cursor();
			max = strlen(tempstr);
			rest = max-index;
			for(i = maxline;i >= currline;--i) BUFFER[i+1] = BUFFER[i];
			p = (char *)calloc(1,rest+1);
			test_memory(p);
			strcpy(p,&tempstr[index]);
			BUFFER[currline+1] = p;
			tempstr[index] = '\x0';
			p = (char *)calloc(1,index+1);
            test_memory(p);
			q = BUFFER[currline];
			free(q);
			BUFFER[currline] = p;
			strcpy(p,tempstr);
			currline++;
			if (row <= 241) row = row+14;
			++maxline;
			index = 0;
			col = 1;
			max_index = 0;
			for(i = 0;i <=  89; ++i) tempstr[i] = '\0x';
			strcpy(tempstr,BUFFER[currline]);
			if (row <= 241)
				{
					   if (currline != maxline){
										row = row-14;
										moveto(1,row);
										write_n_str(BUFFER[currline-1]);
										moveto(col,row);
										ins_line();
										row = row+14;
										write_n_str(BUFFER[currline]);
										moveto(col,row);
										cursor();
								}
					else {
						moveto(col,row);
						cursor();
					}
				}
			else {
					moveto(1,1);
					delete_line();
					row = row - 14;
					moveto(col,row);
				/*	write_n_str(BUFFER[currline]);
					row = row-14;
					moveto(col,row);
					delete_line();
					moveto(col,row);*/
					cursor();
			}
	}
	else down_arrow();
}

/*************************************************************************
			   NAME: write_default();
			   FUNCTION: To write when alphanum keys are pressed.
			   LOCAL VARIABLES USED : length,length_next,i,tempch.
			   GLOBAL VARIABLES USED : index,currline,ins,clength,
						   tempc,temp1.
			   MACROS USED : None.
			   ROUTINES CALLED : push().
**************************************************************************/

void write_default(void)
{
 int length,length_next,i,j,changeline;
 char *tempch;
 char *p,*q;
   status[0][currline] = -1;
	 if (index == 69)
   {        put_in_buffer();
			tempstr[index+1] = '\x0';
			index = 0;
			cursor();

			col = 1;
			moveto(col,row);
			ins_line();

			++currline;

			++maxline;
			changeline = maxline;
			for(i = changeline;i >= currline;--i)
					BUFFER[i+1] = BUFFER[i];


			tempch = (char *)calloc(1,90);
			test_memory(tempch);
			for(i = 69;i >= 50;--i)
					if (tempstr[i] == ' ') break;
			if (i != 49){
					strcpy(tempch,tempstr);
					tempch[i] = '\x0';
					length = 69 - i;
					BUFFER[currline] = tempstr;
					strcpy(BUFFER[currline],tempch);
					strcpy(tempstr,&tempstr[i+1]);
					strcpy(BUFFER[currline-1],tempch);
					tempstr[length] = '\x0';
			}
			else tempstr[0] = '\x0';

			free(tempch);
			write_n_str(BUFFER[currline-1]);
			if (row<241) row = row+14;
			setviewport(12,58+row,maxx-13,58+row+14,1);
			clearviewport();
			setviewport(12,58,maxx-13,maxy-26,1);
			 moveto(col,row);
			if (i != 49) {
			write_n_str(BUFFER[currline]);
			length = strlen(tempstr);
			col = length*10+1;
			moveto(col,row);
			cursor();
			index = length;
			return;
			}
			p = (char *)calloc(1,index +1);
            test_memory(p);
			q = BUFFER[currline];
			free(q);
			BUFFER[currline] = p;
			strcpy(BUFFER[currline],tempstr);
			length = strlen(tempstr);
			tempstr[length] = '\x0';
		}
   length = strlen(tempstr);
	 if (check_status())
		 return;
	 if (ins != 1)				/* INSERT ON MODE */
   {
		if(length <= 68)
		{
			if (index == length){
				put_in_buffer();
				max_index++;
				index++;
				tempstr[index] = '\x0';
				p = (char *)calloc(1,index+1);
                test_memory(p);
				strcpy(p,tempstr);
				q = BUFFER[currline];
				if (q) free(q);
				BUFFER[currline] = p;
				if (i == 49) {
						write_n_str(BUFFER[currline]);
						length = strlen(tempstr);
						col = length*10 +1;
						moveto(col,row);
						cursor();
						return;
				}
				cursor();
				writechar(ascii_val);
				col = col+10;
				moveto(col,row);
				cursor();
			}
		else push();
		}
     else {
			tempch = (char *)calloc(1,90);
			test_memory(tempch);
			for(i = 69 ; i >= 50 ; --i)
					if (tempstr[i] == ' ') break;
			if (i != 49) {
					length = strlen(tempstr);
					for (j = length;j >=  index;--j)
							tempstr[j+1] = tempstr[j];
					tempstr[index] = key.ch[0];
					strcpy(tempch,tempstr);
					*(tempch+(i+1)) = '\x0';
					strcpy(BUFFER[currline],tempch);
					strcpy(tempstr,&tempstr[i+2]);
					free(tempch);
					index = 0;					/*testing*/
					cursor();

					col = 1;
					moveto(col,row);
					ins_line();			/*new inserted for test*/

					write_n_str(BUFFER[currline]);
					++currline;

								/*new inserted for test*/
					++maxline;
					changeline = maxline;
					for(i = changeline;i>currline;--i)
					BUFFER[i+1] = BUFFER[i];

					strcpy(BUFFER[currline],tempstr);
					length = strlen(BUFFER[currline]);
					col = length*10 +1;
					if (row<241) row = row+14;
					setviewport(12,58+row,maxx-13,58+row+14,1);
					clearviewport();
					setviewport(12,58,maxx-13,maxy-26,1);
					write_n_str(BUFFER[currline]);
					moveto(col,row);
					cursor();

				}
			else {
				tempch = (char *)calloc(1,90);
				test_memory(tempch);
				strcpy(tempch,tempstr);
				*(tempch+68) = '\x0';
				strcpy(BUFFER[currline],tempch);
				strcpy(tempch,&tempstr[68]);
				strcpy(tempstr,BUFFER[currline]);
				push();
				strcpy(tempstr,tempch);
				free(tempch);
				index = 0;
				cursor();

				col = 1;
				moveto(col,row);
				ins_line();			/*new inserted for test*/

				++currline;

								/*new inserted for test*/
				strcpy(BUFFER[currline],tempstr);
				++maxline;
				changeline = maxline;
				for(i = changeline;i >= currline;--i)
						BUFFER[i+1] = BUFFER[i];
				tempstr[0] = '\x0';



				if (row<241) row = row+14;
				setviewport(12,58+row,maxx-13,58+row+14,1);
				clearviewport();
				setviewport(12,58,maxx-13,maxy-26,1);
				write_n_str(BUFFER[currline]);
				moveto(col,row);
				cursor();
				strcpy(tempstr,BUFFER[currline]);
				length = strlen(tempstr);
				tempstr[length] = '\x0';
			}
	 }

    }
    else
    {
		cursor();
		put_in_buffer();
		max_index++;
		index++;
		moveto(col,row);
		writechar(ascii_val);
		col = col+10;
		moveto(col,row);
		cursor();
	}
}
/*************************************************************************
				 NAME: put_in_buffer();
				 FUNCTION: To keep the value of pressed key in tempstr.
			   LOCAL VARIABLES USED : None.
				 GLOBAL VARIABLES USED:key
															 tempstr
															 index.
				 MACROS USED : None.
			   ROUTINES CALLED : None.
**************************************************************************/
void put_in_buffer()
{
	switch (mode)
	{
		case 0 : tempstr[index] = key.ch[0];
						 return;
		case 1:  if (((int)key.ch[0] >= 48)	&& ((int)key.ch[0] <= 57))															 /**subscript**/
							 {tempstr[index] = key.ch[0] + 80;
								return;
							 }
						 if (((int)key.ch[0] >= 65)	&& ((int)key.ch[0] <= 90))
							{tempstr[index] = key.ch[0] + 73;
								return;
							}
						 if (((int)key.ch[0] >= 97)	&& ((int)key.ch[0] <= 122))
							 {tempstr[index] = key.ch[0] + 67;
								return;
							 }
							break;
		case 2:
							if (((int)key.ch[0] >= 48)	&& ((int)key.ch[0] <= 57))
							 {tempstr[index] = key.ch[0] + 142;
								return;
							 }

							if (((int)key.ch[0] >=  65)	&& ((int)key.ch[0] <= 90))
							 {tempstr[index] = key.ch[0] + 135;
								return;
							 }

							if (((int)key.ch[0]  >= 97)	&& ((int)key.ch[0] <= 122))
							 {tempstr[index] = key.ch[0] + 129;
								return;
							 }
							break;
				}
}


/*************************************************************************
			   NAME: left_arrow();
			   FUNCTION: To control memory position with left
				     arrow.
			   LOCAL VARIABLES USED : None.
			   GLOBAL VARIABLES USED : index.
			   MACROS USED : None.
			   ROUTINES CALLED : None.
**************************************************************************/

void left_arrow()
{
 if (index != 0)
 {
	cursor();
	--index;
	col = col - 10;
	moveto(col,row);
	cursor();
 }
}

/*************************************************************************
			   NAME: right_arrow();
			   FUNCTION: To control memory position with right
				     arrow .
			   LOCAL VARIABLES USED : max.
			   GLOBAL VARIABLES USED : tempstr,index.
			   MACROS USED : None.
			   ROUTINES CALLED : None.
**************************************************************************/

void right_arrow()
{
 int max;
 max  =  strlen(tempstr);
 if (index < max)
 {
	cursor();
	++index;
	col = col+10;
	moveto(col,row);
	cursor();
 }
}

/*************************************************************************
			   NAME: up_arrow()
			   FUNCTION: To control memory position with up arrow.
			   LOCAL VARIABLES USED : currlength,postlength.
			   GLOBAL VARIABLES USED : currline,BUFFER,index,
						   tempstr.
			   MACROS USED : None.
			   ROUTINES CALLED : None.
**************************************************************************/

void up_arrow()
{
  int curlength;
  if (row>-11) row = row-14;
  if(currline != 0)
  {
	{
		cursor();
		if (currline != 0) --currline;
		curlength = strlen(BUFFER[currline]);
		if(curlength <= index)
			{
				tempstr[index] = '\0x';
				index = strlen(BUFFER[currline]);
			    col = index*10+1;
			    strcpy(tempstr,BUFFER[currline]);
			}
			else strcpy(tempstr,BUFFER[currline]);
  }
  if (row == -11) {
					row = row + 14;
					if (currline >= 0)
					{
									moveto(1,1);
									ins_line();
									moveto(1,1);
									write_n_str(BUFFER[currline]);
					}
					moveto(col,row);
					cursor();
  }
  else {
		moveto(col,row);
		cursor();
  }
 }
 if (row < 1 ) row = row + 14;
}

/*************************************************************************
			   NAME: down_arrow()
			   FUNCTION: To control memory with down arrow .
			   LOCAL VARIABLES USED : currlength,postlength.
			   GLOBAL VARIABLES USED : maxline,BUFFER,currline,
						   index,tempstr.
			   MACROS USED : None.
			   ROUTINES CALLED : None.
**************************************************************************/

void down_arrow()
{
 int curlength,postlength;
 if(currline < maxline)
 {
		cursor();
		++currline;
		if (row<265) row = row+14;
		curlength  =  strlen(BUFFER[currline]);
		if(curlength <= index)
			{
				index = strlen(BUFFER[currline]);
			    col = index*10+1;
			    strcpy(BUFFER[currline-1],tempstr);
			    strcpy(tempstr,BUFFER[currline]);
			}
			else strcpy(tempstr,BUFFER[currline]);
  if (row == 265) {
		 moveto(1,1);
		 delete_line();
		 row = row - 14;
		 moveto(1,row);
		 write_n_str(BUFFER[currline]);
		 moveto(col,row);
		 cursor();
  }
  else {
		moveto(col,row);
		cursor();
  }
 }
}
/*************************************************************************
				 NAME: changemode();
				 FUNCTION:To change writing mode.
				 LOCAL VARIABLES USED : none
				 GLOBAL VARIABLES USED : mode.
			   MACROS USED : None.
			   ROUTINES CALLED : None.
**************************************************************************/
void changemode()
{
	do {
			scanf("%d",&mode);
		 } while ( ( mode < 0 ) || (mode > 2));
}
/*************************************************************************
				 NAME: check_status();
				 FUNCTION:To check whether current line contains graphic work.
				 GLOBAL VARIABLES USED : currline.
			   MACROS USED : None.
			   ROUTINES CALLED : None.
**************************************************************************/
int check_status()
{
	if (status[0][currline] > 0) return 1;
	else return 0;
}
/*************************************************************************
			   NAME: home();
			   FUNCTION: To go to the begining of the line.
			   LOCAL VARIABLES USED : None.
			   GLOBAL VARIABLES USED : index.
			   MACROS USED : None.
			   ROUTINES CALLED : None.
**************************************************************************/

void home()
{
 cursor();
 index = 0;
 col = 1;
 moveto(col,row);
 cursor();
}

/*************************************************************************
			   NAME: end()
			   FUNCTION: To go to the end of the line.
			   LOCAL VARIABLES USED : max.
			   GLOBAL VARIABLES USED : tempstr,max.
			   MACROS USED : None.
			   ROUTINES CALLED : None.
**************************************************************************/

void end()
{
 int max;
 cursor();
 max = strlen(tempstr);
 index = max;
 col = index*10+1;
 moveto(col,row);
 cursor();
}

/*************************************************************************
			   NAME: ctrl_home()
			   FUNCTION: To go to the beginning of the file.
			   LOCAL VARIABLES USED : None.
			   GLOBAL VARIABLES USED : index,currline,tempstr,
						   BUFFER.
			   MACROS USED : None.
			   ROUTINES CALLED : None.
**************************************************************************/

void ctrl_home()
{
 cursor();
 index = 0;
 currline = 0;
 col = 1;
 row = 1;
 moveto(col,row);
 display_a_page(currline);
 col = 1;
 row = 1;
 strcpy(tempstr,BUFFER[0]);
 moveto(col,row);
 cursor();
}

/*************************************************************************
			   NAME: ctrl_end();
			   FUNCTION: To go to the end of the file .
			   LOCAL VARIABLES USED : None.
			   GLOBAL VARIABLES USED : maxline,index,currline.
			   MACROS USED : None.
			   ROUTINES CALLED : None.
**************************************************************************/

void ctrl_end()
{
 cursor();
 currline = maxline;
 index = strlen(BUFFER[currline]);
 col = 1;
 row = 1;
 if (currline > 18) {
		++index;
		currline = currline - 10;
		moveto(col,row);
		display_a_page(currline);
		col = index * 10 + 1;
		row = 109;
		currline = currline +10;
		strcpy(tempstr,BUFFER[currline]);
		moveto(col,row);
		cursor();
 }
 else {
		row = currline * 14 + 1;
		col = index * 10 + 1;
		strcpy(tempstr,BUFFER[currline]);
		moveto(col,row);
		cursor();
 }
}

/*************************************************************************
			   NAME: insert();
			   FUNCTION: To check insert mode ,on or off .
			   LOCAL VARIABLES USED : None.
			   GLOBAL VARIABLES USED : ins.
			   MACROS USED : None.
			   ROUTINES CALLED : None.
**************************************************************************/

void insert()
{
 ins = !ins;
}

/*************************************************************************
			   NAME: push();
			   FUNCTION: To insert a char in insert on mode.
			   LOCAL VARIABLES USED : length,i.
			   GLOBAL VARIABLES USED : tempstr,index,max_index,
						   currline,p.
			   MACROS USED : None.
			   ROUTINES CALLED : None.
**************************************************************************/

void push()
{
 int length,i;
 char *q;
 cursor();
 length = strlen(tempstr);
 for (i = length;i >=  index;--i) tempstr[i+1] = tempstr[i];
 tempstr[index] = key.ch[0];
 max_index++;
 index++;
 col = col+10;
 p = (char *)calloc(1,length+1);
 test_memory(p);
 tempstr[length+1] = '\x0';
 strcpy(p,tempstr);
 q = BUFFER[currline];
				free(q);
 BUFFER[currline] = p;
 write_n_str(BUFFER[currline]);
 moveto(col,row);
 cursor();
}




/*************************************************************************
			   NAME: delete_char();
			   FUNCTION: To del a char at current location.
			   LOCAL VARIABLES USED : None.
			   GLOBAL VARIABLES USED : index,j,tempstr,currline,
						   BUFFER,maxline.
			   MACROS USED : None.
			   ROUTINES CALLED : None.
**************************************************************************/

void delete_char()
{
 j = index;
 cursor();
 while(tempstr[j] != '\x0')
 {
	tempstr[j] = tempstr[j+1];
	++j;
 }
 tempstr[j] = '\x0';
 if (j == index)
 {
	clength = strlen(BUFFER[currline+1]);
	if ((69-strlen(BUFFER[currline]))>clength){
		strcat(BUFFER[currline],BUFFER[currline+1]);
		strcpy(tempstr,BUFFER[currline]);
	}
		for(count = currline+1;count <=  maxline;++count)
			BUFFER[count] = BUFFER[count+1];
		--maxline;
	moveto(1,row);
	delete_line();
 }
 strcpy(BUFFER[currline],tempstr);
 moveto(1,row);
 write_n_str(BUFFER[currline]);
 moveto(col,row);
 cursor();
}

/*************************************************************************
			   NAME: undelete();
			   FUNCTION: To restore a line previously deleted.
			   LOCAL VARIABLES USED : count.
			   GLOBAL VARIABLES USED : BUFFER,maxline,currline,loc,
						   index.
			   MACROS USED : None.
			   ROUTINES CALLED : None.
**************************************************************************/

void undelete()
{
 int count;
 cursor();
 for (count = maxline;count >=  loc;count--)
		 BUFFER [count+1] = BUFFER[count];
 BUFFER[loc] = dline;
 maxline++;
 if(currline >= loc)
		 currline +=  1;
 row = row+14;
 moveto(col,row);
 cursor();
}

/*************************************************************************
			   NAME: del_line();
			   FUNCTION: To delete a line.
			   LOCAL VARIABLES USED : counter.
			   GLOBAL VARIABLES USED : currline,tempstr,BUFFER,
						   index,max_index,dline.
			   MACROS USED : None.
			   ROUTINES CALLED : None.
**************************************************************************/

void del_line()
{
 int counter;
 cursor();
	if ((currline == 0) && (maxline == 0))
		{
		 index = 0;
		 max_index = 0;
		 tempstr[0] = '\x0';
		 return;
		}
	else
		{ dline = BUFFER[currline];
		  loc = currline;
		  if ( currline !=  maxline)
				for ( counter = currline;counter < maxline; ++counter)
					BUFFER[counter] = BUFFER[counter + 1];
			 else
				currline -=  1;
			 maxline--;
			 max_index = strlen(BUFFER[currline]);
			 tempstr[0] = '\x0';
			 strcpy(tempstr,BUFFER[currline]);
			 if (index > max_index)
				 index = max_index;
		}
	BUFFER[maxline+1] = '\x0';
   moveto(1,row);
   delete_line();
   col = index*10+1;
   moveto(col,row);
   cursor();
}

/*************************************************************************
			   NAME: pg_up();
			   FUNCTION: To go one page up.
			   LOCAL VARIABLES USED : None.
			   GLOBAL VARIABLES USED : currline,index,BUFFER,
						   max_index.
			   MACROS USED : PAGE_LINE_NUM.
			   ROUTINES CALLED : None.
**************************************************************************/

void pg_up()
{
 int row1,col1;
	cursor();
	if (currline - PAGE_LINE_NUM <= 0 ){
		 currline = 0;
		 index = 0;
		 col = 1;
		 row = 1;
		 display_a_page(currline);
		 col = 1;
		 row = 1;
		 moveto(col,row);
		 cursor();
		 strcpy(tempstr,BUFFER[currline]);
	}
	else {
		 currline = currline - PAGE_LINE_NUM;
		 row1 = row;
		 col1 = col;
		 max_index = strlen(BUFFER[currline]);
		 row = 1;
		 moveto(1,1);
		 display_a_page(currline);
		 row = row1;
		 col = col1;
		 currline = currline+(row/14);
		 strcpy(tempstr,BUFFER[currline]);
		 if (strlen(BUFFER[currline]) < index){
			index = strlen(BUFFER[currline]);
			col = index*10+1;
		 }
		 moveto(col,row);
		 cursor();
	}
}

/*************************************************************************
			   NAME: pg_down();
			   FUNCTION: To go one page down.
			   LOCAL VARIABLES USED : None.
			   GLOBAL VARIABLES USED : currline,index,BUFFER,maxline
						   tempstr,max_index.
			   MACROS USED : PAGE_LINE_NUM.
			   ROUTINES CALLED : None.
**************************************************************************/

void pg_down()
{
  cursor();
	if ((currline+PAGE_LINE_NUM)<maxline)
		currline = currline + PAGE_LINE_NUM;
	strcpy(tempstr,BUFFER[currline]);
	if (strlen(BUFFER[currline]) < index) index = strlen(BUFFER[currline]);
	if (currline>18) display_a_page(currline);
	col = 1;
	row = 1;
	moveto(col,row);
	cursor();
}

/*************************************************************************
			   NAME: block();
			   FUNCTION: To do block operations (copy,del,move).
			   LOCAL VARIABLES USED : mark1,mark2,diff,counter.
			   GLOBAL VARIABLES USED : currline,maxline,BUFFER,p.
			   MACROS USED : None.
			   ROUTINES CALLED : None.
**************************************************************************/

void block()
{
 static int mark1,mark2,diff,counter;
 char *q;

	switch(key.ch[0])
	{
	 case '\v' : mark1  =  currline;   /*^k*/
			   break;

	 case '\x5' : mark2  =  currline;   /*^e*/
			    break;

	 case '\x3' : diff  =  mark2 - mark1 +1;  /*^c*/
			    for (counter  =  maxline;counter > currline;--counter)
			    {
						p = (char *) calloc(1,90);
                        test_memory(p);
						strcpy(p,BUFFER[counter]);
						q  =  BUFFER[counter + diff];
						free(q);
						BUFFER[counter + diff]  =  p;
				}
				for(counter = currline;mark1 != mark2+1;++mark1,++counter)
				{
						p = (char *) calloc(1,90);
                        test_memory(p);
						strcpy(p,BUFFER[mark1]);
                        q  =  BUFFER[counter + diff];
						free(q);
						BUFFER[counter] = p;
				}
				maxline += diff;
				strcpy(tempstr,BUFFER[currline]);
				break;

	 case '\x4' : for(counter  =  mark1;counter <=  maxline - diff;++counter) /*d*/
					BUFFER[counter]  =  BUFFER[counter + diff];
				maxline -= diff;
				break;

	 }
}

/*************************************************************************
			   NAME: back_space()
			   FUNCTION: To del one char before the cur position.
			   LOCAL VARIABLES USED : None.
			   GLOBAL VARIABLES USED : index,currline,BUFFER,tempc,
						   maxline,count.
			   MACROS USED : None.
			   ROUTINES CALLED : delete_char().
**************************************************************************/

void back_space()
{
 int clength,clength1;
 char *q;
 --index;
 if (index<0){
		cursor();
		clength = strlen(BUFFER[currline]);
		clength1 = strlen(BUFFER[currline-1]);
		if ((69-strlen(BUFFER[currline-1]))>clength){
			strcat(BUFFER[currline-1],BUFFER[currline]);
			tempstr[0] = '\x0';
		}
		for(count  =  currline;count <=  maxline;++count)
			BUFFER[count]  =  BUFFER[count+1];
		--maxline;
		if (row>1) row = row-14;
		index = clength1;
		col = clength1*10+1;
		moveto(1,row);
		write_n_str(BUFFER[currline-1]);
		moveto(col,row);
		cursor();
		--currline;
		strcpy(tempstr,BUFFER[currline]);
		moveto(1,row+14);
		delete_line();
		moveto(col,row);
		cursor();
    }
 else {delete_char();
	cursor();
	if (col>1) col = col-10;
	moveto(col,row);
	cursor();
 }
}

/****************************************************************************
	   NAME : Display_A_Page(int Start_line).

	   FUNCTION : It displays a page containing 20 lines, starting from
		      a given line.

	   VARIABLES USED :
			   start_line  =  The starting line of the page.
			   Dummy[70]  =  A dummy string that contains current
				       line temporarily.
			   last_line  =  It indicates the line no. upto which
				       the function should display.
			   y  =  It stores the y co-ordinate of screen.
			   *bitmap  =  It stores the bit-image of a graphic
				     line pointed by BUFFER[start_line].

	   ROUTINES  NEEDED : graphics.h , string.h .
****************************************************************************/

void display_a_page(int start_line)
{
    static unsigned char Dummy[70] ;
	int last_line, y  =  0, Dummy_len  =  70 ;
    char *bitmap ;
		clearviewport() ;
    moveto(col,row);
	last_line  =  start_line + 18;   /* Maximum Line on screen is assumed to
				       be 18 */
    if ( last_line > maxline )
       last_line  =  maxline ;
    do {
	   if ( status[0][start_line] <=0)
       {
	   for ( ; Dummy_len >=  0 ; --Dummy_len)
	      Dummy[Dummy_len]  =  '\x0' ;
	   strcpy( Dummy, BUFFER[start_line] ) ;
	   Dummy_len  =  strlen(Dummy) ;
	   write_n_str(Dummy) ;   /* PRINT ACC. TO CURR_FONT */
	   row = row+14;
	   y+=14;
	   }
       else
       {
		putimage(0,y,BUFFER[start_line],COPY_PUT) ;
		row +=14;
		y+=14;
       }
	   start_line++;
	} while ( start_line <=  last_line ) ;
}
/***************************************************************************

		NAME : write_n_str(char *)
		FUNCTION : it prints a line in current position
		ARGUMENT VARIABLES  : st takes a string to print
		LOCAL VARIABLES :lcv : loop  control  variable;
		FUNCTION CALLED : writechar(char) :- it prints the line
							using a character
							at a time.
		VALUE RETURNED : none

***************************************************************************/

void write_n_str(char  *st)
{
	int    lcv,col1,row1;

	setviewport(12,row+58,maxx-13,row+58+14,0);
	clearviewport();
	setviewport(12,58,maxx-13,maxy-26,0);
	col1 = 1;
	row1 = row;
	moveto(col1,row1);
	lcv = 0;
	if  (status[0][currline] <= 0)
	{
		while ( st[lcv] )
		{
			writechar((unsigned  char) st[lcv]);
			col1 = col1+10;
			moveto(col1,row1);
			lcv++;
		}
	}
	else
		putimage(1,gety(),BUFFER[currline],COPY_PUT);
}

/***************************************************************************

			NAME: cursor(int x,int y)
			FUNCTION : To draw a cursor.
			LOCAL VARIABLE : None.
			GLOBAL VARIABLE : None.
			ROUTINES CALLED : None.

***************************************************************************/
void cursor()
{
 int x,y;
 x = getx();
 y = gety();
 line(x,y+12,x+8,y+12);
 line(x,y+13,x+8,y+13);
}

void delete_line()
{
	int  x,y;
	void	*pa;

	y = gety();
	x = getx();
	if (row == 253) ++marker;
	else marker = 0;
	if ((marker == 1) && (key.ch[1] == 80)) ++currline;
	pa = (void *)calloc(1,imagesize(1,1,maxx-30,maxy-96));
	test_memory((char *)pa);
	getimage(1,y+12,maxx-30,maxy-85,pa);
	putimage(1,y,pa,COPY_PUT);
	free(pa);
	moveto(x,y);
}

void  clear_line(int y)
{
	setviewport(1,y+70,maxx-10,y+70+14,0);
	clearviewport();
	setviewport(10,70,maxx-10,maxy-26,0);
}

void writechar(unsigned  char ascii_code)
{
	unsigned	int lcv,tempx,tempy,number,subst;
	void  *p;
	char change[16];

	 p = (char *) calloc (1,200);
	 tempy = gety();
	 tempx = getx();
	 getimage(tempx,tempy,tempx+10,tempy+14,p);
	 putimage(tempx,tempy,p,XOR_PUT);
	 change[0] = 0x7;
	 change[1] = 0x0;
	 change[2] = 0xb;
	 change[3] = 0x0;
	 for (lcv = 0;lcv<12;lcv++)
		change[lcv+4]  =  normal_font[ascii_code][lcv];
	 putimage(tempx,tempy,change,COPY_PUT);
	 putimage(tempx+1,tempy,change,OR_PUT);
	 moveto(getx()+10,gety());
	 free(p);
}

void  ins_line()
{
	void	*pa;
	unsigned   y;
	y  =  gety();
	if (row !=  1) y = y+14;
	pa = (void *)calloc(1,imagesize(1,1,maxx-30,maxy-96));
	test_memory((char *)pa);
	getimage(1,y,maxx-30,maxy-96,pa);
	putimage(1,y+14,pa,COPY_PUT);
	moveto(1,y);
	free(pa);
}

void setup()
{
 int driver = DETECT,mode;
 FILE *fp;
 char filename[25] = "QUIZ.FNT";

 initgraph(&driver,&mode,"");
 buf_initialize();
 maxx = getmaxx();
 maxy = getmaxy();
 fp = fopen(filename,"rb");
 fread(normal_font,sizeof(normal_font),1,fp);
 setfillstyle(SOLID_FILL,WHITE);
}

void buf_initialize()
{
 int i;
 for(i = 0;i<maxline;++i) if (BUFFER[i]) free(BUFFER[i]);
 for(i = 0;i<200;++i) BUFFER[i]  =  NULL;
 for(i = 0;i<200;++i) status[0][i] = status[1][i] = 0;
 for(i=0;i<100;i++) {
	if (minibuf[i]) free(minibuf[i]);
	minibuf[i] = NULL;
 }
}

void test_memory(char *memo)
{
	if (!memo){
			clearviewport();
			moveto(300,40);
			outtext("OUT OF MEMORY..ABORTING.");
			getch();
	}
}

void write_col_row(void)
{
	setviewport(12,38,maxx -14,53,0);
	moveto(30,6);
	outtext("COL : ");
	moveto(120,6);
	outtext("LINE : ");
	moveto(220,6);
	outtext("INSERT MODE : ");
	moveto(420,6);
	outtext("FILE NAME : ");
}


void write_col (void)
{
	int x1,y1,xpos;
	char temp[6];
    struct viewporttype vport;
		x1 = getx();
		y1 = gety();
		getviewsettings(&vport);
		setviewport(90,38,130,53,0);
		clearviewport();
		xpos = col/10;
		++xpos;
		itoa(xpos,temp,10);
		moveto(2,6);
		outtext(temp);
		setviewport(vport.left,vport.top,vport.right,vport.bottom,vport.clip);
		moveto(x1,y1);
}



void write_line (void)
{
	int x1,y1;
	char temp[6];
	int line;
    struct viewporttype vport;
	x1 = getx();
	y1 = gety();
	getviewsettings(&vport);
	setviewport(190,38,230,53,0);
	clearviewport();
	line  =  currline +1;
	itoa(line,temp,10);
	moveto(2,6);
	outtext(temp);
	setviewport(vport.left,vport.top,vport.right,vport.bottom,vport.clip);
	moveto(x1,y1);
}

void insert_mode(void)
{
	int x1,y1;
    struct viewporttype vport;
	x1 = getx();
	y1 = gety();
	getviewsettings(&vport);
	setviewport(360,38,400,53,0);
	clearviewport();
	moveto(2,6);
	if (ins == 0) outtext("ON");
	else outtext("OFF");
	setviewport(vport.left,vport.top,vport.right,vport.bottom,vport.clip);
	moveto(x1,y1);
}

void find_drive_dir_filename(void)
{
	char dir[MAXDIR],drive[4];
	char *curfile = "\\NONAME.C";
	int length;
    int x1,y1;
	char *temp;
	struct viewporttype vport;
	x1 = getx();
	y1 = gety();
	getviewsettings(&vport);
	setviewport(530,38,690,53,0);
	clearviewport();
	moveto(2,6);
	length = strlen(curfile);
	temp = (char *)calloc(1,MAXDIR + length);
	getcurdir(0,dir);
	drive[0] = getdisk()+'A';
	drive[1] = ':';
	drive[2] = '\\';
	drive[3] = NULL;
	strcpy(temp,drive);
	strcat(temp,dir);
	strcat(temp,curfile);
	outtext(temp);
	free(temp);
	setviewport(vport.left,vport.top,vport.right,vport.bottom,vport.clip);
	moveto(x1,y1);

}

/**************************************************************************/
      /*
	 NAME         : menu
	 FUNCTION     : to create menu
	 MACRO USED   : none
	 ROUTIN CALLED: submenu, reverse.
      */

void menu() {  /* MALEQ */
	 char option[7][12] = {"File","Graphics","foNt","Option","Setup","Calculator","Help"};
	       /* option of manin menu in an array */
	 char ch, low_letter[7] = "fgnosch";/* all cap letter of option */
     char sub_menu = 'F'; /* for checking user in submenu or main menu */
     int i, startx[]  =  {35,92,178,233,306,368,474,523};
		  /* starting point of main menu option in pixel */
	 setviewport(0,0,maxx,maxy,1);
	 reverse(35,7,91,22,option[0],'W');
		  /* to make back ground of first option of main menu */
     for (i = 0; ;) { /* to get corect input for choice */
GETCH:	 ch  =  tolower(getch());
	 if (ch == 13 || ch == 27 || (strchr(low_letter,ch) && ch != 0)) break;
	     /* if input is enter(13) or Esc(27) or cap letter of options
		input is correct and break the for loop */
	 if (ch == 0) {       /* if any extended key pressed */
	    ch  =  getch();   /* read upper byte of extended key */
		if (sub_menu == 'T' && (ch == 77 || ch == 75)) goto OPTION;
	       /* if user in sebmenu and left(75) or right(77) arrow pressed
			then to sending user next submenu goto OPTION which
			control submenu */
	    switch (ch) {
		   case 75: /* left arrow */
			reverse(startx[i],7,startx[i+1]-1,22,option[i],'B');
			    /* to change background of previous option */
			if (i == 0) i  =  6; /*if corsur in left most option then
					goto right most option */
			else --i;
			reverse(startx[i],7,startx[i+1]-1,22,option[i],'W');
			       /* chage back ground of new choosen option */
			break;
		   case 77:  /*
		   right arrow */
			reverse(startx[i],7,startx[i+1]-1,22,option[i],'B');
                             /* to change background of previous option */
			if (i == 6) i  =  0;
				  /*if corsur in right most option then
					goto left most option */
			else ++i;
			reverse(startx[i],7,startx[i+1]-1,22,option[i], 'W');
                              /* chage back ground of new choosen option */
			break;
	    }
		if (ch == 80 && i<5) {    /* if down arrow pressed and and choosen
			  option is in file to set up which has submenu work
			  as enter i.e. work as ch = 13 */
	       ch = 13;
	       break;
	    }
	 }
     }
	 if (ch == 27) reverse(startx[i],7,startx[i+1]-1,22,option[i], 'B');
	/* if Esc is pressed change the back ground from white to black */
     else {
	  if (ch != 13){ /* if ch! = 13, ch must lie in low_letter i.e. ch is
	  any one of the cap letter of optoins */
		 reverse(startx[i],7,startx[i+1]-1,22,option[i], 'B');
		    /* chage back ground of previous option */
		 for (i = 0; ch != low_letter[i]; i++);
		   /* find position of pressed cap letter */
		 if (i<5) reverse(startx[i],7,startx[i+1]-1,22,option[i], 'W');
		    /* chage back ground of new choosen option */
	  }
CALL:	  switch (i) {
		 case 0: ch = submenu(0,30,25, "Load,New,Save,Dir,lisT,Exit");
				/* calling submenu for main menu file */
		      break;
		 case 1: ch = submenu(6,87,25, "sine Wave,Rectanguler wave,Freehand,Edit,Symbol");
				/* calling submenu for main menu graphics */
		      break;
		 case 2: ch = submenu(11,175,25, "Font1,fonT2");
			     /* calling submenu for main menu font */
		      break;
		 case 3: ch = submenu(13,230,25, "Print,Option2");
			     /* calling submenu for main menu option */
		      break;
		 case 4: ch = submenu(15,305,25, "Setup,Question info");
			     /* calling submenu for main menu setup */
		      break;


		      /* function caclator() and help() has no sub menu so
			 no sub menu is called but if left arrow(75) or right
			 arrow(77) pressed  then read akey pressed to do this
			 goto in the for loop by jumping calling label GETCH
			 ELSE call function calculator or help */
		 case 5: if (ch == 77 || ch == 75) {
			    sub_menu = 'T';
			    goto GETCH;
			 }
			 /*calculator();*/
		      break;
		 case 6: if (ch == 77 || ch == 75) {
			    sub_menu = 'T';
			    goto GETCH;
			 }
			 help();
			 break;
		  }
OPTION:	setviewport(0,0,maxx,maxy,1);
		switch (ch) { /* ch is returned by function submenu */
	   case 75:  /* left arrow */
		   reverse(startx[i],7,startx[i+1]-1,22,option[i],'B');
		   if (i == 0) i  =  6;
	       else i--;
		   reverse(startx[i],7,startx[i+1]-1,22,option[i],'W');
	       goto CALL;
	   case 77:   /* right arrow */
		   reverse(startx[i],7,startx[i+1]-1,22,option[i],'B');
		   if (i == 6) i  =  0;
	       else i++;
		   reverse(startx[i],7,startx[i+1]-1,22,option[i],'W');
	       goto CALL;
	   case 27:  /* Ecs, out of submenu so sub_menu  =  'F'(false)  */
	       sub_menu = 'F';
	       goto GETCH;
	   case 13: /* enter */
		   reverse(startx[i],7,startx[i+1]-1,22,option[i], 'B');
	  }
      }
      return;
}


/**************************************************************************/
      /*
	 NAME         : submenu
	 FUNCTION     : to create submenu
	 MACRO USED   : none
	 ROUTIN CALLED: box, reverse.
      */


char submenu(int begin, int x, int y, char str[]) {   /* MALEQ */
     int Y = 9,i, k, number = 1; /* to count number of option */
     int max_width = 0; /*maximum lenth of options */
     int cap_position[10]; /* to store capital letter's position of options*/
     char option[10][30], ch, caps  =  'F', cap_letter[10];
     void *scr;
     struct viewporttype vport;

/** seperating & printing funtion name and finding cap letter and it's position */
/*------------------------------------------------------------------------------*/
     for (i = 0, k = 0; str[i]; i++) {
	 if (caps == 'F') ++cap_position[number-1]; /* to store cap position */
	 if (str[i] == ',') {
	    option[number-1][k]  =  0; /* terminate option by null */
	    if (max_width<strlen(option[number-1])) max_width  =  strlen(option[number-1]);
			/* finding maximum length of option */
	    ++number;
		caps  = 'F'; /* initialize caps by 'F' for next option */
	    k = 0;    /* initialize k by 0 for next option */
	 }
	 else option[number-1][k++]  =  str[i];
	 if (str[i] >= 65 && str[i] <= 90) { /* if str[i] is a capital letter */
	    caps  =  'T';
	    cap_letter[number-1] =  str[i];
	 }
     }
     option[number-1][k]  =  0; /* terminate last option by NULL(0) */
     if (max_width<strlen(option[number-1])) max_width  =  strlen(option[number-1]);
		/* checking maximum length for last option */
     cap_letter[number]  = 0 /*NULL*/;
	 scr  =  calloc(1,imagesize(x-3, y, x+max_width*8+21, y+number*14+Y));
	 getimage(x-3, y,x+max_width*8+21, y+number*14+Y, scr);
	  /* getimage of the area in which options of menu is written */
	 getviewsettings(&vport);
	  /* get current viewport seting. set viewport for the area in which
	  options are written and clear this area after doing this again set
	  initial viewport */
	 setviewport(x-3, y, x+max_width*8+21, y+number*14+Y, 1);
     clearviewport();
	 setviewport(vport.left,vport.top,vport.right,vport.bottom,vport.clip);

	    /* print option */
	 for (i = 0; i<number; i++) outtextxy(x+10, y+i*14+7, option[i]);
	 box(x, y+1, x+max_width*8+18, y+number*14+6);
	    /* a box around the options */

		/**** selection *****/
     reverse(x+5, y+4, x+max_width*8+13, y+15, option[0], 'B');
     for (i = 0; ;) {
	 ch  =  toupper(getch());
	 if (ch == 13 || ch == 27 || (strchr(cap_letter,ch) && ch != 0)) break;
	 if (ch == 0) {
	    switch (ch = getch()) {
		case 72:
			 reverse(x+5, y+i*14+4, x+max_width*8+13, y+i*14+15, option[i], 'W');
			 if (i == 0) i  =  number - 1;
		     else --i;
			 reverse(x+5, y+i*14+4, x+max_width*8+13, y+i*14+15, option[i], 'B');
		     break;
		     case 80:
			  reverse(x+5, y+i*14+4, x+max_width*8+13, y+i*14+15, option[i], 'W');
			  if (i == number-1) i  =  0;
			  else ++i;
			  reverse(x+5, y+i*14+4, x+max_width*8+13, y+i*14+15, option[i], 'B');
			  break;
	    }
		if (ch == 75 || ch == 77) break;
	 }
     }
    putimage(x-3,y, scr, COPY_PUT);
	free(scr);
    setviewport(12,58,maxx-13,maxy-26,1);
	if (ch != 27 && ch != 75 && ch != 77) {
		if (ch != 13) for (i = 0; ch != cap_letter[i]; i++);
		(*function[begin+i])();
	 }
     return ch;
}


/**************************************************************************/
      /*
	 NAME         : reverse
	 FUNCTION     : to reverse the back ground of given area and string
	 MACRO USED   : none
	 ROUTIN CALLED: none.
      */

void reverse(int x1, int y1, int x2, int y2, char *scr, char color) {
	 if (color == 'W') {
	struct viewporttype vport;
	getviewsettings(&vport);
	setviewport(x1, y1, x2, y2, 1);
	clearviewport();
	outtextxy(5,2,scr);
    setviewport(vport.left,vport.top,vport.right,vport.bottom,vport.clip);
     }
     else {
	  bar(x1,y1,x2,y2);
	  setcolor(BLACK);
	  outtextxy(x1+5,y1+2,scr);
	  setcolor(WHITE);
	 }
}

/**************************************************************************/
      /*
	 NAME         : screen
	 FUNCTION     : to nake screen area
	 MACRO USED   : none
	 ROUTIN CALLED: box.
      */


void screen() {
	 bar(9,0,maxx-11,26);        /* to print a box around menu-head-line*/
	 box(9,35,maxx-11,maxy-18);      /* to print a box around edit area*/
	 setcolor(BLACK);
	 outtextxy(40,9,"File   Graphics   foNt   Option   Setup   Calculater   Help");
				  /* to print menu-head-line on the screen*/
	 setcolor(WHITE);
	 line(9,56,maxx-11,56);
	 line(9,33,maxx-11,33);
	 line(9,34,10,34);
	 line(maxx-11,34,maxx-12,34);
	 outtextxy(300,maxy-8,"F10-MENU");
	 return;
}


/**************************************************************************/
      /*
	 NAME         : box
	 FUNCTION     : to make a box by given coordinate
	 MACRO USED   : none
	 ROUTIN CALLED: box.
      */
void box(int x1,int y1,int x2,int y2) {
	 rectangle(x1,y1,x2,y2);
				 /* to print a ractangle where box is needed */
	 line(x1+1,y1,x1+1,y2);
				 /* to print a line one pixel right of left arm to
					make it thick */
	 line(x2-1,y1,x2-1,y2);
				 /* to print a line one pixel left of right arm to
					make it thick */
}




      /* some function are made for testing performanec of menu */
      /*--------------------------------------------------------*/
/***/
	void save(){codefile();}
	void load() {
				 currline = 0;
				 buf_initialize();
				 decodefile();
				 currline = 0;
				 display_a_page(currline);
				 currline  = 0;
				 index = 0;
				 strcpy(tempstr,BUFFER[currline]);
				 col = 1;
				 row = 1;
				 moveto(col,row);
				 }
	void new() {
	}
	void dir() { outtextxy(100,200,"You have choosen dir");}
	void list() { outtextxy(100,200,"You have choosen list");}
	void quit() { buf_initialize();closegraph();exit(0);}
	void edit() { ;	}
	void symbol_call() {
		if(!set_graph_area()) return ;
		cur_x = lx;
		cur_y = ty;
		symbol();
		store_image();
		}
	void f1() { outtextxy(100,200,"You have choosen f1");}
	void f2() { outtextxy(100,200,"You have choosen f2");}
	void o1() { outtextxy(100,200,"You have choosen o1");}
	void o2() { outtextxy(100,200,"You have choosen o2 ");}
	void help() { setcolor(BLACK);outtextxy(100,200,"You have choosen          help");setcolor(WHITE);}
/***/

/****			GIVEN BY MAMUNUL ISLAM		****/
/* This function is to be put in the main program . Two global variables
   gline(int) and i_size(int) are used. One extern function error_msg(..)
   is also used . Use the next line in the initialize function
   */


set_graph_area( void )
{
/*   NAME : set_graph_area
     FUNCTION : This function is used to set the area to work in graphics
	mode. It draws a imaginary rectangle around the current workspace
	and the user can  change the  area by using the cursor keys. This
	funtion returns 0 if the job is abandoned ,if not it returns 1.
     EXAMPLE : set_graph_area()
     GLOBAL VARIABLES USED : gline, row, maxx, maxy, currline, status, lx,
	rx, ty, by
     LOCAL VARIABLES USED : i, ch
     MACROS USED : UP_ARROW, DOWN_ARROW
     ROUTINES CALLLED : rectangle, error_msg, setviewport, getch.
     ROUTINE DEVELOPED BY : Md. Mamunul Islam .
*/

char ch;
int i;

gline  =  currline;
setviewport( 0, 0, maxx, maxy, 0);
if (status[0][gline]  ==  -1) {
	error_msg("You are on text");
        setviewport( 12, 58, maxx - 13, maxy - 26, 0);
	return 0;
	}
lx = 13;
rx = lx+10;
ty = row + 57; /******** edit here ********/
by = ty + 13;
do {
	rectangle( lx, ty, rx, by);
	ch = getch();
        rectangle( lx, ty, rx, by);
	if( !ch )
		switch( getch() ) {
			case UP_ARROW : if ( by > row+69 ) {
						by -= 14;
						gline--;
					       }
					break;
			case DOWN_ARROW :
			if(( by < maxy-27 ) && (gline < 199) && (status[0][gline+1] != -1)) {
				gline++;
				by += 14;
				}
			break;
			case RIGHT_ARROW : if (rx+10 <= maxx-13) {
								rx += 10;
								}
								break;
			case LEFT_ARROW : if (rx-10 >= lx) {
								rx -=10;
								}
								break;
		}
   } while( (ch != 13) && (ch != 27) );
setviewport( 12, 58, maxx - 13, maxy - 26, 0); /**** edit *****/
if (ch  == 13 ) return 1;
   else return 0;
}

store_image( void )
{
/*   NAME : store_image
     FUNCTION : This function is used to save the drawings in the graphics
	work area . It returns 1 if successful , otherwise 0.
     EXAMPLE : store_image()
     GLOBAL VARIABLES USED : currline, maxline, maxx, maxy, gline, ty, by,
	 row, status, BUFFER.
     LOCAL VARIABLES USED : i, add, err
     MACROS USED : NULL
     ROUTINES CALLLED : free, calloc, setviewport, getimage, error_msg.
     ROUTINE DEVELOPED BY : Md. Mamunul Islam .
*/
int i,j, add,i_size;
char err = 0;

i_size = imagesize(lx,0,rx,13);
for ( i = currline; i <= gline; i++) {
	if ( status[0][i] > 0 ) free( BUFFER[i] );
	if (( BUFFER[i]  =  (char *) calloc(1,i_size ) )  ==  NULL) {
		err  =  1;
		gline  =  i;
		break;
		}
	}
setviewport( 0, 0, maxx , maxy, 0);
for ( i  =  currline; i <= gline; i++) {
	status[0][i]  = i_size;
	add = (i - currline) * 14;
	getimage( lx, ty + add, rx, ty + add + 13, BUFFER[i]);
	}
setviewport( 12, 58, maxx - 13, maxy - 26, 0); /**** edit *****/
if ( gline > maxline) maxline = gline;
if( err ) {
	error_msg("Not enough space in RAM");
	return 0;
       }
++maxline;
col = 1;
return 1;
}

void set_stat(void)
/*
	Function : set_stat
	Purpose  : Take input of status of examination such as exam time,
			   save time & indecator of auto save.The inputs are then
			   stored in a small buffer named minibuf for further saving
			   in the question file.
	Programmed by : Masud Karim Khan
*/
{
	char t_time[5],s_time[3],auto_save;
	int i,j,ind=0,digit,pos[5][2]={471,120,455,150,392,180,222,215,470,215};
	char ch,dummy[15],*tempstr;
	void *dump;

	setviewport(0,0,maxx,maxy,0);
	strcpy(dummy,minibuf[0]);
	t_time[4] = s_time[2] = 0;
	if (minibuf[0]) {
		for (i=0,j=3;i<4;i++,j++) t_time[i] = dummy[j];
		digit = atoi(t_time);
		digit /= 60;
		itoa(digit,t_time,10);
		for (i=0,j=8;i<2;i++,j++) s_time[i] = dummy[j];
		auto_save = dummy[11];
	}
	else {
		for (i=0;i<4;i++) t_time[i] = '0';
		for (i=0;i<2;i++) s_time[i] = '0';
		auto_save = '0';
	}
	dump = (void *)calloc(1,imagesize(151,81,600,250));
	getimage(151,81,600,250,dump);
	dialog_box(151,81,600,250,"Examinatin Settings");
	setcolor(BLACK);
	outtextxy(200,120,"Examination Time (Minutes):");
	outtextxy(200,150,"Saving Time (Minutes):");
	outtextxy(200,180,"Autosave is currently");
	if (auto_save == '0') outtextxy(200+24*8,180,"Off");
	else outtextxy(200+24*8,180,"On");
	setcolor(WHITE);
	setfillstyle(SOLID_FILL,BLACK);
	bar(430,115,520,130);
	bar(430,145,520,160);
	bar(200,210,260,225);
	bar(460,210,520,225);
	outtextxy(440,120,t_time);
	outtextxy(440,150,s_time);
	outtextxy(222,215,"Ok");
	outtextxy(470,215,"Escape");
	setfillstyle(SOLID_FILL,WHITE);
	moveto(pos[ind][0],pos[ind][1]);
	do {
		mycursor();
		ch=getch();
		if (!ch) ch = getch();
        mycursor();
		if (isdigit(ch) && (ind==0 || ind==1)) {
			ungetch(ch);
			moveto(440,pos[ind][1]);
			tempstr = getstring(1);
			if (ind) strcpy(s_time,tempstr);
			else strcpy(t_time,tempstr);
		}
		else {
		switch (ch) {
			case RIGHT_ARROW :
			case DOWN_ARROW  :	ind++;
								if (ind == 5) ind = 0;
                                moveto(pos[ind][0],pos[ind][1]);
								break;
			case UP_ARROW    :
			case LEFT_ARROW  :	ind--;
								if (ind<0) ind = 4;
                                moveto(pos[ind][0],pos[ind][1]);
								break;
			case BACKSPACE   :  if (ind==0 || ind == 1) {
									moveto(440,pos[ind][1]);
									setfillstyle(SOLID_FILL,BLACK);
									bar(430,115+ind*30,520,130+ind*30);
                                    setfillstyle(SOLID_FILL,WHITE);
								}
								break;
			case ENTER       :  if (ind==2) {
    								bar(390,175,430,190);
									setcolor(BLACK);
									if (auto_save=='0') {
										auto_save='1';
										outtextxy(200+24*8,180,"On");
									}
									else {
										auto_save='0';
										outtextxy(200+24*8,180,"Off");
									}
									setcolor(WHITE);
								}
								break;
			case 'o'		 :
			case 'O'		 :  ind=3;ch=13;break;
			case ESC		 :
			case 'e'         :
			case 'E'         :  ind=4;ch=13;break;
		}
		}
	}while (!((ind==3 ||ind==4) && ch==13));
	putimage(151,81,dump,COPY_PUT);
	free(dump);
	if (ind==3) {
	for (i=0;i<12;i++) dummy[i] = '0';
	digit = atoi(t_time);
	digit *= 60;
	itoa(digit,t_time,10);
	for (i=6,j=strlen(t_time)-1;j>=0;i--,j--)
		dummy[i] = t_time[j];
	for (i=9,j=strlen(s_time)-1;j>=0;i--,j--)
		dummy[i] = s_time[j];
	dummy[12] = 0;
	dummy[0] = 254;
	dummy[11] = auto_save;
	dummy[1] = 'S';
	dummy[2] = 'L';
	dummy[7] = 'E';
	dummy[10] = 'A';
	if (minibuf[0]) free(minibuf[0]);
	minibuf[0] = (char *)calloc(1,12);
	strcpy(minibuf[0],dummy);
	}
        setviewport(12,58,maxx-13,maxy-26,1);
}

void mycursor()
{
	int x,y;
	x = getx();
	y = gety();
	line(x,y+8,x+7,y+8);
	line(x,y+9,x+7,y+9);
}

int strpos(char *string,char ch)
{
	register int i;
	for (i=0;string[i];i++)
		if (string[i]==ch) return i;
	return -1;
}

void set_ques()
/*
	Function :set_ques
	Purpose  :	Takes inputs of status of different questions such as start
				line of the question,mark of the question,question type,
				answer if the type is T/F or Multiple Choice.The status
				is kept in minbuf for further disk storage after linking
				with corresponding question.
	Programmed By : Masud Karim Khan
*/
{
	int l_no,i,j,mpos=0,ind=0,miniptr;
	int qind[4] = {328,432,504,584};
	int pos[7][2] = {474,110,466,140,312,170,384,200,222,220,360,220,520,220};
	int taind[2] = {408,456};
	char qstr[5] ="TMFD",tastr[3]="TF",tmstr[5]="ABCDE";
	char value[3],tanswer[2],manswer[6]="     ";
	char qtype,tempstr[20],*dummy,t_lno[5],ch;
	void *dump,*lbar;

	setviewport(0,0,maxx,maxy,0);
	lbar = (void *)calloc(1,imagesize(115,100,130,107));
	l_no = currline+1;
	value[2] = 0;
	if (status[1][currline]) {
		strcpy(tempstr,minibuf[status[1][currline]]);
		value[0] =tempstr[4];value[1]=tempstr[5];
		qtype = tempstr[7];
		if (qtype =='T') {
			tanswer[0] = tempstr[10];
			tanswer[1] = 0;
		}
		if (qtype == 'M') {
			for  (i=0,j=10;i<5;i++,j++) {
				manswer[i] = tempstr[j];
				if (strpos(tmstr,manswer[i]) != -1) mpos = i;
			}
			manswer[i] = 0;
		}
	}
	else {
		value[0] = value[1] = '0';
		qtype = 'T';
		tanswer[0] = 'T';
		tanswer[1] = 0;
	}
	dump = (void *)calloc(1,imagesize(151,81,640,250));
	getimage(151,81,640,250,dump);
	dialog_box(151,81,640,250,"Question Settings");
	getimage(200,115,207,130,lbar);
	setcolor(BLACK);
	outtextxy(200,110,"Starting line of the question");
	outtextxy(200,140,"Marks of the question");
	outtextxy(200,170,"Question type : True/false  mUltiple  Fillgaps  Descrip");
	outtextxy(200,200,"Answer of the question :");
	if (qtype=='T')  outtextxy(408,200,"True  False");
	else if (qtype=='M') outtextxy(408,200,"A  B  C  D  E");
	setcolor(WHITE);
	setfillstyle(SOLID_FILL,BLACK);
	bar(440,105,520,120);
	bar(440,135,520,150);
	itoa(l_no,t_lno,10);
	outtextxy(450,110,t_lno);
	outtextxy(450,140,value);
	bar(200,215,260,230);
	bar(350,215,420,230);
	bar(510,215,580,230);
	setfillstyle(SOLID_FILL,WHITE);
	outtextxy(222,220,"Ok");
	outtextxy(360,220,"caNcel");
	outtextxy(510,220,"eScape");
	putimage(qind[strpos(qstr,qtype)],165,lbar,XOR_PUT);
	if (qtype=='T')
		putimage(taind[strpos(tastr,tanswer[0])],195,lbar,XOR_PUT);
	else if (qtype=='M') putimage(408+24*mpos,195,lbar,XOR_PUT);
	moveto(pos[0][0],pos[0][1]);
	ind = 0;
	do {
		mycursor();
		ch=toupper(getch());
		if (!ch) ch = toupper(getch());
        mycursor();
		if (isdigit(ch) && (ind==0 || ind==1)) {
			ungetch(ch);
			moveto(450,pos[ind][1]);
			dummy = getstring(1);
			if (ind) strcpy(value,dummy);
			else strcpy(t_lno,dummy);
		}
        else {
		switch (ch) {
			case RIGHT_ARROW:
			case DOWN_ARROW:ind++;
							if (ind == 7) ind = 0;
                                moveto(pos[ind][0],pos[ind][1]);
								break;
			case UP_ARROW  :
			case LEFT_ARROW:ind--;
							if (ind<0) ind = 6;
							moveto(pos[ind][0],pos[ind][1]);
							break;
			case BACKSPACE :if (ind==0 || ind == 1) {
								moveto(450,pos[ind][1]);
								setfillstyle(SOLID_FILL,BLACK);
								bar(440,105+ind*30,520,120+ind*30);
								setfillstyle(SOLID_FILL,WHITE);
							}
							break;
			case 'F'       :
			case 'T'	   :if (ind==3 && qtype=='T') {
								putimage(taind[strpos(tastr,tanswer[0])],195,lbar,XOR_PUT);
								tanswer[0]=ch;
								putimage(taind[strpos(tastr,tanswer[0])],195,lbar,XOR_PUT);
								break;
							}
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':if (ind==3 && qtype=='M') {
						putimage(408+24*mpos,195,lbar,XOR_PUT);
						manswer[mpos] = ' ';
						mpos = ch-'A';
						manswer[mpos] = ch;
						putimage(408+24*mpos,195,lbar,XOR_PUT);
						break;
					 }
					 if ((ch>='A' &&  ch<='C') || ch=='E') break;
			case 'U':if (ind==2) {
						putimage(qind[strpos(qstr,qtype)],165,lbar,XOR_PUT);
						if (ch=='U') ch='M';
						qtype = ch;
						putimage(qind[strpos(qstr,qtype)],165,lbar,XOR_PUT);
						bar(408,190,518,210);
						setcolor(BLACK);
						if (qtype=='T')  {
							outtextxy(408,200,"True  False");
							putimage(taind[strpos(tastr,tanswer[0])],195,lbar,XOR_PUT);
						}
						else if (qtype=='M') {
							outtextxy(408,200,"A  B  C  D  E");
							putimage(408+24*mpos,195,lbar,XOR_PUT);
						}
						setcolor(WHITE);
					}
					break;
			case 'O'		 :  ind=4;ch=13;break;
			case 'N'		 :  ind=5;ch=13;break;
			case ESC		 :
			case 'S'         :  ind=6;ch=13;break;
		}
		}
	}while (!((ind==4 ||ind==5 || ind==6) && ch==13));
	putimage(151,81,dump,COPY_PUT);
	free(dump);
	free(lbar);
	l_no = atoi(t_lno)-1;
	if (ind==4) {
		for(miniptr=1;minibuf[miniptr];miniptr++);
		status[1][l_no] = miniptr;
		minibuf[miniptr]  = (char *)calloc(1,16);
		tempstr[4]=tempstr[5]='0';
		for (i=5,j=strlen(value)-1;j>=0;i--,j--)
		tempstr[i] = value[j];
		tempstr[0]=tempstr[2]=tempstr[6]=254;
		tempstr[1]='N';
		tempstr[3]='V';
		tempstr[7]=qtype;
		if (qtype=='M' || qtype=='T') {
			tempstr[8]=254;tempstr[9]='A';
			if (qtype=='M') {
				for (i=0,j=10;i<5;i++,j++) tempstr[j] = manswer[i];
				tempstr[15]=0;
			}
			else {
				tempstr[10]=tanswer[0];
				tempstr[11]=0;
			}
		}
		else tempstr[8]=0;
		strcpy(minibuf[miniptr],tempstr);
	}
	else if (ind==5) {
		miniptr=status[1][l_no];
		if (miniptr) {
			free(minibuf[miniptr]);
			minibuf[miniptr] = NULL;
			status[1][l_no] = 0;
		}
	}
	setviewport(12,58,maxx-13,maxy-26,1);
}