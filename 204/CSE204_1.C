/* 	Bit representation	*/

#include <stdio.h>
#include <ctype.h>

int input(void);
int process(int);
int multiply(int);

main()
{
	int data,answer;

	data=input();
	answer=process(data);
	printf("\nThe answer is : %d",answer);
	getch();
}

int input(void)
{
	char ch;
	int temp,i=0;

	printf("\nEnter a four digit decimal number : ");
	do {
		ch=getch();
		if (isdigit(ch)) {
			putch(ch);
			ch &= 0xf;
			temp <<= 4;
			temp = temp | ch;
			i++;
		}
	}while(i != 4);
	return temp;
}

int process(int data)
{
	int sum=0;

	do {
		 sum = multiply(sum) + ((data & 0xf000) >> 12);
		 data <<= 4;
	} while (data);
	return sum;
}

int multiply(int number)
{
	int factor=10,sum=0;

	do {
		if (factor & 0x1) sum += number;
		factor >>=1;
		number <<=1;
	} while (factor);
	return sum;
}