/* Merging two array mapped linked list */
#include <stdio.h>

#define MAX 50

int data[MAX],index[MAX];
int list1,list2;

void initindex(void);
void input (void);
void align(void);
void print(void);

main()
{
	initindex();
	input();
	align();
	print();
}

void initindex(void)
{
	register int i;

	for (i=0;i<MAX-1;i++) index[i]=i+1;
	index[MAX-1]=-2;
}

void input(void)
{
	int current=-1;

	list1=0;
	clrscr();
	printf("Enter elements of first list. ( 0 to end ) :\n");
	do {
		current++;
		scanf("%d",&data[current]);
	}while (data[current]);
	index[--current]=-1;
	list2=current+1;
	printf("\nEnter elements of second list. ( 0 to end ) :\n");
	do {
		current++;
		scanf("%d",&data[current]);
	}while (data[current]);
	index[current-1]=-1;
}

void align(void)
{
	int temp,temp1=list1,temp2=list2,continu=1;

	while (continu) {
		if (index[temp1]==-1) continu=0;
		temp=index[temp1];
		index[temp1]=temp2;
		temp1=temp;
		if (continu) {
			if (index[temp2]==-1) continu=0;
			temp=index[temp2];
			index[temp2]=temp1;
			temp2=temp;
		}
	}
}

void print(void)
{
	int temp=list1;

	printf("\nThe aligned list is :\n");
	do {
		printf("%8d",data[temp]);
		temp=index[temp];
	} while (temp !=-1);
	getch();
}