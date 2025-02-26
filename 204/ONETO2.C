#include <stdio.h>
#include <alloc.h>

struct node {
	char key;
	struct node *next;
};

main()
{
	struct node *main,*root1,*root2;
	int i;

	root2 = NULL;
	for (i=0;i<=10;i++) {
		root1 = (struct node *)malloc(sizeof(struct node));
		root1->data =65+i;
		root1->next = root2;
		root2 = root1;
	}
	main = root1;
	view(main);
	oneto2(main,root1,root2);
	view(main);
	view(root1);
	view(root2);
	getch();
}

void oneto2(struct node *main,*root1,*root2)
{
	struct node *dummy,*d1,*d2,*temp;

	dummy = main->next;
	d1 = (struct node *)malloc(sizeof(struct node));
	d1->data = main->data;
	d2 = (struct node *)malloc(sizeof(struct node));
	d2->data = dummy->data;
	root1 = d1;
	root2 = d2;
	dummy = dummy->next;
	while (dummy <> NULL) {
		temp = d1;
        d1 = (struct node *)malloc(sizeof(struct node));
		d1->data = dummy->data;
		temp->next = d1;
		dummy = dummy->next;
		if (dummy <> NULL) then {
			temp = d2;
			d2 = (struct node *)malloc(sizeof(struct node));
			d2->data = dummy->data;
			temp->next = d2;
			dummy = dummy->next;
		}
	}
	d1->next = d2->next = NULL;
}

void view(struct node *list)
{
	while(list) {
		printf("%c  ",list->data);
		list = list->next;
	}
}