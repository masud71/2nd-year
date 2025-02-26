#include <stdio.h>
#include <ctype.h>
#include <conio.h>
#include <alloc.h>
#include <dos.h>
#include <graphics.h>

struct node {
	char data[5];
	struct node *left,*right;
};

int lx=90;

struct node *prtree(char *);
void preorder(struct node *);
void postorder(struct node *);
void inorder(struct node *);
int findint(char *,int *);
int findrint(char *,int *);
char intopre(char *newstr,char *equation);
struct node *pstree(char *);
float value(struct node *);
void box(void);
void reversevideo(int);
char choice(void);
void setcursor(int);
void display(struct node *,int,int);

main()
{
	char ch1,str[60],nstr[60];
	struct node *head;
	void *area;
	int driver,mode;

	box();
	do {
		ch1 = choice();
		reversevideo(0);
		window(31,12,79,13);
		clreol();
		setcursor(2);
		if (ch1 == 3) exit(1);
		gets(str);
		switch (ch1) {
			case 0:head = prtree(str);break;
			case 1:if (intopre(nstr,str)) head = prtree(nstr);
					break;
			case 2:head = pstree(str);break;
		}
		area = (void *)malloc(4000);
		window(1,1,80,25);
		gettext(1,1,80,25,area);
		driver = DETECT;
		initgraph(&driver,&mode,"");
		display(head,350,20);
		getch();
		closegraph();
		puttext(1,1,80,25,area);
		free(area);
		window(2,15,79,18);
		gotoxy(30,1);clreol();preorder(head);
		gotoxy(30,2);clreol();inorder(head);
		gotoxy(30,3);clreol();postorder(head);
		gotoxy(30,4);clreol();cprintf("%.2f",value(head));
	} while (ch1 != 3);
}

struct node *prtree(char *pre)
{
	int i =0,index = -1,temp;
	struct node *stack[10],*dummy,*current,*root;

    while (pre[i]==' ') i++;
	root = (struct node *)malloc(sizeof(struct node));
	root->data[0] = pre[i++];
	root->data[1] = '\0';
	root->left = root->right = NULL;
	current = root;
	for (;pre[i];) {
        while (pre[i]==' ') i++;
		dummy =(struct node *)malloc(sizeof(struct node));
		if (isdigit(pre[i])) {
			temp = findint(pre,&i);
			itoa(temp,dummy->data,10);
		}
		else {
			dummy->data[0] = pre[i++];
			dummy->data[1] = '\0';
		}
		dummy->left = dummy->right = NULL;
		if (!current->left) {
			current->left = dummy;
			stack[++index] = current;
			current = dummy;
		}
		else {
			current->right = dummy;
			current = dummy;
		}
		if (isdigit(current->data[0])) current = stack[index--];
	}
	return  root;
}

void preorder(struct node *root)
{
	struct node *temp,*dummy[10];
	int current = 0;

	dummy[0] = NULL;
	temp = root;
	while (temp) {
		printf(" %s",temp->data);
		if (temp->right) dummy[++current] = temp->right;
		if (temp->left) temp = temp->left;
		else temp = dummy[current--];
	}
}

void inorder(struct node *root)
{
	struct node *temp,*dummy[10];
	int ok,current = 0;

	dummy[0] = NULL;
	temp = root;
	while (temp) {
		while (temp->left) {
			dummy[++current] = temp;
			temp = temp->left;
		}
		ok = 1;
		do {
			printf(" %s",temp->data);
			if (temp->right) {
				temp = temp->right;
				ok = 0;
			}
			else {
				temp = dummy[current--];
				if (!temp) ok = 0;
			}
		} while (ok);
	}
}

void postorder(struct node *root)
{
	struct node *temp,*dummy[10];
	int current=0,ok,cstat=1,status[10];

	dummy[0] = NULL;
	temp = root;
	while (temp) {
		while (temp->left) {
			dummy[++current] = temp;
			status[current] = 1;
			temp = temp->left;
		}
		ok = 1;
		do {
			if (cstat) {
				if (temp->right) {
					dummy[++current] = temp;
					status[current] = 0;
					temp = temp->right;
					ok = 0;
				}
				else {
					printf(" %s",temp->data);
					temp = dummy[current];
					if (!temp) ok = 0;
					cstat = status[current--];
				}
			}
			else {
				printf(" %s",temp->data);
				temp = dummy[current];
				if (!temp) ok = 0;
				cstat = status[current--];
			}
		} while (ok);
	}
}

int findint(char *eq,int *i)
{
	int temp=0;

	while(isdigit(eq[*i])) {
		temp=temp*10+eq[*i]-'0';
		(*i)++;
	}
	return temp;
}

int findrint(char *eq,int *i)
{
	int temp=0,j;

	while (isdigit(eq[*i])) (*i)--;
	j = *i + 1;
	while(isdigit(eq[j])) {
		temp=temp*10+eq[j]-'0';
		j++;
	}
	return temp;
}

char intopre(char *newstr,char *equation)
{
	int tcurrent=-1,i,ccurrent=0,temp;
	char cstack[10],tstack[10][60];
	char temp1[60],temp2[60];

	for(i=0;equation[i];) {
		while (equation[i] == ' ') i++;
		if (isdigit(equation[i])) {
			temp = findint(equation,&i);
			itoa(temp,tstack[++tcurrent],10);
		}
		else {
			switch (equation[i]) {
				case '+' :
				case '-':if (cstack[ccurrent]=='*' || cstack[ccurrent]=='/') {
							strcpy(temp1,tstack[tcurrent--]);
							strcpy(temp2,tstack[tcurrent--]);
							sprintf(newstr," %c %s %s",cstack[ccurrent],temp2,temp1);
							cstack[ccurrent] =equation[i];
							strcpy(tstack[++tcurrent],newstr);
						} else cstack[++ccurrent] =equation[i];
						break;
				case '(':
				case '*':
				case '/':cstack[++ccurrent] =equation[i];break;
				case ')':while (cstack[ccurrent] !='(') {
							strcpy(temp1,tstack[tcurrent--]);
							strcpy(temp2,tstack[tcurrent--]);
							sprintf(newstr," %c %s %s",cstack[ccurrent--],temp2,temp1);
							strcpy(tstack[++tcurrent],newstr);
						}
						ccurrent--;break;
			}
            i++;
		}
	}
	while (ccurrent) {
		strcpy(temp1,tstack[tcurrent--]);
		strcpy(temp2,tstack[tcurrent--]);
		sprintf(newstr," %c %s %s",cstack[ccurrent--],temp2,temp1);
		strcpy(tstack[++tcurrent],newstr);
	}
	if (tcurrent) return 0;
	strcpy(newstr,tstack[tcurrent]);
	return 1;
}

struct node *pstree(char *pst)
{
	int i,index = -1,temp;
	struct node *stack[10],*dummy,*current,*root;

	i = strlen(pst)-1;
	while (pst[i]==' ') i--;
	root = (struct node *)malloc(sizeof(struct node));
	root->data[0] = pst[i--];
	root->data[1] = '\0';
	root->left = root->right = NULL;
	current = root;
	for (;i>=0;) {
        while (pst[i]==' ') i--;
		dummy =(struct node *)malloc(sizeof(struct node));
		if (isdigit(pst[i])) {
			temp = findrint(pst,&i);
			itoa(temp,dummy->data,10);
		}
		else {
			dummy->data[0] = pst[i--];
			dummy->data[1] = '\0';
		}
		dummy->left = dummy->right = NULL;
		if (!current->right) {
			current->right = dummy;
			stack[++index] = current;
			current = dummy;
		}
		else {
			current->left = dummy;
			current = dummy;
		}
		if (isdigit(current->data[0])) current = stack[index--];
	}
	return  root;
}

float value(struct node *root)
{
	float res;

	if (isdigit(root->data[0])) {
		res = atoi(root->data);
		free(root);
		return res;
	}
	switch (root->data[0]) {
		case '+' :return(value(root->left) + value(root->right));
		case '-' :return(value(root->left) - value(root->right));
		case '*' :return(value(root->left) * value(root->right));
		case '/' :return(value(root->left) / value(root->right));
	}
}

void box(void)
{
	register int i;

	clrscr();
	gotoxy(1,6);putch(201);
	for (i=2;i<=79;i++) putch(205);
	putch(187);
	for (i=7;i<=18;i++) {
		putch(186);
		gotoxy(80,i);putch(186);
	}
	putch(200);
    for (i=2;i<=79;i++) putch(205);
	putch(188);
	gotoxy(1,10);putch(204);
    for (i=2;i<=79;i++) putch(205);
	putch(185);
	gotoxy(1,14);putch(199);
	for (i=2;i<=79;i++) putch(196);
	putch(182);
	gotoxy(35,6);printf("Choice Menu");
	gotoxy(34,10);printf("Input Section");
	gotoxy(34,14);printf("Output Section");
	gotoxy(9,12);printf("Enter the expression : ");
	gotoxy(9,15);printf("Prefix expression   : ");
	gotoxy(9,16);printf("Infix expression    : ");
	gotoxy(9,17);printf("Postfix expression  : ");
	gotoxy(9,18);printf("Value of expression : ");
	window(2,7,79,9);
	reversevideo(1);
	clrscr();
}

void reversevideo(int mode)
{
	if (mode) {
		textcolor(0);
		textbackground(15);
	}
	else {
		textcolor(15);
		textbackground(0);
		lowvideo();
	}
}

char choice()
{
	int x[4]={12,27,42,57};
	char ch,i=0,*name[4]={" Preorder  "," Inorder   "," Postorder ","   Quit    "};

	setcursor(0);
	window(2,7,79,9);
	reversevideo(0);
	gotoxy(12,2);
	cprintf(" Preorder  ");
	reversevideo(1);
	gotoxy(27,2);
	cprintf(" Inorder        Postorder        Quit    ");
	do {
		ch=getch();
		if (ch==000) ch=getch();
		switch (ch) {
			case 75:gotoxy(x[i],2);reversevideo(1);
					cprintf("%s",name[i]);
					if (i==0) i=3;
					else i--;
					gotoxy(x[i],2);reversevideo(0);
					cprintf("%s",name[i]);
					break;
			case 77:gotoxy(x[i],2);reversevideo(1);
					cprintf("%s",name[i]);
					if (i==3) i=0;
					else i++;
					gotoxy(x[i],2);reversevideo(0);
					cprintf("%s",name[i]);
					break;
		}
	}while (ch !=13);
	return i;
}

void setcursor(int size)
{
	_AH = 1;
	_CH = 14-size;
	_CL = 13;
	geninterrupt(0x10);
}

void display(struct node *root,int x,int y)
{
	outtextxy(x,y,root->data);
	lx-=10;
	if(root->left) {
		line(x,y+10,x-lx,y+35);
		display(root->left,x-lx-5,y+40);
	}
	lx+=10;
	if(root->right) {
		lx-=10;
		line(x,y+10,x+lx,y+35);
		display(root->right,x+lx+5,y+40);
	}
}