/*	Multiply two 16 bit number & keep the result in 16 bit number pair */

#include <stdio.h>

main()
{
	unsigned int top1,top2,bottom,ans1,ans2,temp;

	top1=ans1=ans2=0;
	printf("\nEnter two 16 bit integer to multiply : ");
	scanf("%d%d",&top2,&bottom);
	while (bottom) {
		if (bottom & 0x1) {
            temp=ans2;
			ans2+=top2;                  /* Add top data pair with  */
			ans1+=top1;                  /* Sum pair                */
			if (ans2 < temp) ans1++;     /* If carry increase left word  */
		}
		bottom >>=1;
		top1=(top1 << 1) | ((top2 & 0x8000) >> 15); /* Shift left most bit
											to left word  */
		top2 <<=1;
	}
	printf("\nThe answer in hex : %X%X",ans1,ans2);
	getch();
}