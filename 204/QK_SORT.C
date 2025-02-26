#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define SIZE 1000

void qksort(int,int);
int partition(int,int);

int data[SIZE+1];

main()
{
	int i;
	clock_t start,end;

	randomize();
	for (i=0;i<SIZE;i++) data[i] = random(1000);
	data[SIZE] = INT_MAX;
	start = clock();
	qksort(0,SIZE-1);
	end = clock();
	for (i=0;i<SIZE;i++) printf("%8d",data[i]);
	printf("\nTime requared for %d numbers : %f seconds.",SIZE,(end-start)/CLK_TCK);
	getch();
}

void qksort(int start,int end)
{
	int mid;

	if (start >= end) return;
	mid = partition(start,end);
	qksort(start,mid-1);
	qksort(mid+1,end);
}

int partition(int start,int end)
{
	int left,right,dummy;

	left = start+1;
	right = end;
	while (left <= right) {
		while (data[left] <= data[start]) left++;
		while (data[right] > data[start]) right--;
		if (left < right) {
			dummy = data[right];
			data[right] = data[left];
			data[left] = dummy;
		}

	}
	dummy = data[right];
	data[right] = data[start];
	data[start] = dummy;
	return right;
}