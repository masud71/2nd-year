#include <stdio.h>
#include <alloc.h>
#include <ctype.h>

struct node {
	char data;
	struct node *left,*right;
};

struct node *tree(char *);
void preorder(struct node *);
void postorder(struct node *);

main()
{
	char string[60],newstr[60];
	struct node *head;

	clrscr();
	printf("Enter a mathematical expression : ");
	gets(string);
	head = tree(string);
	preorder(head);
	printf("\n");
	postorder(head);
	printf("\n");
	getch();
}

struct node *tree(char *pre)
{
	int i =0,index = -1;
	struct node *stack[10],*dummy,*current,*root;

	root = (struct node *)malloc(sizeof(struct node));
	root->data = pre[0];
	root->left = root->right = NULL;
	current = root;
	for (i=1;pre[i];i++) {
		dummy =(struct node *)malloc(sizeof(struct node));
		dummy->data = pre[i];
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
		if (isalpha(current->data)) current = stack[index--];
	}
	return  root;
}

void preorder(struct node *root)
/* Preorder Travarse   */
{
	struct node *temp,*dummy[10];
	int current = 0;

	dummy[0] = NULL;
	temp = root;
	while (temp) {
		printf(" %c",temp->data);
		if (temp->right) dummy[++current] = temp->right;
		if (temp->left) temp = temp->left;
		else temp = dummy[current--];
	}
}

void postorder(struct node *root)
/*  Post Order Travarse  */
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
					printf(" %c",temp->data);
					temp = dummy[current];
					if (!temp) ok = 0;
					cstat = status[current--];
				}
			}
			else {
				printf(" %c",temp->data);
				temp = dummy[current];
				if (!temp) ok = 0;
				cstat = status[current--];
			}
		} while (ok);
	}
}