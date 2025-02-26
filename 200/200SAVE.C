#include <stdio.h>

#define MAX_QUE	30
#define MAX_BUF 200
#define	ESC		27

typedef  enum	{ERR_FREE, DR_UNREADY_ERR, WRITE_ERR, FOPEN_ERR } error_type;

/* EXTERN VARIABLE DECLARATION	*/

extern struct que_type{
	unsigned  line_no;
	unsigned char q_type, ans_set, marks_set, answer, marks, obtained;
	char   far *ptr_ans, far *ans;
	} questions[ MAX_QUE ];
extern unsigned max_question;
extern unsigned long l_time;
extern char savefile[13];

extern void error_msg(char *, char * );
extern void g_printf( char , char, char *, int );

typedef struct que_type Q_type ;

unsigned getlen(Q_type far *p)
{
	unsigned i = 0;

	if( p->ans_set ) for( ; p->ptr_ans[i] ; i++) ;
	return i + 5 ;
}

int	save(void)
{
	FILE * Outfile;
	char	i, j;
	unsigned len;
	error_type	flag;
	static char *message[] = {  "" ,
					"Drive not ready : Press ENTER ." ,
					"Write Error : Press ENTER ." ,
					"File Open Error : Press ENTER ."} ;
	Outfile = NULL;
	flag = ERR_FREE ;
	if (!flag && (Outfile = fopen( savefile, "r+b"))== NULL )
		flag = FOPEN_ERR;
	if (!flag) {
		g_printf( 24, 12, "Saving The Answer.......             ", 0);
		len = getw(Outfile);
		fseek( Outfile, len, SEEK_SET);
		putw( l_time, Outfile );
		for( i = 0 ; !flag && i <= max_question ; i++) {
			len = getlen( &questions[i] );
			putc( len, Outfile) ;
			putc( questions[i].marks_set, Outfile);
			putc( questions[i].ans_set, Outfile);
			putc( questions[i].marks, Outfile);
			putc( questions[i].obtained, Outfile);
			putc( questions[i].q_type, Outfile);
			for( j = 0, len -= 5; j < len; j++ )
				putc( questions[i].ptr_ans[j], Outfile);
			if (ferror(Outfile))
				flag = WRITE_ERR;
			}
		}
	if (flag)  				/*	error occurs	*/
		error_msg( message[flag], "Report to your teacher");
	fclose(Outfile) ;
	g_printf(24,12,"                                            ", 0);
	if( !flag ) return 1;
	return 0;
}