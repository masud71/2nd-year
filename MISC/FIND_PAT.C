/*	Display lines containing particular word	*/

#include <stdio.h>
#include <string.h>

main(int argc,char *argv[])
{
	FILE *fp;
	char line[80];

	if (argc !=3) {
		printf("Parameter mismatched.Type  :- find_pat word f_name");
		exit(1);
	}
	if ((fp=fopen(argv[2],"rb"))==NULL) {
		printf("File not found.");
		exit(1);
	}
	clrscr();
	while (fgets(line,80,fp))
		if (strstr(line,argv[1])) printf("%s",line);
	fclose(fp);
}                                                                   