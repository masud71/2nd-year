#include <stdio.h>

main()
{
	char data[10];
	int dollar,cent,base=100;

	printf("Enter the amount of wages : ");
	scanf("%s",data);
	sscanf(data,"%d.%d",&dollar,&cent);
	while (dollar) {
		if (dollar / base) {
			printf("%d Dollar  : %d\n",base,dollar/base);
			dollar %= base;
		}
		base /=2;
		if (base==25) base=20;
	}
	base=50;
	while (cent) {
		if (cent / base) {
			printf("%d Cent  : %d\n",base,cent/base);
			cent %= base;
		}
		base /=2;
		if (base==25) base=20;
	}
	getch();
}