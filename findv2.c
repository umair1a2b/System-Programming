//-------------------------------------------------------- HEADER SECTION ------------------------------------- (^.^)
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <string.h> // string manipulations 


extern int errno;


void do_find(char *path,char type){
	struct dirent *entry;
	DIR *dp = opendir(path);
	if(dp==NULL){
		perror("Directory :"); exit(0);

	}
	char array[1000];
	while((entry=readdir(dp))!=NULL){

		if(entry==NULL && errno!=0){

			printf("Something is wrong!\n");exit(0);
		}

		
		 for(int i=0;i<1000;i++){
	
			array[i]='\0';
		    
	            }  //endfor */
	
		strncpy(array,path,strlen(path));  // copy the original path say -> t/t1
		

		if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0){


		if(entry->d_type==DT_DIR && type=='d'){
			printf("%s/%s \n",path,entry->d_name); 
		}		
		else if(entry->d_type==DT_BLK && type=='b'){
		
			printf("%s/%s \n",path,entry->d_name); 
	
		}
		else if(entry->d_type==DT_CHR && type=='c'){
		
			printf("%s/%s \n",path,entry->d_name); 
		}
		else if(entry->d_type==DT_FIFO && type=='p'){
			printf("%s/%s \n",path,entry->d_name); 

		}
		else if(entry->d_type==DT_LNK && type=='l'){
			printf("%s/%s \n",path,entry->d_name); 

		}

		else if(entry->d_type==DT_REG && type=='f'){
			printf("%s/%s \n",path,entry->d_name); 

		}
		else if(entry->d_type==DT_SOCK && type == 's'){

			printf("%s/%s \n",path,entry->d_name); 
		}
		else {
			//printf("%s/%s \n",path,entry->d_name); 

		}



			
			if(entry->d_type == DT_DIR){						

			    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0){
					int j=0;
			 		while(array[j]!='\0'){j++;}   array[j]='/';  //     t/t1/t3
						for(int k=0;k<strlen(entry->d_name);k++){
							array[++j]=entry->d_name[k];
			
						}
						//printf("%s\n",array);
						do_find(array,type);
                           } // endif 
		    


		} //endif

        } //endif
   }        //endwhile

	closedir(dp);
}
void welcome(){
	printf("\n\t\t ---------------------------------\n");
	printf("\t\t This is version 2 of find utility\n");
	printf("\n\t\t ---------------------------------\n\n");

}
int main(int argc,char *argv[]){

	
	
	welcome();
	if(argc<4){
		
		printf("Please follow the exact command pattern \n [find] [path] [-type] [f, d, b, c, p ,s ] \n");
		exit(0);
	}
	
	do_find(argv[2],argv[4][0]);


}
