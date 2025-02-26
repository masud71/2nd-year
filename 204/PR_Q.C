#include <stdio.h>

#define MAX 10
#define PR  5

void init(void);
void add(char,int);
char serve(void);
void show(int);

char array[PR][MAX];
int head[PR],tail[PR];
char empty[PR],full[PR];

void init(void)
{
	register int i;

	for (i=0;i<PR;i++) {
		head[i] = tail[i] = 0;
		full[i] = 0;
		empty[i] = 1;
	}
}

void add(char ch,int prt)
{
	if (!full[prt]) {
		array[prt][tail[prt]] = ch;
		tail[prt] = tail[prt] % MAX + 1;
		if (tail[prt] == head[prt]) full[prt] = 1;
	}
}

char serve(void)
{
	char ch;
	register int i;

	for (i= 0; empty[i] && i<MAX; i++);
	if (i < MAX) {
		ch = array[i][head[i]];
		head[i] = head[i] % MAX +1;
		if (head[i] == tail[i]) empty[i] = 1;
	}
	return ch;
}

void show(int id)
{
	int i;

	i = head[id];
	while (i != tail[id]) {
		printf("%c ",array[id][i]);
		i = i % MAX +1;
	}
	printf("\n");
}