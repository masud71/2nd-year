/*
  The object of this program is to take char and then store it in a int as
  nibble.Then the number are retrived part by part as nibble and the original
  number is formed.
*/
#include <stdio.h>

main()
{
 unsigned int loop,compare,storenumber,sum;

 clrscr();
 printf("Please enter a number:->");

 for (loop = 0; loop<4;)
  {
    compare = getch();
    if (compare>=48 && compare<=57)
    {
       putch(compare);
       storenumber = storenumber<<4;
       storenumber += (compare-48);
       loop++;
    }
  }

  for (compare = 0xF000 ,loop = 12 ,sum = 0;compare != 0;loop -= 4)
  {
    sum *=10;
    sum += ((storenumber&compare)>>loop);
    compare = compare >> 4;
  }
  printf("\nThe decimal equivalent is =%i",sum);
}