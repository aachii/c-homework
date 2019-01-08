
#include <stdio.h>
#include <dirent.h>

#define RSS "VmRSS:"
#define PID "PID:"
#define NAME "Name:"

void printFirstLine() {
    printf("%s %s %s", "PID","COMMAND","RSS");
}

int getAllDirectory() {
    struct dirent *de; 
    DIR *dr = opendir("/proc/"); 
  
    if (dr == NULL) { 
        printf("Could not open current directory" ); 
        return 1; 
    } 
  
    while ((de = readdir(dr)) != NULL) 
            printf("%s\n", de->d_name); 
  
    closedir(dr);     
    return 0;
}

void getProcess() {
    printFirstLine();
    char pid[256];
    char command[256];
    char rss[256];
}


int main(){
    getAllDirectory();
    getProcess();
    return 0;
}


