//-------------------------------------------------------- HEADER SECTION ------------------------------------- (^.^)
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
//--------------------------------------------------------- USER DEFINED FUNCTIONS ----------------------------- ('.')
extern int errno;
void read_directory(char *directory_name){
       DIR *dp = opendir(directory_name); // open the directory 
       //chdir(directory_name);
       struct dirent *entry;      // create a structure to read the directory contents
       errno = 0;
       if(dp==NULL){
	fprintf(stderr,"Cannot open directory:%s\n",directory_name); 
	return;

	}
      while((entry = readdir(dp))!=NULL){
	   
	   if(entry==NULL && errno != 0){
		perror("Read-dir error : ");
		break;

		}

	    printf("%s\n",entry->d_name);
          
	  
      }
	closedir(dp);

}
void welcome(){
	printf("\n\t\t ---------------------------------\n");
	printf("\t\t This is version 1 of ls utility\n");
	printf("\n\t\t ---------------------------------\n\n");

}
//-------------------------------------------------------- Main function ------------------------------------------ ('!')
int main(int argc,char *argv[]){


	welcome();
        if(argc==1){
		 // show the contents of present directory in case of no command line arguments 
	     printf("./\n");
	     read_directory(".");
	     return 0;	
	
	}

       for(int i=1;i<argc;i++){
	    printf("%s:\n",argv[i]);
	    read_directory(argv[i]);
            printf("\n");
	}
	

	return 0;
}
