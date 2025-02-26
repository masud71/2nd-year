#include <graphics.h>
#include <alloc.h>

extern dialog_box(int,int,int,int,char *);
extern g_outtextxy(int,int,char *,int);
extern void save();
extern void leave_now();

void about(void);
void page1(void);

extern char quit_now,save_now;

void  about(void)
{
/*   NAME : about
	 FUNCTION : Display information about the Program.
     VARIABLES USED : box, size
     MACROS USED : NULL
	 ROUTINES CALLLED:imagesize, getimage, putimage, dialog_box, g_outtextxy,
	  rectangle, farfree, farmalloc, getch ,setcolor, putch.
*/
 char  far *box;
 unsigned size;

 size = imagesize( 175, 55, 575, 223);
 /* If enough memory cannot be found then only make a beep sound */
 if( (box = (char   far *) farmalloc(size) ) != NULL) {
	 getimage( 175, 55, 575, 223, box);
	     /* Save the image of the area where the message will appear */
	 viewclear( 175, 55, 575, 223);
	 setcolor( WHITE );
	 dialog_box( 175, 55, 575, 223, "       About Quiz Taker ...");
	     /* generate dialog box for the message */
	 setcolor(BLACK);
	 page1();
	 rectangle(340,202,390,217);
	 g_outtextxy(355,203,"OK", XOR_PUT);
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
	 putimage(  175, 55, box, COPY_PUT); /* Restore the original  image */
	 farfree(box);   /* free the allocated memory */
	 }
   else putch(7);
}

void page1()
{
	g_outtextxy(190, 76,"Automatic Quiz Taker System", XOR_PUT);
	g_outtextxy(250, 95,"Programmed by :-", XOR_PUT);
	g_outtextxy(275, 114,"Md. Mamunul Islam",XOR_PUT);
	g_outtextxy(275, 128,"Asad Roni",XOR_PUT);
	g_outtextxy(275, 142,"Md. Mostofa Akbar",XOR_PUT);
	g_outtextxy(275, 156,"Masud Karim Khan",XOR_PUT);
	g_outtextxy(275, 170,"Md. Farhad Hossain",XOR_PUT);
	g_outtextxy(275, 184,"Shibir Kumar Shaha",XOR_PUT);
}