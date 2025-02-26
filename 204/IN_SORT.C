#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000

int data[SIZE],store[SIZE],dummy[2*SIZE-1];

void initdata(void);
void inssort(void);
void binsort(void);
void shell(void);
void two_way(void);
void leftsort(int,int,int);
void rightsort(int,int,int);
void display();

main()
{
	int i,j;
	clock_t start,stop;
	void (*sort[4])(void) = {inssort,binsort,shell,two_way};
	char string[4][20] = {"Normal Insertion","Binary Insertion","Shell","Two way Insertion"};

    initdata();
	for (i=0;i<=3;i++) {
		for (j=0;j<SIZE;j++) data[j]=store[j];
		start=clock();
		sort[i]();
		stop=clock();
/*		display();*/
		printf("\nRequared time of %s sort is : %f seconds.",string[i],(stop-start)/CLK_TCK);
		getch();
	}
}

void initdata()
{
	register int i;

	randomize();
	for (i=0;i<SIZE;i++) store[i] = random(1000);
}

void inssort()
{
	register int i,j;
	int dummy;
	long comp=0,swap=0;

	for (i=1;i<SIZE;i++) {
		dummy = data[i];
		for (j=i-1;(data[j]>dummy) && j>=0;j--,comp++,swap++)
			data[j+1] = data[j];
		data[j+1] = dummy;
		swap++;
	}
	printf("\nNo. of comparison : %ld",comp);
	printf("\nNo. of movement : %ld",swap);
}

void binsort()
{
	register int i,j;
	int low,high,dummy;
	long comp=0,swap=0;

	for (i=1;i<SIZE;i++) {
		dummy = data[i];
		low=0;
		high=i-1;
		while (low<=high) {
			j=(low+high)/2;
			if (data[j] > dummy) high = j-1;
			else low = j+1;
			comp++;
		}
		for (high=i;high>low;high--,swap++) data[high]=data[high-1];
		data[low]=dummy;
		swap++;
	}
	printf("\nNo. of comparison : %ld",comp);
	printf("\nNo. of movement : %ld",swap);
}

void shell(void)
{
	register int i,j,k;
	int size,dummy;
	long swap=0,comp=0;

	for (size=SIZE/2;size;size=size/2)
		for (k=0;k<size;k++)
		for (i=k+size;i<SIZE;i += size) {
			dummy = data[i];
			for (j=i-size;(data[j]>dummy) && j>=k;j -= size,comp++,swap++)
				data[j+size] = data[j];
			data[j+size] = dummy;
			swap++;
		}
	printf("\nNo. of comparison : %ld",comp);
	printf("\nNo. of movement : %ld",swap);
}

void two_way(void)
{
	int left=SIZE,right=SIZE;
	register int i,j;

	dummy[left] = store[0];
	for (i=1;i<SIZE;i++)
		if (store[i] < (dummy[left] + dummy[right])/2)
			leftsort(--left,right,store[i]);
		else rightsort (left,++right,store[i]);
	for (i=left,j=0;i<=right;i++,j++) data[j] = dummy[i];
}

void leftsort(int left,int right,int val)
{
	register int i;

	for (i=left+1;(i<=right) && (dummy[i] < val);i++) dummy[i-1] = dummy[i];
	dummy[i-1] = val;
}

void rightsort(int left,int right,int val)
{
	register int i;

	for (i=right-1;(i>=left) && (dummy[i] > val);i--) dummy[i+1] = dummy[i];
	dummy[i+1] = val;
}

void display()
{
	register int i;

	clrscr();
	for (i=0;i<SIZE;i++)
		printf("%8d",data[i]);
}