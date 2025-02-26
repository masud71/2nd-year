#include <graphics.h>
#include<alloc.h>

#define MAX_QUE 30
#define MAX_BUF 200


extern char LINE_HEIGHT,CHAR_WIDTH,V_MODE;
extern struct que_type{
	unsigned  line_no;
	unsigned char q_type, ans_set, marks_set, answer, marks, obtained;
	char   far *ptr_ans, far *ans;
	} questions[ MAX_QUE ];
extern unsigned max_x,max_y,cur_question,cur_line,max_line,max_question;
extern unsigned Q_X1,Q_Y1,Q_X2,Q_Y2,A_X1,A_Y1,A_X2,A_Y2;
extern char far *buffer[MAX_BUF],far *font[256];
extern int status[MAX_BUF];
char far *mostofa, far *mamun;
void screen_setup(void);
void up(void);
void down(void);
void pg_up(void);
void pg_dn(void);
void show_page(int);
int jump(int);

extern void g_printf(char , char , char *, int );
extern void far_g_printf(char , char , char far *, int );
extern void viewclear(int , int, int , int );

void screen_setup(void)
{
	char my_pat[8] = {0x11,0x44,0x11,0x44,0x11,0x44,0x11,0x44};
	int points[20];

	bar(0,0,max_x,LINE_HEIGHT-1);
        bar(3,LINE_HEIGHT*18,max_x-3,LINE_HEIGHT*19-1);
	setcolor( BLACK );
	line(76*CHAR_WIDTH+4,5,77*CHAR_WIDTH,10);
	line(77*CHAR_WIDTH,10,78*CHAR_WIDTH-4,5);
	line(76*CHAR_WIDTH+4,5,78*CHAR_WIDTH-4,5);
	line(78*CHAR_WIDTH+4,10,79*CHAR_WIDTH,5);
	line(79*CHAR_WIDTH,5,80*CHAR_WIDTH-4,10);
	line(78*CHAR_WIDTH+4,10,80*CHAR_WIDTH-4,10);
	points[0] = points[6] = points[8] = 0;
	points[1] = points[3] = points[9] = LINE_HEIGHT - 1;
	points[2] = points[4] = max_x;
	points[5] = points[7] = 24 * LINE_HEIGHT + 5;
	points[10] = points[16] = points[18] = 4;
	points[11] = points[13] = points[19] = LINE_HEIGHT * 2 - 1;
	points[12] = points[14] = max_x - 4;
	points[15] = points[17] = 24 * LINE_HEIGHT + 1;
	setfillpattern( my_pat, 1) ;
	setcolor( WHITE );
	fillpoly( 10, points );
	setcolor( BLACK );
	rectangle(CHAR_WIDTH-3,5,2*CHAR_WIDTH,8);
	line(CHAR_WIDTH-2,9,2*CHAR_WIDTH,9);
	line(2*CHAR_WIDTH+1,6,2*CHAR_WIDTH+1,9);
	line(3*CHAR_WIDTH,0,3*CHAR_WIDTH,LINE_HEIGHT);
	line(76*CHAR_WIDTH,0,76*CHAR_WIDTH,LINE_HEIGHT);
	line(78*CHAR_WIDTH,0,78*CHAR_WIDTH,LINE_HEIGHT);
	setcolor( WHITE );
        line(4,LINE_HEIGHT*22,max_x-4,LINE_HEIGHT*22);
	line(4,23*LINE_HEIGHT-1,max_x-4,23*LINE_HEIGHT-1);
	g_printf( 1, 5, "FILE              JUMP          HELP",XOR_PUT);
	g_printf( 19, 3, "Question # ",XOR_PUT);
	g_printf( 19, 37,"ANSWER",XOR_PUT);
	g_printf( 2 , 10, "FILE :                           TYPE :",OR_PUT);
	g_printf(24,2,"Message :",COPY_PUT);
	setfillstyle(SOLID_FILL,WHITE);
	mostofa=(char far *)farmalloc(imagesize(50*CHAR_WIDTH,LINE_HEIGHT,70*CHAR_WIDTH,2*LINE_HEIGHT));
	mamun = ( char far * ) farmalloc( imagesize( 1,1, 2*CHAR_WIDTH, LINE_HEIGHT));
	getimage(49*CHAR_WIDTH,LINE_HEIGHT,69*CHAR_WIDTH,2*LINE_HEIGHT,mostofa);
	getimage( 13*CHAR_WIDTH, 18*LINE_HEIGHT, 15*CHAR_WIDTH - 1, 19*LINE_HEIGHT - 1, mamun );
	show_page(0);
	show_answer(0);
}

void up(void)
{
	int x,y,i,j,k,newy;
	void far *dump;

	if (cur_question) {
		--cur_question;
		newy = questions[cur_question+1].line_no - questions[cur_question].line_no;
		dump = (void far *) farmalloc(imagesize(CHAR_WIDTH,2*LINE_HEIGHT,79*CHAR_WIDTH,(18-newy)*LINE_HEIGHT-1));
                if( dump == NULL ) {
			++cur_question;
			putch(7);
			return;
			}
		getimage(CHAR_WIDTH,2*LINE_HEIGHT,79*CHAR_WIDTH,(18-newy)*LINE_HEIGHT-1,dump);
		putimage(CHAR_WIDTH,(2+newy)*LINE_HEIGHT,dump,COPY_PUT);
		farfree(dump);
                show_answer(cur_question);
		viewclear(CHAR_WIDTH,2*LINE_HEIGHT,79*CHAR_WIDTH,(2+newy)*LINE_HEIGHT-1);
		for (i=questions[cur_question].line_no,j=0;j<newy;i++,j++) {
			x = CHAR_WIDTH;
			y = (2+j)*LINE_HEIGHT;
			k = 0;
			if (status[i] <=0)
			while (buffer[i][k]) {
				putimage( x, y, font[(unsigned char) buffer[i][k]], COPY_PUT);
				k++;
				x += CHAR_WIDTH;
			}
			else putimage(x,y,buffer[i],COPY_PUT);
		}
	}
}

void down(void)
{
	char  far *dump;
	int newy,i,j,k,x,y;

	if (cur_question != max_question) {
		++cur_question;
		newy = questions[cur_question].line_no - questions[cur_question-1].line_no;
		i =  imagesize( CHAR_WIDTH, (2+newy)*LINE_HEIGHT, 79*CHAR_WIDTH, 18*LINE_HEIGHT-1);
		dump = ( char far *) farmalloc(i);
		if( dump == NULL ) {
			--cur_question;
			putch(7);
			return;
			}
		getimage(CHAR_WIDTH,(2+newy)*LINE_HEIGHT,79*CHAR_WIDTH,18*LINE_HEIGHT-1,dump);
		putimage(CHAR_WIDTH,2*LINE_HEIGHT,dump,COPY_PUT);
		farfree(dump);
                show_answer(cur_question);
		viewclear(CHAR_WIDTH,(18-newy)*LINE_HEIGHT,79*CHAR_WIDTH,18*LINE_HEIGHT-1);
		for (i=questions[cur_question].line_no+16-newy,j=16-newy;j < 16 && i<=max_line;i++,j++) {
			x = CHAR_WIDTH;
			y = (2 + j)*LINE_HEIGHT;
			k = 0;
			if (status[i] <=0)
			while (buffer[i][k]) {
				putimage( x, y, font[(unsigned char) buffer[i][k]], COPY_PUT);
				k++;
				x += CHAR_WIDTH;
			}
			else putimage(x,y,buffer[i],COPY_PUT);
		}
	}
}

void pg_up(void)
{
	char disp = 0;

	if (!cur_question) return;
	if (questions[cur_question].line_no > 15) {
		do {
			disp +=questions[cur_question].line_no - questions[--cur_question].line_no;
		}while (disp <= 16 && cur_question > 0);
		cur_line = questions[++cur_question].line_no;
	}
	else {
		cur_question = 0;
		cur_line = 0;
	}
	show_page(cur_question);
	show_answer(cur_question);
}

void pg_down(void)
{
	char disp = 0;

	if (cur_question == max_question) return;
	if (questions[cur_question].line_no + 15 < questions[max_question].line_no) {
		do {
			disp +=questions[++cur_question].line_no - questions[cur_question-1].line_no;
		}while (disp <= 16 && cur_question < max_question);
		cur_line = questions[--cur_question].line_no;
	}
	else {
		cur_question = max_question;
		cur_line = questions[cur_question].line_no;
	}
	show_page(cur_question);
	show_answer(cur_question);
}

int jump(int serial)
{
	if (serial >=0 && serial <=max_question) {
		cur_question = serial;
		show_page(cur_question);
		show_answer(cur_question);
		return 1;
	}
	return 0;
}

void show_page(int q_no)
{
	int start,i=0, x= CHAR_WIDTH, y = 2*LINE_HEIGHT;
	register unsigned char j=0;

	viewclear( Q_X1, Q_Y1, Q_X2, Q_Y2);
	for (start=questions[q_no].line_no;i<16 && start<=max_line; j = 0, x = CHAR_WIDTH, y += LINE_HEIGHT, i++,start++)
		if (status[start] <= 0)
		while( buffer[start][j] != '\0') {
			putimage( x, y, font[(unsigned char) buffer[start][j]], COPY_PUT);
			x += CHAR_WIDTH;
			j++;
		}
		else putimage(x,y,buffer[start],COPY_PUT);
}
