/* CHECKING OF VALID STRING */
#include <stdio.h>
main()
{
	int input[500],newdata[500];
	int n=0,i=1,run=1;
	printf("Enter a string : ");
	while (((input[n]=getchar()) !='$')&&(run))
		if (input[n] !=EOF)
			++n;
		else run=0;
	run=1;
	if (input[n] == '$')
		while (((newdata[i]=getchar()) !=EOF)&&(run))
		{
			if (newdata[i] != input[n-i])
				run=0;
			i++;
		}
	if ((run)&&(i-1==n))
		printf("The string is valid\n");
	else
		printf("The string is invalid\n");
	getch();
}