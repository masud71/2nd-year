#include <stdio.h>
#include <alloc.h>

#define MAX 20

struct tree {
	char key;
	struct tree *left,*right;
} *root;
struct tree *array[MAX];
int head=0,tail=1;

void inittree(void);
void levelord(void);
void entry(struct tree *);
struct tree *serve(void);

main()
{
	inittree();
	preorder(root);
	printf("\n");
	levelord();
	printf("\n");
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

void levelord(void)
{
	struct tree *temp;

	entry(root);
	do {
		temp = serve();
		if (temp) {
			printf("%c ",temp->key);
			if (temp->left) entry(temp->left);
			if (temp->right) entry(temp->right);
		}
	} while (temp);
}

void entry(struct tree *node)
{
	array[tail] = node;
	if (tail != MAX-1) tail ++;
	else tail = 0;
}

struct tree *serve(void)
{
	if (head == tail) return NULL;
	else {
		if (head != MAX-1) head++;
		else head = 0;
		return (array[head]);
	}
}