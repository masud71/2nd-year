#include <stdio.h>
#include <graphics.h>

#define ESC 27
#define QUIT 17;
#define UP 72;
#define LEFT 75;
#define RIGHT 77;
#define DOWN 80;

char POINT1=0,POINT2=0,TIME =200,BEEP=0,NAME1[30],NAME2[30],CHR;
int DRIVER,MODE,THIGHT,TWIDTH;
/*-------------------------------------------------------*/
void level(void)
{
	char CH;

	outtextxy(0,8*THIGHT,"SELECT YOUR LEVEL TO PLAY (1 [FAST]----5 [SLOW]) : ");
	CH=getch();
	switch (ch) {
		case '1': TIME=50;
		case '2': TIME=100;
		case '3': TIME=150;
		case '4': TIME=200;
		case '5': TIME=250
	 }
	 outtextxy(0,10*THIGHT,"BEEP WHILE PLAY (Y/N) ? ");
	 if (toupper(getch()) == 'Y') BEEP=TRUE;
}
/*-------------------------------------------------------*/
void instruction(void)
{
   outtextxy(30*TWIDTH,THIGHT,"OBSTACLE GAME");
   outtextxy(35*TWIDTH,2*THIGHT,"BY");
   outtextxy(29*TWIDTH,3*THIGHT,"MASUD KARIM KHAN");
   setviewport(8*TWIDTH,5*THIGHT,70*TWIDTH,19*THIGHT,1);
   outtext("    INSTRUCTIONS : THIS IS A GAME OF TWO PLAYERS,BOTH OF THEM");
   outtextxy(0,THIGHT,"POSSESING A POINTER.THE PURPOSE OF EACH PLAYER IS TO OBSTRUCT");
   outtextxy(0,2*THIGHT,"OPPOSITE PLAYER''S POINTER BY CREATING LINES USING HIS POINTER.");
   outtextxy(0,3*THIGHT,"BOTH CAN MOVE THEIR POINTERS IN LEFT ,RIGHT,  UP & DOWN USING");
   outtextxy(0,4*THIGHT,"SPECIFIED KEYS . IT IS NOT REQUARED TO PRESS THE KEYS ALL THE");
   outtextxy(0,5*THIGHT,"TIME. THEY SHOULD BE PRESSED ONLY TO CHANGE THE DIRECTION.");
   outtextxy(0,6*THIGHT,"      A PLAYER WILL LOSS IF HE OVERLAP : a) THE BORDER OF THE ");
   outtextxy(0,7*THIGHT,"GAME BOX ,b)THE LINE CREATED BY OPPOSITE PLAYER OR BY HIMSELF.");
   outtextxy(0,8*THIGHT,"     THE FIRST 9 SET WINNER IS THE WINNER OF THE GAME.");
   setviewport(0,0,719,347,1);
   outtextxy(5*TWIDTH,16*THIGHT,"KEY INSTRUCTIONS : ");
   outtextxy(5*TWIDTH,18*THIGHT,"FOR FIRST PLAYER                             FOR SECOND PLAYER");
   outtextxy(44*TWIDTH,20*THIGHT,"KEYBOARD ARROW KEYS");
   outtextxy(7*TWIDTH,19*THIGHT,#24);
   outtextxy(7*TWIDTH,20*THIGHT,"W");
   moveto(TWIDTH+7,21*THIGHT);
   outtext(#27);OUTTEXT(" A   S   D ");OUTTEXT(#26);
   outtextxy(7*TWIDTH,22*THIGHT,#25);
   bar(21*TWIDTH,24*THIGHT,48*TWIDTH+3,25*THIGHT+2);
   setcolor(BLACK);
   outtextxy(22*TWIDTH,24*THIGHT+4,"  PRESS ANY KEY TO CONTINUE");
   setcolor(WHITE);
   CHR=getch();
END;
/*---------------------------------------------------------*/
char *readname(X,Y:INTEGER)
{
	char CH,I,DUMMY[40];

	 I=0;
	 do {
		   CH=getch();
		   if (CH != 13) {
				DUMMY[I++]=CH;
				outtextxy(X,Y,CH);
				X=X+TEXTWIDTH(CH)+1
		   }
	 while (CH != 13);
	 DUMMY[I]:=0x0;
	 return DUMMY;
END;
/*---------------------------------------------------------*/
void takename(void)      {TAKE NAMES OF THE PLAYERS}
{
	 clearviewport();
	 setviewport(9*TWIDTH,4*THIGHT,79*TWIDTH,19*THIGHT,1);
	 setlinestyle(0,0,3);
	 line(0,2,TWIDTH*64,2);
	 line(0,6*THIGHT,TWIDTH*64,6*THIGHT);
	 moveto(0,THIGHT*2);outtext("FIRST PLAYER''S NAME : ");
	 moveto(0,THIGHT*4);outtext("SECOND PLAYER''S NAME : ");
	 strcpy(NAME1,readname(20*TWIDTH,THIGHT*2));
	 strcpy(NAME2,readname(20*TWIDTH,THIGHT*4));
	 level();
	 setviewport(0,0,719,347,1);
}
/*--------------------------------------------------------*/
void point(POINT1,POINT2:BYTE)    {DISPLAYING POINTS OF BOTH PLAYERS}
{
	 setviewport(4*TWIDTH,THIGHT-5,75*TWIDTH,2*THIGHT-3,1);
	 clearviewport();
	 outtextxy(TWIDTH,2,NAME1);
	 outtextxy(36*TWIDTH,2,NAME2);
	 if (POINT1 != 10) outtextxy(20*TWIDTH,2,POINT1+48);
	 else outtextxy(20*TWIDTH,2,"10");
	 if (POINT2 != 10) outtextxy(56*TWIDTH,2,POINT2+48);
	 else outtextxy(56*TWIDTH,2,"10");
	 setviewport(0,0,719,347,1);
}
/*--------------------------------------------------------*/
void BOX(void)               {CREATING GAME SCREEN}
{
	 cleardevice();
	 rectangle(28,1,691,27*THIGHT+2);
	 line(28,2*THIGHT-1,691,2*THIGHT-1);
	 outtextxy(13*TWIDTH+4,14*THIGHT,16);
	 outtextxy(66*TWIDTH-3,14*THIGHT,17);
	 point(POINT1,POINT2);
	 outtextxy(19*TWIDTH,27*THIGHT+6,"PRESS <Esc> TO HALT THE GAME OR ^Q TO QUIT");
}
/*--------------------------------------------------------*/
void PLAY(void)	  {OPERATIONS OF THE GAME}
{
	char FLAG,CONTINU,X1,Y1,X2,Y2,DIR_X1,DIR_X2,DIR_Y1,DIR_Y2,TEMPTIME;
	char FILLER[68][52],CH;
	int  COUNTER;
	void *LPT,*RPT;
BEGIN
     FOR Y1:=1 TO 6 DO
         FOR X1:=1 TO 10 DO
			 putpixel(200+X1,300+Y1,WHITE);
	 LPT = (void *)malloc(imagesize(1,1,6,10));
	 getimage(201,301,210,306,LPT);
     FOR Y1:=1 TO 6 DO
         FOR X1:=1 TO 5 DO
			 putpixel(499+X1*2,300+Y1,WHITE);
     GETMEM(RPT,18);
     GETIMAGE(501,301,510,306,RPT^);
     BOX;
     TEMPTIME:=TIME;
     CONTINU:=TRUE;
     DIR_X1:=1; DIR_Y1:=0; DIR_X2:=-1; DIR_Y2:=0;
     FOR Y1:=0 TO 51 DO
         FOR X1:=0 TO 67 DO
             FILLER[X1,Y1]:=1;
     FOR Y1:=1 TO 50 DO
         FOR X1:=1 TO 66 DO
             FILLER[X1,Y1]:=0;
     FILLER[10,25]:=1; FILLER[57,25]:=1;
     X1:=10; Y1:=25; X2:=57; Y2:=25;
     FLAG:=TRUE;
     COUNTER:=1;
     SETVIEWPORT(20,19,699,330,TRUE);
     WHILE CONTINU DO
     BEGIN
          IF FLAG THEN
          CH:=UPCASE(READKEY);
          IF ORD(CH)=0 THEN CH:=READKEY;
          FLAG:=FALSE;
          CASE CH OF
          'A':BEGIN
                   DIR_X1:=-1; DIR_Y1:=0
              END;
          'S':BEGIN
                   DIR_X1:=0; DIR_Y1:=1
              END;
          'D':BEGIN
                   DIR_X1:=1; DIR_Y1:=0
              END;
          'W':BEGIN
                   DIR_X1:=0; DIR_Y1:=-1
              END;
         LEFT:BEGIN
                   DIR_X2:=-1; DIR_Y2:=0
              END;
         DOWN:BEGIN
                   DIR_X2:=0; DIR_Y2:=1
              END;
        RIGHT:BEGIN
                   DIR_X2:=1; DIR_Y2:=0
              END;
           UP:BEGIN
                   DIR_X2:=0; DIR_Y2:=-1
              END;
          ESC:BEGIN
                   SETVIEWPORT(18*TWIDTH,27*THIGHT+6,75*TWIDTH,29*THIGHT-3,TRUE);
                   CLEARVIEWPORT;
                   OUTTEXTXY(TWIDTH,0,'GAME HALTED! PRESS <SPACE BAR> TO CONTINUE');
                   REPEAT
                         CH:=READKEY
                   UNTIL CH=' ';
                   CLEARVIEWPORT;
                   OUTTEXTXY(TWIDTH,0,'PRESS <Esc> TO HALT THE GAME OR ^Q TO QUIT');
                   SETVIEWPORT(20,19,699,330,TRUE)
              END;
          QUIT:BEGIN CLOSEGRAPH;HALT END
          END;
          X1:=X1+DIR_X1; Y1:=Y1+DIR_Y1; X2:=X2+DIR_X2; Y2:=Y2+DIR_Y2;
          PUTIMAGE(X1*10,Y1*6,LPT^,0);
          PUTIMAGE(X2*10,Y2*6,RPT^,0);
          IF ((COUNTER MOD 10 = 0) AND (TEMPTIME <> 5)) THEN
             DEC(TEMPTIME,5);
          IF BEEP THEN SOUND(256);
          DELAY(TEMPTIME); NOSOUND;
          INC(COUNTER);
          IF (FILLER[X1,Y1]=1) OR (FILLER[X2,Y2]=1) THEN
             BEGIN
                  CONTINU:=FALSE;
                  IF FILLER[X1,Y1]=1 THEN INC(POINT2); {CHECKING OF FAILURE}
                  IF FILLER[X2,Y2]=1 THEN INC(POINT1); {OF ANY PLAYER      }
                  POINT(POINT1,POINT2);
                  SETVIEWPORT(18*TWIDTH,27*THIGHT+6,75*TWIDTH,29*THIGHT-3,TRUE);
                  CLEARVIEWPORT;
                  OUTTEXTXY(TWIDTH,0,'PRESS ANY KEY TO CONTINUE OR ^Q TO QUIT');
                  SETVIEWPORT(0,0,719,347,TRUE);
                  DELAY(2000);
                  CHR:=READKEY;
                  IF ORD(CHR)=17 THEN BEGIN CLOSEGRAPH;HALT END
             END;
          IF FILLER[X1,Y1]=0 THEN FILLER[X1,Y1]:=1;
          IF FILLER[X2,Y2]=0 THEN FILLER[X2,Y2]:=1;
          IF KEYPRESSED THEN FLAG:=TRUE;
     END;
     FREEMEM(LPT,18);
     FREEMEM(RPT,18);
END;
{---------------------------------------------------------}
PROCEDURE GREETING;       {MASSAGE TO THE PLAYERS AT THE END OF THE GAME}
   VAR WIN,LOSS:STRING;
BEGIN
     CLEARVIEWPORT;
     LINE(0,4*THIGHT,60*TWIDTH,4*THIGHT);
     OUTTEXTXY(0,6*THIGHT,'SCORES :- ');
     OUTTEXTXY(0,8*THIGHT,NAME1);OUTTEXTXY(5*TWIDTH,8*THIGHT,' : ');
     IF POINT1 <> 10 THEN OUTTEXTXY(8*TWIDTH,8*THIGHT,CHAR(POINT1+48))
     ELSE OUTTEXTXY(8*TWIDTH,8*THIGHT,'10');
     OUTTEXTXY(0,10*THIGHT,NAME2);OUTTEXTXY(5*TWIDTH,10*THIGHT,' : ');
     IF POINT2 <> 10 THEN OUTTEXTXY(8*TWIDTH,10*THIGHT,CHAR(POINT2+48))
     ELSE OUTTEXTXY(8*TWIDTH,10*THIGHT,'10');
     LINE(0,12*THIGHT,70*TWIDTH,12*THIGHT);
     IF POINT1=POINT2 THEN
        BEGIN
             OUTTEXTXY(0,14*THIGHT,'THE GAME IS DRAW');
             OUTTEXTXY(0,15*THIGHT,'THANKS TO YOU BOTH .')
        END
     ELSE BEGIN
               OUTTEXTXY(0,14*THIGHT,'CONGRATULATION ! ! ! ');
               IF POINT1 = 10 THEN BEGIN
                  WIN:=NAME1;LOSS:=NAME2
               END ELSE BEGIN
                   WIN:=NAME2;LOSS:=NAME1
               END;
               OUTTEXTXY(20*TWIDTH,14*THIGHT,WIN);
               MOVETO(20*TWIDTH+TEXTWIDTH(WIN),14*THIGHT);
               OUTTEXT(' ! YOU HAVE WON THE GAME.');
               OUTTEXTXY(0,16*THIGHT,LOSS);
               MOVETO(TEXTWIDTH(LOSS),16*THIGHT);
               OUTTEXT(' ,THANK YOU TO PARTICIPATE IN THE GAME.')
          END;
     LINE(0,18*THIGHT,70*TWIDTH,18*THIGHT);
     OUTTEXTXY(20*TWIDTH,20*THIGHT,'PRESS ANY KEY TO QUIT ');
     DELAY(4000);
     CHR:=READKEY
END;
{---------------------------------------------------------}
BEGIN                     {MAIN BODY OF THE PROGRAM}
     DRIVER:=DETECT;
     INITGRAPH(DRIVER,MODE,'');
     THIGHT:=TEXTHEIGHT('A')+4;
     TWIDTH:=TEXTWIDTH('A')+1;
     INSTRUCTION;
     TAKENAME;
     REPEAT
         PLAY
     UNTIL (POINT1=10) OR (POINT2=10);
     GREETING;
     CLOSEGRAPH;
END.                      {END OF MAIN BODY}