/* MONTI CARLO INTEGRAL */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

main()
{
	int a=0,b=1,n,cycle=0,counter=0;
	clock_t start,end;
	float rx,ry,y,ymax=1,f,answer=0;
	start=clock();
	clrscr();
	printf("This Program Can Find Integration Of Y=e^(-xý) \n");
	printf("Enter no of cycles : ");
	scanf("%d",&n);
	printf("\nWait a while .....\n");
	randomize();
	do {
		rx=(float) rand()/(float) RAND_MAX;
		y=exp(-rx*rx);
		ry=(float) rand()/(float) RAND_MAX;
		if (ry<=y) counter++;
		cycle++;
	} while (cycle!=n);
	f=(float) counter/(float) cycle;
	answer=f*ymax*(b-a);
	printf("The answer is : %f\n\n",answer);
	end=clock();
	printf("The program was active for %f seconds." ,(end-start)/CLK_TCK);
	getch();
}
