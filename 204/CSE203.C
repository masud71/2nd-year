#include <stdio.h>

main()
{
	struct {
		int info;
		int newpos;
	}inforec[20],dummy;
	int total,continu=1,count=0;

	clrscr();
	printf("Enter some integers seperated by spaces.Type 0 to quit\n");
	do {
		scanf("%d",&inforec[count++].info);
	} while (inforec[count-1].info);
	total=count-1;
	printf("Enter new position of the entered charecters (Total: %d)\n",total);
	for (count=0;count<total;count++) {
		printf("%d : ",inforec[count].info);
		scanf("%d",&inforec[count].newpos);
	}
	while (continu) {
		continu=0;
		for (count=0;count<total;count++)
			if (inforec[count].newpos !=count+1) {
				 dummy=inforec[inforec[count].newpos-1];
				 inforec[inforec[count].newpos-1]=inforec[count];
				 inforec[count]=dummy;
				 continu=1;
			}
	}
	printf("\nThe transfered charecters are :\n");
	for(count=0;count<total;count++) printf("%d  ",inforec[count].info);
	getch();
}