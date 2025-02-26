#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>

struct link {
	char data;
	struct link *next;
};

struct link *create(void);
struct link *mystery(struct link *,struct link *);
void show (struct link *);

main()
{
	struct link *l1,*l2;

	l1 = create();
	show(l1);
	l1 = mystery(l1,NULL);
	show(l1);
}

struct link *create(void)
{
	register int i;
	struct link *temp,*temp1=NULL;

	randomize();
	for (i=0;i<=4;i++) {
		temp =(struct link *)malloc(sizeof(struct link));
		temp->data =random(26)+65;
		temp->next = temp1;
		temp1 = temp;
	}
	return temp;
}

struct link *mystery(struct link *aa,struct link *bb)
{
	struct link *temp;

	if (aa == NULL) return bb;
	else {
		temp = mystery(aa->next,aa);
		aa->next = bb;
		return temp;
	}
}

void show(struct link *temp)
{
	while (temp) {
		printf(" %c ",temp->data);
		temp = temp->next;
	}
	printf("\n");
}