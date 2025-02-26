#include <stdio.h>
#include <stdlib.h>

struct node {
	int key;
	struct node *next;
} *root = NULL;

void create(void);
void display(void);

main()
{
	int dummy;

	clrscr();
	printf("Enter integer data to sort.Type 0 to stop.\n");
	scanf("%d",&dummy);
	if (dummy) {
		root = (struct node *)malloc(sizeof(struct node));
		root->key = dummy;
		root->next = NULL;
		create();
	}
	display();
}

void create(void)
{
	int dummy;
	struct node *curr,*new,*prev;

	do {
		scanf("%d",&dummy);
		if (dummy) {
			new = (struct node *)malloc(sizeof(struct node));
			new->key = dummy;
			for (curr=root;(curr->key<new->key) && curr;prev=curr,curr=curr->next);
			if (curr == root) {
				root = new;
				root->next = curr;
			}
			else {
				prev->next = new;
				new->next = curr;
			}
		}
	} while(dummy);
}

void display(void)
{
	struct node *curr;

	printf("\nThe sorted numbers are :\n");
	for (curr=root;curr;curr=curr->next)
		printf("%8d",curr->key);
	getch();
}
