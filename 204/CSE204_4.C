#include <stdio.h>
#define MAX 10

struct record{
	short int sign;
	short int number;
	short int power;
}list[MAX];
short int index[MAX];

void initindex(void);
void input(void);
void diff(void);
void error(void);

main()
{
	initindex();
	input();
	diff();
}

void initindex(void)
{
	register int i;

	for (i=0;i<MAX-1;i++) index[i]=i+1;
	index[MAX-1]=-1;
}

void input(void)
{
	char instr[40],tempstr[40];
	short int continu=1,current=0,first=1;

	clrscr();
	printf("Enter an equation to differentiate : ");
	gets(instr);
	while (continu) {
		if (instr[0]=='-') list[current].sign=0;
		else if (instr[0]=='+') list[current].sign=1;
		else if (first) list[current].sign=1;
		else error();
		if (!first || (instr[0]=='-')) instr[0]=' ';
		first=0;
		strcpy(tempstr,instr);
		sscanf(instr," %d%s",&list[current].number,tempstr);
		if (!tempstr) continu=0;
		if (!strcmp(tempstr,instr))
			if (instr[1]=='X') list[current].number=1;
			else continu=0;
		if (continu) {
			strcpy(instr,tempstr);
			sscanf(instr,"*X**%d%s",&list[current].power,tempstr);
			if (!strcmp(tempstr,instr)) {
				if (!list[current].power) list[current].power=1;
				else continu=0;
				if (list[current].power==1 && (instr[0]=='+' || instr[0]=='-'))
					list[current].power=0;
				if(strcmp(instr,"*X")) sscanf(instr,"*X%s",tempstr);
				else continu=0;
			}
			if (tempstr) strcpy(instr,tempstr);
			else error();
			if (continu) current++;
		}
	}
	index[current]=-1;
}

void diff(void)
{
	short int locate=0,first=1,temp;

	printf("\nThe derivative of the equation : ");
	do {
		temp=list[locate].number*list[locate].power;
		if (temp < 0) {
			temp=-temp;
			list[locate].power = !list[locate].power;
		}
		if (temp) {
        	if (first && list[locate].sign) printf(" ");
			else if (list[locate].sign) printf("+");
			else printf("-");
			first=0;
			printf("%d",temp);
			switch (list[locate].power) {
				case 1:break;
				case 2:printf("*X");break;
				default:printf("*X**%d",list[locate].power-1);break;
			}
		}
		locate=index[locate];
	}while (locate !=-1);
	getch();
}

void error(void)
{
	printf("\aError in input.");
	exit(1);
}