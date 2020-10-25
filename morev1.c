#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_LEN 512
#define PAGE 12
int get_option(FILE *in){
     printf("\033[7m --more--(%%) \033[m");
     int option;		
     option=getc(in);
      if(option == 'q')
	 return 0;
      if ( option ==' ')			
	 return 1;
      if ( option == '\n' )	
	 return 2;	
   return 4;
}
void read_more(FILE *fp){
	char buffer[BUFFER_LEN];
	int read_lines = 0;
	int option;
	FILE *tty = fopen("/dev//tty","r");
	while(fgets(buffer,BUFFER_LEN,fp)){
	     fputs(buffer,stdout);
	     read_lines ++;
	     if(read_lines == PAGE){
		option = get_option(tty);
		
		if(option==0){
			printf("\033[1A \033[2K \033[1G");
			exit(0);
	        }
		else if(option ==1){
			printf("\033[1A \033[2K \033[1G");
		   read_lines-=PAGE;
		}
		else if(option==2){
		printf("\033[1A \033[2K \033[1G");
		   read_lines-=1;
		}
	    } 
	}
}

int main(int argc,char *argv[]){
		
	printf("\n\t\t ---------------------------------\n");
	printf("\t\t This is version 1 of more utility\n");
	printf("\n\t\t ---------------------------------\n\n");
	
	if(argc==1){
	  read_more(stdin);
	  return 0;
	}
	FILE *fp;	
	for(int i=1;i<argc;i++){

		
		fp =fopen(argv[i],"r");
		if(fp==NULL){
	  		printf("Sorry Something goes wrong ! \n");
		exit(0);	
	}
		printf("::::::::::::::\n");
		printf("%s\n",argv[i]);
		printf("::::::::::::::\n");

		read_more(fp);
	}

	fclose(fp);	
	

	return 0;
}
