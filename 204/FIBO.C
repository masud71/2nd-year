/* Representation of an integer in fibonacci number system  */

#include <stdio.h>

main()
{
	unsigned int input;
	int i;
	long int fibo[32],store=0;

	fibo[31]=1;fibo[30]=2;
	for(i=29;i>=0;i--) fibo[i]=fibo[i+1]+fibo[i+2]; /* creating fibo array */
	printf("Enter an integer number : ");
	scanf("%d",&input);
	for (i=0;i<32;i++) {
		store=(store << 1) | ((input >= fibo[i]) ? 1:0);
		if (input >= fibo[i]) input-=fibo[i];          /* convert number */
	}
	printf("The number in fibonacci number system is : ");
	for (i=0;i<32;i++) {
		printf("%d",(store & 0x80000000) >>31);       /* print number */
		store <<=1;
	}
	printf("\n");
	getch();
}