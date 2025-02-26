#include <stdio.h>
#include <ctype.h>

#define MAX 10

void push(float);
float pop(void);

float stack[MAX];
int current=-1;

main()
{
	int i;
    float temp,temp2;
	char equation[50];

	printf("Enter an expression : ");
	gets(equation);
	for(i=0;equation[i];) {
		if (isdigit(equation[i])) {
			temp=0;
			while(isdigit(equation[i])) {
				temp=temp*10+equation[i]-'0';
				i++;
			}
			push(temp);
		}
		else if (equation[i] !=' '){
			temp=pop();
			temp2=pop();
			switch (equation[i]) {
				case '+':push(temp+temp2);break;
				case '-':push(temp2-temp);break;
				case '*':push(temp*temp2);break;
				case '/':push(temp2/temp);break;
			}
            i++;
		}
		else i++;
	}
	if (current !=0) printf("\aError in expression !\n\n");
	else printf("The result is : %6.2f\n\n",pop());
	getch();
}

void push(float data)
{
	current++;
	stack[current]=data;
}

float pop(void)
{
	current--;
	return (stack[current+1]);
}