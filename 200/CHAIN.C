#include <graphics.h>
#include <alloc.h>
#include <stdio.h>
#include <dos.h>
#include <bios.h>

#define ENTER		13
#define ESC		27
#define	F1		59
#define	F2		60
#define	F3		61
#define	F4		62
#define	F5		63
#define F6		64
#define	F10		68
#define F9		67
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

char	  quit_now, save_now;
char 	  loadfile[13]="TEST.QUE", savefile[13], password[5];
unsigned  Q_X1, Q_X2, Q_Y1, Q_Y2, A_X1, A_X2, A_Y1, A_Y2, cur_line, max_line,
	  max_x, max_y, max_question, cur_question, SAVE_TIME = 1000, clock_x, clock_y;
char      LINE_HEIGHT, V_MODE, CHAR_WIDTH, next, fraction, pulse, sec, min, hour, saving;
unsigned  long  t_time, l_time, table[10]={16294,24486,0,8192,16384,24576,90,8282,16474,24666};
char 	  far *buffer[MAX_BUF], far *font[256], far *screen;
int       status[MAX_BUF];
char	  len[10],   far *identies[10], data[10], auto_save = 1;
int 	  TOPX,TOPY;
char 	  quit;
menu file_menu[5]={{"Auto Save   ON",3},{"Save",4},{"Set Time",5},{"Status",6},{"Quit",7}};
menu jump_menu[5]={{"Next",3},{"Previous",4},{"First",5},{"Last",6},{"Goto X",7}};
menu help_menu[3]={{"About..",3},{"DEF",4},{"GHI",5}};

extern void descriptive(unsigned int );
extern  void identy( void );
extern load( void );
extern void screen_setup( void );
extern void make_cursor( void );
extern void up( void );
extern void down( void );
extern void pg_up( void );
extern void pg_down( void );
extern int jump( int );
extern void ans_set( char input, int);
extern save( void );
extern void init_identy( void );
extern void analyze_ans( int );
extern void display_result( void );/**************/
extern char *con(char *str);/****************/
extern void get_screen( void );/**************/
extern void about(void);

void clear_slow( int x1, int y1, int x2, int y2, char inc );
void display_hga_time(char ch1, char ch2, char i);
void display_vga_time(char ch1, char ch2, char i);
void display_cga_time(char ch1, char ch2, char i);
void setup( void );
void set_timer( void );
void g_putchar(char row, char col, char ch ,int op);
void g_printf(char row,char col, char  * , int op);
void g_outtextxy(int x, int y, char  * , int op);
void far_g_printf(char row,char col, char  far* , int op);
void error_msg( char *s, char *t);
char load_font( char *mask, char *fname );
void call_menu(char );
void interrupt  (*old_1cx)();
void interrupt  new_1cx();
void dialog_box(int , int , int , int , char * );
void show_status( void );
int get_num( char *, char *, int * );
int varify_identy( void );
int initialize_file( void );
void leave_now( void );

main(int argc,char *argv[])
{
  int num;
  FILE *fp;

  if (argc !=2 || strcmp(argv[1],"Pass2")) {
	printf("This program requares Quiz Taker (QT.EXE)\n");
	exit(0);
  }
  setup();
  make_cursor();
  clearviewport();
  varify_identy();
  if( !load()) {
	error_msg("Question file is bad or missing","Abnormal Termination");
	restorecrtmode();
	exit(0);
	}
  if( !initialize_file()) {
	error_msg("There is trouble with answer file","Report to your teacher");
	restorecrtmode();
	exit(1);
	}
  TOPX = A_X1;
  TOPY = A_Y1;
  screen_setup();
  g_printf( 2, 16, loadfile, OR_PUT);
  set_timer();
  next = 1;
  quit = 0;
  do {
	while( !kbhit() && next ) {
        	if( quit_now) leave_now();
		if( save_now ) {
			save_now = 0;
			save();
			}
		}
	if( next ) in.i = bioskey(0);
	next = 1;
	if( in.ch[0] )
		switch( in.ch[0] ) {
			case 'a':case 'A':case 'b':case 'B':case 'c':case 'C':
			case 'd':case 'D':case 'e':case 'E':case 'g':case 'G':
			case 't':case 'T':
			case 'f':case 'F':
			ans_set( in.ch[0] ,cur_question);
			break;
			}
	else switch( in.ch[1] ) {
		case F1: error_msg("Help is not available, because","Shibir Shaha didn't do his job");
			 break; /* for help */
		case F2: save();
			 break;
		case F3: if( get_num("         Goto X","Give the question number", &num) ) jump( num - 1);
			 break;
		case F4: if( get_num("      Set Save Time","Give the time for saving (in minutes)", &num) )
				if( num > 0 ) SAVE_TIME = num;
			 break;
		case F5: show_status();
			 break;/* show_status*/
		case F9: about();break;
		case F10: if( questions[cur_question].q_type == 4 ) {
					 g_printf(24,12,"Press Ctrl-Z to terminate                      ",COPY_PUT);
					 descriptive(cur_question);
					 g_printf(24,12,"Press F10 to edit answer              ",COPY_PUT);
				  }
			  else if(questions[cur_question].q_type == 3 )
			  ans_set( in.ch[0] ,cur_question);
			  break;/* Descriptive Answer Toggle */
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
		case LEFT: break;
		case RIGHT: break;
		case FILE_MENU: call_menu(0); break;
		case JUMP_MENU: call_menu(1); break;
		case HELP_MENU: call_menu(2); break;
		case QUIT: quit = 1;
		}
     }while( !quit );
  setvect( 0x1c, old_1cx);
  get_screen();
  analyze_ans( max_question );
  display_result();
  save();
  /* Protect the answer file aginst any other attempt to answer */
  if( (fp = fopen(savefile,"r+b")) != NULL ) {
	fseek( fp, 12, SEEK_SET);
	putc( 3,fp);
	fclose(fp);
	}
  restorecrtmode();
  return 0;
}

void viewclear(int x1, int y1, int x2, int y2 )
{
 setviewport(x1, y1, x2, y2, 0);
 clearviewport();
 setviewport(0, 0, max_x, max_y, 0);
}

void clear_slow( int x1, int y1, int x2, int y2, char inc )
{
 int i, j;

 setcolor( BLACK );
 for( i = 0; i < inc ; i++ )
	for( j = y1 + inc - i - 1; j <= y2 ; j += inc )
		line( x1, j, x2, j);
 setcolor( WHITE );
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
	 do {
		while( !kbhit() ) {
        	if( quit_now) leave_now();
		if( save_now ) {
			save_now = 0;
			save();
			}
		}
	    } while(getch() != 13);
	    /* Wait till <ENTER> key is pressed */
	 setcolor(WHITE);
	 putimage(  213, 55, box, COPY_PUT); /* Restore the original  image */
	 farfree(box);   /* free the allocated memory */
         }
   else putch(7);
}

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
		 screen = MK_FP( 0xB800, 0x1CCE);
		 break;
	case 3 : V_MODE = 2;
		 gm = 1;
                 CHAR_WIDTH = 8;
		 LINE_HEIGHT = 14;
		 fname[0] = mask[16] = 'E';
		 screen = MK_FP( 0xA000, 0x60EE);
		 break;
	case 5 : V_MODE = 2;
		 gm = 3;
                 CHAR_WIDTH = 8;
		 LINE_HEIGHT = 14;
		 fname[0] = mask[16] = 'E';
		 screen = MK_FP( 0xA000,0x60EE);
		 break;
	case 7 : V_MODE = 3;
		 gd = 7;
		 gm = 0;
		 CHAR_WIDTH = 9;
		 LINE_HEIGHT = 14;
		 fname[0] = 'H';
		 mask[16] = 'H';
		 screen = MK_FP( 0xB000, 0x1B7C);
		 break;
	case 9 : V_MODE = 2;
		 gm = 1;
                 CHAR_WIDTH = 8;
		 LINE_HEIGHT = 14;
		 fname[0] = mask[16] = 'E';
		 screen = MK_FP( 0xA000,0x60EE );
		 break;
	default : puts("Graphics card not supported");
		  exit(0);
	}
  init_identy();
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
  clock_x = CHAR_WIDTH * 2 ;
  clock_y = LINE_HEIGHT * 23 ;
  if( !load_font( mask, fname ) ) {
	restorecrtmode();
	puts("Error reading Font file. Abnormal termination.");
	exit(0);
	}
  for( i = 0; i < MAX_QUE; i++) {
	questions[i].ptr_ans = NULL;
	questions[i].ans = NULL;
	}
  for( i = 0; i < MAX_BUF; i++)  status[i] = 0;
  quit_now = 0;
  save_now = 0;
}

void set_timer( void )
{
  char temp[10];

  fraction = pulse = 0;
  saving = 0;
  old_1cx = getvect(0x1c);
  g_printf( 23, 4, "Time to go ", OR_PUT);
  temp[0] = hour / 10 + 48;
  temp[1] = hour % 10 + 48;
  temp[2] = ':';
  temp[3] = min / 10 + 48;
  temp[4] = min % 10 + 48;
  temp[5] = ':';
  temp[6] = sec / 10 + 48;
  temp[7] = sec % 10 + 48;
  temp[8] ='\0';
  if( V_MODE == 3 ) {
	putimage(136,308,font[hour/10+48],OR_PUT);
	putimage(145,308,font[hour%10+48],OR_PUT);
	putimage(157,308,font[':'],OR_PUT);
	putimage(168,308,font[min/10+48],OR_PUT);
	putimage(177,308,font[min%10+48],OR_PUT);
	putimage(189,308,font[':'],OR_PUT);
	putimage(200,308,font[sec/10+48],OR_PUT);
	putimage(209,308,font[sec%10+48],OR_PUT);
	}
  else g_printf( 23, 15, temp, OR_PUT);
  g_printf( 23, 59, "Total Time ",OR_PUT);
  temp[0] = (t_time/3600) / 10 + 48;
  temp[1] = (t_time/3600) % 10 + 48;
  temp[3] = ((t_time/60)%60) / 10 + 48;
  temp[4] = ((t_time/60)%60) % 10 + 48;
  temp[6] = (t_time%60) / 10 + 48;
  temp[7] = (t_time%60) % 10 + 48;
  g_printf( 23, 70, temp, OR_PUT);
  setvect( 0x1c, new_1cx);
}

void display_hga_time(char ch1, char ch2, char disp)
{
  unsigned base;
  char i, index;

  for( i = 0, index = 8; i<=9 ; index += 2,i++) {
	base = table[i] + disp;
	*(screen + base) = font[ch1][index];
	base++;
	*(screen + base) = font[ch1][index+1] | (font[ch2][index] >>1);
	base ++;
	*( screen + base) = ((font[ch2][index+1] & 128) >> 1)+ (font[ch2][index] <<7);
	}
}

void display_ega_time(char ch1, char ch2, char disp)
{
  unsigned base = disp;
  char i, index;

  for( i = 0, index = 14; i<=9 ; base += 79,index += 4,i++) {
	*(screen + base) = font[ch1][index];
	base++;
	*(screen + base) = font[ch2][index] ;
	}
}

void display_cga_time( char ch1, char ch2, char disp)
{
  unsigned base = disp;
  char i, j, index;

  for( i = 0, index = 14, j = -1; i<=9 ; j = -j, base += 40 + j * 8152 ,index += 4,i++) {
	*(screen + base) = font[ch1][index];
	*(screen + base + 1) = font[ch2][index] ;
	}
}

void interrupt new_1cx()
{
  old_1cx();
  pulse++;
  if( pulse > 17 ) {
	pulse = 0;
	l_time++;
	sec--;
	if( quit_now ) return;
	if( V_MODE == 3) display_hga_time(sec / 10 + '0', sec % 10 + '0', 9);
	else if( V_MODE == 2) display_ega_time(sec / 10 + '0', sec % 10 + '0', 6);
	else display_cga_time(sec / 10 + '0', sec % 10 + '0', 6);
	fraction++;
	if( fraction > 4 ) {
		pulse--;
		fraction = 0;
		}
	if( sec < 0 ) {
		sec = 59;
		min--;
		saving++;
		if( V_MODE == 3 ) {
			display_hga_time(sec / 10 + '0', sec % 10 + '0', 9);
			display_hga_time(min / 10 + '0', min % 10 + '0', 5);
			}
		else if( V_MODE == 2) {
			display_ega_time(sec / 10 + '0', sec % 10 + '0', 6);
			display_ega_time(min / 10 + '0', min % 10 + '0', 3);
			}
		else {
			display_cga_time(sec / 10 + '0', sec % 10 + '0', 6);
			display_cga_time(min / 10 + '0', min % 10 + '0', 3);
		     }
		if( min < 0 ) {
			min = 59;
			if( l_time >= t_time ) quit_now = 1;
			hour--;
                        if( V_MODE == 3 ) {
				display_hga_time(min / 10 + '0', min % 10 + '0', 9);
				display_hga_time(hour / 10 + '0', hour % 10 + '0', 5);
				}
			else if( V_MODE == 2) {
				display_ega_time(min / 10 + '0', min % 10 + '0', 6);
				display_ega_time(hour / 10 + '0', hour % 10 + '0', 3);
				}
			else {
				display_cga_time(min / 10 + '0', min % 10 + '0', 6);
				display_cga_time(hour / 10 + '0', hour % 10 + '0', 3);
			     }
			}
		if( auto_save )
			if( saving >= SAVE_TIME ) {
				saving = 0;
				save_now = 1;
				}
		}
  }
}

void g_putchar(char row, char col, char ch, int op)
{
  if( font[ch] != NULL ) putimage( (col - 1)* CHAR_WIDTH, (row - 1)* LINE_HEIGHT, font[ch], op );
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
  if( !op && !auto_save ) g_printf( 3, 17, "OFF", COPY_PUT);
  active -= item;
  current = 1;
  do {
	putimage( x1+ CHAR_WIDTH, y1 + current * LINE_HEIGHT, bar, XOR_PUT);
	while( !kbhit() ) {
        	if( quit_now) leave_now();
		if( save_now ) {
			save_now = 0;
			save();
			}
		}
	in.i = bioskey(0);
        putimage( x1+ CHAR_WIDTH, y1 + current * LINE_HEIGHT, bar, XOR_PUT);
	if( in.ch[0] == ENTER ) {
		if( (op == 0) && ( current == 1)) {
			if( auto_save ) {
				auto_save = 0;
				g_printf( 3, 17, "OFF", COPY_PUT);
				}
			  else {
				auto_save = 1;
				g_printf( 3, 17, " ON",COPY_PUT);
				}
			}
			else {
			  done = 1;
			  next = 0;
			  switch( op ) {
				case 0 : if( current == 2) in.i = F2*256;
					   else if( current == 3) in.i = F4*256;
					   else if( current == 4) in.i = F5*256;/* value*/
					   else in.i = QUIT*256;
					 break;
				case 1 : if( current == 1) in.i = DOWN*256;
					   else if( current == 2) in.i = UP*256;
					   else if( current == 3) in.i = HOME*256;
					   else if( current == 4) in.i = END * 256;
					   else in.i = F3*256 ;
					 break;
				case 2 :if (current == 1) in.i = F9*256;
						else next = 1;
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

int get_num( char *s, char *t, int *i )
{
 char  far *pic;
 int x1, y1, x2, y2, size;
 char num = 0, ch;

 x1 = 20*CHAR_WIDTH;
 x2 = 60*CHAR_WIDTH;
 y1 = 7*LINE_HEIGHT;
 y2 = 12*LINE_HEIGHT-1;
 size = imagesize( x1, y1, x2, y2);
 if( ( pic = ( char * far ) farmalloc( size ) ) == NULL ) {
	putch(7);
	return 0;
	}
 getimage( x1, y1, x2, y2, pic );
 dialog_box( x1, y1, x2, y2, s);
 g_printf( 10, 22, t, XOR_PUT);
 y2 = 10 * LINE_HEIGHT + 3;
 x2 = 38 * CHAR_WIDTH + 1;
 viewclear( x2, y2, x2 + 2* CHAR_WIDTH , y2 + LINE_HEIGHT - 1);
 setcolor( BLACK );
 rectangle( x2 - 2, y2 - 2, x2 + 2* CHAR_WIDTH + 2, y2 + LINE_HEIGHT + 1);
 setcolor( WHITE );
 size = 0;
 do {
        while( !kbhit() ) {
        	if( quit_now) leave_now();
		if( save_now ) {
			save_now = 0;
			save();
			}
		}
	ch = getch();
	if( (ch >= '0') && (ch <= '9') && (size < 2 ) ) {
		putimage( x2, y2, font[ch], COPY_PUT);
		x2 += CHAR_WIDTH;
		num = num*10 + ch - '0';
		size++;
		}
	 else if( (ch == 8) && size ) {
		x2 -= CHAR_WIDTH;
		num /= 10;
		putimage( x2, y2, font[32], COPY_PUT );
		size--;
		}
	 else if( !ch ) getch();
    } while( ( ch != 27 ) && ( ch != 13 ) );
 putimage( x1,y1,pic,COPY_PUT);
 farfree( pic );
 *i = num;
 if( ch == 27 ) return 0;
 return 1;
}

int varify_identy( void )
{
 FILE * fp;
 char ok , i, j;

 do{
    ok = 1;
    identy();
    if( !len[2] || !len[5] || !len[6] || !len[7] ) {
	error_msg("You must specify all informations","Try agian ");
	ok = 0;
	}
   } while( !ok );
 for( i = 0; i < len[6]; i++) loadfile[i] = identies[6][i];
 loadfile[i++] = '.';
 loadfile[i++] = 'Q';
 loadfile[i++] = 'U';
 loadfile[i++] = 'E';
 loadfile[i] = '\0';
 for( i = 0; i < len[6] && i < 3; i++ ) savefile[i] = identies[6][i];
 for( i = 0, j = 3; i < len[7] && j < 8; i++, j++ ) savefile[j] = identies[7][i];
 savefile[j++] = '.';
 savefile[j++] = 'A';
 savefile[j++] = 'N';
 savefile[j++] = 'S';
 savefile[j] = '\0';
 if( (fp = fopen(savefile,"rb") ) != NULL ) {
	fseek( fp, 12, SEEK_SET);
	if( getc(fp) ) {
		fclose( fp);
		error_msg("Invalid attempt to answer ", " Go to hell");
		restorecrtmode();
		exit(0);
		}
	fclose( fp );
	}
 return 1;
}

int initialize_file( void )
{
 FILE * fp;
 int i,j,k ,total;
 char size;

 if( ( fp = fopen(savefile,"rb") ) != NULL ) {
	fseek( fp, 17, SEEK_SET);
	for( j = 0; j < 7; j++) {
		size = getc(fp);
		fseek( fp, size, SEEK_CUR);
		}
	size = getc(fp);
	for( i = 0; getc(fp) == identies[7][i] ; i++);
	if( i < len[7] ) return 0;
	fseek(fp, 0, SEEK_SET);
	i = getw( fp );
	fseek( fp, i, SEEK_SET);
	l_time = getw(fp);
	i = t_time - l_time;
	sec = i%60;
	min = (i/60)%60;
	hour = i/3600;
	for( i = 0; i <= max_question ; i++ ) {
		size = getc( fp ) - 5;
		questions[ i ].marks_set = getc(fp);
		questions[ i ].ans_set = getc(fp);
		questions[ i ].marks = getc( fp );
		questions[ i ].obtained = getc( fp );
		questions[ i ].q_type = getc( fp );
		if( !size ) continue;
		if( questions[i].ptr_ans == NULL )
			if( (questions[i].ptr_ans = ( char far *) farmalloc( size )) == NULL ) return 0;
		for( j = 0; j < size ; j++ ) questions[i].ptr_ans[j] = getc( fp );
		}
	fclose(fp);
	}
 else if( (fp= fopen( savefile,"wb") ) != NULL ) {
	i = 19;
	for( j = 0; j < 8; j++, i++) i += len[j];
	for( j = 0, total = 0; j <= max_question; j++) total += questions[j].marks;
	putw( i, fp);
	fputs("QUIZ TAKER",fp);
	putc(0,fp);
	putw(total,fp);
	putw(0,fp);
	for( j = 0; j < 8; j++, i++) {
		putc(len[j],fp);
		for( k = 0; k < len[j]; k++ ) putc( identies[j][k],fp);
		}
	putw( t_time, fp);
        fclose(fp);
	save();
	}
 else return 0;
 return 1;
}

void show_status( void )
{
 int t_mark = 0, ans_mark = 0, x1, y1, x2, y2;
 char tot[4]={0,0,0,0}, ans[4]={0,0,0,0}, i, j, temp[15], total = 0;

 for( i = 0; i <= max_question; i++ ) {
	t_mark += questions[i].marks;
	j = questions[i].q_type - 1;
	tot[j] += 1;
	if( questions[i].ans_set ) {
		ans[j] += 1;
		total++;
		ans_mark += questions[i].marks;
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
 g_printf(12, 12, "Time Elapsed :", XOR_PUT);
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
 g_printf( 12, 27,temp, XOR_PUT);
 do{
	while( !kbhit() ) {
        	if( quit_now) leave_now();
		if( save_now ) {
			save_now = 0;
			save();
			}
		}
   } while( getch() != ENTER );
 viewclear( Q_X1, Q_Y1, Q_X2, Q_Y2);
 show_page( cur_question );
}

void leave_now( void )
{
 FILE *fp;

 setvect( 0x1c, old_1cx);
 get_screen();
 analyze_ans( max_question );
 display_result();
 save();
 if( (fp = fopen(savefile,"r+b")) != NULL ) {
	fseek( fp, 12, SEEK_SET);
	putc(3,fp);
	fclose(fp);
	}
  restorecrtmode();
  exit(0);
}