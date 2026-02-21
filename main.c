#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>



int main () {
    
    DIR *d = opendir("/proc");
    if(!d) {
        perror("file:");
        return 1;
    }

    struct dirent *a, *c;
    int task = 0;
    int sleeping = 0;
    int runing = 0;
    char path[6024] = "/proc/";


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
                    printf("Proc : %s | pid : %s\n", name, pid);
                }
             

            }
            

        break;
       }


    }

printf("\nTotal task : %d  | running : %d | sleeping : %d\n", task, runing, task - runing);

    return 0;
}