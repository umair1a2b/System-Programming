#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#define BUFFER_LEN 512
int getPageSize ()
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;  
}

int get_option(int read_lines,int total_lines,FILE *in){
	
     	printf("\033[7m --more-- %d (%%) \033[m",(read_lines*100)/total_lines); 
	int c;	c=getc(in);


      if(c == 'q')
	 return 0;
      if ( c ==' ')			
	 return 1;
      if ( c == '\n' )	
	 return 2;	
      return 3;
   return 0;

}
int get_lines(FILE *fp){
	char buffer[BUFFER_LEN];
	int i=0;
        while(fgets(buffer,BUFFER_LEN,fp)){
		i+=1;
	}
	return i;

}
void read_more(FILE *fp){
	char buffer[BUFFER_LEN];
	int read_lines = 0;
	int option;
	FILE *tty = fopen("/dev//tty","r");
	// set the file offset to zero after getting total lines
	fpos_t position;	
	int total_lines = get_lines(fp);
	fsetpos(fp,&position);	

	int read_lines_so_far=0;
	while(fgets(buffer,BUFFER_LEN,fp)){
	     fputs(buffer,stdout);
	     read_lines_so_far+=1;
	     read_lines ++;
	     int PAGE=getPageSize();    // get the currentl page size 
		
	     if(read_lines == PAGE){
		option = get_option(read_lines_so_far,total_lines,tty);
		if(option == 0){
					
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

	    } // endif
	     	
	}
}

int main(int argc,char *argv[]){
	
	
	printf("\n\t\t ---------------------------------\n");
	printf("\t\t This is version 2 of more utility\n");
	printf("\n\t\t ---------------------------------\n\n");


	
	
	if(argc==1){
	  read_more(stdin);
	}
	
        else{
	
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
		if(i!=1){
		   int pause=getchar();
		 }
	  
          read_more(fp);
	  
	 
	  fclose(fp);	
	}
    }
	
	return 0;
}
