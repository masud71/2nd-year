/*----- Demo of data entry , deletion & display of integer numbers in a
Sparse Matrix using orthogonal list. Maximum row or column is infinite
but should be limited to Maxrow = 24 & Maxcolumn = 10 for proper operation
of display function.
				Programmed by :
						Masud Karim Khan
						2nd year , C S E
						Roll : 17                                   -----*/
#include <stdio.h>
#include <alloc.h>

struct node {
	int row,col;
	int data;
	struct node *down,*right;
};

struct node *super=NULL;

void entry(int,int,int,int);
void display(void);

main()
{
	char ch;
	int row,col,data;

	do {
		clrscr();
		printf("1.Entry Data\n");
		printf("2.Display Matrix\n");
		printf("3.Delete Data\n");
		printf("4.Quit\n\n");
		printf("Enter your choice : \n");
		ch = getch();
		switch (ch) {
			case '1': do {
						printf("\nEnter data (Type 0 to stop) : ");
						scanf("%d",&data);
						if (data) {
							printf("Enter location (row col) : ");
							scanf("%d %d",&row,&col);
							entry(col,row,data,0);
						}
					  } while(data);
					  break;
			case '3': if (!super) printf("\nNo matrix is available.\a");
					  else {
						printf("Enter location to be deleted (row col) : ");
						scanf("%d %d",&row,&col);
						entry(col,row,data,1);
					  }
					  break;
			case '2': display();
					  break;
		}
	} while(ch !='4');
}

void entry(int x,int y,int newdata,int delete)
/*
	Function for data entry & delition in the orthogonal list.
*/
{
	struct node *temp1,*temp2,*temp3,*temp4,*new,*dummy;

	if (!super) {
		super=(struct node *)malloc(sizeof(struct node));
		super->row=super->col=0;
		super->down=super;             /* If new matrix then create */
		super->right=super;            /* Super Node                */
	}
	temp2=super;
	temp1=super->down;
	while ((temp1 !=super) && (temp1->row < y)) {
		temp2=temp1;
		temp1=temp1->down;
	}
	if (temp1->row !=y) {
		if (delete) {
			printf("No data is available in specified location.\a");
			return;
		}
		dummy=(struct node *)malloc(sizeof(struct node));
		dummy->row=y;
		dummy->col=0;            /* Create row dummy node for the       */
		temp2->down=dummy;       /* perticular position if not existing */
		dummy->down=temp1;
		temp1=temp2=dummy;
	}
	else {
		dummy=temp2=temp1;
		temp1=temp1->right;
		while ((temp1 !=dummy) && (temp1->col < x)) {
			temp2=temp1;
			temp1=temp1->right;
		}
	}
	temp4=super;
	temp3=super->right;
	while ((temp3 !=super) && (temp3->col < x)) {
		temp4=temp3;
		temp3=temp3->right;
	}
	if (temp3->col !=x) {
        if (delete) {
			printf("No data is available in specified location.\a");
			return;
		}
		dummy=(struct node *)malloc(sizeof(struct node));
		dummy->col=x;
		dummy->row=0;            /* Create col dummy node for the       */
		temp4->right=dummy;      /* perticular position if not existing */
		dummy->right=temp3;
		temp3=temp4=dummy;
	}
	else {
		dummy=temp4=temp3;
		temp3=temp3->down;
		while ((temp3 !=dummy) && (temp3->row < y)) {
			temp4=temp3;
			temp3=temp3->down;
		}
	}
	if (delete) {
		printf("Deleting data ...");
		temp2->right = temp1->right;
		temp4->down = temp1->down;
		free(temp1);
		if (temp2 == temp2->right) {
			for(dummy=super;dummy->down !=temp2;dummy = dummy->down);
			dummy->down = temp2->down;
			free(temp2);
		}
		if (temp4 == temp4->down) {
			for(dummy=super;dummy->right !=temp4;dummy = dummy->right);
			dummy->right = temp4->right;
			free(temp4);
		}
		delay(100);
		return;
	}
	if (temp1->row==y && temp1->col==x) {
		printf("Currently data is present in this location.\n");
		printf("The data is %d. Want to change ? (Y/N) ",temp1->data);
		if (toupper(getch()) == 'N') return;
		temp1->data=newdata;
		return;
	}
	new=(struct node *)malloc(sizeof(struct node));
	new->row=y;new->col=x;
	new->data=newdata;
	temp2->right=new;
	new->right=temp1;
	temp4->down=new;
	new->down=temp3;
}

void display(void)
{
	struct node *dummy,*temp;
	register int i,j;

	clrscr();
	if (!super) {
		printf("Blank Matrix\n");return;
	}
	for (i=1;i<=24;i++)
		for (j=1;j<=10;j++) {
			gotoxy(j*8-7,i);
			putch('0');
		}
	dummy=super->down;
	while (dummy !=super) {
		temp=dummy->right;
		while (temp !=dummy) {
			gotoxy(temp->col *8 - 7,temp->row);
			printf("%d",temp->data);
			temp=temp->right;
		}
		dummy=dummy->down;
	}
	gotoxy(25,25);printf("Press any key to continue...");
	getch();
}
