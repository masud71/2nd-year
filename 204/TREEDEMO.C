/*  Tree Travarsel Demo */
/*  By   :- Masud Karim Khan   */
#include <stdio.h>
#include <alloc.h>

struct tree {
	char key;
	struct tree *left;
	struct tree *right;
} *root;

void inittree();
void preorder(void);
void inorder(void);
void postorder(void);

main()
{
	inittree();
	printf("\n");
	preorder();
    printf("\n");
	inorder();
	printf("\n");
	postorder();
	getch();
}

void inittree(void)
/* Creation of a tree to Test the travarsel functions   */
{
	struct tree *temp1,*temp2;
	int i;

	root = (struct tree *)malloc(sizeof(struct tree));
	root->key = 'A';
	temp1 = root;
	for (i=1;i<=3;i++) {
		temp2 = (struct tree *)malloc(sizeof(struct tree));
		temp1->left = temp2;
		temp2->key = 'A'+i;
		temp1->right = NULL;
		temp1 = temp2;
	}
	temp1->left = temp1->right = NULL;
	temp2 = (struct tree *)malloc(sizeof(struct tree));
	temp2->key = 'E';
	temp2->right = NULL;
	root->left->right = temp2;
	temp1 = (struct tree *)malloc(sizeof(struct tree));
	temp1->key = 'F';
	temp2->left = temp1;
	temp1->left = NULL;
    temp2 = (struct tree *)malloc(sizeof(struct tree));
	temp2->key = 'G';
	temp1->right = temp2;
	temp2->left = temp2->right = NULL;
    temp1 = root;
	for (i=1;i<=5;i++) {
		temp2 = (struct tree *)malloc(sizeof(struct tree));
		temp1->right = temp2;
		temp2->key = 'G'+i;
		temp2->left = NULL;
		temp1 = temp2;
	}
	temp2->right = NULL;
    temp1 = (struct tree *)malloc(sizeof(struct tree));
	temp1->key = 'M';
	root->right->right->left = temp1;
	temp1->left = NULL;
	temp2 = (struct tree *)malloc(sizeof(struct tree));
	temp2->key = 'N';
	temp1->right = temp2;
    temp1 = (struct tree *)malloc(sizeof(struct tree));
	temp1->key = 'O';
	temp2->left = temp1;
    temp1->left = temp1->right = NULL;
	temp1 = (struct tree *)malloc(sizeof(struct tree));
	temp1->key = 'P';
	temp2->right = temp1;
	temp1->left = temp1->right = NULL;
}

void preorder(void)
/* Preorder Travarse   */
{
	struct tree *temp,*dummy[10];
	int current = 0;

	dummy[0] = NULL;
	temp = root;
	while (temp) {
		printf(" %c",temp->key);
		if (temp->right) dummy[++current] = temp->right;
		if (temp->left) temp = temp->left;
		else temp = dummy[current--];
	}
}

void inorder(void)
/*  Inorder  Travarse  */
{
	struct tree *temp,*dummy[10];
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
			printf(" %c",temp->key);
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

void postorder(void)
/*  Post Order Travarse  */
{
	struct tree *temp,*dummy[10];
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
					printf(" %c",temp->key);
					temp = dummy[current];
					if (!temp) ok = 0;
					cstat = status[current--];
				}
			}
			else {
            	printf(" %c",temp->key);
				temp = dummy[current];
				if (!temp) ok = 0;
				cstat = status[current--];
			}
		} while (ok);
	}
}