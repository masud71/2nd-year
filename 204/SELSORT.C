#include <stdio.h>
#include <stdlib.h>

struct node {
	int key;
	struct node *next;
} *root;

void create(void);
void sort(void);
void display(void);

main()
{
	create();
	sort();
	display();
}

void create(void)
{
	struct node *dummy=NULL;
	int data;

	clrscr();
	printf("Enter data to sort :\n");
	do {
		scanf("%d",&data);
		if (data) {
			root = (struct node *)malloc(sizeof(struct node));
			root->key = data;
			root->next = dummy;
			dummy = root;
		}
	}while (data);
}

void sort(void)
{
	struct node *d1,*d2,*min;
	int temp;

	for (d1=root;d1->next;d1=d1->next) {
		min = d1;
		for (d2=d1->next;d2;d2=d2->next)
			if (d2->key < min->key) min = d2;
		temp = min->key;
		min->key = d1->key;
		d1->key = temp;
	}
}

void display(void)
{
	struct node *curr;

	printf("The sorted numbers are :\n");
	for (curr=root;curr;curr=curr->next)
		printf("%8d",curr->key);
	getch();
}