#include <stdio.h>
#include <ctype.h>

void prefix(char *);
void postfix(char *);

char dstk[10][40],ostk[10];
int di=0,oi=0;

void main()
{
	char expr[40];

	clrscr();
	printf("Enter an inorder expression : ");
	gets(expr);
	prefix(expr);
	postfix(expr);
	getch();
}

void prefix(char *expr)
{
	char dummy[40];
	int i;

	for (i=0;expr[i];i++) {
		if (isalpha(expr[i])) {
			dstk[di][0] = expr[i];
			dstk[di++][1] = '\0';
		}
		else
			switch (expr[i]) {
				case '*' :
				case '/' :
				case '(' :ostk[oi++] = expr[i];break;
				case '+' :
				case '-' :if (ostk[oi-1]=='*' || ostk[oi-1]=='/') {
							sprintf(dummy,"%c%s%s",ostk[--oi],dstk[di-2],dstk[di-1]);
							di-=2;
							strcpy(dstk[di++],dummy);
						  }
						  ostk[oi++] = expr[i];break;
				case ')' :while(ostk[--oi] != '(') {
							sprintf(dummy,"%c%s%s",ostk[oi],dstk[di-2],dstk[di-1]);
							di-=2;
							strcpy(dstk[di++],dummy);
						  }
						  break;
			}
	}
	while(--oi >=0) {
		sprintf(dummy,"%c%s%s",ostk[oi],dstk[di-2],dstk[di-1]);
		di-=2;
		strcpy(dstk[di++],dummy);
	}
	printf("\nThe prefix expression :- %s\n",dstk[0]);
}

void postfix(char *expr)
{
	char dummy[40];
	int i;

	oi=di=0;
	for (i=0;expr[i];i++) {
		if (isalpha(expr[i])) {
			dstk[di][0] = expr[i];
			dstk[di++][1] = '\0';
		}
		else
			switch (expr[i]) {
				case '*' :
				case '/' :
				case '(' :ostk[oi++] = expr[i];break;
				case '+' :
				case '-' :if (ostk[oi-1]=='*' || ostk[oi-1]=='/') {
							sprintf(dummy,"%s%s%c",dstk[di-2],dstk[di-1],ostk[--oi]);
							di-=2;
							strcpy(dstk[di++],dummy);
						  }
						  ostk[oi++] = expr[i];break;
				case ')' :while(ostk[--oi] != '(') {
							sprintf(dummy,"%s%s%c",dstk[di-2],dstk[di-1],ostk[oi]);
							di-=2;
							strcpy(dstk[di++],dummy);
						  }
						  break;
			}
	}
	while(--oi >=0) {
		sprintf(dummy,"%s%s%c",dstk[di-2],dstk[di-1],ostk[oi]);
		di-=2;
		strcpy(dstk[di++],dummy);
	}
	printf("\nThe postfix expression :- %s\n",dstk[0]);
}