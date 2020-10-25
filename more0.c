#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_LEN 512

//=================================================== More Version 1 ====================================
void read_more(FILE *fp){
	char buffer[BUFFER_LEN];
	while(fgets(buffer,BUFFER_LEN,fp)){
	    fputs(buffer,stdout);	
	}
}
int main(int argc,char *argv[]){
	printf("\n\t\t ---------------------------------\n");
	printf("\t\t This is version 0 of more utility\n");
	printf("\n\t\t ---------------------------------\n\n");
	if(argc==1){
	  printf("Please Pass the file name \n!");
	  return 0;
	}
	FILE *fp;
	fp =fopen(argv[1],"r");
	if(fp==NULL){
	  printf("Sorry Something goes wrong ! \n");
		exit(0);	
	}
	read_more(fp);
	fclose(fp);	
	

	return 0;
}
