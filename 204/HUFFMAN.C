/*		Program to produce Huffman Codes analysing frequencies of letters
of a given string.After coding the prgram displays Huffman tree in the
screen & the codes of different letters. The second part of the prgram can
decode a code sequance using Huffman tree which was produced by the first
part of the program.
		Programmed By : Masud Karim Khan ,2nd Year C S E ,Roll - 17
*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <graphics.h>

void coding(char *);
void decode(char *);
void display(struct c_rec *,int,int);
void code_disp(struct c_rec *,char *);

struct c_rec {
	char ch;
	struct c_rec *left;
	struct c_rec *right;
} *root;

main()
{
	int driver,mode;
	char line[40],code[20]="\0";

	printf("Enter a string : ");
	gets(line);
	coding(line);
	printf("\nDo you want to display the tree ? (y/n) ");
	if (toupper(getch())=='Y') {
		driver=DETECT;
		initgraph(&driver,&mode,"");
		display(root,100,10);
		getch();
		closegraph();
	}
	code_disp(root,code);
	printf("\n\nEnter a code to decode : ");
	gets(line);
	decode(line);
}



void coding(char *line)
/* Function to generate code using a given string */
{
	char array[27];
	register int i,j,k;
	char max,max_index,curr=0,total;
	struct c_rec *dummy;
	struct {
		struct c_rec *addr;
		char size;
	} stack[20],right,left;

	for (i=0;i<27;i++) array[i]=0;
	for (i=0;line[i];i++)
		if (line[i] !=' ') array[toupper(line[i])-'A']++;
		else array[26]++;
	do {
		max=0;
		for (i=0;i<27;i++)
			if (array[i] > max) {
				max = array[i];
				max_index=i;
			}
		if (max) {
			array[max_index]=0;
			dummy = (struct c_rec *)malloc(sizeof(struct c_rec));
			if (max_index != 26) dummy->ch = max_index + 'A';
			else dummy->ch=' ';
			dummy->left=dummy->right=NULL;
			stack[curr].addr = dummy;
			stack[curr++].size = max;
		}
	}while (max);
	total=curr;
	for (i=1;i<total;i++) {
		right = stack[--curr];
		left = stack[--curr];
		dummy = (struct c_rec *)malloc(sizeof(struct c_rec));
		dummy->ch = 0;
		dummy->left = left.addr;
		dummy->right = right.addr;
		left.addr = dummy;
		left.size = left.size + right.size;
		for (j=0;j<curr && left.size<stack[j].size;j++);
		for (k=curr;k>j;k--) stack[k] = stack[k-1];
		stack[j] = left;
		curr++;
	}
	root = stack[0].addr;
}

void display(struct c_rec *root,int x,int y)
/*	Functin to display the tree in graphic screen  */
{
	char dummy[2];

	if (root->ch == 0) outtextxy(x,y,"Nil");
	else if (root->ch == ' ') outtextxy(x,y,"' '");
	else {
		sprintf(dummy,"%c",root->ch);
		outtextxy(x,y,dummy);
	}
	if(root->left) {
		line(x,y+10,x-30,y+20);
		display(root->left,x-35,y+25);
	}
	if(root->right) {
		line(x,y+10,x+30,y+20);
		display(root->right,x+35,y+25);
	}
}

void code_disp(struct c_rec *head,char *code)
/*	Function to display codes of different letters  */
{
	char dummy[20];

	if (isalpha(head->ch) || (head->ch == ' ')) {
		printf("\nThe code of '%c' is : %s",head->ch,code);
		return;
	}
	strcpy(dummy,code);
	if (head->left) {
		strcat(code,"0");
		code_disp(head->left,code);
	}
	if (head->right) {
		strcat(dummy,"1");
		code_disp(head->right,dummy);
	}
}

void decode(char *line)
/*	Function to decode a given code sequance  */
{
	struct c_rec *curr;

	curr = root;
	printf("\nThe decoded massage is : ");
	while(*line) {
		if (*line == '0') curr = curr->left;
		else curr = curr->right;
		line++;
		if (curr->ch) {
			putch(curr->ch);
			curr = root;
		}
	}
	getch();
	printf("\n");
}