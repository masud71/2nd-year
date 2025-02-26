
#define NULL 0
#define random(num)     (rand() % (num))
#define randomize()     srand((unsigned)time(NULL))

void input(void);
void process(void);

struct node {
	char name[20];
	struct node *next;
} *Head = NULL;

main()
{
	input();
	process();
}

void input(void)
{
	struct node *current,*prev=NULL;
	char tempname[20];

	clrscr();
	printf("Enter names (Type End to stop) :\n");
	do {
		gets(tempname);
		if (stricmp(tempname,"END"))
			if (Head) {
				current = (struct node *) malloc(sizeof(struct node));
				strcpy(current->name,tempname);
				prev->next = current;
				prev = current;
			}
			else {
				Head = (struct node *) malloc(sizeof(struct node));
				strcpy(Head->name,tempname);
				prev = Head;
			}
		else prev->next = Head;
	} while (stricmp(tempname,"END"));
}

void process(void)
{
	struct node *current,*prev;
	int i,counter;

	randomize();
	i = random(20);
	current = Head;
	while (current != current->next) {
		for (counter = 1;counter < i;counter++) {
			prev = current;
			current = current->next;
		}
		prev->next = current->next;
		free(current);
		current = prev->next;
	}
	printf("\nThe selected person is : %s",current->name);
	getch();
}