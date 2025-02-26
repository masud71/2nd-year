#include <dos.h>
#include <time.h>

void tsr_clock();
void chk_mode();
void where(int *,int *);
void interrupt dos_idle();
void interrupt new_int8();
void interrupt (*old_int28)();
void interrupt (*old_int8)();
void interrupt (*old_int64)();
void print_ram(int,int,char);

int count=0;
char far *dos_active,busy=0;
long video_base;
struct time t;

main()
{
	union REGS r;
	struct SREGS s;
	int x,y,i;
	char data1[30]="Masud's TSR Clock is installed",data2[30]="TSR Clock is already installed";

	chk_mode();
	old_int64 = getvect(0x64);
	if (!old_int64) {
		setvect(0x64,new_int8);
		where(&x,&y);
		for (i= 0;i<30;i++) print_ram(x+i,y,data1[i]);
	}
	else {
		where(&x,&y);
		for (i= 0;i<30;i++) print_ram(x+i,y,data2[i]);
		return;
	}
	r.h.ah = 0x34;
	int86x(0x21,&r,&r,&s);
	dos_active = MK_FP(s.es,r.x.bx);
	old_int28 = getvect(0x28);
	old_int8 = getvect(0x8);
	setvect(0x28,dos_idle);
	setvect(0x8,new_int8);
	keep(1,400);
}

void interrupt new_int8()
{
	count++;
	(*old_int8)();
	if (!dos_active && !busy) tsr_clock();
}

void interrupt dos_idle()
{
	(*old_int28)();
	if (!busy) tsr_clock();
}

void tsr_clock()
{
	if (count >= 18) {
		count = 0;
		if (!busy) {
			gettime(&t);
			busy = !busy;
			print_ram(75,1,t.ti_sec % 10 +0x30);
			print_ram(74,1,t.ti_sec / 10 +0x30);
			print_ram(73,1,':');
			print_ram(72,1,t.ti_min % 10 +0x30);
			print_ram(71,1,t.ti_min / 10 +0x30);
			print_ram(70,1,':');
			print_ram(69,1,(t.ti_hour % 12) % 10 +0x30);
			print_ram(68,1,(t.ti_hour % 12) / 10 +0x30);
			if (t.ti_hour / 12) print_ram(77,1,'p');
			else print_ram(77,1,'a');
			print_ram(78,1,'m');
			busy = !busy;
		}
	}
}

void print_ram(int x,int y,char ch)
{
	char far *p;

	x = (x+ (y-1)*80)*2;
	p = MK_FP(video_base,x);
	*p = ch;
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
}