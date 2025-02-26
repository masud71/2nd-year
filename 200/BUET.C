#include<graphics.h>
#include<stdlib.h>
#include "dos.h"

unsigned max_x, max_y;
char V_MODE;
int left_x;

void movex(int,int,int);
void movey(int,int,int);

void main(int argc,char *argv[])
{
  int x[]={309,283,269,257,249,244,237,233,231,230,231,233,237,244,249,257,269,283,309};
  int y[2][19]= {{212,229,241,249,256,261,264,267,268,269,268,267,264,261,256,249,241,229,212},
		{117,123,128,132,135,138,140,141,142,142,142,141,140,138,135,132,128,123,117}};
  char data[]={11,14,3,12,12,5,20,17,18};
  int i, j, start, end, mid_x, mid_y , y_inc, no_cir = 10, no_arc = 10, r, start_y, gd, gm;
  char ch_y = 0, far *screen;

  if (argc !=2 || strcmp(argv[1],"Pass0")) {
	printf("This program requares Quiz Taker (QT.EXE)\n");
	exit(0);
  }
  detectgraph( &gd, &gm);
  switch( gd ) {
	case 1 : gm = 3;
		 V_MODE = 1;
		 break;
	case 3 :
	case 4 : gm = 1;
		 V_MODE = 2;
		 break;
	case 5 : gm = 3;
		 V_MODE = 2;
		 break;
	case 7 : gm = 0;
		 V_MODE = 3;
		 break;
	case 9 : gm = 1;
		 V_MODE = 2;
		 break;
	default :exit(0);
	}
  initgraph(&gd,&gm,"");
  max_x = getmaxx();
  max_y = getmaxy();
  if( V_MODE == 1 ) for( i = 0 ; i < 19 ; i++ ) x[i] -= 48;
     else if( V_MODE == 2 ) for( i = 0 ; i < 19 ; i++ ) x[i] -= 40;
  if( V_MODE == 1 ) {
	y_inc = 6;
	start_y = 31;
	mid_x = 320;
	mid_y = 85;
	r = 140;
	ch_y = 1;
	left_x = 190;
	}
    else if( V_MODE == 2 ) {
	y_inc = 10;
	start_y = 80;
	mid_x = 320;
	mid_y = 170;
	r = 130;
	left_x = 190;
	}
    else {
    	y_inc = 10;
	start_y = 80;
	mid_x = 360;
	mid_y = 170;
	r = 130;
	left_x = 230;
	}
  i = start_y;
  for(j = 0; j < 19; j++) {
	if( kbhit() ) return;
	movex( x[j], i, 2*( mid_x - x[j] ) );
	i += y_inc;
	sound(1000);
	delay(50);
	nosound();
	}
  i = left_x + 13;
  for( j = 0; j < 19; j++ ) {
        if( kbhit() ) return;
	movey( i, y[ch_y][j], 2 * ( y[ch_y][j] - mid_y ));
	i += 13;
	sound(1000);
	delay(50);
	nosound();
	}
  setcolor( WHITE );
  for( i = 0; i < no_cir; i++, r++) {
	circle( mid_x, mid_y, r);
	if( kbhit() ) return;
	}
  for( i = 0; i < no_arc; i++, r++) {
        if( kbhit() ) return;
	start=10;
	end=20;
	     do{
		arc(mid_x,mid_y,start,end,r);
		start=end+10;
		end=start+10;
		} while( end <= 380 );
	};
  setlinestyle( SOLID_LINE, 0, THICK_WIDTH );
  rectangle( 1, 1, max_x - 1, max_y - 1);
  setlinestyle( SOLID_LINE, 0, NORM_WIDTH );
  start = left_x + 25;
  end = start_y + 14 * y_inc + 1;
  for( j = 0; j < 9; j++ ) {
	for( i = 0; i < data[j]; i++ ){
                if( kbhit() ) return;
		floodfill( start, end - i * y_inc, WHITE );
		sound( 300 );
		delay( 20 );
		nosound();
		delay( 20 );
		}
	start += 13;
	if( j < 5 )  end += y_inc;
	}
  i = left_x + 11;
  j = start_y + 9 * y_inc + 1;
  start = left_x + 248;
  end = j + 4 * y_inc - 2;
  setcolor( BLACK );
  line( left_x, j + y_inc - 1, start + 12, j + y_inc - 1 );
  line( left_x + 2, j + y_inc * 2 - 1, start + 9, j + y_inc * 2 - 1);
  line( left_x + 5, j + y_inc * 3 - 1, start + 7, j + y_inc * 3 - 1);
  setcolor( WHITE );
  setviewport( i, j, start, end, 0);
  clearviewport();
  setviewport( 0, 0, max_x, max_y, 1);
  rectangle( i , j + 4, start , end - 4);
  mid_y = ( end - j - 8)/3 + j + 4;
  mid_x = ( end - j - 8)/3 + mid_y;
  if( kbhit() ) return;
  rectangle( i, mid_y, start, mid_x);
  rectangle( left_x + 156, j + 2, left_x + 195, end - 2);
  line( left_x + 176, j + 2, left_x + 176, end - 2 );
  settextstyle( 4, HORIZ_DIR, 6);
  start = max_x/2 - textwidth("Quiz Taker")/2;
  outtextxy( start, 6, "Quiz Taker");
  settextstyle( 2, HORIZ_DIR, 5);
  start = max_x/2 - textwidth("Examineer  Of")/2;
  end = max_y - textheight("T") * 4;
  outtextxy( start, end, "Examineer Of");
  start = max_x/2 - textwidth("Bangladesh University Of Engineering & Technology")/2;
  end = max_y - textheight("T") * 3;
  outtextxy( start, end, "Bangladesh University Of Engineering & Technology");
  start = max_x/2 - textwidth("Developed By Students Of Computer Science & Engineering Department")/2;
  end = max_y - textheight("T") * 2;
  outtextxy( start, end, "Developed By Students Of Computer Science & Engineering Department");
  getch();
  if( V_MODE == 3 ) {
	screen = MK_FP( 0xB000, 0x0000);
	for( i = 0; i < 406; i++) {
	    for( j = 0; j < 80; j++)
		*screen++ = 0;
	    if( !(i%100) ) delay(100);
	    }
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void movex(int x,int y,int len)
{
 int i;

 for( i = max_x; i >= x; i -= 8) {
	setcolor( WHITE );
	line( i, y, i+len, y);
	setcolor( BLACK );
	line( i+len+1, y, i+len+8, y);
	}
 setcolor( WHITE );
 for( i += 8; i >= x; i--) {
	line( i, y, i+len, y);
	putpixel( i + len + 1, y, BLACK );
	}
 for( i = left_x; i <= 480; i += 13 ) putpixel( i, y, BLACK);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void movey(int x,int y,int len)
{
 int i;

 for( i = 0; i <= y; i += 8 ) {
	setcolor( WHITE );
	line( x, i , x , i - len );
	setcolor( BLACK );
	line( x, i - len - 1, x, i - len - 8 );
	}
 setcolor( WHITE );
 for( i -= 8; i <= y; i++ ) {
	line( x, i , x , i - len );
	putpixel( x, i - len - 1, 0);
	}
}