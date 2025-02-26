#include <dos.h>

void goxy(int,int);
char get(void);
void put(char);

void main(void)
{
	register int i,j;

	for (j=1;j<=25;j++)
		for (i=1;i<=80;i++) {
			goxy(i,j);
			if (get() == 'E') put('M');
		}
}

void goxy(int x,int y)
{
	_AH = 2;
	_BH = 0;
	_DH = y-1;
	_DL = x-1;
	geninterrupt(0x10);
}

char get(void)
{
	_AH = 8;
	_BH = 0;
	geninterrupt(0x10);
	return _AL;
}

void put(char ch)
{
	_AH = 9;
	_AL = ch;
	_BL = 7;
	_BH = 0;
	_CX = 1;
	geninterrupt(0x10);
}