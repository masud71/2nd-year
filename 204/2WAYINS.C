#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int data[1001];

main()
{
	int i;

	clrscr();
	randomize();
	for (i=0;i<500;i++) data[i]=random(1000);
	for (i=501;i<1001;i++) data[i]=random(1000);
	twoway(0,500,1000);
	for(i=0;i<1000;i++) printf("%8d",data[i]);
	getch();
}

void twoway(int st,int blank,int end)
{
	int left,right;

	left=blank+1;
	right=blank+2;
	mid=(left+right)/2;
	while(left!=st && right!=end) {
