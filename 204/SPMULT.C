#include <stdio.h>
#include <alloc.h>

struct node {
	int row,col;
	int data;
	struct node *down,*right;
};

struct node *super1=NULL,*super2=NULL,*super3=NULL;

void entry(int,int,int,int);
void display(int);
void matcreate(int,int,int);
void multiply(void);

main()
{
	int d1,d2;

	printf("Enter dimention of first matrix :(row col) ");
	scanf("%d %d",&d1,&d2);
	matcreate(1,d1,d2);
	printf("Matrix 1 : \n");
	display(1);
	printf("\nEnter dimention of second matrix :(row col) ");
	scanf("%d %d",&d1,&d2);
	matcreate(2,d1,d2);
	printf("Matrix 2 : \n");
	display(2);
	printf("\n\nPress any key to multiply ... ");
	getch();
	multiply();
	printf("The multiplied matrix :\n");
	display(3);
	getch();
}

void entry(int id,int x,int y,int newdata)
{
	struct node *temp1,*temp2,*temp3,*temp4,*new,*dummy,*super;

	if (id==3) super=super3;else if (id==2) super=super2;else super=super1;
	if (!super) {
		super=(struct node *)malloc(sizeof(struct node));
		super->row=super->col=0;
		super->down=super;
		super->right=super;
		if (id==3) super3=super;else if (id==2) super2=super;else super1=super;
	}
	temp2=super;
	temp1=super->down;
	while ((temp1 !=super) && (temp1->row < x)) {
		temp2=temp1;
		temp1=temp1->down;
	}
	if (temp1->row !=x) {
		dummy=(struct node *)malloc(sizeof(struct node));
		dummy->row=x;
		dummy->col=0;
		temp2->down=dummy;
		dummy->down=temp1;
		temp1=temp2=dummy;
	}
	else {
		dummy=temp2=temp1;
		temp1=temp1->right;
		while ((temp1 !=dummy) && (temp1->col < y)) {
			temp2=temp1;
			temp1=temp1->right;
		}
	}
	if (temp2->col !=y) {
		temp4=super;
		temp3=super->right;
		while ((temp3 !=super) && (temp3->col < y)) {
			temp4=temp3;
			temp3=temp3->right;
		}
		if (temp3->col !=y) {
			dummy=(struct node *)malloc(sizeof(struct node));
			dummy->col=y;
			dummy->row=0;
			temp4->right=dummy;
			dummy->right=temp3;
			temp3=temp4=dummy;
		}
		else {
			dummy=temp4=temp3;
			temp3=temp3->down;
			while ((temp3 !=dummy) && (temp3->row < x)) {
				temp4=temp3;
				temp3=temp3->down;
			}
		}
	}
	if (temp1->row==x && temp1->col==y) {
		printf("Currently data is present in this location.\n");
		printf("The data is %d. Want to change ? (Y/N) ",temp1->data);
		if (toupper(getch()) == 'N') return;
		temp1->data=newdata;
		return;
	}
	new=(struct node *)malloc(sizeof(struct node));
	new->row=x;new->col=y;
	new->data=newdata;
	temp2->right=new;
	new->right=temp1;
	temp4->down=new;
	new->down=temp3;
}

void display(int id)
{
	struct node *dummy,*temp,*super;

	clrscr();
	if (id==3) super=super3;else if (id==2) super=super2;else super=super1;
	if ( !super) {
		printf("Blank Matrix\n");return;
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
}

void matcreate(int id,int row,int col)
{
	int data,x,y,tempy;

	printf("Data entry for Matrix %d . '-1' to stop.\n",id);
	for (x=1;x<=row;x++)
		for (y=1;y<=col;y++) {
			scanf("%d",&data);
			if (data == -1) return;
			if (data) entry(id,x,y,data);
	}
}

void multiply()
{
	struct node *temp1,*temp2;
	int product;

	temp1=super1->down;
	temp2=super2->right;
	while(temp1 !=super1) {
		product=0;
		temp1=temp1->right;
		temp2=temp2->down;
		while (temp1->col && temp2->row) {
			if (temp1->col == temp2->row) {
				product=product+(temp1->data * temp2->data);
				temp1=temp1->right;
				temp2=temp2->down;
			}
			else if (temp1->col > temp2->row) temp2=temp2->down;
			else temp1=temp1->right;
		}
		if (product) entry(3,temp1->row,temp2->col,product);
		while(temp1->col) temp1=temp1->right;
		while(temp2->row) temp2=temp2->down;
		temp2=temp2->right;
		if (temp2==super2) {
			temp1=temp1->down;
			temp2=temp2->right;
		}
	}
}