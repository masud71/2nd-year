#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 900

int data[SIZE];

void selsort(int,int);

main()
{
	int i;
	clock_t start,end;

	randomize();
	for (i=0;i<SIZE;i++) data[i] = random(1000);
	start = clock();
	selsort(0,SIZE-1);
	end = clock();
	for (i=0;i<SIZE;i++) printf("%8d",data[i]);
	printf("\nTime requared : %f seconds\n",(end-start)/CLK_TCK);
	getch();
}

void selsort(int st,int end)
{
	register int i,j,min;

	for (i=st;i<end;i++) {
		min = i;
		for (j=i+1;j<=end;j++)
			if (data[j] < data[min]) min = j;
		j = data[i];
		data[i] = data[min];
		data[min] = j;
	}
}