/*
	Decimal to Roman converter program. This program takes input of Decimal
	integer & then convert it into Roman digits. For example :-If the input
	is 2328 then the output is MMCCCXXVIII .The progrem is operated by four
	functions. The program takes data from the keyboard. The input function
	is designed in such a way,it takes input only digits. The input session
	is terminated by <Enter> Key or by taking maximum length of input. Then
	the data is converted to Roman by the process  function & echoed in the
	CRT by  put function . The program terminates if <Enter> key is pressed
	without entering any data during a input session.
							Designed by
								MASUD KARIM KHAN
								Roll - 17
								2nd year , C S E
								B U E T
*/

#include <stdio.h>  /* For standerd input & output      */
#include <ctype.h>  /* To use "isdigit" function        */
#include <dos.h>
#include <conio.h>
/*--------------------------------------------------------*/
#define LENGTH 7	/* Defining maximum length of input */
#define ENTER 13	/* ASCII value of <Enter> button    */
#define HLINE 205
#define V_LINE 186
#define LTC 201
#define LBC 200
#define RTC 187
#define RBC 188
/*--------------------------------------------------------*/
int input(char *);
void process(char *,int);
void put(int);
void underscore(void);
void box(int,int,int,int);
void setcursor(unsigned int);
/*--------------------------------------------------------*/
main()
	/* 	Main block of Decimal to Roman converter program .
		VARIABLES : size - Stores the size of input data returned from
						   the function input.
					data - A charecter array stores the input data
		MACRO USED: LENGTH
	*/
{
	int size;
	char data[LENGTH];

	clrscr();
    box(10,6,70,19);
	textcolor(128);
	textbackground(15);
	gotoxy(26,6);cprintf(" Decimal To Roman Converter ");
	textcolor(15);
	textbackground(0);
	window(12,8,68,18);
	do {
		size=input(data); /* Takes input of decimal data by input function */
		if (size  >= 0) 	/* Process if any data entered */
			process(data,size);
	} while (size >= 0);  		/* Exit if no data entered 	   */
	window(1,1,80,25);
	clrscr();
	printf("Good Bye from ROMAN CONVERTER program\n");
}
/*--------------------------------------------------------*/
int input(char data[])
	/*
		NAME     : 	input
		FUNCTION : 	To take input of a digital data whoes maximum length
					is 7 digit
		EXAMPLE  : 	size=input(data);
		VARIABES : 	counter -To index the data array & find its length
					ch      -For taking input of each charecter from Keyboard
		MACRO    :  ENTER , LENGTH
		ROUTINES : 	Stdio.h ,Ctype.h
   */
{
	int counter=0;
	char ch;

	clrscr();
	highvideo();
	cprintf("Enter an integer data (Max 7 digit) , then press <Enter>\n\r");
	cprintf("        Press <Enter> without any input to quit");
	gotoxy(10,5);
	cprintf("Input   :");
	gotoxy(10,7);
	cprintf("Output  :");
	gotoxy(10,9);
	cprintf("Massage :");
	lowvideo();
	gotoxy(20,5);
	setcursor(2828);
	do {
		ch=getch();
		if (isdigit(ch))   /* If entered data is a digit store & echo */
			{
				putch(ch);
				data[counter++]=ch;
			}
	} while ((ch != ENTER) && (counter < LENGTH)); /* Checks termination */
	return (counter - 1);  /* Returns last array index */
}
/*--------------------------------------------------------*/
void process(char data[],int counter)
	/*
		NAME     : 	process
		FUNCTION : 	To convert a digital data into Roman & show it in CRT
					using put function
		EXAMPLE  : 	process(data,size);
		VARIABES : 	indicator - points the active charecter to convert
					position  - to store the position , which charecter
								to be printed
					i         - store how many repeatable charecter to be
								printed
		ROUTINES : 	Stdio.h
   */
{
	int indicator,position,i;

	setcursor(-1);
	if ((counter == LENGTH - 1) && (data[0] > '3'))
		{
			gotoxy(20,9);
			putch(7);
			cprintf("Higher than maximum processable value");
			gotoxy(20,10);
			cprintf("Press any key to continue . . .");
			getch();
			return;    /* Exit from current function */
		}
	gotoxy(20,7);
	for (indicator = 0;indicator <= counter; indicator++)
		{
			position=(counter - indicator) * 2;
			/* calculates the position of charecters to be printed */
			switch (data[indicator])
				{
					case '9' :	put(position); /*displays 9,90,900,...*/
								put(position + 2); 	/* by 1,10,100,... */
								break;				/* at left         */
					case '8' :
					case '7' :
					case '6' :
					case '5' :  i = data[indicator] - '5';
								/* calculates no. of repeatable digits */
								put(position + 1);
								/* puts position value of 5 */
								while(i--)
									put(position);
								break;
					case '4' :	put(position); /*displays 4,40,400,...*/
								put(position + 1); /* by 1,10,100,... */
								break;			   /* at left         */
					case '3' :
					case '2' :
					case '1' :	i = data[indicator] - '0';
								/* calculates no. of repeatable digits */
								while( i-- )
									put(position);
								break;
				}
		}
	gotoxy(20,9);
	cprintf("Press any key to continue . . .");
	getch();
}
/*--------------------------------------------------------*/
void put (int number)
	/*
		NAME     : 	put
		FUNCTION : 	To put a Roman charecter in CRT
		EXAMPLE  : 	process(data,size);
		VARIABES : 	store - a charecter array containing all Roman digits
					according to their values . From this arrray different
					charecters are taken according to the parameter number
					& printed in CRT
		ROUTINES : 	Stdio.h
   */
{
	char store[7]="IVXLCDM";

	if (number > 6)   /* print underscored number if the value of the digit
						 is higher then normal Roman digits              */
		{
			underscore();
			number -= 6;
		}
	putch(store[number]);
}
/*--------------------------------------------------------*/
void underscore(void)
	/*
		NAME     : 	underscore
		FUNCTION : 	To put a undrscore one line up to the current position
		EXAMPLE  : 	underscore(void);
		ROUTINES : 	Stdio.h
   */
{
	gotoxy(wherex(),wherey() - 1); /* Go one charecter up */
	putch('_');
	gotoxy(wherex() - 1,wherey() + 1); /* Go back to previous position */
}
/*--------------------------------------------------------*/
void box(int  X1,int Y1,int X2,int Y2)
{
	int i;

	gotoxy(X1,Y1);
	putch(LTC);
	for(i=1;i<=(X2-X1-1);i++) {
		gotoxy(X1+i,Y1);
		putch(HLINE);
        gotoxy(X1+i,Y2);
		putch(HLINE);
	}
	gotoxy(X2,Y1);
	putch(RTC);
	for(i=0;i<=(Y2-Y1-1);i++){
		gotoxy(X2,Y1+1+i);
		putch(V_LINE);
		  gotoxy(X1,Y1+1+i);
		putch(V_LINE);
	}
	gotoxy(X2,Y2);
	putch(RBC);
	gotoxy(X1,Y2);
	putch(LBC);
}
void setcursor(unsigned int shape)
{
	union REGS reg;

	reg.h.ah=1;
	reg.x.cx=shape;
	int86(0x10,&reg,&reg);
}