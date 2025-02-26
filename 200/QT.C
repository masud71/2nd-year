/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	This program is part of the QUIZ TAKER program. It is the controller
 program , which calls  all the  other programs developed independently. Its
 job is to call the program BUET.EXE and  then show a  bolted menu. The user
 can select any program from the menu and execute it . It contains following
 programs
		1) Question setter
		2) Examinee
		3) Examineer
	These programs can also be executed from the  DOS command prompt. It
 just ease the job of the user .
	This part  of the program  is developed by Md. Mamunul Islam on 24th
 July, 1993.
 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include <graphics.h>
#include <alloc.h>
#include "process.h"

/* - - - - - - - - - - - - - Global Variables - - - - - - - - - - - - - - -*/
char *bitmap, err;
char *opt[4]={"Set Question","Appear Exam","Evaluate","Quit"};
int lx, rx, ty, by, lb, rb, tb, bb, midx, midy, max_x, max_y, line_height, char_width;

void screen( void );

main()
{
  int i, gd, gm, x, y;
  char ch, choice = 0, quit = 0;

  detectgraph( &gd, &gm);
  switch( gd ) {
	case 1 : gm = 3;
		 break;
	case 3 :
	case 4 : gm = 1;
		 break;
	case 5 : gm = 3;
		 break;
	case 7 : gm = 0;
		 break;
	case 9 : gm = 1;
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
  spawnl( P_WAIT,"BUET.EXE","Buet.exe","Pass0", NULL);
  max_x = getmaxx();
  max_y = getmaxy();
  midx = max_x/2;
  midy = max_y/2;
  settextstyle( 1, HORIZ_DIR, 1);
  line_height = textheight("T");
  char_width = textwidth("T");
  i = (line_height + 4) * 2 + 2;
  ty = midy - i;
  y = ty + 4;
  by = midy + i;
  i = char_width * 7;
  x = lx = midx - i;
  rx = midx + i;
  lb = lx - 2 * char_width - 6;
  rb = rx + 2 * char_width + 6;
  tb = ty - line_height - 4;
  bb = by + line_height + 4;
  screen();
  do {
	ch = getch();
	if( ch == 13 ) {
		err = -1;
		free(bitmap);
	    switch( choice ) {
		case 0 : err = spawnl( P_WAIT,"qt1.exe","qt1.exe","Pass1", NULL );
			 break;
		case 1 : err = spawnl( P_WAIT,"qt2.exe","qt2.exe","Pass2", NULL);
			 break;
		case 2 : err = spawnl( P_WAIT,"qt3.exe","qt3.exe","Pass3", NULL);
			 break;
		case 3 : quit = 1;
		}
	    if( err != -1 ) {
                initgraph(&gd,&gm,"");
		screen();
		settextstyle(1, HORIZ_DIR, 1);
		choice = 0;
		y = ty + 4;
		}
	    }
	else if( !ch )
	    switch( getch() ) {
		case 72 : putimage( x, y, bitmap, COPY_PUT);
                          outtextxy( x + 4, y, opt[choice] );
			  if( !choice ) choice = 3;
				else choice--;
			  y = ty + choice * ( line_height + 4 ) + 4;
			  putimage( x, y, bitmap, XOR_PUT);
                          outtextxy( x + 4, y, opt[choice] );
			  break;
		case 80 : putimage( x, y, bitmap, XOR_PUT);
                          outtextxy( x + 4, y, opt[choice] );
			  if( choice==3 ) choice = 0;
				else choice++;
			  y = ty + choice * ( line_height + 4 ) + 4;
			  putimage( x, y, bitmap, XOR_PUT);
                          outtextxy( x + 4, y, opt[choice] );
			  break;
		}
     } while( !quit );
  restorecrtmode();
}

/* - - - - - - - - - -Function for initializing the screen - - - - - - - - */
void screen( void )
{
  int i, x, y, p[10];
  unsigned char pattrn1[8] = {0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55};
  unsigned char pattrn3[8] = {0x10,0x01,0x10,0x01,0x10,0x01,0x10,0x01};

  cleardevice();
  settextstyle( 7, HORIZ_DIR, 5);
  i = textwidth("Quiz Taker") / 2;
  x = midx - i;
  setfillpattern( pattrn3, 1);
  p[0] = p[6] = p[8] = x - 10;
  p[1] = p[3] = p[9] = 6;
  p[2] = p[4] = midx + i + 10;
  p[5] = p[7] = 14 + textheight("T");
  fillpoly( 5, p);
  outtextxy( x, 4, "Quiz Taker");
  p[0] = p[6] = p[8] = lb;
  p[1] = p[3] = p[9] = tb;
  p[2] = p[4] = rb;
  p[5] = p[7] = bb;
  setfillpattern( pattrn1, 1);
  fillpoly( 5, p);
  x = imagesize( lx, ty + 4 , rx, ty + line_height + 8);
  bitmap = ( char * ) malloc( i );
  if( bitmap == NULL ) {
	restorecrtmode();
	exit(0);
	}
  getimage( lx, ty + 4, rx, ty + line_height + 8, bitmap);
  setcolor( 0 );
  rectangle( lx - 2, ty - 2, rx + 2, by + 2);
  y = tb + line_height / 2;
  i = bb - line_height / 2;
  circle( lb + char_width + 3, y, char_width - 3);
  circle( lb + char_width + 3, i, char_width - 3);
  circle( rb - char_width - 3, i, char_width - 3);
  circle( rb - char_width - 3, y, char_width - 3);
  line( lb + 6, y, lb - 3 + 2 * char_width, y);
  line( lb + 6, i, lb - 3 + 2 * char_width, i);
  line( rb - 6, y, rb + 3 - 2 * char_width, y);
  line( rb - 6, i, rb + 3 - 2 * char_width, i);
  setcolor( WHITE );
  settextstyle( 2, HORIZ_DIR, 6);
  x = midx - textwidth("Use cursor keys to select any item") / 2;
  i = textheight("T");
  y = max_y - 3 * i;
  outtextxy( x, y, "Use cursor keys to select any item");
  y = y + i + 2;
  x = midx - textwidth("Press <ENTER> when done") / 2;
  outtextxy( x, y,"Press <ENTER> when done");
  settextstyle( 1, HORIZ_DIR, 1);
  putimage( lx, ty + 4, bitmap, XOR_PUT);
  for( i = 0, y = ty + 4, x = lx + 4; i < 4; i++, y += line_height + 4)
	outtextxy( x, y, opt[i]);
}