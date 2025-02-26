#include <stdio.h>
#include <math.h>

main()
{
	long int data;
	register int i;

	do {
		printf("Enter the number : ");
		scanf("%ld",&data);
		printf("The factors : %ld = ",data);
		i = 2;
		while (i <= sqrt(data)) {
			if (!(data % i)) {
				printf("%d x ",i);
				data /= i;
			}
			else i++;
		}
		printf("%ld\n\n",data);
	} while (getch() != 32);
}