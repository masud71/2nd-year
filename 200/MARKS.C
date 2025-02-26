#include <graphics.h>
#include <string.h>


#define	MAX_QUE		30
#define	MAX_BUF		200

extern struct que_type{
	unsigned  line_no;
	unsigned char q_type, ans_set, marks_set, answer, marks, obtained;
	char   far *ptr_ans, far *ans;
	} questions[ MAX_QUE ];

struct result{
	unsigned int overall_marks;
	unsigned char total_question[4];
	unsigned char total_answered[4];
	unsigned int total_marks[4];
	unsigned int marks[4];
	}info;

unsigned char pattrn1[8] = {0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x01};
unsigned char pattrn2[8] = {0x88,0x00,0x22,0x00,0x88,0x00,0x22,0x00};
unsigned char pattrn3[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
unsigned char pattrn4[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

extern unsigned  max_x, max_y, max_question, cur_question;
extern char      LINE_HEIGHT, V_MODE, CHAR_WIDTH;
extern char	 len[10],   far *identies[10], data[10];

extern void g_printf( char row, char col, char  * , int op);
extern void g_putchar( char row, char col, char ch, int op );

char	  buf[80];/***************/

void analyze_ans( int );
void display_result( void );/**************/
char *con(char *str);/****************/
void g_window(int,int,int,int, unsigned char* );/************/
void get_screen( void );/**************/

/*-------------------------------------------------------------------------*/
void analyze_ans( int count )
{
 int i , type;

 info.overall_marks = 0;
 for( i = 0; i <= 4; i++) info.total_question[ i ] = 0;
 for( i = 0; i <= 4; i++) info.total_answered[ i ] = 0;
 for( i = 0; i <= 4; i++) info.total_marks[ i ] = 0;
 for( i = 0; i <= 4; i++) info.marks[ i ] = 0;

 for( i = 0; i <= count; i++){
	type = questions[i].q_type;
	info.total_marks[ type ] = info.total_marks[ type ] + questions[ i ].marks;
	info.overall_marks = info.overall_marks + (unsigned int ) questions[ i ].marks;
	info.total_question[ type ] ++;
	if( (type==1) || (type==2) )
	    if( questions[i].ans_set ){
		if( ! strcmp( (char *)questions[i].ptr_ans,(char *)questions[i].ans) ){
			info.marks[ type ] = info.marks[ type ] + questions[ i ].marks;
			info.total_answered[ type ] ++;
			questions[i].marks_set = 1;
			questions[i].obtained = questions[i].marks;
		}
	    else  {
			questions[i].marks_set = 1;
			questions[i].marks = 0;
		  }
	    }
 }
}

/*-------------------------------------------------------------------------*/
void display_result()
{int base = 10;
 unsigned int sum;
 char string[10],i,j,choice;
 char type[][8] = {" ","M.C.Q","T/F","BLANK","DESCRIP"};

 g_printf( 3 , 12 , strcat( con("NAME    ::"), identies[0] ),COPY_PUT);
 g_printf( 4 , 12 , strcat( con("ROLL    ::"), identies[1] ),COPY_PUT);
 g_printf( 5 , 12 , strcat( con("REG.NO. ::"), identies[5] ),COPY_PUT);
 g_printf( 6 , 12 , strcat( con("DEPT    ::"), identies[2] ),COPY_PUT);
 choice = 1;
 sum = 0;
 for( j = 1 ; j <= 4; j++ )
	sum += info.marks[j];
 do{
	g_window(10,12,66,22,pattrn4);
	switch(choice){
		case 1: itoa( info.overall_marks,string,base );
			g_printf(16,25,strcat(con("TOTAL MARKS OF THE EXAMINATION ::"), string ),COPY_PUT);
				choice = 2;
				break;
		case 2: g_printf(14,12,":TYPE:",COPY_PUT);
			g_printf(14,20,":AMOUNT:",COPY_PUT);
			g_printf(14,30,":MARKS:",COPY_PUT);
			g_printf(14,39,":CORRECT:",COPY_PUT);
			g_printf(14,52,":MARK OBTAIN:",COPY_PUT);
			for( j = 1 , i = 16; j <= 4; j++ , i++)
				g_printf(i,12,strcat(con("  "),type[j]),COPY_PUT);
			for( j = 1 , i = 16; j <= 4; j++ , i++) {
				itoa(info.total_question[j],string,base);
				g_printf(i,20,strcat(con("  "), string),COPY_PUT);
				}
			for( j = 1 , i = 16; j <= 4; j++ , i++) {
				itoa(info.total_marks[j],string,base);
				g_printf(i,30,strcat(con("  "), string),COPY_PUT);
				}
			for( j = 1 , i = 16; j <= 2; j++ , i++) {
				itoa(info.total_answered[j],string,base);
				g_printf(i,39,strcat(con("  "),string),COPY_PUT);
				}
			for( j = 1 , i = 16; j <= 2; j++ , i++) {
				itoa(info.marks[j],string,base);
				g_printf(i,52,strcat(con("  "),string),COPY_PUT);
				}
			g_putchar( 18, 41, '_', COPY_PUT);
			g_putchar( 19, 41, '_', COPY_PUT);
			g_putchar( 18, 54, '_', COPY_PUT);
			g_putchar( 19, 54, '_', COPY_PUT);
			itoa(sum,string,base);
			g_printf(21,25,strcat(con("Total marks obtained ::  "),string),COPY_PUT);
			choice = 1;
			break;
		}
	}while( getch() != 27 );
}

/*-------------------------------------------------------------------------*/
char *con(char *str)
{
 buf[0] = '\0';
 return(strcat(buf,str));
}

/*-------------------------------------------------------------------------*/
void g_window(int u_row, int l_col, int d_row , int r_col , unsigned char  *attr)
{
 int x1,y1,x2,y2;

 x1 = (u_row - 1) * CHAR_WIDTH;
 y1 = (l_col - 1) * LINE_HEIGHT;
 x2 = d_row * CHAR_WIDTH - 1;
 y2 = r_col * LINE_HEIGHT - 1;
 setfillpattern(attr , 1);
 bar(x1 , y1 , x2 , y2);
}

void get_screen()
{
  g_window(1,1,80,25,pattrn1);
  rectangle(0 , 0 , max_x , max_y);
  g_window(10,3,70,8,pattrn2);
  g_window(8,2,68,7,pattrn3);
  g_window(10,3,66,6,pattrn4);

  g_window(10,12,70,24,pattrn2);
  g_window(8,11,68,23,pattrn3);
  g_window(10,12,66,22,pattrn4);
}