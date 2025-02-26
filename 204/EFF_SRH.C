#include <stdio.h>
#include <time.h>

int binsearch(int);
int array[30000];

main()
{
	int i;
	clock_t start,end;

	for (i=0;i<30000;i++) array[i]=i+1;
	start=clock();
	for (i=1;i<=30000;i++)
		binsearch(i);
	end=clock();
	printf("Requared time : %f\n",(end-start)/CLK_TCK);
	getch();
}

int binsearch(int key)
{
	int	first=0,last=29999,found=0;
	int mid,value;

	do {
		mid=(first+last)/2;
		value=array[mid];
		if (key > value) first=mid+1;
		else if (key < value) last=mid-1;
		else found=1;
	} while ((first <= last) && (!found));
	return mid;
}