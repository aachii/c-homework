// author: Julian Haldimann
// date: 16.01.2019
// updated: 16.01.2019

// includes of different libraries
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

// define strings for line read
#define RSS "VmRSS:"
#define PID "Pid:"
#define NAME "Name:"
#define KB " kB"

// print function for output
void printFirstLine(char *pid, char *rss, char *comm) {
    // format output
    printf("%5.5s %-15.15s %5s", pid,comm,rss);
}

// method to remove unused characters in string
void removeSubstr(char *string, char *sub){
    char *match;
    int len = strlen(sub);
    while((match = strstr(string, sub))){
        *match = '\0';
        strcat(string, match+len);
    }
}

void getProcess(char *processId) {
    // define strings
    char pid[10];
    char rss[256];
    char comm[256];
    char uid[256];
    
    // prepare file and filename to read
    FILE *file;
    char filename[sizeof("/proc/") + sizeof(processId) + sizeof("/status")];

    // build pathname to the status file
    strcpy(filename, "/proc/");
    strcat(filename, processId);
    strcat(filename, "/status");

    // set pid 
    strcpy(pid,processId);

    // open file and save the content in file
    file = fopen(filename,"r");

    // define line string and set capacity
    char line[256];

    // read the file line by line
    while(fgets(line, sizeof(line), file) != NULL){  
        // check if there is a line with the RSS keyword 
        if(strncmp(RSS, line, strlen(RSS)) == 0) {        
            strcpy(rss, line);
            removeSubstr(rss, "VmRSS:\t");
            removeSubstr(rss, KB);
        // check for a line with the name keyword (get the name of the process)
        } else if(strncmp(NAME, line, strlen(NAME)) == 0) {
            strcpy(comm, line);
            removeSubstr(comm, "Name:\t");
            removeSubstr(comm, "\n");
        // check for a line with the uid (get user process)
        } else if(strncmp("Uid:", line, strlen("Uid:")) == 0) {
            strcpy(uid, line);
            removeSubstr(uid, "Uid:\t");
        }
    }
    // only print the processes for your userid 
    if(getuid() == atoi(uid)){
        printFirstLine(pid, rss, comm);
    }
}

// method to get all folders in the /proc folder 
int getAllDirectory() {

    DIR *dr = opendir("/proc/");
    struct dirent *de;
    char procs = 0;

    // loop over all folder and call the getProcess with the pid
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
    // close the directory
    closedir(dr);     
    return 0;
}

int main(){
    getAllDirectory();
    return 0;
}