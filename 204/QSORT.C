#include <stdio.h>
#include <stdlib.h>

void qksort(int *,int,int);
void swap(int *,int,int);

main()
{
	int data[100],i;

	clrscr();
	randomize();
	for (i=0;i<100;i++)
		data[i]=random(1000);
	qksort(data,0,99);
	for (i=0;i<100;i++) {
		if (! i%10) printf("\n");
		printf("%5d",data[i]);
	}
	getch();
}

void qksort(int data[],int left,int right)
{
	int last,i;

	if (left>=right) return;
	swap(data,left,(left+right)/2);
	last=left;
	for (i=left+1;i<=right;i++)
		if (data[i]<data[left]) swap(data,i,++last);
	swap(data,left,last);
	qksort(data,left,last-1);
	qksort(data,last+1,right);
}

void swap(int data[],int one,int two)
{
	int temp;

	temp=data[one];
	data[one]=data[two];
	data[two]=temp;
} 