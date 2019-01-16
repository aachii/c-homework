
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define RSS "VmRSS:"
#define PID "Pid:"
#define NAME "Name:"
#define KB " kB"

void printFirstLine(char *pid, char *rss, char *comm) {
    printf("%5.5s %-15.15s %5s", pid,comm,rss);
}

void removeSubstr(char *string, char *sub){
    char *match;
    int len = strlen(sub);
    while((match = strstr(string, sub))){
        *match = '\0';
        strcat(string, match+len);
    }
}

void getProcess(char *processId) {
    char pid[10];
    char rss[256];
    char comm[256];
    char uid[256];
    
    FILE *file;
    char filename[sizeof("/proc/") + sizeof(processId) + sizeof("/status")];

    strcpy(filename, "/proc/");
    strcat(filename, processId);
    strcat(filename, "/status");
    strcpy(pid,processId);

    file = fopen(filename,"r");

    char line[256];

    while(fgets(line, sizeof(line), file) != NULL){  
        if(strncmp(RSS, line, strlen(RSS)) == 0) {        
            strcpy(rss, line);
            removeSubstr(rss, "VmRSS:\t");
            removeSubstr(rss, KB);
        } else if(strncmp(NAME, line, strlen(NAME)) == 0) {
            strcpy(comm, line);
            removeSubstr(comm, "Name:\t");
            removeSubstr(comm, "\n"); 
        } else if(strncmp("Uid:", line, strlen("Uid:")) == 0) {
            strcpy(uid, line);
            removeSubstr(uid, "Uid:\t");
        }
    }
    if(getuid() == atoi(uid)){
        printFirstLine(pid, rss, comm);
    }
}

int getAllDirectory() {
    DIR *dr = opendir("/proc/");
    struct dirent *de;
    char procs = 0;
  
    if (dr == NULL) { 
        printf("Could not open current directory"); 
        return 1; 
    } 

    
    //printf("%5.5s %-15.15s %5s\n", "PID","COMMAND","RSS");
  
    while((de = readdir(dr)) != NULL){
        if(procs){
            getProcess(de -> d_name);
        }else{
            if(strcmp(de ->d_name,"1") == 0) {
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


