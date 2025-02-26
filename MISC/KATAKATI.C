/* ---KATAKATI--- A classical game. Programmed by Masud Karim Khan.  */

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

int entry(char *,char *);
void showpoint(int,int);
void gamebox(void);
int check_win();
void play(char *,char *);
void setcursor(unsigned int);

int point1,point2,plr1;
int matrix[3][3];

main()
{
	char name1[40],name2[40];
	int count;

	point1=point2=0;
	count=entry(name1,name2);
	clrscr();
	gotoxy(35,1);
	printf("KATAKATI GAME\n\n");
	printf("[ X ] %-50s[ O ] %-20s",name1,name2);
	showpoint(point1,point2);
	randomize();
	plr1=random(2);
	do {
		gamebox();
		play(name1,name2);
		count--;
	} while (count);
	window(1,1,80,25);
	clrscr();
	printf("%s ,your points : %d\n",name1,point1);
	printf("%s ,your points : %d\n\n",name2,point2);
	if (point1>point2) printf("%s",name1);
	else if (point2>point1) printf("%s",name2);
	if (point1 !=point2) printf(",congratulation ! You have won the game.\n");
	else printf("Thank you both to contest equally in the game.\n");
	setcursor(2828);
}

int entry(char *name1,char *name2)
{
	int tempnum;

	clrscr();
	printf("Enter first player's name : ");
	gets(name1);
	printf("\nEnter second player's name : ");
	gets(name2);
	printf("\nEnter total round to play : ");
	scanf("%d",&tempnum);
	return tempnum;
}

void showpoint(int pt1,int pt2)
{
	window(1,5,80,5);
	printf("%5d%60d",pt1,pt2);
}

void gamebox(void)
{
	register int i,j;

	window(29,8,80,25);
	clrscr();
	putch(218);
	for (i=2;i<=24;i++) putch(196);
	putch(191);
	for (i=2;i<=12;i++) {
		gotoxy(1,i);putch(179);
		gotoxy(9,i);putch(179);
		gotoxy(17,i);putch(179);
		gotoxy(25,i);putch(179);
	}
	gotoxy(1,13);putch(192);
	for (i=2;i<=24;i++) putch(196);
	putch(217);
	gotoxy(1,5);putch(195);
	for (i=2;i<=24;i++) putch(196);
	putch(180);
	gotoxy(1,9);putch(195);
	for (i=2;i<=24;i++) putch(196);
	putch(180);
	gotoxy(9,1);putch(194);
	gotoxy(17,1);putch(194);
	gotoxy(9,13);putch(193);
	gotoxy(17,13);putch(193);
	gotoxy(9,5);putch(197);
	gotoxy(9,9);putch(197);
	gotoxy(17,5);putch(197);
	gotoxy(17,9);putch(197);
	for (i=0;i<=2;i++)
		for (j=0;j<=2;j++)
			matrix[i][j]=0;
}

void play(char *name1,char *name2)
{
	char ch;
	int x=13,y=7,run,win,count=0;

	do {
		gotoxy(1,16);clreol();
		if (plr1)
			cprintf("%s your move.",name1);
		else cprintf("%s your move.",name2);
		run=1;
		setcursor(12);
		do {
			gotoxy(x,y);
			ch=getch();
			if (! ch) ch=getch();
			switch (ch) {
			case 75:	if (x==5) x=21;
							else x-=8;
						break;
			case 77:	if (x==21) x=5;
							else x+=8;
						break;
			case 72:	if (y==3) y=11;
							else y-=4;
						break;
			case 80:	if (y==11) y=3;
							else y+=4;
						break;
			case 13: if (matrix[y/4][x/8] !=0) putch(7);
						else {
							if (plr1) { putch('X');matrix[y/4][x/8]=1;}
								else {putch('O');matrix[y/4][x/8]=-1;}
							run=0;
						}
						break;
			}
		} while(run);
		plr1= ! plr1;
		win=check_win();
		count++;
	}while ((win==0) && (count < 9));
	if (win==1) point1++;
	if (win==-1) point2++;
	showpoint(point1,point2);
	window(29,8,80,25);
	gotoxy(1,16);clreol();
	setcursor(13);
	if (win==1) cputs(name1);else if (win==-1) cputs(name2);
	if (win) cprintf(" won the game.\n\r");
	else cprintf("The game is draw.\n\r");
	cprintf("Press any key to continue.");
	getch();
}

int check_win()
{
	int *temp,i;

	for (i=0;i<=2;i++) {
		temp=&matrix[i][0];
		if (*temp==*(temp+1) && *(temp+1)==*(temp+2)) return *temp;
	}
	for (i=0;i<=2;i++) {
		temp=&matrix[0][i];
		if (*temp==*(temp+3) && *(temp+3)==*(temp+6)) return *temp;
	}
	if (matrix[0][0]==matrix[1][1] && matrix[1][1]==matrix[2][2])
			return matrix[0][0];
	if (matrix[0][2]==matrix[1][1] && matrix[1][1]==matrix[2][0])
			return matrix[0][2];
	return 0;
}

void setcursor(unsigned int shape)
{
	union REGS reg;

	reg.h.ah=1;
	reg.x.cx=shape;
	int86(0x10,&reg,&reg);
}
