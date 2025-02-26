#include <stdio.h>
#include <ctype.h>

void infix(char *);
void push(float);
float pop(void);

float stack[10];
int current=-1;

main()
{
	char expr[60];
	float result;

	do {
		printf("\nEnter an expression : ");
		gets(expr);
		infix(expr);
		printf("Do another (Y/N) ? ");
	} while (toupper(getch()) == 'Y');
}

void infix(char *equation)
{
	int data,tcurrent=0,i;
	char ch,cstack[10];
    float temp,temp2;

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
			switch (equation[i]) {
				case '+' :
				case '-':if (cstack[tcurrent]=='*' || cstack[tcurrent]=='/') {
							temp=pop();temp2=pop();
							if (cstack[tcurrent]=='*') push(temp*temp2);
							else push(temp2/temp);
							cstack[tcurrent] =equation[i];
						} else cstack[++tcurrent] =equation[i];
						break;
				case '(':
				case '*':
				case '/':cstack[++tcurrent] =equation[i];break;
				case ')':while (cstack[tcurrent] !='(') {
							temp=pop();temp2=pop();
							switch (cstack[tcurrent--]) {
                            	case '+':push(temp+temp2);break;
								case '-':push(temp2-temp);break;
								case '*':push(temp*temp2);break;
								case '/':push(temp2/temp);break;
							}
						}
						tcurrent--;break;
			}
            i++;
		}
		else i++;
	}
	while (tcurrent) {
    	temp=pop();temp2=pop();
		switch (cstack[tcurrent--]) {
			case '+':push(temp+temp2);break;
			case '-':push(temp2-temp);break;
			case '*':push(temp*temp2);break;
			case '/':push(temp2/temp);break;
		}
	}
	if (current !=0) printf("\aError in expression !\n\n");
	else printf("The result is : %6.2f\n\n",pop());
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
}
