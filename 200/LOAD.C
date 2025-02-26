#include <stdio.h>
#include <alloc.h>

#define MAX_BUF	200
#define MAX_QUE  30
#define MAX	 77

extern char	loadfile[13], password[5], auto_save;
extern unsigned max_question, cur_question, cur_line, max_line, SAVE_TIME;
extern struct que_type{
	unsigned  line_no;
	unsigned char q_type, ans_set, marks_set, answer, marks, obtained;
	char far *ptr_ans, far *ans;
	} questions[30];
extern char far *buffer[MAX_BUF],V_MODE, sec, min, hour;
extern int status[MAX_BUF];
extern unsigned long l_time, t_time;
extern void str_cpy( char far *ch, char *p);

char temp[82], i = 0;
int line_no = 0;

void process( char * s)
{
 int i = 0;
 while( s[i] != 13 ) {
	if( s[i] == 'L' ){
		t_time = (((s[i+1] - 48 )* 10  + s[i+2] - 48) *10 + s[i+3] - 48 ) * 10 + s[i+4] -48;
		i += 4;
		}
	else if(s[i] == 'E' ) {
		SAVE_TIME = (s[i+1] - 48) * 10 + s[i+2] -48;
		i += 2;
		}
	else if(s[i] == 'A') {
		auto_save = s[i+1] - 48;
		i += 2;
		}
	else i++;
	}
}


transfer( void )
{
 register char j;

 temp[i++] = '\0';
 if( (buffer[ line_no ] = ( char far * ) farmalloc( i ) ) == NULL )
	return 0;
 for( j = 0; j < i; j++) buffer[line_no][j] = temp[j];
 status[ line_no ] = -1;
 line_no++;
 i = 0;
 return 1;
}


load( void )
{
 FILE *fp;
 char ch = 10, new[100], j, key;
 int size, k;

 t_time = 1000;
 SAVE_TIME = 5;
 l_time = 0;
 if( (fp = fopen( loadfile, "rb")) == NULL) return 0;
 fgets(new,25,fp);
 getc(fp);
 key = getc(fp);
 for( j = 0; j < 4; j++) password[j] = key + getc(fp);
 password[j] = '\0';
 while( !feof( fp ) && (ch != 26)) {
	ch = key + getc( fp );
	if( ( ch != 'þ' ) && ( ch != 13 ) ) {
		temp[i] = ch;
		i++;
		if( i >= 78 ) if( !transfer() ) return 0;
		}
	else if( ch == 13 ) {
		if( !transfer() ) return 0;
		if( (ch = key + getc(fp)) != 10) temp[i++] = ch;
		}
	else switch( ch = key + getc( fp ) ) {
		   case 'N': max_question++;
			     if( i > 0 ) if( !transfer() ) return 0;
			     questions[ max_question - 1 ].line_no = line_no;
			     questions[ max_question - 1 ].obtained = 0;
			     questions[ max_question - 1 ].marks_set = 0;
			     questions[ max_question - 1 ].q_type = 1;
			     questions[ max_question - 1 ].ans_set = 0;
                             questions[ max_question - 1 ].marks = 1;
			     break;
		   case 'A': if( questions[ max_question - 1 ].q_type == 1 ) size = 5;
				else size = 1;
			     if(( questions[ max_question - 1 ].ans = (char far *) farmalloc(size+1) ) == NULL ) return 0;
			     for( j = 0; j < size ; j++ )  questions[ max_question - 1 ].ans[j] = key + getc(fp);
			     questions[ max_question - 1 ].ans[j] = '\0';
			     break;
		   case 'M': questions[ max_question - 1 ].q_type = 1;
			     questions[ max_question - 1 ].ptr_ans=(char far *) farmalloc(6);
			     str_cpy(questions[max_question - 1].ptr_ans,"     ");
			     break;
		   case 'T': questions[ max_question - 1 ].q_type = 2;
			     questions[ max_question - 1 ].ptr_ans = (char far *) farmalloc(2);
			     str_cpy(questions[max_question - 1].ptr_ans," ");
			     break;
		   case 'F': questions[ max_question - 1 ].q_type = 3;
			     questions[ max_question - 1 ].ptr_ans=(char far *) farmalloc(MAX+1);
			     str_cpy(questions[ max_question - 1 ].ptr_ans, "");
			     break;
		   case 'D': questions[ max_question - 1 ].q_type = 4;
			     questions[ max_question - 1 ].ptr_ans=(char far *) farmalloc(3*(MAX+1)+1);
			     str_cpy(questions[max_question - 1].ptr_ans,"\n\n\n\0");
			     break;
		   case 'V': new[0] = key + getc(fp);
			     new[1] = key + getc(fp);
			     if( ferror(fp) ) return 0;
			     questions[ max_question - 1 ].marks = (new[0]-48)*10 + new[1]-48;
			     break;
		   case 'G': size = key + getw( fp );
			     if( ( buffer[ line_no ] = ( char far * ) farmalloc( size ) ) == NULL ) return 0;
				 fread(buffer[line_no],size,1,fp);
				 if( ferror(fp) ) return 0;
				 status[line_no] = size;
			     line_no++;
			     i = 0;
			     break;
		   case 'S': if( i > 0 )
				if( !transfer() ) return 0;
			     j = 0;
			     while( (ch = key + getc(fp)) != 10 ) new[ j++ ] = ch;
			     process( new );
			     break;
		   default : temp[i++] = 'þ';
			     temp[i++] = ch;
			     if(i > 77) {
				i = 77;
				if( !transfer() ) return 0;
				}
			     break;
		   }
	if( ferror(fp) ) return 0;
	if( max_question > MAX_QUE ) break;
	if( line_no > MAX_BUF ) {
		i = 0;
		break;
		}
	}
 fclose(fp);
 if( i > 0 ) if( !transfer() ) return 0;
 max_line = line_no - 1;
 if( max_question == 0 ) return 0;
 max_question--;
 cur_question = 0;
 cur_line = 0;
 sec = t_time%60;
 min = (t_time/60)%60;
 hour = (t_time/3600)%60;
 return 1;
}