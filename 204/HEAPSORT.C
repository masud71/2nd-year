#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 5000

void createheap(int []);
void swap(int *,int *);
void restore(int [],int);



main()
{
	int i,n=SIZE,data[SIZE+2];
	clock_t start,end;

	randomize();
	for (i=1;i<=SIZE;i++) data[i]=random(1000);
	data[i] = 0;
	createheap(data);
	start = clock();
	for(i=n;i>1;i--) {
		swap(&data[1],&data[i]);
		restore(data,i-1);
	}
	end = clock();
	clrscr();
	for (i=1;i<=SIZE;i++) printf("%8d",data[i]);
	printf("\nTime requared to sort : %f seconds.",(end-start)/CLK_TCK);
	getch();
}

void createheap(int data[])
{
	register int i,dummy,ok;

	do {
		ok=1;
		for(i=SIZE/2;i>=1;i--) {
			dummy = (data[i*2] > data[i*2+1]) ? i*2 : i*2+1;
			if (data[i] < data[dummy]) {
				ok=0;
				swap(&data[i],&data[dummy]);
			}
		}
	}while (!ok);
}

void swap(int *d1,int *d2)
{
	int dummy;

	dummy = *d1;
	*d1 = *d2;
	*d2 = dummy;
}

void restore(int data[],int index)
{
	int i = 1,j;

	for(;2*i<=index;i=j) {
		if (i*2 < index) j = (data[i*2]>data[i*2+1]) ? i*2 : i*2+1;
		else if (i*2 == index) j = i*2;
		if (data[i] < data[j]) swap(&data[i],&data[j]);
		else return;
	}
}