//-------------------------------------------------------- HEADER SECTION ------------------------------------- (^.^)
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>     // stat
#include <string.h>      // string manipulations 
#include <fcntl.h>      // if you are using constant O_RDONLY
#include <pwd.h>       // for reading /etc/passwd 
#include <grp.h>      // for reading /etc/group
#include <time.h>    // for time functions
//--------------------------------------------------------- USER DEFINED FUNCTIONS ----------------------------- ('.')
extern int errno;
int swtch=0;
void get_user_name(int uid){
     struct passwd * pwd;
     pwd = getpwuid(uid);
     if(pwd==NULL){
         printf("No user found against %d\n",uid);
         return;
	}
     printf("%-5s",pwd->pw_name);
}
void get_group_name(int gid){
   struct group * gwd;
   gwd = getgrgid(gid);
   if(gwd == NULL){
      printf("No group found against %d\n",gid); return;
   }
   printf("%-5s",gwd->gr_name);

}
void set_permission(struct stat buf){
	// ---- --- --- --- ---
          char mode[11];strcpy(mode, "----------");
           if ((buf.st_mode &  0170000) == 0010000) 
                        mode[0]='p';
	   else if ((buf.st_mode &  0170000) == 0020000) 
			mode[0]='c';
	   else if ((buf.st_mode &  0170000) == 0040000) 
			mode[0]='d';
	   else if ((buf.st_mode &  0170000) == 0060000) 
			mode[0]='b';
	   else if ((buf.st_mode &  0170000) == 0100000) 
			mode[0]='-';
	   else if ((buf.st_mode &  0170000) == 0120000) 
			mode[0]='l';
	   else if ((buf.st_mode &  0170000) == 0140000) 
			mode[0]='s';
           else 
  		

	mode[0]=' ';   // No type specified
	//special  permissions
	   if((buf.st_mode & 0004000) == 0004000) mode[2] = 's';
	   if((buf.st_mode & 0002000) == 0002000) mode[5] = 's';
	   if((buf.st_mode & 0001000) == 0001000) mode[9] = 't';

	// owner permissions
	   if((buf.st_mode & 0000400) == 0000400) mode[1] = 'r';
	   if((buf.st_mode & 0000200) == 0000200) mode[2] = 'w';
	   if((buf.st_mode & 0000100) == 0000100) mode[3] = 'x';
	//group permissions
	   if((buf.st_mode & 0000040) == 0000040) mode[4] = 'r';
	   if((buf.st_mode & 0000020) == 0000020) mode[5] = 'w';
	   if((buf.st_mode & 0000010) == 0000010) mode[6] = 'x';
	//others  permissions
	   if((buf.st_mode & 0000004) == 0000004) mode[7] = 'r';
	   if((buf.st_mode & 0000002) == 0000002) mode[8] = 'w';
	   if((buf.st_mode & 0000001) == 0000001) mode[9] = 'x';
	
           printf("%-6s  ",mode);
        
}
void do_long_listing(char *filename,char *file){
	
	struct stat info;
        int status = lstat(filename,&info);
	if(status==-1){
	perror("ERROR:");return;
	}

	set_permission(info);
	printf("%-7ld",info.st_nlink);
	get_user_name(info.st_uid);
        get_group_name(info.st_gid);
	printf("%-5ld  ",info.st_size);
        long sec = info.st_mtime;
	int j=0;

	char *time = ctime(&sec);

	while(time[j++]!='\n');
	time[j-1]=' ';

        printf("%10s  ",time);
        //printf("%s  ",file);
	
	


}
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
	  // printf("atif.txt   ");
	   if(entry==NULL && errno != 0){
		perror("Read-dir error : ");
		break;

		}
          if(entry->d_name[0] !='.' && entry->d_name !=".."){
	     char path[100];
             strcpy(path,directory_name);strcat(path,"/");strcat(path,entry->d_name);
	     

	     if(swtch==1 || swtch==3){
	          printf("%-10ld",entry->d_ino);
             }
             if(swtch ==2 || swtch==3){
		 do_long_listing(path,entry->d_name);
             }

	    printf("%s\n",entry->d_name);
	
	  }
	  
      }
	closedir(dp);

}
void welcome(){
	printf("\n\t\t ---------------------------------\n");
	printf("\t\t This is version 3 of ls utility\n");
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

       char *s = (malloc)(sizeof(char*)*4);
       s="";
       for(int i=1;i<argc;i++){
           // printf("%s\n",argv[i]);
	  if(strcmp(argv[i],"-i")==0){
               s="-i"; swtch=1; break;
          }
          if(strcmp(argv[i],"-l")==0){
	       s="-l"; swtch=2; break;
          }
         if( strcmp(argv[i],"-il")==0 || strcmp(argv[i],"-li")==0){
   	       s="-il"; swtch=3; break;
          }
      }
  
	
    
       for(int i=1;i<argc;i++){
            if(strcmp(argv[i],s)!=0 && strcmp(argv[i],"-li")!=0){
		 printf("%s:\n",argv[i]);
	    read_directory(argv[i]);
           }
	   
            printf("\n");
	}
	

	return 0;
}
