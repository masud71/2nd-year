#include <stdio.h>
#include <math.h>

void inword(int j);
main()
{
	int n=0,i,data,temp;
	clrscr();
	printf("Enter An Integer Type Value : ");
	scanf("%d",&data);
	temp=data;
	while (temp !=0) {
		temp /=10;
		n++;
	}
	printf("\n\nThe reversed value in word : ");
	for (i=n;i>0;i--) {
		temp+=(data%10)*power(i-1);
		inword(data%10);
		data /=10;
	}
	printf("\n\nThe reversed value : %d",temp);
	printf("\n\nThe value in word : ");
	for (i=1;i<=n;i++) {
		 inword(temp%10);
		 temp /=10;
	}
	getch();
}
void inword(int j)
{
	switch (j) {
		case 1:printf("ONE ");break;
		case 2:printf("TWO ");break;
		case 3:printf("THREE ");break;
		case 4:printf("FOUR ");break;
		case 5:printf("FIVE ");break;
		case 6:printf("SIX ");break;
		case 7:printf("SEVEN ");break;
		case 8:printf("EIGHT ");break;
		case 9:printf("NINE ");break;
		case 0:printf("ZERO ");break;
		defult:printf("INVALID ");
	}
}
int power(int k)
{
	int j,temp=1;
	for (j=k;j>=1;j--)
		temp *=10;
	return temp;
}DIS   PAS           =�s�H  TEST    C             N�N
5  TEST1   C             ���P
�   TEST2   C             ���Q
+  SHOWSUM C             \L�s
�  COS     C             �	�S
�   GAME    C             �J?~�  PRIME   C             B��
�  �AME    PAS           o?��  TABLE   C             �=X�  EQUATIONC             ��7�+  FIRST   C             `v��
�   PRIME   OBJ           �u�\i  AVG     C             Τ�i
�  WINDOW  C             v���
�  �PPER   PAS           "z��V  