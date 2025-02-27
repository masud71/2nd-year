#include <stdio.h>

int init(void);
void process(int);
void display(int);

struct data {
	int element;
	int position;
} array[10];

main()
{
	int total;

	total = init();
	process(total);
	display(total);
}

void box(void)
{
	int i,j;

	clrscr();gotoxy(1,5);printf("�");
	for (i=2;i<80;i++) printf("�");
	printf("�");
	for (i=1;i<4;i++) { gotoxy(1,i+5);printf("�");gotoxy(80,i+5);printf("�");}
	printf("�");for (i=2;i<80;i++) printf("�");printf("�");
	gotoxy(1,7);printf("�");for (i=2;i<80;i++) printf("�");printf("�");
	for (i=1;i<4;i++)
		for (j=1;j<10;j++) {
			gotoxy(8*j+1,i+5);printf("�");
		}
	for (j=1;j<10;j++) {
		gotoxy(8*j+1,5);printf("�");
		gotoxy(8*j+1,7);printf("�");
		gotoxy(8*j+1,9);printf("�");
	}
}

int init(void)
{
	int i=0,total,number;

	box();
	gotoxy(20,10);printf("Enter each data & type 0 to stop.");
	do {
		gotoxy(i*8+3,6);
		scanf("%d",&number);
		if (number) array[i].element=number;
		else {
			gotoxy(i*8+3,6);printf(" ");i--;
		}
		i++;
	} while (i <10 && number);
	gotoxy(20,10);clreol();
	printf("Enter new position avoiding duplication.");
	total=i;
	for (i=0;i<total;i++) {
		gotoxy(i*8+3,8);
		scanf("%d",&number);
		array[i].position=number-1;
	}
	return total;
}

void process(int total)
{
	struct data dummy;
	int i,j;

	for (i=0;i<total-1;i++) {
		for (j=i;array[j].position != i;j=array[j].position);
		while (array[j].position != j) {
			dummy = array[array[j].position];
			array[array[j].position] = array[j];
			array[j] = dummy;
		}
	}
}

void display(int total)
{
	int i;

	box();
    gotoxy(10,3);printf("The rearranged data : ");
	for (i=0;i<total;i++) {
		gotoxy(i*8+3,6);printf("%d",array[i].element);
		gotoxy(i*8+3,8);printf("%d",array[i].position + 1);
	}
	getch();
}