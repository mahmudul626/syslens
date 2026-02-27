#include "../include/main.h"

#define BUFFER_SIZE 2026
#define MAX_BAR 10


void print_bar(int length, const char *color) {

    if (length < 0)
        length = 0;
    if (length > MAX_BAR)
        length = MAX_BAR;

    printf("%s[", color);

    for (int i = 0; i < length; i++) {
        printf("#");
    }

    for (int i = length; i < MAX_BAR; i++) {
        printf(".");
    }

    printf("]%s", RESET);
}



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

void mem() {

    FILE *file = fopen("/proc/meminfo", "r");
    if (!file) return;

    char buffer[BUFFER_SIZE];
    unsigned long availablemem = 0, mem = 0;
    double swaptotal = 0, swapfree = 0;
    int found = 0;


    while (fgets(buffer, sizeof(buffer), file))
    {
        if (strncmp(buffer, "MemTotal:", 9) == 0)
        {
            sscanf(buffer, "%*s %ld", &mem);
            found++;
        }
        
        if (strncmp(buffer, "MemAvailable:", 13) == 0)
        {
            sscanf(buffer, "%*s %ld", &availablemem);
            found++;
        }

        if (strncmp(buffer, "SwapTotal:", 10) == 0)
        {
            sscanf(buffer, "SwapTotal: %le", &swaptotal);
            found++;
        }

        if (strncmp(buffer, "SwapFree:", 9) == 0)
        {
            sscanf(buffer, "SwapFree: %le", &swapfree);
            found++;
        }
        
        if(found == 4) break;
    }
    
    fclose(file);

    /*---------Ram section-----------*/
    
    double totalgb = (double)mem / (1024.0 * 1024.0);

    double used = (double)mem - availablemem;

    double usedgb = used/(1024.0 * 1024.0);
    
    int percent = (mem>0) ? (int)((used/(double)mem) * 100) : 0;


    char *color = GREEN;

    if (percent >= 80) {
        color = RED;
    } else if (percent >= 50) {
        color = YELLOW;
    } else {
        color = GREEN;
    }
    
    printf("RAM         "RED":"RESET" ");
    int bar = (percent * MAX_BAR) / 100;
    print_bar(bar, color);
    printf("%s %d%%"RESET" ",color, percent);
    printf("(%.1fGiB/%.1fGiB) \n",usedgb, totalgb);


    /*---------swap section----------*/

    double swaptotalgb = swaptotal/(1024.0 * 1024.0);
    double swapused = (swaptotal-swapfree);
    double swapusedgb = swapused/(1024.0 * 1024.0);
    int swappersent = (swaptotal>0) ? (int)(swapused* 100.0) / swaptotal : 0;


    char *swapcolor = GREEN;

    if (swappersent >= 80) {
        swapcolor = RED;
    } else if (swappersent >= 50) {
        swapcolor = YELLOW;
    } else {
        swapcolor = GREEN;
    }

    int swapbar = (swappersent * MAX_BAR) / 100;
    printf("Swap        "RED":"RESET" ");
    print_bar(swapbar, swapcolor);
    printf("%s %d%%"RESET" ",swapcolor, swappersent);
    printf("(%.1fGiB/%.1fGiB)\n", swapusedgb, swaptotalgb);

}

void load_avg() {
    FILE *load = fopen("/proc/loadavg", "r");
    if (!load)
    {
        perror("load file");
        return;
    }

    char buffur[1014];
    fgets(buffur, sizeof(buffur), load);

    char str1[100], str2[100], str3[100];
    sscanf(buffur, "%s %s %s", str1, str2, str3);

    printf("Load avg    "RED":"RESET" %s, %s, %s\n", str1, str2,str3);

    fclose(load);
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

int main() {
    printf(BLUE "=========== " RESET);
    printf(CYAN "SysLens v1.0" RESET);
    printf(BLUE " ===========\n" RESET);
    printf("[ System Info ]\n");

        getos();
        kernel();
        cpu();
        uptime();
        user();

        printf("\n");

    printf("[ Resource Usage ]\n");

        mem();
        load_avg();

        printf("\n");

    printf("[ Active Processes ]\n");

    proc();

    printf(BLUE"===================================\n"RESET);
    return 0;
}