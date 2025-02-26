	   /*-------- Designed Under Course No. CSE 204 ---------*/
/* This Program is a simulator of Computer Communication system. It operates
three windows to display input, transmitted & output data of the system.User
has to give input of a string of any length. The string is then transmitted
through the channel in encoded form using Dynamic Huffman Tree. The transmi
tted string is received by receiving end and then decoded to form original
string. Total process  can be stoped by pressing the <ENTER> button.
					Programmed by :
						Masud Karim Khan
						2nd Year C S E
						Roll No : 17
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct node {
	char ch;
	int friq;
	struct node *father;
	struct node *left,*right;
} *root;

struct node *array[256],*last;
char gcode[256];

void init()
{
	register int i;

	root = NULL;
	for (i=0;i<27;i++) array[i] = NULL;
}

void tree(char ch)
{
	register int i,j;
	struct node *dummy;

	for (i=0;array[i] && (array[i]->ch !=ch);i++);
	if (array[i]) {
		for (j=0;array[j]->friq != array[i]->friq;j++);
		if (array[i] == array[i]->father->left)
			array[i]->father->left = array[j];
		else array[i]->father->right = array[j];
		if (array[j] == array[j]->father->left)
			array[j]->father->left = array[i];
		else array[j]->father->right = array[i];
		dummy = array[j]->father;
		array[j]->father = array[i]->father;
		array[i]->father = dummy;
		dummy = array[i];
		array[i] = array[j];
		array[j] = dummy;
		(array[j]->friq)++;
		for (dummy = array[j]->father;dummy != root;dummy = dummy->father) {
			(dummy->friq)++;
			if (dummy->father->left == dummy && dummy->father->right->friq < dummy->friq) {
				dummy->father->left = dummy->father->right;
				dummy->father->right = dummy;
			}
		}
		(root->friq)++;
	}
	else {
		if (!i) {
			root = (struct node *)malloc(sizeof(struct node));
			root->left = (struct node *)malloc(sizeof(struct node));
			array[i] = (struct node *)malloc(sizeof(struct node));
			array[i]->left = array[i]->right = NULL;
			root->father = root->left->left = root->left->right = NULL;
			root->right = array[i];
			root->friq = array[i]->friq = 1;
			root->left->friq = 0;
			root->left->father = root->right->father = root;
			array[i]->ch = ch;
			last = root->left;
		}
		else {
			last->left = (struct node *)malloc(sizeof(struct node));
			last->right = (struct node *)malloc(sizeof(struct node));
			last->left->left = last->left->right = NULL;
			array[i] = last->right;
			array[i]->left = array[i]->right = NULL;
			array[i]->ch = ch;
			array[i]->friq = last->friq = 1;
			last->left->friq = 0;
			last->left->father = last->right->father = last;
			last = last->left;
			for (dummy = last->father;dummy != root;dummy = dummy->father) {
				(dummy->friq)++;
				if (dummy->father->left == dummy && dummy->father->right->friq < dummy->friq) {
					dummy->father->left = dummy->father->right;
					dummy->father->right = dummy;
				}
			}
			(root->friq)++;
		}
	}
}

int code_disp(struct node *head,char ch,char *code)
/*	Function to display codes of different letters  */
{
	char dummy[256];

	if (head->ch ==ch) {
		printf("%s",code);
		strcpy(gcode,code);
		return 1;
	}
	strcpy(dummy,code);
	if (head->left) {
		strcat(code,"0");
		if (code_disp(head->left,ch,code)) return 1;
	}
	if (head->right) {
		strcat(dummy,"1");
		if (code_disp(head->right,ch,dummy)) return 1;
	}
	return 0;
}

char *encode(char ch)
{
	register int i;
	char code[256] = "";

	for (i=0;array[i] && (array[i]->ch !=ch);i++);
	if (array[i]) {
		code_disp(root,ch,code);
		return gcode;
	}
	else {
		putch(ch);
		return code;
	}
}

void decode(char *line)
/*	Function to decode a given code sequance  */
{
	struct node *curr;

	curr = root;
	while(*line) {
		if (*line == '0') curr = curr->left;
		else curr = curr->right;
		line++;
	}
	putch(curr->ch);
}

void main()
{
	char ch,code[256];
	int x1=1,y1=2,x2=1,y2=7,x3=1,y3=20,i;

	clrscr();
	init();
	printf("Enter String to transmit :");
	gotoxy(1,6);printf("The transmitted code is ....");
	gotoxy(1,19);printf("The received string is :");
	gotoxy(x1,y1);
	while ((ch = getch()) != 13) {
		putch(ch);
		x1 = wherex(); y1 = wherey();
		gotoxy(x2,y2);
		strcpy(code,encode(ch));
		x2 = wherex(); y2 = wherey();
		gotoxy(x3,y3);
		if (!code[0]) putch(ch);
		else decode(code);
		x3 = wherex(); y3 = wherey();
		gotoxy(x1,y1);
		tree(ch);
	}
}