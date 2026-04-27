#include "../include/main.h"

typedef struct {
    char name[512];
    char state[64];
    char pid[64];
    char ppid[64];
    int vmrss;
}process;


void proc() {
    
    DIR *d = opendir("/proc");
    if(!d) {
        perror("dir:");
        return;
    }

    struct dirent *a;

    process plist[4096];
    int pindex = 0;

    int task = 0;
    int running = 0;
    int sleeping = 0;
    int zombie = 0;

    while ((a = readdir(d)) != NULL)
    {
       if(isdigit(a->d_name[0])) {

                char path[512];
                snprintf(path, sizeof(path), "/proc/%s/status", a->d_name);

                FILE *file = fopen(path, "r");
                if (!file)
                {
                    continue;
                }

                char buffer[256];

                char proc_name[512];
                char pid[64];
                char ppid[64];
                char state[64] = {0};
                int vmrss = 0;
                int found = 0;

                while (fgets(buffer, sizeof(buffer), file) != NULL)
                {
                    if (strncmp(buffer, "Name:", 5) == 0)
                    {
                        strncpy(proc_name, buffer+6, sizeof(proc_name) - 1);
                        proc_name[sizeof(proc_name) - 1] = '\0';
                        proc_name[strcspn(proc_name, "\n")] = 0;
                        found ++;
                    } else if (strncmp(buffer, "State:", 6) == 0)
                    {
                        strncpy(state, buffer+7, sizeof(state) - 1);
                        state[sizeof(state) - 1] = '\0';
                        state[strcspn(state, "\n")] = 0;
                        found ++;
                    } else if (strncmp(buffer, "Pid:", 4) == 0)
                    {
                        strncpy(pid, buffer+5, sizeof(pid) - 1);
                        pid[sizeof(pid) - 1] = '\0';
                        pid[strcspn(pid, "\n")] = 0;
                        found ++;
                    } else if (strncmp(buffer, "PPid:", 5) == 0)
                    {
                        strncpy(ppid, buffer+6, sizeof(ppid) - 1);
                        ppid[sizeof(ppid) - 1] = '\0';
                        ppid[strcspn(ppid, "\n")] = 0;
                        found ++;
                    } else if (strncmp(buffer, "VmRSS:", 6) ==  0)
                    {
                        sscanf(buffer, "VmRSS:  %d", &vmrss);
                        found ++;
                    }
                    
                    
                     
                    if(found == 5) {
                        strncpy(plist[pindex].name, proc_name, sizeof(plist[pindex].name));
                        strncpy(plist[pindex].state, state, sizeof(plist[pindex].state));
                        strncpy(plist[pindex].pid, pid, sizeof(plist[pindex].pid));
                        strncpy(plist[pindex].ppid, ppid, sizeof(plist[pindex].ppid));
                        plist[pindex].vmrss = vmrss;
                        pindex++;
                        break;
                    }
                }

                if (strncmp(state, "R (running)", 11) == 0)
                {
                    running++;
                } else if (strncmp(state, "S (sleeping)", 12) == 0 || strncmp(state, "I (idle)", 8) == 0)
                {
                    sleeping++;
                } else if (strncmp(state, "Z (zombie)", 10) == 0)
                {
                    zombie++;
                }
                

             fclose(file);

       }


    }
    closedir(d);
    
    task = running+sleeping+zombie;

    printf("Task        "RED":"RESET" %d total  | %d running | %d sleeping | %d zombie\n", task, running, sleeping, zombie);


    for (int i = 0; i < pindex-1; i++)
    {
        for (int j = i+1; j < pindex; j++)
        {
            if (plist[i].vmrss < plist[j].vmrss)
            {
                process temp = plist[i];
                plist[i] = plist[j];
                plist[j] = temp;

            }
            
        }
        
    }

    printf("\n-------------------------------------------------------------------------\n");
    printf("%-10s %-10s %-15s %-10s %-10s %-10s\n", "PID", "PPID", "STATUS", "RES", "%MEM", "COMMAND");
    printf("-------------------------------------------------------------------------\n");

    for(int i = 0; i < 5; i++)
    {
        if (strncmp(plist[i].state, "Z (zombie)", 10) == 0) {

            float pmem = ((float)plist[i].vmrss / totalram) * 100;

            printf(RED"%-10s %-10s %-15s %-10d %-10.1f %-10s\n"RESET,
                plist[i].pid,
                plist[i].ppid,
                plist[i].state,
                plist[i].vmrss,
                pmem,
                plist[i].name);
        } else {

            float pmem = ((float)plist[i].vmrss / totalram) * 100;

            printf("%-10s %-10s %-15s %-10d %-10.1f %-10s\n",
                plist[i].pid,
                plist[i].ppid,
                plist[i].state,
                plist[i].vmrss,
                pmem,
                plist[i].name);
        }
    }
}
