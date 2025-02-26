#include <dos.h>
#include <stdlib.h>

int cur_video(void);
void interrupt tsr_part(void);
void grab(void);
void set_vmem(void);
void tsr(int);

char far *buffer;
char running = 0;

void main()
{
	struct address {
		char far *p;
	};
	struct address far *addr = (struct address far *) 20;

	addr->p = (char far *) tsr_part;
	set_vmem();
	tsr(2000);
}

int cur_video()
{
	_AH = 0x0f;
	geninterrupt(0x10);
	return ( _AL );
}

void set_vmem()
{
	int mode;

	mode = cur_video();
