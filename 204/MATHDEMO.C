#include <stdio.h>
#include <math.h>
main()
{
	int i,start,last;
	clrscr();
	printf("Enter Starting & Final Value Of The List : ");
	scanf("%d%d",&start,&last);
	printf("    I        I�       �I        log(I)        exp(I) \n");
	printf("------------------------------------------------------\n");
	for(i=start;i<=last;i++)
	 printf("%5d%9d%10.2f%14.2f%14.2f\n",i,i*i,sqrt(i),log(i),exp(i));
	getch();
}   2�           r      �      �     �     �      N      �      �      