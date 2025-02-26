#include <stdio.h>
#include <time.h>

#define SIZE 10

float data[SIZE][SIZE];

void init(void);
void process(void);
float avg(int,int);

main()
{
	clock_t start,end;

	init();
	start=clock();
	process();
	end=clock();
	gotoxy(1,20);
	printf("Processing time requared : %f seconds",(end-start)/CLK_TCK);
	getch();
}

void init(void)
{
	register int i,j;

	clrscr();
	printf("The initial condition of the metal sheet :-\n\n");
	for (i=0;i<SIZE-1;i++) {
		for (j=0;j<SIZE;j++) {
			data[i][j]=0;
			printf("%7.2f",data[i][j]);
		}
		printf("\n");
	}
	data[SIZE-1][0]=50;
	printf("%7.2f",data[SIZE-1][0]);
	for (i=1;i<SIZE-1;i++) {
		data[SIZE-1][i]=100;
		printf("%7.2f",data[SIZE-1][i]);
	}
	data[SIZE-1][SIZE-1]=50;
	printf("%7.2f",data[SIZE-1][SIZE-1]);
}

void process(void)
{
	register int i,j;
	char true=1;
	float temp;

	gotoxy(1,1);
	printf("The temperature status of the metal sheet :- ");
	do {
		true=0;
		for (i=1;i<SIZE-1;i++) {
			gotoxy(8,i+3);
			for (j=1;j<SIZE-1;j++) {
				temp=data[i][j];
				data[i][j]=avg(i,j);
                printf("%7.2f",data[i][j]);
				if (data[i][j]-temp > 0.01) true=1;
			}
		}
	} while (true);
	gotoxy(1,1);
	printf("The final temperature status of the metal sheet :- ");
}

float avg(int i,int j)
{
   return ((data[i-1][j]+data[i+1][j]+data[i][j+1]+data[i][j-1]+data[i-1][j-1]+data[i-1][j+1]+data[i+1][j-1]+data[i+1][j+1])/8);
}