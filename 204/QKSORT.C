#define SIZE 5000

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

int data[SIZE+1];

int partition(int,int);
void rquick(int,int);
void swap(int *,int *);
void squick(int,int);

main()
{
	int sdata[SIZE],i;
	clock_t start,end;

	clrscr();
	randomize();
	for (i=0;i<SIZE;i++) sdata[i]=data[i]=random(1000);
	data[SIZE]=INT_MAX;
	start=clock();
	rquick(0,SIZE-1);
	end=clock();
	for(i=0;i<SIZE;i++) printf("%8d",data[i]);
	printf("\nRequared time is : %f seconds",(end-start)/CLK_TCK);
	getch();
	for (i=0;i<SIZE;i++) data[i]=sdata[i];
	start=clock();
	squick(0,SIZE-1);
	end=clock();
	for(i=0;i<SIZE;i++) printf("%8d",data[i]);
	printf("\nRequared time is : %f seconds",(end-start)/CLK_TCK);
	getch();
}

int partition(int left,int right)
{
	register int i,j;

	i=left+1;
	j=right;
	while (i <= j) {
		while(data[i] <= data[left]) i++;
		while(data[j] > data[left]) j--;
		if (i < j) swap(&data[i],&data[j]);
	}
	swap(&data[left],&data[j]);
	return j;
}

void swap(int *d1,int *d2)
{
	int dummy;

	dummy = *d1;
	*d1 = *d2;
	*d2 = dummy;
}

void rquick(int left,int right)
{
	int part;

	if (left<right) {
		part = partition(left,right);
		rquick(left,part-1);
		rquick(part+1,right);
	}
}

void squick(int left,int right)
{
	int stack[20],ptr,part;

	ptr=0;
	for(;;) {
		while (left<right) {
			part=partition(left,right);
			if (part-left < right-part) {
				stack[ptr+1] = part+1;
				stack[ptr+2] = right;
				right = part-1;
			}
			else {
				stack[ptr+1] = left;
				stack[ptr+2] = part-1;
				left = part+1;
			}
			ptr += 2;
		}
		if (ptr) {
			left = stack[ptr-1];
			right =stack[ptr];
			ptr -= 2;
		} else return;
	}
}