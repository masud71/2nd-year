#include <stdio.h>

void error(int code);

main(int argc,char *argv[])
{
	FILE *fp1,*fp2;
	char oneln[80],temp[80],err;
	static char *list[18]={"LR","AR","SR","MR","DR","CR","L","A","S","M",
							"D","C","MVC","AP","CLC","DP","SP","ED"};
	static char *intlist[18]={"18","1A","1B","1C","1D","19","58","5A","5B",
							"5C","5D","59","D2","FA","D5","FD","FB","DE"};
	int i,j,k,l,m,n,count,lineno=1;

	if (argc != 3) error(0);
	if ((fp1=fopen(argv[1],"rt")) == NULL) error(1);
	if ((fp2=fopen(argv[2],"wt")) == NULL) error(2);
	while (fgets(oneln,80,fp1)) {
		err=0;
		sscanf(oneln,"%s %d,%d(%d,%d",temp,&i,&j,&k,&l);
        for( count=0;(strcmp(temp,list[count]) && (count<18));count++);
		if (count >=12) sscanf(oneln,"%s %d(%d,%d),%d(%d,%d)",temp,&i,&j,&k,&l,&m,&n);
		if (count < 12) {
			if (i>16) err=1;
			if (count<6) {
				if (j>16) err=1;
				sprintf(oneln,"%s%X%X",intlist[count],i,j);
			}
			else {
				if ((k > 16) || (l > 16)) err=1;
				sprintf(oneln,"%s%X%X%X%3X",intlist[count],i,k,l,j);
			}
		}
		else if (count < 18) {
			if ((j>16) || (k>16) || (m>16) || (n>16)) err=1;
			sprintf(oneln,"%s%X%X%X%3X%X%3X",intlist[count],j-1,m-1,k,i,n,l);
		}
		else err=1;
		for (i=0;oneln[i];i++) if (oneln[i]==' ') oneln[i]='0';
		if (!err) fprintf(fp2,"%s\n",oneln);
		else fprintf(fp2,"Error found at line no. : %d\n",lineno);
		lineno++;
	}
	fcloseall();
}

void error(int code)
{
	static char *dat[3]={"Parameter mismatched","Source file not found",
					"Destination file not found"};
	puts(dat[code]);
	exit(1);
}