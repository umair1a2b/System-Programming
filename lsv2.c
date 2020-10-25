//-------------------------------------------------------- HEADER SECTION ------------------------------------- (^.^)
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h> // stat
#include <string.h> // string manipulations 
#include <fcntl.h> // if you are using constant O_RDONLY
#include <pwd.h>   // for reading /etc/passwd 
#include <grp.h>   // for reading /etc/group
#include <time.h>  // for time functions
#include <sys/ioctl.h> // for getting the dimension (row vs col)
//--------------------------------------------------------- USER DEFINED FUNCTIONS ----------------------------- ('.')
extern int errno;
int compare (const void * a, const void * b ) {
    const char *pa = *(const char**)a;
    const char *pb = *(const char**)b;

    return strcmp(pa,pb);
}
int * get_row_col(){
    int *dimension=(malloc)(sizeof (int)*2);
   
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    dimension[0]=w.ws_row;  // make sure your main returns int
    dimension[1]=w.ws_col;

    return dimension;
}

int get_count(char *directory_name){
	DIR *dp = opendir(directory_name);
        struct dirent * entry; errno = 0;
        if(dp==NULL){
	  fprintf(stderr,"Cannot open directory:%s\n",directory_name);
		exit(0);
	}
	int n=0;
	while((entry=readdir(dp))!=NULL){
	     if(entry==NULL && errno != 0){
		perror("Read-dir error : ");
		break;

		}
          if(entry->d_name[0] !='.' && entry->d_name !=".."){
	     n++;
	  }

	} //endwhile
	
        closedir(dp); 
	return n;

}
void read_directory(char *directory_name){
       int count = get_count(directory_name);
	printf("Total files:%d\n\n",count);
       DIR *dp = opendir(directory_name); // open the directory 
       

       struct dirent *entry;      // create a structure to read the directory contents
       errno = 0;
       if(dp==NULL){
	fprintf(stderr,"Cannot open directory:%s\n",directory_name); 
	return;
	}


     // char *dir = (malloc)(sizeof (char *) * count);

	char *list[count];
        int *dimension=get_row_col();
        
	
      int i=0;
      while((entry = readdir(dp))!=NULL){
	   
	   if(entry==NULL && errno != 0){
		perror("Read-dir error : ");
		break;

	  }

          if(entry->d_name[0] !='.' && entry->d_name !=".."){

		list[i++]=entry->d_name;
          
	  }
	  
      }

 //============================================== QSORT =================================================================

       int size = sizeof(list)/sizeof(char *); 
       qsort(list,size,sizeof(char *),compare);
       
       for(int i=0;i<count;i++){
	printf("%s \n",list[i]);

       }
	closedir(dp);

}
void welcome(){
	printf("\n\t\t ---------------------------------\n");
	printf("\t\t This is version 2 of ls utility\n");
	printf("\n\t\t ---------------------------------\n\n");

}
//-------------------------------------------------------- Main function ------------------------------------------ ('!')
int main(int argc,char *argv[]){
	welcome();
        if(argc==1){
	     // show the contents of present directory in case of no command line arguments 
	     read_directory(".");
	     return 0;	
	}

       for(int i=1;i<argc;i++){
	    printf("%s/:\n",argv[i]);
	    read_directory(argv[i]);
            printf("\n");
	}
	

	return 0;
}
