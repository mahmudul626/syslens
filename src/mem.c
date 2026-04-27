#include "../include/main.h"
#include <sys/statvfs.h>

unsigned long totalram = 1;

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
    totalram = mem;
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


    /*----------disk-------------*/
    
        struct statvfs vfs;

        if (statvfs("/", &vfs) == 0) {
            unsigned long long block_size = vfs.f_frsize;
            
            unsigned long long total = vfs.f_blocks * block_size;
            
            unsigned long long free = vfs.f_bfree * block_size;
            
            unsigned long long used = total - free;

            double disktotal_gib = (double)total / (1024 * 1024 * 1024);
            double diskused_gib = (double)used / (1024 * 1024 * 1024);
            int diskpercentage = (used * 100) / total;

            char *diskcolor = GREEN;

            if (diskpercentage >= 80) {
                diskcolor = RED;
            } else if (diskpercentage >= 50) {
                diskcolor = YELLOW;
            } else {
                diskcolor = GREEN;
            }

            int diskbar = (diskpercentage * MAX_BAR) / 100;
            printf("Disk        "RED":"RESET" ");
            print_bar(diskbar, diskcolor);
            printf("%s %d%%"RESET" ",diskcolor, diskpercentage);
            printf("(%.1fGiB/%.1fGiB)\n", diskused_gib, disktotal_gib);
        }
 
}



void load_avg() {
    FILE *load = fopen("/proc/loadavg", "r");
    if (!load)
    {
        return;
    }

    char buffur[1014];
    fgets(buffur, sizeof(buffur), load);

    char str1[100], str2[100], str3[100];
    sscanf(buffur, "%s %s %s", str1, str2, str3);

    printf("Load avg    "RED":"RESET" %s, %s, %s\n", str1, str2,str3);

    fclose(load);
}

void cpu_per() {
    FILE *file = fopen("/proc/stat", "r");
    if(!file) return;

    char buffer[512];
    fgets(buffer, sizeof(buffer), file);

    int user,nice,system,idle,iowait,irq,softirq;

    sscanf(buffer, "cpu %d %d %d %d %d %d %d", &user, &nice, &system, &idle, &iowait, &irq, &softirq);

    int total_1st = user+nice+system+idle+iowait+irq+softirq;
    int idle_1st = idle+iowait;
    fclose(file);

    usleep(100000);

    FILE *file1 = fopen("/proc/stat", "r");
    if(!file1) return;

    char buffer1[512];
    fgets(buffer1, sizeof(buffer1), file1);

    int user1,nice1,system1,idle1,iowait1,irq1,softirq1;

    sscanf(buffer1, "cpu %d %d %d %d %d %d %d", &user1, &nice1, &system1, &idle1, &iowait1, &irq1, &softirq1);

    int total_2nd = user1+nice1+system1+idle1+iowait1+irq1+softirq1;
    int idle_2nd = idle1+iowait1;
    fclose(file1);

    int delta_total = total_2nd - total_1st;
    int delta_idle = idle_2nd - idle_1st;

    float cpu = (1.0 - (float)delta_idle / delta_total) * 100;
    int cpubar = (cpu * MAX_BAR) / 100;

    char *cpucolor;
    if (cpu < 50)
    {
        cpucolor = GREEN;
    } else if (cpu < 90)
    {
        cpucolor = YELLOW;
    } else {
        cpucolor = RED;
    }

    printf("CPU         "RED":"RESET" ");
    print_bar(cpubar, cpucolor);
    printf(" %s%.0f%%"RESET"\n", cpucolor, cpu);
}