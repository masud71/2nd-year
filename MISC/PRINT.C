#include <stdio.h>
#include <dos.h>
#include <bios.h>

void check_prn(void);
void set_draft_fill(void);
void set_nlq(int );
void set_master(int );
void set_style(char *);

main(int argc,char *argv[])
{
	FILE *fp;
	unsigned char cc;

	if (argc != 3) {
		printf("\aSyntex error. Type :-  Print Filename Print_option\n");
		printf("Options :-\n");
		printf(" 0 - Draft ; 1 - Roman ; 2 - Sens Serif ; 3 - Pica ; 4 - 12 cpi ;\n");
		printf(" 5 - Condenced ; 6 - Emphasized ; 7 - Double Strile ;8 - Double wedth\n");
		printf(" 9 - Italic ; 10 - Underlined ");
		exit(1);
	}
	if ((fp=fopen(argv[1],"rt"))==NULL) {
		printf("\aFile Access Error.");
		exit(1);
	}
	set_style(argv[2]);
	do {
		check_prn();cc=fgetc(fp); biosprint(0,cc,0);
	} while( ! feof(fp));
	fclose(fp);
}

void check_prn(void)
{
	unsigned char TEMP;
	union REGS reg;

	reg.h.ah=0x02;
	int86(0x17,&reg,&reg);
	TEMP=reg.h.ah & 0x08;
/*	if (TEMP == 0x08) {
		cprintf("Printer not ready.On the printer & press any key.");
		getch();
	}
	TEMP=reg.h.ah & 0x20;
	if (TEMP ==0x20) {
		cprintf("Printer out of paper.Make it OK & press any key.");
		getch();
	} */
}

void set_draft(void)
{
	biosprint(1,0,0);biosprint(0,27,0);
	biosprint(0,0x78,0);biosprint(0,0,0);
}

void set_nlq(int mode)
{
	biosprint(1,0,0);biosprint(0,27,0);
	biosprint(0,0x78,0);biosprint(0,1,0);
	biosprint(0,27,0);biosprint(0,0x6b,0);
	biosprint(0,mode,0);
}

void set_master(int mode)
{
	biosprint(1,0,0);biosprint(0,27,0);
	biosprint(0,0x21,0);biosprint(0,mode,0);
}

void set_style(char *i)
{
	switch(*i) {
		case '0' :set_draft();break; 		/*	Draft print */
		case '1' :set_nlq(0);break;  		/*	Roman NLQ	*/
		case '2' :set_nlq(1);break;  		/*	Sens Serif	*/
		case '3' :set_master(0);break;	/*	Pica      	*/
		case '4' :set_master(1);break;	/*	12 cpi    	*/
		case '5' :set_master(4);break;	/*	Condensed	*/
		case '6' :set_master(8);break;	/*	Emphasized	*/
		case '7' :set_master(16);break;   /*  Double strike */
		case '8' :set_master(32);break;	/*	Double wedth  */
		case '9' :set_master(64);break;	/*	Italic		*/
		case '10':set_master(128);break;	/*	Underlined	*/
	}
}