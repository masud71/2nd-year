#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 900

int data[SIZE+1],newdata[SIZE],index[30],ni=0;

void selsort(int,int);
void adselect(int,int);
int find();

main()
{
	int i;
	clock_t start,end;

	randomize();
	for (i=1;i<=SIZE;i++) data[i] = random(1000);
	start = clock();
	adselect(1,SIZE);
	end = clock();
	for (i=0;i<SIZE;i++) printf("%8d",newdata[i]);
	printf("\nTime requared : %f seconds\n",(end-start)/CLK_TCK);
	getch();
}

void adselect(int start,int end)
{
	int size,i,indecator;

	size = sqrt(end-start+1);
	for (i=0;i<size;i++) index[i] =i*size+1;
	for (i=0;i<size;i++)
		selsort(i*size+1,(i+1)*size);
	indecator = 0;
	do {
		indecator += find(size);
	}while (indecator<size);
}

int find(int size)
{
	int i,min;

	for (min=0;!index[min];min++);
	for (i=min+1;i<size;i++)
		if (data[index[i]] < data[index[min]] && index[i]) min = i;
	newdata[ni++] = data[index[min]];
	index[min]++;
	if (index[min] % size == 1) {
		index[min] = 0;
		return 1;
	}
	else return 0;
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