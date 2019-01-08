
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>


#define RSS "VmRSS:"
#define PID "PID:"
#define NAME "Name:"

void printFirstLine(char *pid, char *rss, char *uid) {
    printf("%s %s %s", pid,rss,uid);
}
void getProcess(char *name) {
    printf("%s",name);
    char pid[256];
    char rss[256];
    char uid[256];
    
    FILE *file;
    char filename[sizeof("/proc") + sizeof(name) + sizeof("/status")];
    
    strcpy(filename, "/proc");
    strcat(filename, name);
    strcpy(filename, "/status");
    strcpy(rss,"0"); // set rss, if its not found in status file
    file = fopen(filename,"r");
    for(;;){
        char line[256];
        if(1 != fscanf(file,"%255s",line)){
            break;
        }
        if(strcmp(PID,line) == 0) {
            fscanf(file, "%255s", pid);
        } else if(strcmp(RSS, line) == 0) {
            fscanf(file, "%255s", rss);
        } else if(strcmp(NAME, line) == 0) {
            fscanf(file, "%255s", uid);
            fscanf(file, "%255s", uid);
            int tmp_uid = atoi(uid);
            if(tmp_uid == getuid()){
                printFirstLine(pid, rss, uid);
            }
        }
    }
}

int getAllDirectory() {
    DIR *dr = opendir("/proc/");
    struct dirent *de;
    char procs = 0;
  
    if (dr == NULL) { 
        printf("Could not open current directory" ); 
        return 1; 
    } 
  
    while((de = readdir(dr)) != NULL){
        if(procs){
            printf("%s",de -> d_name);
            getProcess(de -> d_name);
        }else{
            if(strcmp(de ->d_name,"1") == 0) {
                printf("%s",de -> d_name);
                procs = 1;
                getProcess(de -> d_name);
            }
        }
    }  
  
    closedir(dr);     
    return 0;
}

int main(){
    getAllDirectory();
    return 0;
}


