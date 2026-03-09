#include "../include/main.h"

void proc() {
    
    DIR *d = opendir("/proc");
    if(!d) {
        perror("dir:");
        return;
    }

    struct dirent *a;
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
                char state[64] = {0};

                while (fgets(buffer, sizeof(buffer), file) != NULL)
                {
                    if (strncmp(buffer, "State:", 6) == 0)
                    {
                        strncpy(state, buffer+7, sizeof(state) - 1);
                        state[sizeof(state) - 1] = '\0';
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


}
