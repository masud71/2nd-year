 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *		Symbol portion of QUIZ TAKER program                     *
 *   Written by Md. Mamunul Islam for group-6 on the 12th December, 1992 *
 *   This part of the program works in graphics mode only. Its job is to *
 *   extract data from specified library  files and generate sybols with *
 *   those data . The user can place the  generated symbol in  any place *
 *   he wants.As this part works in graphics mode ,it can only be called *
 *   while working with  graphics environment and several data has to be *
 *   supplied to this part. these data are declared as exetern bellow.   *
 *      	      ***      Features        ***                       *
 *   The user can rotate any symbol in 0 , 90 , 180 and 270 degree of    *
 *   angle . He can also change the moving distance . The program uses a *
 *   very short memory by using dynamiã allocation for storing the  data *
 *   and as the job is done with the symbols this memory is freed.       *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <ctype.h>

/*  The following identifiers are declared in other parts of the program  .
    These are neccessary foe this part .
    cur_x , cur_y : position of the cursor ,
    mov : moving distance of the cursor ,
    lx, rx, ty, by : these denote the area of the screen in graphics mode .
	  These identifiers are neccessary to separate the text from the
	  graphics .
    maxx, maxy : they determine the resolution the whole screen. */

extern int cur_x , cur_y, lx, rx, ty, by, maxx, maxy;

#define MAXMENU 200
#define UP    72
#define DOWN  80
#define LEFT  75
#define RIGHT 77

char  sym_lib_file[14] = "quiz.LIB";
char *s_menu[ MAXMENU ], *symbols, *image_sym, *bar_image, *move_image;
int s_col , s_row, x , y, angle = 0, rotate, max_sym = 0;
int mov;
FILE *fp;

void flush_mem( void )
{
/*   NAME : flush_mem
     FUNCTION : it frees the memory that was allocated for the symbol names
     EXAMPLE : flush_mem();
     VARIABLES USED : i
     MACROS USED : MAXMENU
     ROUTINES CALLLED : free
*/
 unsigned char i;

 for(i = 0; i < max_sym; i++)
	if( s_menu[i] ) free( s_menu[i] );
	  /* Try to free memory only if it was previously allocated */
}

void viewclear(int x1, int y1, int x2, int y2 )
{
 setviewport(x1, y1, x2, y2, 0);
 clearviewport();
 setviewport(0, 0, maxx, maxy, 0);
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
 int points[20] ;

 setwritemode(COPY_PUT);
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
 fillpoly(10, points);
 setcolor(1);
 setfillstyle(SOLID_FILL,1);
 fillpoly(5, &points[10]);
 viewclear(x1 + 30, y1 + 3, x2 - 5, y1 + 17);
 setcolor(0);
 line(x1+5, y1 + 17, x1 + 30, y1 + 17);
 rectangle(x1 + 7, y1 + 7, x1 + 20, y1 + 10);
 line(x1 + 8, y1 + 11, x1 + 21, y1 + 11);
 line(x1 + 21, y1 + 8, x1 + 21, y1 + 11);
 setcolor(1);
 outtextxy( x1 + 38, y1 + 6, s);
 setwritemode(XOR_PUT);
}

void error_msg(char *s)
{
/*   NAME : error_msg
     FUNCTION : If any error occurs this function is called to display that
	  error message .
     EXAMPLE : error_msg( "Symbol not found");
     VARIABLES USED : box, size
     MACROS USED : NULL
     ROUTINES CALLLED:imagesize, getimage, putimage, dialog_box, outtextxy,
	  rectangle, free, malloc, getch ,setcolor, putch.
*/
 char  *box;
 unsigned size;

 setwritemode(COPY_PUT);
 size = imagesize( 213, 55, 506, 127);
 /* If enough memory cannot be found then only make a beep sound */
 if( (box = (char  *) malloc(size) ) != NULL) {
	 getimage( 213, 55, 506, 127, box);
	     /* Save the image of the area where the message will appear */
         setcolor(1);
	 dialog_box(213,55,506,127,"           Error");
	     /* generate dialog box for the message */
	 setcolor(0);
	 outtextxy(225, 84 + 2, s);
	 outtextxy(225, 96 + 2, "Abnormal Termination");
	 rectangle(324,107,396,120);
	 outtextxy(351,110,"OK");
	 do {} while(getch() != 13);
	    /* Wait till <ENTER> key is pressed */
	 setcolor(1);
	 putimage(  213, 55, box, COPY_PUT); /* Restore the original  image */
	 free(box);   /* free the allocated memory */
         }
   else putch(7);
   setwritemode(XOR_PUT);
}

void sym_name_box( void)
{
/*   NAME : sym_name_box
     FUNCTION : Makes the dialog box to get the symbol name.
     EXAMPLE : sym_name_box();
     VARIABLES USED : None
     MACROS USED : None
     ROUTINES CALLLED : setcolor, dialog_box, rectangle, outtextxy.
*/
 setwritemode(COPY_PUT);
 setcolor(1);
 dialog_box( 213, 55, 506, 188, "     Symbol Selection" );
 setwritemode(COPY_PUT);
 setcolor(0);
 outtextxy(225, 84 + 2, "Symbol Name");
 rectangle(231, 106, 380, 181);
 line(232, 118, 379, 118);
 getimage( 234, 120, 377, 131, bar_image);
 setfillstyle(SOLID_FILL, 0);
 bar(234, 120, 377, 179);
 bar(333, 84, 497, 95);
 setfillstyle(SOLID_FILL, 1);
 rectangle(402, 143, 479, 156);
 rectangle(402, 167, 479, 180);
 outtextxy(279, 108, "Symbols");
 outtextxy(432, 144 + 2, "OK");
 outtextxy(414, 168 + 2, "Cancel");
 setcolor(1);
 setwritemode(XOR_PUT);
}

void change( char r)
{
/*   NAME : change
     FUNCTION : This function is only used by the take_name function. It jod is
	  to place an rectangle around the choice "OK" or "Cancel".
     EXAMPLE : change( r_value );
     VARIABLES USED : None
     MACROS USED : None
     ROUTINES CALLLED : rectangle, setcolor .
*/
 /*  Put an rectangle around the other box in white  so  that  it  will be
     invisible */
 setwritemode(COPY_PUT);
 if( r) rectangle(400, 165, 481, 182);
	else rectangle( 400, 141, 481, 158);
 setcolor(0);

 /*  Put rectangle around the box intended in black */
 if( !r ) rectangle(400 , 165, 481, 182);
	else rectangle( 400, 141, 481, 158);
 setcolor(1);
 setwritemode(XOR_PUT);
}

take_name( void)
{
/*   NAME : take_name
     FUNCTION : takes the name of the symbol from the keyboard.
     EXAMPLE : take_name( name );
     VARIABLES USED : i, r_value, ch, current, top, bar_image, move_image.
     MACROS USED : UP, DOWN
     ROUTINES CALLLED : getch, outtextxy, viewclear, change, putimage,
	  toupper, isalnum .
*/
 unsigned char current = 0, top = 0, i ,ch;
 char  r_value = 1;

 /* initialize the s_menu items in the dialog box */
 viewclear( 332, 84, 498, 98);
 outtextxy(342, 86, s_menu[0]);
 for( ch = 0; (ch < 5)  && (ch < max_sym); ch++ ) outtextxy( 234, 122+ch*12, s_menu[ch]);
 putimage( 234, 120, bar_image, XOR_PUT); /* highlight the first item */
 change(r_value);
 do {
	ch = getch(); /* Take the uppercase input only */
	if( !ch )
		switch( getch() ) {
		   case UP : if( current ) {
				/* if current item is not the first item */
				i = current - top;
				putimage( 234, 120+i*12, bar_image, XOR_PUT);
				/* if current item is not on top position */
				if( current != top ) putimage(234, 120+(i-1)*12, bar_image, XOR_PUT);
				  else {
					/* move the imge lower and put another item at the bottom */
					getimage( 234, 120, 377, 167, move_image);
					putimage( 234, 132, move_image, COPY_PUT);
					viewclear(234,120,377,131);
					top--;
					outtextxy(234, 122, s_menu[top]);
					putimage(234, 120, bar_image, XOR_PUT);
				       }
				current--;  /* decrease current position */
				viewclear(340, 84, 498, 95);
				outtextxy(342, 86, s_menu[current]);
			       }
			       break;
		   case DOWN : if( current + 1 < max_sym) {
				/* if current item is not the last item */
                                i = current - top;
				/* restore the previous item to original color */
				putimage( 234, 120+i*12, bar_image, XOR_PUT);
				current++;  /* increase current position */
				/* if current item is not in the last line in display*/
				if( i < 4 ) putimage(234, 120+(i+1)*12, bar_image, XOR_PUT);
				  else {
					getimage( 234, 132, 377, 179, move_image);
					putimage( 234, 120, move_image, COPY_PUT);
					viewclear(234, 168, 377, 179);
					top++;
					outtextxy(234, 170, s_menu[current]);
					putimage(234, 168, bar_image, XOR_PUT);
				       }
				viewclear(340, 84, 498, 95);
				outtextxy(342, 86, s_menu[current]);
			       }
		   }
	else if( ch == 9) {   /* tab key is pressed */
		if (r_value) r_value = 0;
			else r_value = 1;
		change(r_value);
		}
	else if( ch == 27) return -1;    /* if escape key is pressed discard
					everything and return 0 */
    } while ( ch != 13);
	  /* continue untill enter key is pressed */
 if( r_value ) return current;  /* if "OK" return the current position */
  else return -1;
}

getmenu( void )
{
/*   NAME : getmenu
     FUNCTION : takes the symbol names from the library file and stores them
		in the array s_menu .
     EXAMPLE : getmenu();
     VARIABLES USED : ch, str, j, l, fp, max_sym.
     MACROS USED : NULL, MAXMENU, SEEK_SET
     ROUTINES CALLLED : malloc, getc, getw, fopen, fgets, fseek, strcmp,
		ferror, fclose, error_msg.
*/
 unsigned char ch;
 register unsigned char j, l;

 fseek(fp, 41, SEEK_SET);
 /* get the maximum number of symbols present in the library file */
 max_sym = getc( fp );
 fseek(fp, 44, SEEK_SET );
 for( j = 0; j < max_sym; j++) {
	/* if enough memory is not available show error message and abandon */
	if(( s_menu[j] = (char  *) malloc( 17 ) ) == NULL) {
		max_sym = j;  /* make some error sound here */
		return 0;
		}
	/* Read symbol name of k length. if any error occurs abandon job */
	for(l = 0; l < 16; l++) {
		ch = getc(fp);
		if (ferror(fp)) {
			error_msg("Error reading symbol file");
			max_sym = j;
			return 0;
			}
		s_menu[j][l] = ch;
		}
	s_menu[j][l] = '\0';
	getw( fp );
	}
 return 1;
}

get_symbol( void )
{
/*   NAME : get_symbol
     FUNCTION : gets the data neccessary from the symbol library file to
		construct the image of the symbol.
     EXAMPLE : get_symbol();
     VARIABLES USED :  i, j, name, varify, box, fp, s_col, s_row.
     MACROS USED : MAXMENU, NULL, SEEK_SET
     ROUTINES CALLLED : malloc, ferror, fopen, fseek, fclose, imagesize,
	flush_mem, sym_name_box, take_name, getimage, putimage,strcmp,
	getw, getc, free.
*/
 unsigned i, j;
 char name[42], varify;
 char  *box;


 if( (fp = fopen( sym_lib_file, "rb")) == NULL ) {
	error_msg( "Library file not found");
	return 0;
	}
 /* check if the library file is valid . There is a special marker for
    the library file */
 if( fgets( name, 42, fp) == NULL) {
	error_msg( "Error reading symbol file");
	return 0;
	}
 if(strcmp( name, "~QUIZ TAKER SYMBOL FILE : MAMUN,ROLL-13„…")) {
	error_msg("Error in symbol file.");
	return 0;
	}
 /* get the s_menu items from the file */
 if( !getmenu() ) {
	/* if something goes wrong stop and quit */
	flush_mem();
	fclose( fp );
	return 0;
	}
 /* Save area of the screen where the dialog box will appear, if any error
    occurs this module will be abandoned  */
 if( (box = (char  *) malloc(imagesize(213, 55, 506, 188))) == NULL) {
	error_msg("Not enough memory availavle");
	return 0;
	}
 if( (bar_image = (char  *) malloc(imagesize( 1, 1, 146, 12))) == NULL) {
	error_msg("Not enough memory availavle");
	free( box );
	return 0;
	}
 if( (move_image = (char  *) malloc(imagesize(234, 120, 377, 167))) == NULL) {
	error_msg("Not enough memory availavle");
	free( box );
	free( bar_image );
	return 0;
	}
 getimage( 213,55,506,188, box);
 /* take name of the sybol . if any error occurs or the user does not want to
     continue abandon job */
 sym_name_box();
 varify = take_name() ;
 free( bar_image );
 free( move_image );
 /* Restore the original screen and free the memory allocated to store it */
 putimage( 213,55, box, COPY_PUT);
 free( box);
 if( varify == -1 ) return 0 ;
 flush_mem();  /* free allocated memory for s_menu */
 /* search for the position in the file where the data for the desired symbol
    is stored .*/
 fseek( fp, varify * 18 + 42, SEEK_SET);
 i = getw( fp );
 fseek( fp, i, SEEK_SET);
 /* i is the number of bytes the sybol takes  in memory */
 i = getw( fp );
 s_col = getc( fp );	/* first byte contains the no. of column */
 s_row = getc( fp );	/* second byte contains the no. of rows */
 i -= 2;
 if( (symbols = (char  *) malloc(i)) == NULL) {
	error_msg("Memory not available");
	return 0;
	}
 /* get the data for the symbol . If error occcurs abandon */
 for( j = 0; j < i; j++) {
	symbols[j] =  getc(fp);
	if( ferror(fp)) {
		error_msg("File Read error");
		free(symbols);	/* free sybols before leaving */
		return 0;
		}
	}
 fclose( fp);
 return 1;
}

initialize_symbol( void )
{
/*   NAME : initialize_symbol
     FUNCTION : initialize the image of the symbol depending on the angle and
	store the image in the 'image_sym' pointer .
     EXAMPLE : initialize_symbol();
     VARIABLES USED : box, i, j, k, t, size, s_col, s_row, rotate, x, y, angle,
	cur_x, cur_y, lx, rx, ty, by .
     MACROS USED : COPY_PUT , NULL
     ROUTINES CALLLED : imagesize, getimage, putimage, free, malloc, putpixel,
		setviewport, clearviewport, error_msg.
*/
 char  * box;
 register unsigned char t, i, j;
 unsigned char k;
 unsigned size;

 /* set horizontal and vertical width of the symbol dependig on the angle */
 /* angle = 0 :   0 degree
    angle = 1 :  90 degree
    angle = 2 : 180 degree
    angle = 3 : 270 degree */
 if ((!angle) || (angle == 2)) {
	if ((cur_x + s_col <= rx) && (cur_y + s_row <= by)) {
		x = s_col;
		y = s_row;
	       }
	 else return 0;
	 /*if the symbol does not fit in the window then return */
	}
   else if((cur_x + s_row <= rx) && (cur_y + s_col <= by)) {
	x = s_row;
	y = s_col;
	}
   else return 0; /*return if the symbol does not fit in window */
 size = imagesize(cur_x, cur_y, cur_x + x - 1, cur_y + y - 1);
 /* allocate memory to save the image of the screen */
 if( (box = (char  *) malloc(  size)) == NULL) {
	error_msg("Not enough memory available");
	return 0;
	}
 /* store the image of the screen in the allocated memory 'box' */
 getimage( cur_x, cur_y, cur_x + x - 1, cur_y + y - 1, box);
 viewclear( cur_x, cur_y, cur_x + x - 1, cur_y + y - 1 );
 k = 0;
 t = 128;     /* t is the mask . comaring with it it is determined which bit
		 is on of the 'symbols' */
 /* make an image of the sybol . the data stored in 'symbols' determine the
    symbol . it consist of numbers which denote the pixels . if the bit is
    1 it is turned on or if it is 0 nothing is done */
 for( i = 0; i < s_row ; i++)
	for (j = 0; j < s_col; j++) {
	   /* if the bit is 1 then put pixel dependig on s_col and s_row */
	   if( symbols[k] & t) {
		/* put the pixel according to the angle value */
		if( !angle ) putpixel( cur_x + j, cur_y + i, 1);
		else if( angle == 3 ) putpixel( cur_x + i, cur_y + j, 1);
		else if( angle == 2 ) putpixel( cur_x + s_col - j - 1, cur_y + s_row - i - 1, 1);
		else putpixel( cur_x + s_row - i - 1, cur_y + s_col - j - 1, 1);
	       }
	   t >>= 1;	/* mask is shifted 1 bit to check the next bit */
	   if( !t ) {
	   /* if t has the value 0 it is then reassigned with 128 and the
	      next byte is taken from the 'symbols' */
		t = 128;
		k++;
	       }
	   }
 /* if memory was previously allocated for the 'image_sym' , free it */
 if( rotate )  free( image_sym );
 /* allocate memory for the image of the symbol and get the image */
 image_sym = (char  *) malloc( size );
 if( image_sym )  getimage( cur_x, cur_y, cur_x + x - 1, cur_y + y - 1, image_sym);
 /* put the original image stored before and free the memory allocated for it*/
 putimage( cur_x, cur_y, box, COPY_PUT );
 if( box ) free(box);
 if( image_sym )  {
	rotate = 1;
	return 1;
	}
 return 0;
}

void symbol( void )
{
/*   NAME : symbol
     FUNCTION : its the main function of this part of the project . it uses the
	other functions to make the s_menu and get the symbol from the library
	file and make an image of the symbol. Then it uses this image to move
	it around and place in the parfect place, it also rotates it and
	changes the moving distance .
     EXAMPLE : symbol();
     VARIABLES USED : cur_x, cur_y, x, y, lx, rx, ty, by, i, ch, rotate,
	angle, image_sym, symbols, mov.
     MACROS USED : XOR_PUT, OR_PUT, UP, DOWN, LEFT, RIGHT
     ROUTINES CALLLED : error_msg, flush_mem, getmenu, get_symbol, getch,
	initialize_symbol, putimage, free, outtextxy, toupper, strchr.
*/
 unsigned char i, ch;

 /* set the graphics environment to normal */
 setviewport(0, 0, maxx, maxy, 0);
 settextstyle( DEFAULT_FONT, HORIZ_DIR, 1 );
 settextjustify( LEFT_TEXT, TOP_TEXT );
 setfillstyle( SOLID_FILL, 1);
 setcolor( 1 );
 angle = rotate = max_sym = 0;
 mov = 5;
 /* get data required to generate the image of the symbol */
 if (!get_symbol()) return;
 /* get the initial image of the symbol */
 if( ! initialize_symbol()) {
	free( symbols );
	error_msg("Symbol does not fit in");
	return;
	}
 /* show the options here*/
 viewclear(10, maxy - 10, maxx, maxy);
 outtextxy(10, maxy - 10, "Place  Rotate  Normal  Over  Up  Down ('+':inc. '-':dec.)moving distance  Escape");
 do {
	/* put the image temporarily untill a key is pressed */
	putimage(cur_x , cur_y, image_sym, XOR_PUT);
	ch = toupper( getch());
	putimage(cur_x , cur_y, image_sym, XOR_PUT);
	/* turn the image off */
	if ( !ch )
		switch ( getch() ) {
			/* change the current cursor position depending
			   on the key pressed */
			case UP : if ( cur_y - mov > ty ) cur_y -= mov;
				  break;
			case DOWN : if( cur_y + mov + y <= by ) cur_y +=mov;
				    break;
			case LEFT : if( cur_x - mov > lx) cur_x -= mov;
				    break;
			case RIGHT : if(cur_x + mov + x <= rx ) cur_x +=mov;
				     break;
		       }
	/* place the image permanently at current cursor position */
	else if( ch == 'P') putimage(cur_x, cur_y, image_sym, OR_PUT);
	else if( strchr("RUDON",ch) ) {
	      i = angle;	/* save the current angle */
	      if( ch == 'R') {
		/* change angle of the symbol by 90 degree */
		if( angle != 3) angle++;
		  else angle = 0;
		}
		else if( ch == 'U' ) angle = 1;	   /*set angle to 90 degree*/
		else if( ch == 'D' ) angle = 3;    /*set angle to 270 deg. */
		else if( ch == 'O' ) angle = 2;    /*set angle to 180 deg. */
		else if( ch == 'N' ) angle = 0;    /*set angle to 0 deg.   */
	      if( ! initialize_symbol()) angle = i;
		  /* if image in new angle cannot be constructed then restore
		     previous angle */
	      }
	else if( ch == '+' ) mov++;   /* increase moving distance */
	else if( ch == '-' ) if( mov ) mov--;
		/* decrase moving distance if 'mov' is not 0*/
	} while (ch != 27);
 viewclear(10, maxy - 10, maxx, maxy);
 outtextxy( 300, maxy - 8, "F10-MENU");
 if( image_sym ) free(image_sym);
 if( symbols ) free(symbols);
}
