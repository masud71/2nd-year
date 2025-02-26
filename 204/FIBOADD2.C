/*   Fibonacci number addition program   */

#include <stdio.h>

struct table {
	unsigned data[16] : 16;
} result;

void add(unsigned,unsigned,int);
struct table set(char *);

main()
{
	int i,continu=1;
	char dat[16];
	struct table data1,data2;

	printf("Enter first number in fibonacci base : ");
	scanf("%s",dat);
	data1 = set(dat);
	printf("Enter second number in fibonacci base : ");
	scanf("%s",dat);
	data2 = set(dat);
	for (i=0;i<16;i++) add(data1.data[i],data2.data[i],i);
	while (continu) {
		continu=0;
		for (i=15;i>1;i--)
			if ((result.data[i]==1) && (result.data[i-1]==1)) {
				result.data[i]=result.data[i-1]=0;
				add(result.data[i-2]-0,1,i-2);
				continu=1;
			}
	}
	printf("The sum is : ");
	for (i=0;i<16;i++) printf("%d",result.data[i]);
	getch();
}

void add(unsigned num1,unsigned num2,int pos)
{
	int sum;

	sum=num1+num2;
	switch (sum) {
		case 0 :break;
		case 1 :if (result.data[pos]==0) result.data[pos]=1;
				else add(1,1,pos);
				break;
		case 2 :result.data[pos]=0;
				add(result.data[pos-1]-0,1,pos-1);
				if (pos<14) add(result.data[pos+2]-0,1,pos+2);
				else if (pos==14) add(result.data[pos+1]-0,1,pos+1);
				break;
	}
}

struct table set(char temp[16])
{
	struct table tdata;
	register int i,j;

	for (i=0;i<16;i++) tdata.data[i] = 0;
	for (j=strlen(temp)-1,i=15;j>=0;j--,i--) tdata.data[i] = temp[j]-48;
	return tdata;
}