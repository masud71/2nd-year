/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	This is  the main part of  QUIZ TAKER's  examineer mode. It controls
 the flow  of the program. It  also contains some  important  functions  for
 initaialization and some utility functios. This part of the program is dev-
 eloped by Md. Mamunul Islam on 10th July,1993. The other functions required
 are gathered from the previous functions for the Examinee mode . The job of
 getting them together is done by Md. Mamunul Islam .
 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#include <graphics.h>
#include <alloc.h>
#include <stdio.h>
#include <ctype.h>

#define ENTER		13
#define ESC		27
#define	F1		59
#define	F2		60
#define	F3		61
#define	F4		62
#define	F5		63
#define F6		64
#define F7		65
#define F8		66
#define F9		67
#define	F10		68
#define	UP		72
#define	DOWN		80
#define	LEFT		75
#define	RIGHT		77
#define	PGUP		73
#define	PGDN		81
#define	HOME		71
#define	END		79
#define	FILE_MENU	33
#define HELP_MENU	35
#define	JUMP_MENU	36
#define	QUIT		45

#define	MAX_QUE		30
#define	MAX_BUF		200

/* - - - - - - - - - - -Global Variables declaration  - - - - - - - - - - -*/
union {
	int i;
	char ch[2];
      } in;

struct que_type{
	unsigned  line_no;
	unsigned char q_type, ans_set, marks_set, answer, marks, obtained;
	char   far *ptr_ans, far *ans;
	} questions[ MAX_QUE ];

typedef struct menu_type{
	char name[15];
	char  row;
	} menu;

char 	  loadfile[17]="GENERAL.QUE", resultfile[13]="GENERAL.RST", savefile[13], password[15] = "HP2";
unsigned  Q_X1, Q_X2, Q_Y1, Q_Y2, A_X1, A_X2, A_Y1, A_Y2, cur_line, max_line,
	  max_x, max_y, max_question, cur_question;
char      LINE_HEIGHT, V_MODE, CHAR_WIDTH, next, on;
unsigned  long  t_time, l_time ;
char 	  far *buffer[MAX_BUF], far *font[256];
int       status[MAX_BUF];
char	  save_now=0,quit_now=0;
char	  len[10], identies[10][30], data[10] = {30,3,3,3,9,5,8,5}, quit;
int 	  TOPX,TOPY;

menu file_menu[5]={{"Load",3},{"Save",4},{"Evaluate",5},{"Status",6},{"Quit",7}};
menu jump_menu[5]={{"Next",3},{"Previous",4},{"First",5},{"Last",6},{"Goto X",7}};
menu help_menu[3]={{"About..",3},{"DEF",4},{"GHI",5}};

/* - - - - - - - - - - Functions used from other files - - - - - - - - - -*/
extern char far *bits;
extern void identy( void );
extern load( void );
extern void screen_setup( void );
extern void make_cursor( void );
extern void up( void );
extern void down( void );
extern void pg_up( void );
extern void pg_down( void );
extern int jump( int );
extern save( void );
extern void init_identy( void );
extern void about(void);


/* - - - - - - - - - Function Prototype declaration - - - - - - - - - - - -*/
void setup( void );
void g_putchar(char row, char col, char ch ,int op);
void g_printf(char row,char col, char  * , int op);
void g_outtextxy(int x, int y, char  * , int op);
void far_g_printf(char row,char col, char  far* , int op);
void error_msg( char *s, char *t);
char load_font( char *mask, char *fname );
void call_menu(char );
void dialog_box(int , int , int , int , char * );
void show_status( void );
void goto_x( void );
void evaluate( void );
int varify_load( void );
int initialize_file( void );
int load_ans( void );
void leave_now() {};
int edit( int x, int y, char len, char *s, char op);
void analyze_ans( int count );

main(int argc,char *argv[])
{
  int num;

  if (argc !=2 || strcmp(argv[1],"Pass3")) {
	printf("This program requares Quiz Taker (QT.EXE)\n");
	exit(0);
  }
  setup();
  make_cursor();
  cleardevice();
  varify_load();
  TOPX = A_X1;
  TOPY = A_Y1;
  screen_setup();
  g_printf( 2, 16, loadfile, OR_PUT );
  next = 1;
  on = 0;
  quit = 0;
  do {
	if( next ) in.i = bioskey(0);
	next = 1;
	if( !in.ch[0] )
	     switch( in.ch[1] ) {
		case F1: error_msg("Help is not available, because","Shibir Shaha did not work");
			 break; /* for help */
		case F2: if( on ) save();
				else error_msg("Nothing to save","Load a answer file first");
			 break;
		case F6: goto_x();
			 break;
		case F4: if( on ) evaluate();
				else error_msg("You must load an answer file","before you evaluate anything");
			 break;
		case F5: show_status();
			 break;/* show_status*/
		case F3: if( !load_ans() ) error_msg("Error with answer file","Press <ENTER> to continue");
			    else on = 1;
			 break;
		case F7: if( on ) identy();
				else error_msg("Identity is not available,","because there is no answer file");
			 break;
		case F8: if( !on ) error_msg("Cannot evaluate without a answer","");
			   else {
				 analyze_ans( max_question );
				 show_status();
				}
			 break;
		case F9:about();break;
		case PGUP: pg_up(); break;
		case PGDN: pg_down(); break;
		case HOME: jump(0); break; /* jump(0) */
		case END: jump( max_question ); break; /* jump( max_question ) */
		case UP: if( V_MODE != 2 ) up();
				else jump( cur_question - 1 );
			 break;
		case DOWN: if( V_MODE != 2 ) down();
				else jump( cur_question + 1 );
			   break;
		case FILE_MENU: call_menu(0); break;
		case JUMP_MENU: call_menu(1); break;
		case HELP_MENU: call_menu(2); break;
		case QUIT: quit = 1;
			   break;
		}
     }while( !quit );
  restorecrtmode();
  return 0;
}

void viewclear(int x1, int y1, int x2, int y2 )
{
 setviewport(x1, y1, x2, y2, 0);
 clearviewport();
 setviewport(0, 0, max_x, max_y, 0);
}

void dialog_box(int x1 , int y1, int x2, int y2, char *s)
{
/*   NAME : dialog_box
     FUNCTION : genrates a dialog box of the given size at given position
	  and puts the message on top of it .
     EXAMPLE : dialog_box( 100, 55, 300, 85, "My box");
     VARIABLES USED : my_patern, points , maxx, maxy.
     MACROS USED : None
     ROUTINES  CALLLED : fillpoly, setfillstyle, setfillpattern,  setcolor,
	  line, rectangle, outtextxy, setviewport, viewclear .
*/
 char my_patern[8] = {0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55};
 int points[20], t_y;

 points[0] = points[6] = points[8] = x1;
 points[1] = points[3] = points[9] = y1;
 points[2] = points[4] = x2;
 points[5] = points[7] = y2;
 points[10] = points[16] = points[18] = x1 + 3;
 points[11] = points[13] = points[19] = y1 + 2;
 points[12] = points[14] = x2 - 3;
 points[15] = points[17] = y2 - 3;
 setfillpattern( my_patern, 1);
 setcolor( 0 );
 fillpoly( 10, points );
 setfillstyle( SOLID_FILL, WHITE);
 fillpoly( 5, &points[10] );
 viewclear(x1 + 30, y1 + 3, x2 - 5, y1 + LINE_HEIGHT + 3);
 setcolor(BLACK);
 t_y =  LINE_HEIGHT / 2;
 line( x1 + 5, y1 + LINE_HEIGHT + 3, x1 + 30, y1 + LINE_HEIGHT + 3);
 rectangle(x1 + 7, y1 + t_y, x1 + 20, y1 + t_y + 3);
 line(x1 + 8, y1 + 4 + t_y, x1 + 21, y1 + 4 + t_y);
 line(x1 + 21, y1 + t_y + 1, x1 + 21, y1 + t_y + 4);
 setcolor(WHITE);
 g_outtextxy( x1 + 38, y1 + 4, s, COPY_PUT);
}

void error_msg(char *s, char *t)
{
/*   NAME : error_msg
     FUNCTION : If any error occurs this function is called to display that
	  error message .
     EXAMPLE : error_msg( "Symbol not found","Job Terminated");
     VARIABLES USED : box, size
     MACROS USED : NULL
     ROUTINES CALLLED:imagesize, getimage, putimage, dialog_box, outtextxy,
	  rectangle, farfree, farmalloc, getch ,setcolor, putch.
*/
 char  far *box;
 unsigned size;

 size = imagesize( 213, 55, 530, 127);
 /* If enough memory cannot be found then only make a beep sound */
 if( (box = (char   far *) farmalloc(size) ) != NULL) {
	 getimage( 213, 55, 530, 127, box);
	     /* Save the image of the area where the message will appear */
	 viewclear( 213, 55, 530, 127);
	 setcolor( WHITE );
	 dialog_box( 213, 55, 530, 127, "           Error");
	     /* generate dialog box for the message */
	 setcolor(BLACK);
	 g_outtextxy(225, 76, s, XOR_PUT);
	 g_outtextxy(225, 90, t, XOR_PUT);
	 rectangle(336,107,408,122);
	 g_outtextxy(363,108,"OK", XOR_PUT);
	 do {} while(getch() != 13);
	    /* Wait till <ENTER> key is pressed */
	 setcolor(WHITE);
	 putimage(  213, 55, box, COPY_PUT); /* Restore the original  image */
	 farfree(box);   /* free the allocated memory */
         }
   else putch(7);
}

/* - - - - - - - - - Funcyion for loading fonts from file - - - - - - - - -*/
char load_font( char *mask, char *fname )
{
 FILE *fp;
 char n_mask[20];
 unsigned pos, i, j, size;

 if( (fp = fopen(fname, "rb") ) == NULL) return 0;
 fgets( n_mask, 20, fp);
 if( ferror( fp ) ) return 0;
 if( (fname == NULL) || strcmp(mask, n_mask) ) return 0;
 size = imagesize( 1, 1, CHAR_WIDTH, LINE_HEIGHT);
 fseek(fp,531,SEEK_SET);
 for( i = 0 ; i<=255; i++) {
	if( ( font[i] = (char  far *) farmalloc(size) ) == NULL ) return 0;
	for( j=0; j < size; j++) {
		font[i][j] = getc(fp);
		if( ferror( fp ) ) return 0;
		}
	}
 fclose(fp);
 return 1;
}

/* - - - - - - -This function initializes the graphics - - - - - - - - - -*/
void setup( void )
{
  char mask[]="QUIZ TAKER FONT: GA", fname[]=" GA.FNT";
  int gd, gm, i;

  detectgraph(&gd, &gm);
  switch( gd ) {
	case 1 : V_MODE = 1;
		 gm = 4;
                 CHAR_WIDTH = 8;
		 LINE_HEIGHT = 8;
		 fname[0] = mask[16] = 'C';
		 break;
	case 3 : V_MODE = 2;
		 gm = 1;
                 CHAR_WIDTH = 8;
		 LINE_HEIGHT = 14;
		 fname[0] = mask[16] = 'E';
		 break;
	case 5 : V_MODE = 2;
		 gm = 3;
                 CHAR_WIDTH = 8;
		 LINE_HEIGHT = 14;
		 fname[0] = 'E';
		 mask[16] = 'E';
		 break;
	case 7 : V_MODE = 3;
		 gm = 0;
		 CHAR_WIDTH = 9;
		 LINE_HEIGHT = 14;
		 fname[0] = 'M';
		 mask[16] = 'H';
		 break;
	case 9 : V_MODE = 2;
		 gm = 1;
                 CHAR_WIDTH = 8;
		 LINE_HEIGHT = 14;
		 fname[0] = mask[16] = 'E';
		 break;
	default : puts("Graphics card not supported");
		  exit(0);
	}
  initgraph( &gd, &gm, "");
  i = graphresult();
  if( i < 0 ) {
	puts( grapherrormsg( i ) );
	exit(0);
	}
  max_x = getmaxx();
  max_y = getmaxy();
  Q_X1 = CHAR_WIDTH ;
  Q_X2 = CHAR_WIDTH * 79 ;
  Q_Y1 = LINE_HEIGHT * 2 ;
  Q_Y2 = LINE_HEIGHT * 18 - 1;
  A_X1 = Q_X1 ;
  A_X2 = Q_X2 ;
  A_Y1 = LINE_HEIGHT * 19 ;
  A_Y2 = LINE_HEIGHT * 22 - 1;
  if( !load_font( mask, fname ) ) {
	restorecrtmode();
	puts("Error reading Font file. Abnormal termination.");
	exit(0);
	}
  for(i = 0; i < MAX_BUF; i++) buffer[i] = NULL;
  for(i = 0; i < MAX_QUE; i++) {
	questions[i].ptr_ans = NULL;
	questions[i].ans = NULL;
	}
}

void g_putchar(char row, char col, char ch, int op)
{
  if( font[ch] != NULL ) putimage( (col-1) * CHAR_WIDTH, (row-1) * LINE_HEIGHT, font[ch], op );
}

void g_printf( char row, char col, char *s, int op)
{
  int x = (col - 1) * CHAR_WIDTH , y = (row - 1) * LINE_HEIGHT ;
  register char i = 0;

  if( y >= max_y ) return;
  while( s[i] != '\0') {
	if( x < max_x ) putimage( x, y, font[ (unsigned char ) s[i]], op);
	x += CHAR_WIDTH;
	i++;
	}
}

void far_g_printf( char row, char col, char far *s, int op)
{
  int x = (col - 1) * CHAR_WIDTH , y = (row - 1) * LINE_HEIGHT ;
  register char i = 0;

  if( y >= max_y ) return;
  while( s[i] != '\0') {
	if( x < max_x ) putimage( x, y, font[(unsigned char ) s[i]], op);
	x += CHAR_WIDTH;
	i++;
	}
}

void g_outtextxy( int x, int y, char *s, int op)
{
  register char i = 0;

  if( y >= max_y ) return;
  while( s[i] != '\0') {
	if( x < max_x ) putimage( x, y, font[ (unsigned char ) s[i]], op);
	x += CHAR_WIDTH;
	i++;
	}
}

void call_menu( char op )
{
  menu *active;
  char box_x1[3] = {5,23,37}, box_x2[3] = {22,36,48}, col, menu_item[3]={5,5,3}, item, current, done = 0;
  int  x1, x2, y1, y2, i, e[10];
  char   far *box,   far *bar;


  if( !op ) active = file_menu;
	else if( op == 1 ) active = jump_menu;
	else active = help_menu;
  item = menu_item[op];
  col = box_x1[op] + 1;
  x1 = (box_x1[op] - 1) * CHAR_WIDTH;
  x2 = (box_x2[op] - 1)* CHAR_WIDTH;
  y1 = LINE_HEIGHT;
  y2 = (3 + item) * LINE_HEIGHT;
  i = imagesize(x1, y1, x2, y2);
  if( (box = (char far *) farmalloc( i ) ) == NULL ) return;
  getimage( x1, y1, x2, y2, box);
  for( i = x1 + CHAR_WIDTH; i < x2 - CHAR_WIDTH; i += CHAR_WIDTH)
	putimage( i, y2 - LINE_HEIGHT, font[0], XOR_PUT);
  for( i = y1 + LINE_HEIGHT; i < y2; i += LINE_HEIGHT)
	putimage( x2 - CHAR_WIDTH, i, font[0], XOR_PUT);
  viewclear( x1, y1, x2 - CHAR_WIDTH, y2 - LINE_HEIGHT + 3);
  setlinestyle( SOLID_LINE, 0, THICK_WIDTH);
  rectangle( x1+1, y1+3, x2 - CHAR_WIDTH , y2 - LINE_HEIGHT + 3);
  setlinestyle( SOLID_LINE, 0, NORM_WIDTH);
  e[0] = e[6] = e[8] = x1 + CHAR_WIDTH;
  e[1] = e[3] = e[9] = y1 + LINE_HEIGHT;
  e[2] = e[4] = x2 - 2 * CHAR_WIDTH;
  e[5] = e[7] = e[1] + LINE_HEIGHT - 1;
  fillpoly( 5, e);
  i = imagesize( e[0], e[1], e[2], e[5]);
  if( (bar = (char  far *) farmalloc( i ) ) == NULL ) {
	putimage( x1, y1, box, COPY_PUT);
	farfree( box );
	return;
	}
  getimage( e[0], e[1], e[2], e[5], bar );
  viewclear( e[0], e[1], e[2], e[5] );
  for( i = 0; i < item; i++) {
	g_printf( active->row, col, active->name, COPY_PUT);
	active++;
	}
  active -= item;
  current = 1;
  do {
	putimage( x1+ CHAR_WIDTH, y1 + current * LINE_HEIGHT, bar, XOR_PUT);
	in.i = bioskey(0);
        putimage( x1+ CHAR_WIDTH, y1 + current * LINE_HEIGHT, bar, XOR_PUT);
	if( in.ch[0] == ENTER ) {
		done = 1;
		next = 0;
		switch( op ) {
			case 0 : if( current == 2) in.i = F2*256;
				   else if( current == 1) in.i = F3*256;
				   else if( current == 3) in.i = F4*256;
				   else if( current == 4) in.i = F5*256;/* value*/
				   else in.i = QUIT*256;
				 break;
			case 1 : if( current == 1) in.i = DOWN*256;
				   else if( current == 2) in.i = UP*256;
				   else if( current == 3) in.i = HOME*256;
				   else if( current == 4) in.i = END * 256;
				   else in.i = F6*256 ;
				 break;
			case 2 :if (current == 1) in.i = F9*256;
					else {
					error_msg("Help is not available. Ask ","Shibir Shaha (30) about help");
					next = 1;
					}
			}
		}
	if( in.ch[0] == ESC ) done = 1;
	else if( !in.ch[0] )
		switch( in.ch[1] ) {
			case UP : if( current == 1) current = item;
					else current--;
				  break;
			case DOWN : if( current == item ) current = 1;
					else current++;
				    break;
			case LEFT : done = 1;
				    next = 0;
				    if( op == 0) in.i = HELP_MENU*256;
				      else if( op == 1) in.i = FILE_MENU*256;
				      else in.i = JUMP_MENU*256;
				    break;
			case RIGHT : done = 1;
				     next = 0;
				     if( op == 1) in.i = HELP_MENU*256;
				       else if( op == 2) in.i = FILE_MENU*256;
				       else in.i = JUMP_MENU*256;
				     break;
			}
     } while( !done );
  putimage(x1, y1, box, COPY_PUT);
  farfree( box );
  farfree( bar );
}

void goto_x( void )
{
 char  far *pic;
 int x1, y1, x2, y2, size;
 char num = 0, temp[10];

 x1 = 22 * CHAR_WIDTH;
 x2 = 58 * CHAR_WIDTH;
 y1 =  7 * LINE_HEIGHT;
 y2 = 12 * LINE_HEIGHT-1;
 size = imagesize( x1, y1, x2, y2);
 if( ( pic = ( char * far ) farmalloc( size ) ) == NULL ) {
	putch(7);
	return ;
	}
 getimage( x1, y1, x2, y2, pic );
 dialog_box( x1, y1, x2, y2, "           Goto X");
 g_printf( 10, 26, "Give the question number", XOR_PUT);
 x2 = 38 * CHAR_WIDTH + 1;
 y2 = 10 * LINE_HEIGHT + 3;
 viewclear( x2, y2, x2 + 3* CHAR_WIDTH , y2 + LINE_HEIGHT - 1);
 setcolor( BLACK );
 rectangle( x2 - 2, y2 - 2, x2 + 3* CHAR_WIDTH + 2, y2 + LINE_HEIGHT + 1);
 setcolor( WHITE );
 size = 1;
 if( !edit( x2 , y2 , 2, temp, 2) ) size = 0;
 putimage( x1,y1, pic, COPY_PUT);
 farfree( pic );
 pic = NULL;
 if( !size ) return ;
 for( size = 0; (size < 2) && temp[size]; size++)
	num = num * 10 + temp[size] - 48;
 jump( num - 1);
}

void evaluate( void )
{
 char  far *pic;
 int x1, y1, x2, y2, size;
 char num = 0, temp[10];

 x1 = 24*CHAR_WIDTH;
 x2 = 55*CHAR_WIDTH - 1;
 y1 = 7*LINE_HEIGHT - 6;
 y2 = 12*LINE_HEIGHT + 6;
 size = imagesize( x1, y1, x2, y2);
 if( ( pic = ( char * far ) farmalloc( size ) ) == NULL ) {
	putch(7);
	return ;
	}
 getimage( x1, y1, x2, y2, pic );
 dialog_box( x1, y1, x2, y2, "      Evaluate");
 g_printf(  9, 29, "Evaluated :", XOR_PUT);
 g_printf( 10, 29, "Marks     :", XOR_PUT);
 g_printf( 11, 29, "Obtained  :", XOR_PUT);
 if( questions[ cur_question ].marks_set ) g_printf( 9, 41, "Yes",XOR_PUT);
	else g_printf( 9, 41, "No", XOR_PUT);
 temp[0] = questions[cur_question].marks/10 + 48;
 temp[1] = questions[cur_question].marks%10 + 48;
 temp[2] = 0;
 g_printf( 10, 41, temp, XOR_PUT);
 temp[0] = questions[cur_question].obtained/10 + 48;
 temp[1] = questions[cur_question].obtained%10 + 48;
 g_printf( 11, 41, temp, XOR_PUT);
 g_printf( 12, 29, "Give new marks",XOR_PUT);
 x2 = 44 * CHAR_WIDTH;
 y2 = 11 * LINE_HEIGHT;
 viewclear( x2, y2, x2 + 3* CHAR_WIDTH , y2 + LINE_HEIGHT - 1);
 setcolor( BLACK );
 rectangle( x2 - 2, y2 - 2, x2 + 3* CHAR_WIDTH + 2, y2 + LINE_HEIGHT + 1);
 setcolor( WHITE );
 size = 1;
 if( !edit( x2 , y2 , 2, temp, 2) ) size = 0;
 putimage( x1,y1, pic, COPY_PUT);
 farfree( pic );
 pic = NULL;
 if( !size ) return ;
 for( size = 0; (size < 2) && temp[size]; size++)
	num = num * 10 + temp[size] - 48;
 if( num > questions[cur_question].marks ) error_msg("Given marks is out of range", " Evaluate agian ");
	else {
		questions[cur_question].obtained = num ;
		questions[cur_question].marks_set = 1 ;
	     }
}

int edit( int x, int y, char len, char *s, char op)
{
 char pos = 0, ch;

 do{
	putimage(x,y+LINE_HEIGHT-3,bits,XOR_PUT);
	ch = getch();
	putimage(x, y + LINE_HEIGHT - 3, bits, XOR_PUT);
	if( ( isalnum(ch) || (ch == '.')) && (pos < len) && (op < 2)) {
		s[pos] = ch;
		pos++;
		if( op ) putimage( x, y, font[ch], COPY_PUT);
			else putimage( x, y, font[ '*' ], COPY_PUT);
		x += CHAR_WIDTH;
		}
	   else if( (op==2) && isdigit(ch) && (pos < len) ) {
		s[pos] = ch;
		pos++;
		putimage( x, y, font[ch], COPY_PUT);
		x += CHAR_WIDTH;
		}
	   else if( (ch == 8) && pos && op) {
		x -= CHAR_WIDTH;
		pos--;
		putimage( x, y, font[32], COPY_PUT);
		}
	   else if( !ch ) getch();
   }while( (ch != 13) && (ch != 27) );
 s[pos] = '\0';
 if( ch == 27 ) return 0;
 return 1;
}

int varify_load( void )
{
 FILE * fp;
 char ok , i = 0, j, new_pass[12];
 int x1, y1, x2, y2;


 dialog_box( 23 * CHAR_WIDTH, 8 * LINE_HEIGHT, 56 * CHAR_WIDTH, 13 * LINE_HEIGHT,"          Open");
 g_printf(11, 25, "Give question filename to load", XOR_PUT);
 y1 = 11 * LINE_HEIGHT + 3;
 x1 = 31 * CHAR_WIDTH - 3;
 x2 = 47 * CHAR_WIDTH;
 y2 = LINE_HEIGHT + y1 + 6;
 viewclear( x1, y1, x2, y2);
 rectangle( x1 + 1, y1 + 1, x2 - 1, y2 - 1);
 if( !edit( x1 + 3, y1 + 3, 15, loadfile, 1) ) {
	restorecrtmode();
	exit(0);
	}
 if( !load() ) {
	error_msg("Question file is bad or missing","Abnormal Termination");
	restorecrtmode();
	exit(0);
	}
 cleardevice();
 dialog_box( 26 * CHAR_WIDTH, 8 * LINE_HEIGHT, 54 * CHAR_WIDTH, 13 * LINE_HEIGHT,"       Password");
 g_printf(11, 35, "Give passward", XOR_PUT);
 y1 = 11 * LINE_HEIGHT + 3;
 x1 = 35 * CHAR_WIDTH - 3;
 x2 = 45 * CHAR_WIDTH;
 y2 = LINE_HEIGHT + y1 + 6;
 do {
    if( i == 3) {
	restorecrtmode();
	exit(0);
	}
    ok = 1;
    viewclear( x1, y1, x2, y2);
    rectangle( x1 + 1, y1 + 1, x2 - 1, y2 - 1);
    if( !edit( x1 + 3, y1 + 3, 10, new_pass, 0) ) {
	restorecrtmode();
	exit(0);
	}
    if( strcmp( password, new_pass) ) ok = 0;
    i++;
    } while( !ok );
 for( i = 0; loadfile[i] != '.' && ( i < 8) ; i++) resultfile[i] = loadfile[i] ;
 resultfile[i++] = '.';
 resultfile[i++] = 'R';
 resultfile[i++] = 'S';
 resultfile[i++] = 'T';
 resultfile[i] = '\0';
 initialize_resfile();
 return 1;
}

int initialize_resfile( void )
{
 FILE * fp;
 char temp[19];

 if( ( fp = fopen( resultfile, "rb" ) ) != NULL ) {
	fgets( temp, 18, fp);
	if( strcmp( temp, "QUIZ TAKER RESULT") ) {
		fclose( fp );
		error_msg("Error in result file","Abnormal Termination");
		restorecrtmode();
		exit(0);
		}
	}
   else	{
	if( (fp = fopen( resultfile, "wb") ) == NULL ) {
		error_msg(" File Write error","Abnormal termination");
		restorecrtmode();
		exit(0);
		}
	fputs( "QUIZ TAKER RESULT", fp);
	putw(0,fp);
	fclose( fp );
	}
 return 1;
}

void show_status( void )
{
 int t_mark = 0, ans_mark = 0, t_obt = 0, x1, y1, x2, y2;
 char tot[4]={0,0,0,0}, ans[4]={0,0,0,0}, i, j, temp[15], total = 0, eval = 0;

 for( i = 0; i <= max_question; i++ ) {
	t_mark += questions[i].marks;
	j = questions[i].q_type - 1;
	tot[j] += 1;
	if( questions[i].ans_set ) {
		ans[j] += 1;
		total++;
		ans_mark += questions[i].marks;
		}
	if( questions[i].marks_set ) {
		t_obt += questions[i].obtained;
		eval++;
		}
	}
 x1 = 8 * CHAR_WIDTH;
 y1 = 3 * LINE_HEIGHT - 7;
 x2 = 72 * CHAR_WIDTH;
 y2 = 14 * LINE_HEIGHT + 8;
 dialog_box( x1, y1, x2, y2, "                           Status");
 g_printf(5, 12, "Total Questions :", XOR_PUT);
 itoa( max_question + 1, temp , 10);
 g_printf(5, 30, temp, XOR_PUT);
 g_printf(6, 12, "Total Answered :", XOR_PUT);
 itoa( total, temp , 10);
 g_printf(6, 29, temp, XOR_PUT);
 g_printf(7, 12, "MCQ Questions :", XOR_PUT);
 itoa( tot[0], temp , 10);
 g_printf(7, 28, temp, XOR_PUT);
 g_printf(8, 12, "MCQ Answered :", XOR_PUT);
 itoa( ans[0], temp , 10);
 g_printf(8, 27, temp, XOR_PUT);
 g_printf(9, 12, "Descriptive Questions :", XOR_PUT);
 itoa( tot[3], temp , 10);
 g_printf(9, 36, temp, XOR_PUT);
 g_printf(10, 12, "Descriptive Answered :", XOR_PUT);
 itoa( ans[3], temp , 10);
 g_printf(10, 35, temp, XOR_PUT);
 g_printf(11, 12, "Total Time :", XOR_PUT);
 g_printf(12, 12, "Time Taken :", XOR_PUT);
 g_printf(5, 42, "Total Evaluated :", XOR_PUT);
 itoa( eval, temp , 10);
 g_printf(5, 61, temp, XOR_PUT);
 g_printf(6, 42, "Total Obtained :", XOR_PUT);
 itoa( t_obt, temp , 10);
 g_printf(6, 60, temp, XOR_PUT);
 g_printf(7, 42, "T/F Questions :", XOR_PUT);
 itoa( tot[1], temp , 10);
 g_printf(7, 59, temp, XOR_PUT);
 g_printf(8, 42, "T/F Answered :", XOR_PUT);
 itoa( ans[1], temp , 10);
 g_printf(8, 58, temp, XOR_PUT);
 g_printf(9, 42, "Fill gap Questions :", XOR_PUT);
 itoa( tot[2], temp , 10);
 g_printf(9, 64, temp, XOR_PUT);
 g_printf(10, 42, "Fill gap Answered :", XOR_PUT);
 itoa( ans[2], temp , 10);
 g_printf(10, 63, temp, XOR_PUT);
 g_printf(11, 42, "Total Marks :", XOR_PUT);
 itoa( t_mark, temp , 10);
 g_printf(11, 56, temp, XOR_PUT);
 g_printf(12, 42, "Answered for marks :", XOR_PUT);
 itoa( ans_mark, temp , 10);
 g_printf(12, 63, temp, XOR_PUT);
 g_printf(14, 39, "OK", XOR_PUT);
 setcolor( BLACK );
 rectangle( 36 * CHAR_WIDTH, 13 * LINE_HEIGHT - 1, 43 * CHAR_WIDTH, 14 * LINE_HEIGHT );
 setcolor( WHITE );
 temp[0] = t_time/36000 + 48;
 temp[1] = (t_time/3600)%10 + 48;
 temp[2] = ':';
 temp[3] = ((t_time/60)%60) / 10 + 48;
 temp[4] = ((t_time/60)%60) % 10 + 48;
 temp[5] = ':';
 temp[6] = (t_time%60) / 10 + 48;
 temp[7] = (t_time%60) % 10 + 48;
 temp[8] ='\0';
 g_printf( 11, 25, temp, XOR_PUT);
 temp[0] = l_time/36000 + 48;
 temp[1] = (l_time/3600)%10 + 48;
 temp[3] = ((l_time/60)%60) / 10 + 48;
 temp[4] = ((l_time/60)%60) % 10 + 48;
 temp[6] = (l_time%60) / 10 + 48;
 temp[7] = (l_time%60) % 10 + 48;
 g_printf( 12, 25,temp, XOR_PUT);
 do{} while( getch() != ENTER );
 viewclear( Q_X1, Q_Y1, Q_X2, Q_Y2);
 show_page( cur_question );
}


int load_ans( void )
{
 FILE * fp;
 int i,j,k ,total, x1, y1, x2, y2;
 char size, far *map, temp[16];

 x1 = 23 * CHAR_WIDTH;
 x2 = 56 * CHAR_WIDTH;
 y1 = 7 * LINE_HEIGHT - 5;
 y2 = 12 * LINE_HEIGHT;
 i = imagesize( x1, y1, x2, y2);
 if( (map = (char far * ) farmalloc( i )) == NULL) return 0;
 getimage( x1, y1, x2, y2, map );
 dialog_box( x1, y1, x2, y2, "          Load");
 g_printf( 9, 25,"Give the code of the student", XOR_PUT);
 viewclear( 36* CHAR_WIDTH - 3, 10 * LINE_HEIGHT - 3, 42 * CHAR_WIDTH + 3, 11 * LINE_HEIGHT + 3);
 rectangle( 36* CHAR_WIDTH - 1, 10 * LINE_HEIGHT - 1, 42 * CHAR_WIDTH + 1, 11 * LINE_HEIGHT + 1);
 size = 1;
 if( !edit( 36 * CHAR_WIDTH, 10 * LINE_HEIGHT, 5, identies[7], 1) ) size = 0;
 putimage( x1, y1, map, COPY_PUT);
 farfree( map );
 if( !size ) return 0;
 savefile[0] = loadfile[0];
 savefile[1] = loadfile[1];
 savefile[2] = loadfile[2];
 for( i = 0, j = 3; (i < 5) && identies[7][i];i++, j++ ) savefile[j] = identies[7][i];
 savefile[j++] = '.';
 savefile[j++] = 'A';
 savefile[j++] = 'N';
 savefile[j++] = 'S';
 savefile[j] = '\0';
 if( ( fp = fopen(savefile,"rb") ) == NULL ) return 0;
 getw(fp);
 fgets( temp, 11, fp);
 if( strcmp( temp,"QUIZ TAKER") ) return 0;
 if( getc(fp) == 2 ) {
	error_msg( "The answer sheet has allready " ,"been evaluated .");
	return 0;
	}
 size = getw( fp );
 size = getw( fp );
 for( j = 0; j < 8; j++) {
	size = getc(fp);
	for( i = 0; i < size; i++) identies[j][i] = getc( fp);
	if( ferror(fp) ) return 0;
	identies[j][i] = '\0';
	}
 getw( fp );
 l_time = getw(fp);
 for( i = 0; i <= max_question ; i++ ) {
	size = getc( fp ) - 5;
	questions[ i ].marks_set = getc(fp);
	questions[ i ].ans_set = getc(fp);
	questions[ i ].marks = getc( fp );
	questions[ i ].obtained = getc( fp );
	questions[ i ].q_type = getc( fp );
	if( !size ) continue;
	if( (questions[i].ptr_ans == NULL ))
		if( (questions[i].ptr_ans = ( char far *) farmalloc( size )) == NULL ) return 0;
	for( j = 0; j < size ; j++ ) questions[i].ptr_ans[j] = getc( fp );
	questions[i].ptr_ans[j] = '\0';
	}
 fclose(fp);
 show_answer( cur_question );
 return 1;
}

int save( void )
{
   FILE *fp;
   int total = 0, ele, i, j;
   char ok = 0, temp[20];

   for( i = 0; i <= max_question; i++) total += questions[i].obtained;
   if( ( fp = fopen( savefile,"r+b" )) == NULL ) {
	error_msg("File Write Error, Can't Save", savefile );
	return 0;
	}
   fseek( fp, 12, SEEK_SET);
   putc( 2, fp);
   getw(fp);
   putw( total, fp);
   fclose( fp );
   if( ( fp = fopen( resultfile, "r+b" ) ) == NULL ) {
	error_msg( "File write error, can't save", resultfile);
	return 0;
	}
   fgets( temp, 18, fp);
   if( strcmp( temp, "QUIZ TAKER RESULT") ) {
	fclose( fp );
	error_msg("Error in result file","Can't proceed with Save");
	return 0;
	}
   ele = getw(fp);
   ok = 0;
   for( i = 0; i < ele; i++) {
	fseek( fp, 19+i*66, SEEK_SET);
	fgets( temp, 6, fp);
	if( !strcmp( temp, identies[7]) ) {
		ok = 1;
		break;
		}
	}
   fseek(fp, 19+i*66, SEEK_SET);
   for( j = 0; j < data[7]; j++) putc( identies[7][j], fp);
   for( i = 0; i < 7; i++)
	for( j = 0; j < data[i]; j++) putc( identies[i][j], fp );
   putw( total, fp);
   if( ok ) {
	fseek( fp, 17, SEEK_SET);
	putw( ele + 1, fp );
	}
   fclose( fp );
   return 1;
}

void analyze_ans( int count )
{
 int i , type;

 for( i = 0; i <= count; i++){
	type = questions[i].q_type;
	if( (type==1) || (type==2) )
	    if( questions[i].ans_set ){
		if( ! strcmp( (char *)questions[i].ptr_ans,(char *)questions[i].ans) ){
			questions[i].marks_set = 1;
			questions[i].obtained = questions[i].marks;
			}
		else  {
			questions[i].marks_set = 1;
			questions[i].marks = 0;
		      }
		}
	}
}