#include "../include/main.h"

void user() {
    char *ptr;
    ptr = getenv("USER");
    if (ptr != NULL) {
        printf("User "RED":"RESET" %s\n", ptr);
    }
}

void kernel() {
    struct utsname sys;
    uname(&sys);
    printf("Kernel "RED":"RESET" %s\n", sys.release);
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

    printf("OS          "RED":"RESET" %s %s | ", name, version);
    fclose(file);
}

void cpu() {
    char buffer[BUFFER_SIZE];
    char terget[] = "model name";
    FILE *file = fopen("/proc/cpuinfo", "r");
    if(!file) return;

    while (fgets(buffer, sizeof(buffer), file))
    {
        if (strstr(buffer, terget))
        {

            char *ptr = strchr(buffer, ':');
            if (ptr != NULL) {
                ptr++; 
                while (*ptr == ' ') ptr++;
                ptr[strcspn(ptr, "\r\n")] = 0;
                printf("CPU         "RED":"RESET" %s\n", ptr);
            }
            break;
        }
        
    }

    fclose(file);
}




void uptime() {
    FILE *file = fopen("/proc/uptime", "r");
    if (!file) return;

    char buffer[BUFFER_SIZE];
    fscanf(file, "%s", buffer);
    float second = atof(buffer);
    
    float totalmin = second / 60;
    int hour = totalmin / 60;
    float gap = hour * 60;
    int minute = totalmin - gap;

    if(hour > 0) {
        printf("Uptime      "RED":"RESET" %d hours %d mins      | ", hour, minute);
    } else {
        printf("Uptime      "RED":"RESET" %d mins      | ", minute);
    }


    fclose(file);
}