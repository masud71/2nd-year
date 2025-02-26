#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 1000

void init();
void bubble();
void coctail();

int data[SIZE],dummy[SIZE];

void main()
{
	init();
	bubble();
	coctail();
}

void init()
{
	register int i;

	randomize();
	for (i=0;i<SIZE;i++) data[i] = random(1000);
}

void bubble()
{
	register int i,indic;
	int temp,last;
	clock_t start,end;
	long comp=0,swap=0;

	for (i=0;i<SIZE;i++) dummy[i] = data[i];
	start = clock();
	last = SIZE-1;
	do {
		indic = 0;
		for (i=0;i<last;i++)
			if (dummy[i] > dummy[i+1]) {
				indic = i;
				swap++;
				temp = dummy[i+1];
				dummy[i+1] = dummy[i];
				dummy[i] = temp;
			}
		last = indic;
		comp += SIZE-1;
	} while (indic);
	end = clock();
	printf("Report for bubble sort ...\n\n");
	printf("Requared time to sort : %f seconds.\n",(end-start)/CLK_TCK);
	printf("Total comparison : %ld\n",comp);
	printf("Total Numbers of swap : %ld\n\n\n",swap);
}

void coctail()
{
	register int i,indic;
	int temp,lastl,lastr,left=1;
	clock_t start,end;
	long comp=0,swap=0;

	start = clock();
	lastr = SIZE-1;
	lastl = 0;
	do {
		indic = 0;
		if (left) {
			for (i=lastl;i<lastr;i++)
				if (data[i] > data[i+1]) {
					indic = i;
					swap++;
					temp = data[i+1];
					data[i+1] = data[i];
					data[i] = temp;
				}
			left =0;
			lastr = indic;
		}
		else {
			for (i=lastr;i>lastl;i--)
				if (data[i] < data[i-1]) {
					indic = i;
					swap++;
					temp = data[i];
					data[i] = data[i-1];
					data[i-1] = temp;
				}
			left = 1;
			lastl = indic;
		}
		comp += SIZE-1;
	} while (indic);
	end = clock();
	printf("Report for Coctail sort ...\n\n");
	printf("Requared time to sort : %f seconds.\n",(end-start)/CLK_TCK);
	printf("Total comparison : %ld\n",comp);
	printf("Total Numbers of swap : %ld\n\n\n",swap);
	getch();
}