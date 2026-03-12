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
        printf("Uptime      "RED":"RESET" %d hours %d mins| ", hour, minute);
    } else {
        printf("Uptime      "RED":"RESET" %d mins      | ", minute);
    }


    fclose(file);
}


void power() {
    FILE *file = fopen("/sys/class/power_supply/BAT0/capacity", "r");
    if (!file) return;

    char capacity[24];
    fscanf(file, "%s", capacity);
    fclose(file);

    FILE *file1 = fopen("/sys/class/power_supply/BAT0/status", "r");
    if (!file1) return;

    char status[64];
    fscanf(file1, "%s", status);
    fclose(file1);

    FILE *file2 = fopen("/sys/class/power_supply/BAT0/technology", "r");
    if (!file2) return;

    char technology[64];
    fscanf(file2, "%s", technology);
    fclose(file2);

    FILE *file3 = fopen("/sys/class/power_supply/BAT0/type", "r");
    if (!file3) return;

    char type[64];
    fscanf(file3, "%s", type);
    fclose(file3);

    FILE *file4 = fopen("/sys/class/power_supply/BAT0/model_name", "r");
    if (!file4) return;

    char model_name[64];
    fscanf(file4, "%s", model_name);
    fclose(file4);

    FILE *file5 = fopen("/sys/class/power_supply/BAT0/serial_number", "r");
    if (!file5) return;

    char serial_number[64];
    fgets(serial_number, sizeof(serial_number), file5);
    serial_number[strcspn(serial_number, "\n")] = 0;
    fclose(file5);

    FILE *file6 = fopen("/sys/class/power_supply/BAT0/manufacturer", "r");
    if (!file6) return;

    char manufacturer[64];
    fscanf(file6, "%s", manufacturer);
    fclose(file6);


    printf("Power       "RED":"RESET" %s%% (%s) %s %s\n", capacity, status, technology, type);
    printf("Model       "RED":"RESET" %s %s %s\n", model_name, serial_number, manufacturer);
    
}