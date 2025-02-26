/*-----------------------Intigrated Data-Base System---------------------*/
/*----------Include Files-----------*/
#include <bios.h>
#include <ctype.h>
#include <dos.h>
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*------------Difinatios-------------*/
#define ENTER   0x1c0d
#define ESC 	0x011b
#define F10  	0x4400
#define STAB 	0x0f00
#define TAB  	0x0f09

#define LEFT			 4
#define MAX_FIELD   	20
#define MAX_RECORD 	  1000
#define MAX_SIZE		60
#define NM_SIZE			15
#define RIGHT		    76

#define _FH 14
#define _FONTSIZE  34
#define _FW 9
/*---------Global Variables----------*/
enum var_type {STR=1,FLOAT,INT};

struct defrec {
	int serial;
	char fieldname[20];
	enum var_type field_type;
	int size;
	int x,y;
} field[MAX_FIELD];

struct datarec {
	int id;
	char *records[MAX_FIELD];
} *dataarray[MAX_RECORD];

int Num_of_records,Num_of_fields,Max_serial;
char curfile[20];
void *array[256],*sbar,*dline;
/*--------------Function Definations-------------*/

void box(int,int,int,int,int);
void clear(int,int,int,int);
void closefont(void);
void cursor(void);
int  define(void);
void deletestring(char *,char,int,int);
void disp_all(void);
void display_diff(void);
void display_form(int);
void display_pert(int);
int  display_rec(int,int);
void dnarrow(int *,int *,char *[]);
int  element(int);
void enter_element(void);
void font(char, char);
char *getall(int,int);
int  go_to_field(unsigned int,int *);
int  inc_y(int);
int  initfont(void);
void initialize(void);
void lf_ht(int , int);
int  load(char *);
void load_a_file(void);
void make_sort(int);
int  massage(char *,int);
int  menu();
void one_page(int *,int *,int *,int *);
void print(int);
void prints(char *);
int  print_rec(int , char , char);
int  prompt_prn(void);
void put(int,int);
int  save(char *);
void sort(void);
void strput(int,int,char *);
void uparrow(int *,int *,char *[]);

main()			/* Masud */
{
	int field_pos,new_pos,i;
	void *temp;

	initialize();
	do {
		temp = (void *)malloc(imagesize(160,80,460,220));
		getimage(160,80,460,220,temp);
		field_pos=menu();
		if (field_pos > 3 && field_pos < 8) {
			new_pos = massage("1.Single   2.All    3.Multiple",1);
			field_pos +=(new_pos- 49)*10;
		}
		putimage(160,80,temp,COPY_PUT);
		free(temp);
		switch (field_pos) {
			case 1 :load_a_file();break;
			case 2 :save(curfile);break;
			case 3 :enter_element();break;
			case 4 :display_pert(1);break;
			case 5 :print(0);break;
			case 6 :/* delete perticular */
			case 7 :/* modify perticular */
			case 8 :sort();break;
			case 9 :/* search record */
			case 10 :/* information */
			case 16:/* delete all */
			case 17:/* modify all */
			case 14:disp_all();break;
			case 15:print(2);break;
			case 26:/* delete multiple */
			case 27:/* modify multiple */
			case 24:display_diff();break;
			case 25:print(1);break;
		}
	} while (field_pos !=11);
	closegraph();
	closefont();
	/*    give goodbye massage    */
}

void box(int lx,int ty,int rx,int by,int color)  /* Masud */
{
	char my_pat[8] = {0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55};

	setfillpattern(my_pat,1);
	bar(lx,ty,rx,by);
	setfillstyle(SOLID_FILL,color);
	bar(lx+3,ty+2,rx-3,by-2);
}

void clear(int lx,int ty,int rx,int by)          /* Masud */
{
	setfillstyle(EMPTY_FILL,WHITE);
	bar(lx,ty,rx,by);
}

void closefont(void)                             /* Masud */
{
	register int i;

	for (i=0;i<256;i++)
		free(array[i]);
}

void cursor(void)		/* Masud */
{
	int x,y;

	x = getx();
	y = gety();
    setwritemode(XOR_PUT);
	line(x,y+_FH-3,x+_FW,y+_FH-3);
	line(x,y+_FH-2,x+_FW,y+_FH-2);
	setwritemode(COPY_PUT);
}

int define(void)                    /* Masud */
{
	char ch,dummy[60],*ret;
	int i,y=5;

	clear(3,5*_FH,716,21*_FH-1);
	sprintf(dummy,"Enter no. of fields of the database (Max %d) : ",MAX_FIELD);
	strput(3,5*_FH,dummy);
	ret=getall(2,3);
	Num_of_fields=atoi(ret);
	i=0;
	do {
		y=inc_y(y);
		field[i].serial=i+1;
		sprintf(dummy,"Enter name of field %d (Max length %d Char) : ",i+1,NM_SIZE);
		strput(3,y*_FH,dummy);
		ret = getall(1,15);
		strcpy(field[i].fieldname,ret);
		y=inc_y(y);
		strput(3,y*_FH,"Enter field type ( String,Float,Integer ) : ");
		do {
			ch=toupper(getch());
		} while( !strchr("SFI",ch));
		switch (ch) {
			case 'S' : 	field[i].field_type=STR;break;
			case 'F' : 	field[i].field_type=FLOAT;break;
			case 'I' :  field[i].field_type=INT;break;
		}
		y=inc_y(y);
		sprintf(dummy,"Enter length of the field (Max %d Char) : ",MAX_SIZE);
		strput(3,y*_FH,dummy);
		ret=getall(2,3);
		field[i].size = atoi(ret);
		y=inc_y(y);
		sprintf(dummy,"Enter x,y position of the field (Max x position %d) : ",
								(RIGHT-LEFT-strlen(field[i].fieldname)-3));
		strput(3,y*_FH,dummy);
		ret = getall(1,8);
		sscanf(ret,"%d%d",&field[i].x,&field[i].y);
		y=inc_y(y);
		strput(3,y*_FH,"Is this entry OK (Y/N) : ");
		if ((ch=toupper(getch()))=='Y')	i++;
	} while (i < Num_of_fields);
	massage("Press any key to show the layout of the database .....",1);
	display_form(1);
	ch=massage("Is the layout OK (Y/N) :",1);
	if (ch == 'Y') return 1;
	return 0;
}

void deletestring(char *str,char chs,int pos,int start)    /* Tanver */
{
  register temp;
  int temp1,curr;

    curr = strlen(str)-1;
    for (temp = pos; temp<=curr ;temp++) *(str+temp) =*(str+temp+1);
	temp1 = gety();
	strput(start,temp1,str);
	moveto(start+strlen(str)*_FW,temp1);
	put(' ',0);
	moveto(start+pos,temp1);
}

void disp_all(void)     /* Masud */
{
	int i=0,j=0,ti=0,tj=0,y=5,ts=0,bs=0;
	char ch,dummy[80];
	void *dump;

	massage("Use Up/Down arrows ,Home,PgUp,PgDn to move Or <Esc> to quit",0);
	one_page(&y,&i,&j,&bs);
	do {
	ch = getch();
	if (!ch) ch = getch();
	switch (ch) {
		case 72 :if (ti || tj) {
					if (y<20) y++;
					else {
						if ((j==Num_of_fields-1) && bs) bs=0;
						else if (j) j--;
						else {
							j=Num_of_fields-1;
							bs=1;
							i--;
						}
					}
					dump = (void *)malloc(imagesize(3,5*_FH,716,20*_FH-1));
					getimage(3,5*_FH,716,20*_FH-1,dump);
					putimage(3,6*_FH,dump,COPY_PUT);
					clear(3,5*_FH,716,6*_FH-1);
					free(dump);
					if (tj || ts) {
						if (ts) {
							ts=0;
							tj = Num_of_fields-1;
							ti--;
						} else tj--;
						sprintf(dummy,"%s : %s",field[tj].fieldname,(*dataarray[ti]).records[tj]);
						strput(3,5*_FH,dummy);
					} else ts = 1;
				}
				break;
		case 80 :if (i!=Num_of_records-1 || j!=Num_of_fields-1) {
					dump = (void *)malloc(imagesize(3,5*_FH,716,20*_FH-1));
					getimage(3,6*_FH,716,21*_FH-1,dump);
					putimage(3,5*_FH,dump,COPY_PUT);
					clear(3,20*_FH,716,21*_FH-1);
					free(dump);
					if (tj==Num_of_fields-1) { tj=0;ti++;ts=1; }
					else {
						if (tj) tj++;
						else {
							if (ts) ts=0;
							else tj++;
						}
					}
					if (j==Num_of_fields-1) { j=0;i++;bs=1; }
					else {
						if (j) j++;
						else {
							if (bs) bs=0;
							else j++;
						}
					}
					if (!bs) {
						sprintf(dummy,"%s : %s",field[j].fieldname,(*dataarray[i]).records[j]);
						strput(3,20*_FH,dummy);
					}
				}
				break;
		case 71:if (ti || tj) {
					y=5;i=j=ts=bs=ti=tj=0;
					one_page(&y,&i,&j,&bs);
				}
				break;
		case 73:if (ti || tj) {
					y=5;ts=bs=tj=j=0;
					ti=i=(ti-16/(Num_of_fields+1));
					if (ti<0) ti=i=0;
					one_page(&y,&i,&j,&bs);
				}
				break;
		case 81:if (i !=Num_of_records-1 || j !=Num_of_fields-1) {
        			y=5;ts=bs=tj=j=0;
					ti=i=(ti+16/(Num_of_fields+1));
					if (ti>Num_of_records-1) ti=i=Num_of_records-1;
					one_page(&y,&i,&j,&bs);
				}
				break;
	}
	}while(ch !=27);
	massage("",0);
}

void display_diff(void)                      /* Masud */
{
	int serial=0;
	char ch;

	display_rec(serial,0);
	do {
		ch=massage("'N'ext,'P'revious,'A'nother,'E'xit ? ",1);
		switch (ch) {
			case 'N':serial++;
					if (serial==Num_of_records) serial=0;
						display_rec(serial,0);
						break;
			case 'P':serial--;
					if (serial < 0) serial=Num_of_records-1;
					display_rec(serial,0);
					break;
			case 'A':display_pert(0);break;
		}
	} while (ch !='E');
}

void display_form(int mode)            /* Masud */
{
	register int i,j;
	char dummy[40];

	clear(3,5*_FH,716,21*_FH-1);
	for (i=0;i<Num_of_fields;i++) {
		sprintf(dummy,"%s : ",field[i].fieldname);
		strput((field[i].x-1)*_FW+3,(field[i].y-1+5)*_FH,dummy);
		if (!mode) {
			sprintf(dummy,"Serial no : %d",field[i].serial);
			strput(getx(),gety(),dummy);
		}
		else for (j=0;j<field[i].size;j++) put('.',1);
	}
}

void display_pert(int mode)                  /* Masud */
{
	int serial;
	char dummy[60],*temp;

	sprintf(dummy,"Enter record number to display (1 to %d) : ",Num_of_records);
	massage(dummy,0);
	temp = getall(2,4);
	serial = atoi(temp);
	serial=display_rec(serial-1,mode);
	if (serial ==-1) massage("Invalid Record Number ! ! !",0);
}

int display_rec(int index,int mode)       /* Masud */
{
	register int i;
	char ch,temp[40];
	struct datarec dummy;

	if (index >=Num_of_records || index < 0) return -1;
	clear(3,5*_FH,716,21*_FH-1);
	dummy = *dataarray[index];
	for (i=0;i<Num_of_fields;i++) {
		sprintf(temp,"%s : ",field[i].fieldname);
		strput((field[i].x-1)*_FW+3,(field[i].y-1+5)*_FH,temp);
		strput(getx(),gety(),dummy.records[i]);
	}
	if (mode) {
		ch=massage("Press any key to continue or <Esc> to abort.",1);
		if (ch != 27) return 1;
		return 0;
	}
}

void dnarrow(int *curr,int *top,char *array[])    /* Masud */
{
	void *ptr;

	if (*curr == 10) return;
	(*curr)++;
	if ((*top)+4 < *curr) {
		ptr = malloc(imagesize(191,106,329,161));
		putimage(191,162,sbar,XOR_PUT);
		getimage(191,120,329,175,ptr);
		putimage(191,106,ptr,COPY_PUT);
        clear(191,162,329,175);
		strput(194,163,array[*curr]);
		putimage(191,162,sbar,XOR_PUT);
		(*top)++;
		free(ptr);
	}
	else {
		putimage(191,106+14*((*curr) - (*top) - 1),sbar,XOR_PUT);
		putimage(191,106+14*((*curr) - (*top)),sbar,XOR_PUT);
	}
}

int element(int index)                   /* Naimul */
{
	int i,status;
	unsigned int ans;
	char *str,dummy[80];

	display_form(1);
	massage("Press <Enter> to skip any Field.",0);
	for (i=0;i<Num_of_fields;++i){
        moveto((field[i].x+strlen(field[i].fieldname)+2)*_FW+3,(field[i].y+4)*_FH);
		str=getall(field[i].field_type,field[i].size+1);
		dataarray[index]->records[i]=str;
		if (!str) return 0;
	}
 /*if any field is to be changed then tab ,shift_tab may be pressed or esc to
	avoid this form and to go to main menu or F10 to store the record. */
	sprintf(dummy,"Press <Tab> or <Shift Tab> for %c & %c movement.",25,24);
	massage(dummy,0);
    strput(19*_FW,22*_FH,"Press F10 to accept or <Esc> to avoid form.");
	i=Num_of_fields-1;
	moveto((field[i].x+strlen(field[i].fieldname)+2)*_FW+3,(field[i].y+4)*_FH);
	for(;;) {
		ans=bioskey(0);
		status = go_to_field(ans,&i);
		if (!status)
		switch(ans){
			case ESC:return 0;
			case F10:return 1;
			default :str=getall(field[i].field_type,field[i].size+1);
					dataarray[index]->records[i]=str;
					moveto((field[i].x+strlen(field[i].fieldname)+2)*_FW+3,(field[i].y+4)*_FH);
		}
	}
}

void enter_element(void)                  /* Naimul */
{
	int success;

	do {
		dataarray[Num_of_records] = (struct datarec *)malloc(sizeof(struct datarec));
		success=element(Num_of_records);
		if (success) dataarray[Num_of_records++]->id=++Max_serial;
	} while(success);
}

void font(char ch, char ch2) {                       /*ABDUL MALEQ */
		 switch (ch2) {
			case 'B':     /* bold */
			case 'b': biosprint(0, 27, 0);
						biosprint(0, 'W', 0);
						biosprint(0, 0, 0);   break;
			case 'I':     /* italic */
			case 'i': biosprint(0, 27, 0);
						biosprint(0, 45, 0);
						biosprint(0, 0, 0);  break;
			case 'T':  /* bold & italic */
			case 't': biosprint(0, 27, 0);
						biosprint(0, 'H', 0);
						biosprint(0, 27, 0);
						biosprint(0, 'F', 0);   break;
			}
		 switch (ch) {
			 case 'B':     /* bold */
			case 'b': biosprint(0, 27, 0);
						biosprint(0, 'W', 0);
						biosprint(0, 1, 0);   break;
			case 'I':     /* italic */
			case 'i': biosprint(0, 27, 0);
						biosprint(0, 45, 0);
						biosprint(0, 1, 0);  break;
			case 'T':  /* bold & italic */
			case 't': biosprint(0, 27, 0);
						biosprint(0, 'G', 0);
						biosprint(0, 27, 0);
						biosprint(0, 'E', 0);   break;
	 }
}

char *getall(int mode,int lenght)              /* Tanver */
{
 int xpos,  currx , loop = 0,  ypos ;
 char *p;
 char unsigned ch='\0';

 p = (char *)malloc(lenght);
 *p = '\0';
 if (mode>3||mode<1) return p;
 currx = xpos = getx();
 ypos = gety();
 for (currx=xpos;currx<=(lenght*_FW+xpos);currx+=_FW)
 {
   moveto(currx,ypos);
   put(' ',0);
 }
 currx = xpos;
 moveto(currx,ypos);
 while (ch!='\27')
 {
   cursor();
   ch = getch();
   cursor();
   switch (ch)
   {
     case '\0':
     {
	ch = getch();

	if ((ch == 75)&&(currx > xpos)||(ch == 77)&&(loop+1<=strlen(p)))
	{
	  currx = (ch==75) ? currx-_FW :currx+_FW;
	  loop = (ch==75) ? --loop:++loop;
	  moveto(currx , ypos );
	}
	else
	if (ch == 'S'&&strlen(p) > 0&&loop < strlen(p))
	{
	  deletestring(p, ch, loop*_FW, xpos);
	}
	break;
     }
     case '\b':          /* Backspace */
     if (loop>0)
     {
	   deletestring(p , ch ,(--loop)*_FW,xpos);
	   currx-=_FW;
       break;
     }
     case '\r':       return p;         /* Enter  */
     case 27:       /* Escape */
     {
	  for (loop = xpos;loop<(xpos+lenght*_FW);loop+=_FW) {
		moveto(loop, ypos);
		put(' ',0);
       }
	   moveto(xpos , ypos);
       *p = '\0';
       return p;
     }
    default :
    /*
      This part is activated when you want to enter a string or
      integer, float
    */
    if (loop+1<=lenght)
    {
      int ok = 0;
	  if (isprint(ch)&&(mode==1)) ok = 1;  /*To check for valid string*/
      else
      if (isdigit(ch)&&mode==2) ok = 1; /*To check for valid integer */
      else
      if ((isdigit(ch)||(ch=='.'&&strchr(p, '.')==0)||
       (ch=='-'&&strchr(p,'-')==0&&loop==0))&&(mode==3)) ok = 1;
	/* To check for valid float number*/

       if (ok)
       {
	 int temp1;

	 temp1 =strlen(p);
	 put(ch,0);
	 *(p+loop) = ch;
	 if (loop==temp1) *(p+loop+1)='\0';
	 loop++;
	 currx+=_FW;
       }
    }
   }
 }
}

int go_to_field(unsigned int scancode, int *index)   /* Palash */
{
	int		length;

	if (scancode / 256 != 0xf) return	0; /* not TAB or S.TAB */
	switch(scancode % 256) {     /* index means current field position*/
		case 9:	if(field[*index].serial < Num_of_fields) (*index)++;
				else *index = 0;
				break;
		case 0:	if (field[*index].serial > 1 ) (*index)--;
				else *index = Num_of_fields - 1;
				break;
	}
	length = strlen(field[*index].fieldname);
	moveto((field[*index].x+length+3)*_FW+3,(field[*index].y+4)*_FH);
	return   1;
}

int inc_y(int y)                 /* Masud */
{
	void *dummy;

	if (y !=20) return(++y);
	dummy = (void *)malloc(imagesize(3,6*_FH,716,21*_FH-1));
	getimage(3,6*_FH,716,21*_FH-1,dummy);
	putimage(3,5*_FH,dummy,COPY_PUT);
	free(dummy);
	putimage(3,20*_FH,dline,COPY_PUT);
	return y;
}

int initfont(void)               /* Masud */
{
	FILE *fp;
	register int i;

	fp=fopen("Masud.fnt","rb");
	if (!fp) return 0;
	for (i=0;i<256;i++) {
		array[i] = (void *)malloc(_FONTSIZE);
		if (!fread(array[i],_FONTSIZE,1,fp)) return 0;
	}
	fclose(fp);
	return 1;
}

void initialize(void)                    /* Masud */
{
	register int i;
	int driver,mode;

	Num_of_records=0;
	Num_of_fields=0;
	Max_serial=0;
	driver=DETECT;
	registerbgidriver(Herc_driver);
	initgraph(&driver,&mode,"");
	initfont();
	box(0,0,80*_FW-1,_FH*25-3,0);
	dline = malloc(imagesize(3,4*_FH,716,5*_FH-1));
	getimage(3,4*_FH,716,5*_FH-1,dline);
	setfillstyle(SOLID_FILL,WHITE);
	bar(3,4*_FH,716,5*_FH-1);
	bar(3,21*_FH,716,22*_FH-1);
	strput(15*_FW,_FH,"FILE NAME               REC NO          TOTAL REC       DATE ");
	setcolor(BLACK);
	strput(22*_FW,4*_FH,"Integrated Data-Base System");
	strput(35*_FW,21*_FH,"Massage Line");
}

void lf_ht(int line, int position) {
/*ABDUL MALEQ */       /* setting & making horizontal tab */
	 int i;

	 biosprint(0, 13, 0);   /* carige  return */
	 biosprint(0, 27, 0);      /* tab setting */
	 biosprint(0, 'D', 0);
	 biosprint(0, position, 0);
	 biosprint(0, NULL, 0);
	 for (i=0; i<line; i++) biosprint(0, 10, 0);   /* line feed */
	 biosprint(0, 9, 0);    /* horizontal tab */
}

int load(char *fname)                  /* Mamun */
{
  register int i,j;
  FILE *fp;
  char name[40];

  sscanf(fname,"%s.",fname);
  strcpy(name,fname);
  strcat(name,".def");
  if((fp=fopen(name,"rb"))==NULL) {
		massage("Cannot open definition file",0);
		fclose(fp);
		return 0;
  }
  Num_of_fields=getw(fp);
  Num_of_records=getw(fp);
  Max_serial=getw(fp);
  for(i=0;i < Num_of_fields;i++) {
	if(!fread(&field[i],sizeof(struct defrec),1,fp)) {
	  massage("Error reading definition file",0);
      fclose(fp);
      return 0;
    }
  }
  fclose(fp);
  strcpy(name,fname);
  strcat(name,".dat");
  if((fp=fopen(name,"rb"))==NULL) return 0;
  for(i=0;i < Num_of_records;i++) {
	dataarray[i] = (struct datarec *)malloc(sizeof(struct datarec));
	for (j=0;j<Num_of_fields;j++) {
		dataarray[i]->records[j] = (char *)malloc(field[j].size + 1);
		if(!fread(dataarray[i]->records[j],field[j].size+1,1,fp)) {
			massage("Error reading data file",0);
			fclose(fp);
			return 0;
		}
	}
  }
  fclose(fp);
  return 1;
}

void load_a_file(void)                   /* Masud */
{
	char *fname,ch;
	int status;

	massage("Enter file name to load : ",0);
	fname=getall(1,20);
	status=load(fname);
	if (!status) {
		ch=massage("File not found.Create a new one ? (Y/N) ",1);
		if (ch=='Y') {
			strcpy(curfile,fname);
			initialize();
			do {
				status=define();
				if (!status) massage("No DataBase is defined ! !",0);
			} while (!status);
		}
	}
	else strcpy(curfile,fname);
}

void make_sort(int serial)                 /* Palash */
{
	int		lcv1,lcv2;
	struct datarec *temp,*d1,*d2;

	for (lcv1 = 0;lcv1 < Num_of_records-1; lcv1++)
		for (lcv2 = lcv1+1;lcv2 < Num_of_records;lcv2++) {
			d1 = dataarray[lcv1]; d2 = dataarray[lcv2];
			if (stricmp(d1->records[serial],d2->records[serial]) > 0) {
				temp = dataarray[lcv1];
				dataarray[lcv1] = dataarray[lcv2];
				dataarray[lcv2] = temp;
			}
		}
}

int massage(char *line,int status)            /* Masud */
{
	int ch;

	clear(3,22*_FH,716,24*_FH);
	strput((40 - strlen(line)/2)*_FW,23*_FH,line);
	if (status) {
		ch=toupper(getch());
		putimage(3,23*_FH,dline,COPY_PUT);
		return ch;
	}
}

int menu()                       /* Masud */
{
	char ch[2];
	void *indc;
	int i,curr=0,top=0,esc=0;
	char *list[11] = {"Load a File","Save a File","Enter element",
		"Display Records","Print Records","Delete Record","Modify Record",
		"Sort Database","Search Record","Information","Exit to Dos"};

	box(160,80,460,220,15);
	clear(190,87,330,100);
	strput(222,88,"Main Menu");
    setcolor(BLACK);
	rectangle(170,92,180,95);
	line(171,96,180,96);
	line(181,93,181,96);
	line(165,100,189,100);
	rectangle(370,115,440,134);
	rectangle(370,151,440,170);
	rectangle(190,105,330,176);
	strput(395,119,"OK");
	strput(380,155,"ESCAPE");
	rectangle(380,118,430,131);
	clear(191,106,329,175);
	rectangle(340,105,355,176);
	ch[0] = 24;ch[1] = 0x0;
	outtextxy(345,106,ch);
	ch[0] = 25;
	outtextxy(345,169,ch);
	line(340,112,355,112);
	line(340,169,355,169);
	indc = (void *)malloc(imagesize(342,114,353,120));
	sbar = malloc(imagesize(191,106,329,119));
	setfillstyle(SOLID_FILL,WHITE);
    bar(342,114,353,120);
	getimage(342,114,353,120,indc);
	putimage(342,114,indc,XOR_PUT);
	bar(191,106,329,119);
	strput(179,192,"Current Choice");
	rectangle(176,190,445,206);
	clear(305,192,442,204);
	getimage(191,106,329,119,sbar);
	strput(194,107,list[0]);
	setcolor(WHITE);
	strput(308,192,list[0]);
	for (i=1;i<5;i++) strput(194,107+14*i,list[i]);
	curr = 0;
	do {
		i = getch();
		if (!i) i = getch();
		putimage(342,114+(47*curr/10),indc,XOR_PUT);
		switch (i) {
			case 72 :uparrow(&curr,&top,list);break;
			case 80 :dnarrow(&curr,&top,list);break;
			case 9 :setwritemode(XOR_PUT);
                    rectangle(380,118,430,131);
					rectangle(378,154,433,167);
					setwritemode(COPY_PUT);
					esc = !esc;break;
		}
		clear(305,192,442,204);
		strput(308,192,list[curr]);
		putimage(342,114+(47*curr/10),indc,XOR_PUT);
	} while (i != 13  && i != 27);
	free(indc);
	if (i == 13 && !esc) return (curr+1);
	return 27;
}

void one_page(int *y,int *i,int *j,int *bs)   /* Masud */
{
	char dummy[80];

    clear(3,5*_FH,716,21*_FH-1);
	for (;*y<21 && *i<Num_of_records;) {
		sprintf(dummy,"%s : %s",field[*j].fieldname,(*dataarray[*i]).records[*j]);
		strput(3,(*y)*_FH,dummy);
		if (*y !=20) *y = inc_y(*y);
		else *y=21;
		if (*y != 21) {
			(*j)++;
			if (*j == Num_of_fields) {
				*j=0;(*i)++;
				if (*y != 20) *y=inc_y(*y);
				else *y=21;
				if (*y==21) *bs = 1;
			}
		}
	}
	(*y)--;
	if (*j) (*j)--;
	else if (!*bs) {
		*j=Num_of_fields-1;(*i)--;
	}
}

void print(int mode) {    /*ABDUL MALEQ */
	 char field_font, data_font;
	 int i,num=0;

	 biosprint(0, 27, 0);       /* initialize */
	 biosprint(0, '@', 0);
	 if (mode==0 || mode==1) num=prompt_prn();
	 if (num==-1) return;
	 field_font = massage("Field name font: <B>old, <I>talic, bo<T>h ?",1);
	 data_font = massage("Data font : <B>old, <I>talic, bo<T>h ?",1);
	 if (!mode) print_rec(num,field_font,data_font);
	 if (mode==1)
		do {
			print_rec(num,field_font,data_font);
			biosprint(0, 10, 0);
			biosprint(0, 10, 0);
			i=massage("'N'ext,'P'revious,'A'nother,'E'xit ? ",1);
			switch (i) {
				case 'N':if (num<Num_of_records-1) num++;
						else {
							massage("Last record is printed",0);
							return;
						}
						break;
				case 'P':if (num) num--;
						else {
							massage("First record is printed",0);
							return;
						}
						break;
				case 'A':num=prompt_prn();
						if (num==-1) return;
						break;
			}
		}while (i !='E');
	 if (mode==2)
		for (i=0; i<Num_of_records; i++) {
			print_rec(i, field_font, data_font);
			biosprint(0, 10, 0);
			biosprint(0, 10, 0);
			biosprint(0, 10, 0);
		}
}

void prints(char *s) {              /*ABDUL MALEQ */   /* print string */
	 for (; *s; s++) biosprint(0, *s, 0);
}

int print_rec(int index, char f_mode, char d_mode) {   /*ABDUL MALEQ */
	register int i;
	struct datarec dummy;

	if (index >=Num_of_records || index < 0) return -1;
	dummy = *dataarray[index];
	lf_ht(0, field[0].x);
	font(f_mode, 'n');
	for (i=0;i<Num_of_fields;i++) {
		prints(field[i].fieldname);
		prints(" : ");
		font(d_mode, f_mode);
		prints(dummy.records[i]);
		lf_ht(field[i+1].y - field[i].y, field[i+1].x);
		font(f_mode, d_mode);
	}
	font('n', f_mode);
	return 0;
}

int prompt_prn(void)               /* Masud */
{
	char *temp,dummy[60];
	int num,ch;

	sprintf(dummy,"Enter record number to print [1 to %d]: ",Num_of_records);
	massage(dummy,0);
	temp=getall(2,4);
	num=atoi(temp);
	if (num > Num_of_records || num < 1) {
		massage("Record number is out of range",0);
		return -1;
	}
	num--;
	display_rec(num,0);
	ch=massage("Varify  (Y/N)  ? ",1);
	if (ch=='N') return -1;
	return num;
}

void put(int ch,int tp)            /* Masud */
{
	int x,y;

	x = getx();
	y = gety();
	putimage(x,y,array[ch],tp);
	if (x+_FW*2 >getmaxx()) { x=0;y += _FH;}
	else x+=_FW;
	moveto(x,y);
}

int save(char *fname)                 /* Mamun */
{
  register int i,j;
  FILE *fp;
  char name[40];

  sscanf(fname,"%s.",fname);
  strcpy(name,fname);
  strcat(name,".dat");
  if((fp=fopen(name,"wb"))==NULL) {
	massage("Cannot open data file",0);
    fclose(fp);
    return 0;
  }
  for(i=0;i<Num_of_records;i++)
	for(j=0;j<Num_of_fields;j++) {
		fwrite(dataarray[i]->records[j],field[j].size+1,1,fp);
		if(ferror(fp)) {
			massage("File write error",0);
			fclose(fp);
			return 0;
		}
	}
  fclose(fp);
  strcpy(name,fname);
  strcat(name,".def");
  if((fp=fopen(name,"wb"))==NULL) {
	massage("Cannot open definition file",0);
    fclose(fp);
    return 0;
  }
  putw(Num_of_fields,fp);
  putw(Num_of_records,fp);
  putw(Max_serial,fp);
  for(i = 0;i < Num_of_fields;i++) {
	fwrite(&field[i],sizeof(struct defrec),1,fp);
    if(ferror(fp)) {
	  massage("file write error",0);
      fclose(fp);
      return 0;
    }
  }
  fclose(fp);
  return 1;
}

void sort(void)                        /* Palash */
{
	int  num,i;
	char  input[20],ch,*temp;

	display_form(0);
	massage("Select one field for  sort : ",0);
	temp = getall(2,3);
	num = atoi(temp);
	if (num  &&  (num <= Num_of_fields)) make_sort(num-1);
	else return;
	ch=massage("Do you want to display ? (Y/N) ",1);
	if (ch == 'Y') disp_all();
}

void strput(int x,int y,char *str)     /* Masud */
{
	moveto(x,y);
	while(*str) {
		put(*str,1);
		str++;
	}
}

void uparrow(int *curr,int *top,char *array[])   /* Masud */
{
	void *ptr;

	if (! *curr) return;
	(*curr)--;
	if (*curr < *top) {
		ptr = malloc(imagesize(191,106,329,161));
		putimage(191,106,sbar,XOR_PUT);
		getimage(191,106,329,161,ptr);
		putimage(191,120,ptr,COPY_PUT);
        clear(191,106,329,119);
		strput(194,107,array[*curr]);
        putimage(191,106,sbar,XOR_PUT);
		(*top)--;
		free(ptr);
	}
	else {
		putimage(191,106+14*((*curr) - (*top) +1),sbar,XOR_PUT);
		putimage(191,106+14*((*curr) - (*top)),sbar,XOR_PUT);
	}
}