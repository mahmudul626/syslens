#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 2026

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define RESET   "\033[0m"

void user() {
    char *ptr;
    ptr = getenv("USER");
    if (ptr != NULL) {
        printf("User        "RED":"RESET" %s\n", ptr);
    }
}

void kernel() {
    struct utsname sys;
    uname(&sys);
    printf("Kernel      "RED":"RESET" %s\n", sys.release);
}

void getos() {
    char buffer[BUFFER_SIZE];
    int name_found = 0, version_found = 0;
    
    FILE *file = fopen("/etc/os-release", "r");
    if(!file) return;

    char name[1024] = "unknown";
    char version[1024] = "0";

    while (fgets(buffer, sizeof(buffer), file))
    {
        if (strncmp(buffer, "NAME=", 5) == 0)
        {
            int offset = 5;
            if (buffer[5] == '"')
            {
                offset = 6;
            }
            
            strcpy(name, buffer+offset);
            name[strcspn(name, "\"\n")] = 0;
            name_found = 1;
        }
        
        if (strncmp(buffer, "VERSION_ID=", 11) == 0)
        {
            int offset = 11;
            if (buffer[11] == '"')
            {
                offset = 12;
            }

            strcpy(version, buffer+offset);
            version[strcspn(version, "\"\n")] = 0;
            version_found = 1;
        } 
        
        if(name_found == 1 && version_found == 1) break;
    }

    printf("OS          "RED":"RESET" %s %s\n", name, version);
    fclose(file);
}

void cpu() {
    char buffer[BUFFER_SIZE];
    char terget[] = "model name";
    FILE *file = fopen("/proc/cpuinfo", "r");
    if(!file) printf("failed to open file\n");

    while (fgets(buffer, sizeof(buffer), file))
    {
        if (strstr(buffer, terget))
        {

            char *ptr = strchr(buffer, ':');
            if (ptr != NULL) {
                ptr++; 
                while (*ptr == ' ') ptr++;
                printf("CPU         "RED":"RESET" %s", ptr);
            }
            break;
        }
        
    }

    fclose(file);
}

void mem() {
    FILE *file = fopen("/proc/meminfo", "r");
    if (!file) printf("failed to open file\n");
    char buffer[BUFFER_SIZE];
    char usemem[BUFFER_SIZE];

    fgets(buffer, sizeof(buffer), file);
    buffer[strcspn(buffer, "\n")] = 0;

    int availablemem;
    while (fgets(usemem, sizeof(usemem), file))
    {
        if (strstr (usemem, "MemAvailable"))
        {
            sscanf(usemem, "MemAvailable:    %d kB", &availablemem);
            break;
        }
    }
    

    int mem;
    sscanf(buffer, "%*s %d", &mem);

    int totalgb = (mem/1000) / 1000;

    int used = mem - availablemem;

    float usedgb = (used/1000.0) / 1000.0;

    int percent = (usedgb/(float)totalgb) * 100;

    char *color = GREEN;

    if (percent >= 80) {
        color = RED;
    } else if (percent >= 50) {
        color = YELLOW;
    } else {
        color = GREEN;
    }
    
    printf("RAM         "RED":"RESET" %.1fGB/%dGB %s(%d%%)"RESET"\n",usedgb, totalgb, color, percent);


    fclose(file);
}


void uptime() {
    FILE *file = fopen("/proc/uptime", "r");
    if (!file) printf("failed to open file\n");

    char buffer[BUFFER_SIZE];
    fscanf(file, "%s", buffer);
    float second = atof(buffer);
    
    float totalmin = second / 60;
    int hour = totalmin / 60;
    float gap = hour * 60;
    int minute = totalmin - gap;

    if(hour > 0) {
        printf("Uptime      "RED":"RESET" %d hours %d mins\n", hour, minute);
    } else {
        printf("Uptime      "RED":"RESET" %d mins\n", minute);
    }


    fclose(file);
}

int main() {
    printf(BLUE "=========== " RESET);
    printf(CYAN "System Lens" RESET);
    printf(BLUE " ===========\n" RESET);

        getos();
        kernel();
        cpu();
        mem();
        uptime();
        user();

    printf(BLUE"===================================\n"RESET);
    return 0;
}