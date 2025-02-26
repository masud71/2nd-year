/*	Generalized Tower of Hanoi Program.Possible to find optimum move of disks
for any numbers of Pegs & Disks.For the sake of visual display the number of
Pegs & Disks are limited to 15 & 20 respectively.
		Programmed by : Masud Karim Khan ,2nd Year C S E ,Roll - 17
*/
#include <stdio.h>
#include <dos.h>

#define PEG 15
#define DISK 20

int find_n(int,int);
int n_bar(int,int);
int k_max(int,int);
int find_n1(int,int,int);
void move(int,int,int,int);
int find_m(int,int);
void inittable(int,int);
int power(int);
void cursor(int);

int table[PEG][DISK];
int index[PEG];
int total_peg,counter=0;

main()
{
	int n,p;

	clrscr();
	printf("Enter number of disks : ");
	scanf("%d",&n);
	printf("Enter number of pegs : ");
	scanf("%d",&p);
	total_peg = p;
	inittable(n,p);
	move(n,p,1,p);
	cursor(12);
	getch();
}

int find_n(int k,int p)
/*	Finds the value of N(k,p)   recursively */
{
	if (k==0) return 1;
	if (p==3) return 1;
	return (find_n(k-1,p) + find_n(k,p-1));
}

void inittable(int disk,int peg)
/*  Initialize table & screen for visual display  */
{
	register int i;

	clrscr();
	cursor(-1);
	for (i=1;i<=peg;i++) {
		gotoxy(5*i-1,21);
		printf("ÜÜÜÜ");
		gotoxy(5*i-1,22);
		printf("P-%d",i);
	}
	gotoxy(25,23);printf("Generalized Tower of Hanoi");
	gotoxy(5,24);printf("Movement : 0");
	textattr(112);gotoxy(22,24);
	cprintf(" Programmed by Masud Karim Khan ");textattr(15);
	for (i=0;i<disk;i++) table[0][i] = disk-i;
	for (i=1;i<PEG;i++) index[i] = -1;
	index[0] = disk-1;
	for (i=0;i<disk;i++) {
		gotoxy(5,20-i);
		printf("%d",disk-i);
	}
}

int n_bar(int k,int p)
/*	Finds the value of N_BAR(k,p)  */
{
	int i,sum=0;

	for (i=0;i<=k;i++)
		sum +=find_n(i,p);
	return sum;
}

int k_max(int n,int p)
/*	Finds the value of K_MAX(n,p) by checking inequality  */
{
	register int k;

	for (k=1;;k++)
		if ((n_bar(k-1,p) < n) && (n<=n_bar(k,p)))
			return k;
}

int find_n1(int n,int kmax,int p)
/*	Find optimum value of n1   */
{
	int limit1,limit2,temp;

	if (p ==3) return (n-1);
	limit1 = n_bar(kmax-2,p);
	limit2 = n - n_bar(kmax-1,p) + limit1;
	if (!limit1) limit1=1;
	for (temp=limit1;temp<=limit2;temp++)
		if ((n_bar(kmax-1,p-1) <=(n-temp)) && ((n-temp)<=n_bar(kmax,p-1)))
			return temp;
}

void move(int disk,int peg,int from,int to)
/*	Find value of movement & display the movements   */
{
	int kmax,n1,temp,ok;

	if (disk == 1) {
		gotoxy(5*from,20-index[from-1]);printf("     ");
		table[to-1][++index[to-1]] = table[from-1][index[from-1]--];
		gotoxy(5*to,20-index[to-1]);printf("%d",table[to-1][index[to-1]]);
		counter++;
		gotoxy(16,24);
		printf("%d",counter);
		delay(5000);
		return;
	}
	kmax = k_max(disk,peg);
	n1 = find_n1(disk,kmax,peg);
	temp = from+1;ok = 0;
	while ((temp != from) && !ok) {
		if (temp > total_peg) temp = 1;
		else if (temp==to) temp++;
		else {
				if (index[temp-1] == -1) ok = 1;
				else
				if (table[temp-1][index[temp-1]] < table[from-1][index[from-1]-n1+1])
					temp++;
				else ok = 1;
		}
	}
	move(n1,peg,from,temp);
	move(disk-n1,peg-1,from,to);
	move(n1,peg,temp,to);
}

int power(int n)
/*	Function to calculate 2 to the power n  */
{
	register int i,res=1;

	for(i=1;i<=n;i++) res *= 2;
	return res;
}

void cursor(int no)
/*	Funcion to hide cursor during visual display   */
{
	_AH = 1;
	_CH = no;
	_CL = 13;
	geninterrupt(0x10);
}