#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int divide(char *string,float *level,char *title, int *ch,int *x,int *y)
{
	register int i,j;
	int len,count=0;
	char temp[40];

	len = strlen(string);
	for (i=0;(!isalnum(string[i])) && i<len;i++);
	if (!isdigit(string[i])) return 0;
	for (j=0;(isdigit(string[i]) || string[i]=='.') && i<len;j++,i++)
		temp[j] = string[i];
	if (j) {
		temp[j] = 0x0;
		*level = atof(temp);
		count++;
	}
	else return count;
	j = i;
	for (;string[i] != '"' && i<len ;i++);
	if (i==j) return count;
	for (j=0,i=i+1;string[i] != '"' && i<len ;i++,j++)
		temp[j] = string[i];
	if (string[i] =='"') {
		temp[j] =0x0;
		i++;
		strcpy(title,temp);
		count++;
	}
	else return count;
	j = i;
	for (;string[i] == ' ' && i<len ;i++);
	if (i==j) return count;
	if (isalnum(string[i])) {
		*ch = toupper(string[i++]);
		count++;
	}
	else return count;
    for (;string[i] == ' ' && i<len ;i++);
	if (i < len) {
		for (j=0;i<=len;i++,j++)
			temp[j] = string[i];
	}
	else return count;
	sscanf(temp,"%d %d",x,y);
	return (count+2);
}

void main()
{
	char lstr[80],title[40],ch;
	int x,y,no;
	float ft;

	strcpy(lstr,"1.0 \"Masud Karim Khan\" M 100 50");
	no = divide(lstr,&ft,title,&ch,&x,&y);
	getch();
}