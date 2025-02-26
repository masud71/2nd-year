#include <stdio.h>

void main()
{
	int data1,data2,temp;
	char a1,a2,b1,b2;

	printf("Enter two integer data : ");
	scanf("%d %d",&data1,&data2);
	a1 = data1 % 256;
	a2 = data1 / 256;
	b1 = data2 % 256;
	b2 = data2 / 256;
	data1 = data2 =0;
	data2 = a2 * b2;
	data1 = a1 * b1;
	data1 += (a1 * b2) / 256;
	data1 += (a2 * b1) /256;
	temp = data2;
	data2 += (a1 *b2)*256 % 65536;
	data2 += (a2 *b1)*256 % 65536;
	if (temp > data2) data1++;
	printf("The result in hex : %X%X \n",data1,data2);
	getch();
}