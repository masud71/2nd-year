/*   Fibonacci number addition program   */

#include <stdio.h>

char dat1[9],dat2[9],result[9]="00000000";
void add(int,int,int);

main()
{
	int i,continu=1;

	printf("Enter first number in fibonacci base : ");
	scanf("%s",dat1);
	printf("Enter second number in fibonacci base : ");
	scanf("%s",dat2);
	for (i=0;i<8;i++) add(dat1[i]-'0',dat2[i]-'0',i);
	while (continu) {
		continu=0;
		for (i=7;i>1;i--)
			if ((result[i]=='1') && (result[i-1]=='1')) {
				result[i]=result[i-1]='0';
				add(result[i-2]-'0',1,i-2);
				continu=1;
			}
	}
	printf("The sum is : %s\n",result);
	getch();
}

void add(int num1,int num2,int pos)
{
	int sum;

	sum=num1+num2;
	switch (sum) {
		case 0 :break;
		case 1 :if (result[pos]=='0') result[pos]='1';
				else add(1,1,pos);
				break;
		case 2 :result[pos]='0';
				add(result[pos-1]-'0',1,pos-1);
				if (pos<6) add(result[pos+2]-'0',1,pos+2);
				else if (pos==6) add(result[pos+1]-'0',1,pos+1);
				break;
	}
}