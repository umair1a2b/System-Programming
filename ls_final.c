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
#define _XOPEN_SOURCE
//--------------------------------------------------------- USER DEFINED FUNCTIONS ----------------------------- ('.')
extern int errno;
int swtch=0;
int iterator = 0;
long int blocksize=0;
struct data{

     char filename[100];
     char username[100];
     char groupname[100];
     char permission[100];
     long int link;
     long int size;
     char t_time[100];
     long int inode;
     unsigned char  d_type;
     
}obj;

void get_user_name(int uid,struct data *obj){
     struct passwd * pwd;
     pwd = getpwuid(uid);
     if(pwd==NULL){
         printf("No user found against %d\n",uid);
         return;
	}
     strcpy(obj[iterator].username,pwd->pw_name);
}
void get_group_name(int gid,struct data *obj){
   struct group * gwd;
   gwd = getgrgid(gid);
   if(gwd == NULL){
      printf("No group found against %d\n",gid); return;
   }
  strcpy(obj[iterator].groupname,gwd->gr_name);
  

}
void set_permission(struct stat buf,struct data *obj){
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
           strcpy(obj[iterator].permission,mode);
        
}

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}
void do_long_listing(char *filename,char *file,struct data *obj){
	
	struct stat info;
        int status = lstat(filename,&info);
	if(status==-1){
	perror("ERROR:");return;
	}
	
	set_permission(info,obj);
        obj[iterator].link=info.st_nlink;
	get_user_name(info.st_uid,obj);
        get_group_name(info.st_gid,obj);
        obj[iterator].size=info.st_size;
	blocksize+=info.st_blocks;
        long sec = info.st_mtime;
	int j=0;
	char *time = ctime(&sec);
	while(time[j++]!='\n');
	time[j-1]=' ';
        strcpy(obj[iterator].t_time,time);
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
int compare (const void * a, const void * b ) {
	
	const struct data *p1 = a;
        const struct data *p2 = b;
	return p1->filename[0]>p2->filename[0];
   
}
void read_directory(char *directory_name){

	int count = get_count(directory_name);
	struct data* obj = malloc(count * sizeof *obj);	
        DIR *dp = opendir(directory_name); // open the directory 
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
          if(entry->d_name[0] !='.' && entry->d_name !=".."){
	     char path[100];
             strcpy(path,directory_name);strcat(path,"/");strcat(path,entry->d_name);
			     
	     obj[iterator].d_type=entry->d_type;
	     if(swtch==1 || swtch==3){   
		   obj[iterator].inode=entry->d_ino;
             }
             if(swtch ==2 || swtch==3){
		 do_long_listing(path,entry->d_name,obj);
             }
             strcpy(obj[iterator].filename,entry->d_name);
		iterator+=1;
	  }
      }
	closedir(dp);
	int size = sizeof(obj) / sizeof(obj[0]); 
        qsort((void*)obj, count, sizeof(obj[0]), compare); 
	printf("total: %ld\n",(blocksize/2));
	for(int i=0;i<iterator;i++){
	     if(swtch==1 || swtch==3){		  
	          printf("%ld\t",obj[i].inode);
             }   //endif
	
	     if(swtch ==2 || swtch==3){
		 printf("%-4s  ",obj[i].permission);	
		 printf("%-2ld\t",obj[i].link);
		  printf("%-4s  ",obj[i].username);
 		 printf("%s\t",obj[i].groupname);
		 printf("%ld\t",obj[i].size);
        	 printf("%s\t",obj[i].t_time);
             } //endif
	
	      
	      if(strcmp("-rwxr-xr-x",obj[i].permission)==0){
			printf("\033[01;32m%s\n \033[0m",obj[i].filename);continue;
		}
	      if(strcmp(get_filename_ext(obj[i].filename),"zip")==0 || strcmp(get_filename_ext(obj[i].filename),"tar")==0 || 			   			strcmp(get_filename_ext(obj[i].filename),"tgz")==0 || 			   						strcmp(get_filename_ext(obj[i].filename),"arc")==0 ||
					strcmp(get_filename_ext(obj[i].filename),"arj")==0 ||
					strcmp(get_filename_ext(obj[i].filename),"taz")==0 ||
					strcmp(get_filename_ext(obj[i].filename),"lha")==0 ||
					strcmp(get_filename_ext(obj[i].filename),"z")==0 ||
					strcmp(get_filename_ext(obj[i].filename),"Z")==0 ||
					strcmp(get_filename_ext(obj[i].filename),"7z")==0 ){
					printf("\033[01;31m %s\n \033[0m",obj[i].filename);
	     }
	   else if(obj[i].d_type == DT_DIR){
		 printf("\033[01;34m%s\n\033[0m",obj[i].filename);
	    } 
		
	   else if(obj[i].d_type==DT_BLK){
		 printf("\033[40;33;01m %s\n \033[0m",obj[i].filename);
	    }

	else if(obj[i].d_type==DT_CHR){
		 printf("\033[40;33;01m %s\033[0m\n",obj[i].filename);
	    }

	else if(obj[i].d_type==DT_FIFO){
			 printf("\033[40;33m%s\033[0m\n",obj[i].filename);
		    }

	else if(obj[i].d_type==DT_LNK){
		 printf("\033[01;36m%s\033[0m\n",obj[i].filename);
		    }
	else if(obj[i].d_type==DT_REG){
			 printf("\033[0;0m%s\033[0m\n",obj[i].filename);
		    }
	else if(obj[i].d_type==DT_SOCK){
		 printf("\033[01;35m%s\033[0m\n",obj[i].filename);
		    }
	}
	//DT_UNKNOWN
	
	

}
void welcome(){
	printf("\n\t\t ---------------------------------\n");
	printf("\t\t This is final version of ls utility\n");
	printf("\n\t\t ---------------------------------\n\n");

}
//-------------------------------------------------------- Main function ------------------------------------------ ('!')
int main(int argc,char *argv[]){

	printf("\e[8;50;100t");
	welcome();
        if(argc==1){
	     // show the contents of present directory in case of no command line arguments 
	     read_directory(".");
	     return 0;	
	}

       char *s = (malloc)(sizeof(char*)*4);
       s="";
       for(int i=1;i<argc;i++){
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
