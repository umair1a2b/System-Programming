#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#define BUFFER_LEN 512

int grounder = 0;
char file[100];
int isIORedirection=0;   // in case of I/O Redirection 

//function to get the terminal size at run time
int getPageSize ()
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;  
}
int get_option(int read_lines,int total_lines,FILE *in){
	
	
	
	if(grounder==0){
		printf("\033[7m --more-- %d (%%) \033[m",(read_lines*100)/total_lines); 
	}
	int c;		
	back:c=getc(in);
	if(grounder==1){
		grounder=0;
		printf("\033[2A \033[2K \033[1G");
	    }        
	if(c == 'q'){
		printf("\n");
	    return 0;
	}
       if ( c ==' '){
	  printf("\n");
				
	 return 1;
	}
      if ( c == '\n' ){
	 printf("\n");	   
	  return 2;		
       }
	 
      if ( c == '/' ){
	  printf("\n");
 	   return 3;
	}

      if ( c== 'v' ){
	    if(isIORedirection==1){
		goto back;
	    }
	    return 4;
	}	
      return 5;
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

int * search_string(char word[],int pos,FILE *fp){
	char buffer[BUFFER_LEN];
	fseek(fp, pos, SEEK_SET);
	int * ret = (int*) malloc(10);
	int lines=0;	int isFound=0;
	int pos_t = pos;
	while(fgets(buffer,BUFFER_LEN,fp)){
		lines+=1;
		if(strcmp(buffer,word)==0){
		        isFound=1;
			 pos = ftell(fp);
			 ret[0]=pos;ret[1]=lines;
			printf("...skipping\n");
			break;
		}
		
	}
	if(isFound==0){
		grounder = 1;
	        printf("\033[7m pattern not found \033[m");	  
 		ret[0]=pos_t;ret[1]=0;return ret;
	}
   return ret;

}
void read_more(FILE *fp){
	char buffer[BUFFER_LEN];
	int read_lines = 0;
	int option;
	FILE *tty = fopen("/dev//tty","r");

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
			//printf("\033[1A \033[2K \033[1G");
				system("stty -cbreak echo <&2");			
			exit(0);
	        }
		else if(option ==1){
		  
		   read_lines-=PAGE;printf("\033[1A \033[2K \033[1G");
		}
		else if(option==2){
		printf("\033[1A \033[2K \033[1G");
		   read_lines-=1;
		}
		else if(option == 3){
		   	system("stty -cbreak echo <&2");
		   	printf("\033[1A \033[2K \033[1G");
		   	printf("/");
			char word[256];
			fgets(word, sizeof(word), tty);
			int pos = ftell(fp);
			int t = ftell(tty);
			int *ar; // array to get values after search
			ar = search_string(word,pos,fp);//printf("before lines: %d\n",read_lines);
			if(ar[1]==0){
			fseek(fp,ar[0]-10,SEEK_SET);read_lines-=1;
			fflush(stdin);system("stty cbreak -echo <&2");
			}
			 else{

				read_lines-=PAGE;read_lines_so_far+=ar[1]; fseek(fp,-10,SEEK_CUR);
			}		
			system("stty cbreak -echo <&2");

		}

		else if(option == 4){
		  	int pid ;
			pid = fork();
			if(pid == 0){ //child process 
			execlp("vim","vim",file,(char*)NULL);
			}
			else{		
			    read_lines-=1;
			    wait(0);
			    printf("\033[1A \033[2K \033[1G");   
			}	 
		}
	    } 
	     	
	}
}

void welcome(){
	printf("\n\t\t ---------------------------------\n");
	printf("\t\t This is version 5 of more utility\n");
	printf("\n\t\t ---------------------------------\n\n");

}
int main(int argc,char *argv[]){
	
	
	system("stty cbreak -echo <&2");

	welcome();	

        
	if(argc==1){
	 isIORedirection=1;
	  read_more(stdin);
	  
	}
	else{
	
        FILE *fp;
       for(int j=1;j<argc;j++){
		fp =fopen(argv[j],"r");
		if(fp==NULL){
		  printf("Sorry Something goes wrong ! \n");
			exit(0);	
		}
		
		printf("::::::::::::::\n");
		printf("%s\n",argv[j]);
		printf("::::::::::::::\n");
		strncpy(file,argv[j],strlen(argv[j]));
		read_more(fp);
		fclose(fp);	

	}
       }
	system("stty -cbreak echo <&2");
	return 0;
}
