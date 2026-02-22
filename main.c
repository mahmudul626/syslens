#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void meminfo() {
    FILE *file = fopen("/proc/meminfo", "r");
    if (!file) return;

    char buffer[2034];
    double memtotal = 0, memfree = 0, membuffer = 0, cached = 0, used = 0, memavailable = 0, swaptotal = 0, swapfree =0;
    int found = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        if (strncmp(buffer, "MemTotal:", 9) == 0)
        {
            sscanf(buffer, "MemTotal: %le", &memtotal);
            found ++;
        }

        if (strncmp(buffer, "MemFree:", 8) == 0)
        {
            sscanf(buffer, "MemFree: %le", &memfree);
            found++;
        }

        if(strncmp(buffer, "MemAvailable:", 13) == 0) {
            sscanf(buffer, "MemAvailable: %le", &memavailable);
            found++;
        }

        if (strncmp(buffer, "Buffers:", 8) == 0)
        {
            sscanf(buffer, "Buffers: %le", &membuffer);
            found++;
        }

        if (strncmp(buffer, "Cached:", 7) == 0)
        {
            sscanf(buffer, "Cached: %le", &cached);
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
        

        
        if(found == 7) break;
    }

    double cachandbuf = membuffer+cached;
    used = memtotal-(cachandbuf+memfree);

    printf("Mem total : %.1f MiB    |   free : %.1f MiB    |   used : %.1f    |    cached\\buffer : %.1f\n", memtotal/1024, memfree/1024,  used/1024.0, cachandbuf/1024.0);
    printf("Swap total : %.1f MiB   |   free : %.1f MiB    |   used : %.1f    |    mem avl : %.1f\n", swaptotal/1024, swapfree/1024, 0.0, memavailable/1024);
}

int main () {
    
    DIR *d = opendir("/proc");
    if(!d) {
        perror("file:");
        return 1;
    }

    struct dirent *a, *c;
    int task = 0;
    int runing = 0;
    char path[6024] = "/proc/";


    printf("Process         Pid\n");
    printf("=====================\n");

    while ((a = readdir(d)) != NULL)
    {
       if(strcmp(a->d_name, "1") == 0) {

            while ((c = readdir(d)) != NULL)
            {
                char path[6024] = "/proc/";
                task++;
                strcat(path, c->d_name);
                strcat(path, "/status");

                FILE *file = fopen(path, "r");
                if (!file)
                {
                    perror("file:");
                    return 1;
                }

                char buffer[2024];
                char state[1012];
                char name[1013];
                char pid[1010];
                int found = 0;

                while (fgets(buffer, sizeof(buffer), file) != NULL)
                {
                    if (strncmp(buffer, "Name:", 5) == 0)
                    {
                        strcpy(name, buffer+6);
                        name[strcspn(name, "\n")] = 0;
                        found++;
                    } 
                    
                    if (strncmp(buffer, "State:", 6) == 0)
                    {
                        strcpy(state, buffer+7);
                        state[strcspn(state, "\n")] = 0;
                        found ++;
                    }

                    if (strncmp(buffer, "Pid:", 4) == 0)
                    {
                        strcpy(pid, buffer+5);
                        pid[strcspn(pid, "\n")] = 0;
                        found ++;
                    }
                    
                    if(found == 3) break;
                    
                }

                if (strncmp(state, "R (running)", 11) == 0)
                {
                    runing++;
                    printf("%s              %s\n", name, pid);
                }
             fclose(file);

            }
            
        break;
       }


    }
    closedir(d);

    FILE *load = fopen("/proc/loadavg", "r");
    if (!load)
    {
        perror("load file");
        return 1;
    }

    char buffur[1014];
    fgets(buffur, sizeof(buffur), load);

    char str1[100], str2[100], str3[100];
    sscanf(buffur, "%s %s %s", str1, str2, str3);

    printf("\nLoad avg : %s, %s, %s\n", str1, str2,str3);

    fclose(load);
    

    printf("Total task : %d  | running : %d | sleeping : %d\n", task, runing, task - runing);

    meminfo();

    return 0;
}
