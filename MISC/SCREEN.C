#include <dos.h>
#include <time.h>

void tsr_scr();
void chk_mode();
void where(int *,int *);
void interrupt dos_idle();
void interrupt new_int8();
void interrupt (*old_int28)();
void interrupt (*old_int8)();
void interrupt (*old_int64)();
void print_ram(int);
int getram();
void move(int,int);

int count=0,blank=1,oldx,oldy;
int buff[2000];
char far *dos_active,busy=0;
long video_base;
struct time t;

main()
{
	union REGS r;
	struct SREGS s;
	int x,y,i;
	char data1[30]="Masud's Screen saver installed",data2[30]="TSR is already installed";

	chk_mode();
	old_int64 = getvect(0x64);
	if (!old_int64) {
		setvect(0x64,new_int8);
		where(&x,&y);
		for (i= 0;i<30;i++) {
			move(x+i,y);
			print_ram(data1[i]);
		}
	}
	else {
		where(&x,&y);
		for (i= 0;i<30;i++) {
			move(x+i,y);
			print_ram(data2[i]);
		}
		return;
	}
	r.h.ah = 0x34;
	int86x(0x21,&r,&r,&s);
	dos_active = MK_FP(s.es,r.x.bx);
	old_int28 = getvect(0x28);
	old_int8 = getvect(0x8);
	setvect(0x28,dos_idle);
	setvect(0x8,new_int8);
	keep(1,1000);
}

void interrupt new_int8()
{
	register int i,j;

	if (blank && !kbhit()) count++;
	if (kbhit()) {
		count = 0;
		if (!blank) {
			busy = !busy;
			blank = 1;
        	for (i = 1;i <=25;i++)
			for (j = 1;j <=80;j++) {
				move(j,i);
				print_ram(buff[(i-1)*80+(j-1)]);
			}
			move(oldx,oldy);
			busy = !busy;
		}
	}
	(*old_int8)();
	if (!dos_active && !busy) tsr_scr();
}

void interrupt dos_idle()
{
	(*old_int28)();
	if (!busy) tsr_scr();
}

void tsr_scr()
{
	register int i,j;

	if (count >= 1092) {
		count = 0;
		where(&oldx,&oldy);
		if (!busy) {
			busy = !busy;
			for (i = 1;i <=25;i++)
			for (j = 1;j <=80;j++) {
				move(j,i);
				buff[(i-1)*80+(j-1)] = getram();
			}
			blank = 0;
			busy = !busy;
		}
	}
}

void print_ram(int ch)
{
	_AH = 9;
	_AL = ch % 256;
	_BL = ch / 256;
	_BH = 0;
	_CX = 0;
}

void chk_mode()
{
	union REGS r;
	int video;

	r.h.ah = 15;
	video = int86(0x10,&r,&r) & 255;
	if (video == 7) video_base = 0xb000;
	else video_base = 0xb800;
}

void where(int *x,int *y)
{
	_BH = 0;
	_AH = 3;
	geninterrupt(0x10);
	*x = _DL +1;
	*y = _DH +1;
}

void move(int x,int y)
{
	_BH = 0;
	_DH = y-1;
	_DL = x-1;
	geninterrupt(0x10);
}

int getram()
{
	_AH = 8;
	_BH = 0;
	geninterrupt(0x10);
	return _AX;
}