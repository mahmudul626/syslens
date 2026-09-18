#include "../include/main.h"


void proc(struct comp_info *buf, struct comp_info *plist, int *index) {
    
    DIR *d = opendir("/proc");
    if(!d) {
        perror("dir:");
        return;
    }

    struct dirent *a;

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
                char user[64];
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
                    } else if (strncmp(buffer, "Uid:", 4) == 0)
                    {
                        char uid[24];
			sscanf(buffer+5, "%s", uid);

			FILE *p_user = fopen("/etc/passwd", "r");
			if (!p_user) continue;

			char etc_buffer[512];
			while(fgets(etc_buffer, sizeof(etc_buffer), p_user) != NULL) {
				if(strstr(etc_buffer, uid) != NULL) {
					char *ptr = strchr(etc_buffer, ':');
					int index = ptr - etc_buffer;
					snprintf(user, sizeof(user), "%.*s\n", index, etc_buffer);
					user[strcspn(user, "\n")] = '\0';
					break;
				}
			}
			fclose(p_user);
			found ++;
                    } else if (strncmp(buffer, "VmRSS:", 6) ==  0)
                    {
                        sscanf(buffer, "VmRSS:  %d", &vmrss);
                        found ++;
                    }
                    
                    
                     
                    if(found == 5) {
			    plist[pindex].proc_attr.name = strdup(proc_name);
			    plist[pindex].proc_attr.state = strdup(state);
			    plist[pindex].proc_attr.pid = strdup(pid);
			    plist[pindex].proc_attr.user = strdup(user);
			    plist[pindex].proc_attr.vmrss = vmrss;
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

    asprintf(&buf->proc_attr.tasks, "%d, %d R, %d S, %d Z", task, running, sleeping, zombie);


    for (int i = 0; i < pindex-1; i++)
    {
        for (int j = i+1; j < pindex; j++)
        {
            if (plist[i].proc_attr.vmrss < plist[j].proc_attr.vmrss)
            {
                struct proc_info temp = plist[i].proc_attr;
                plist[i].proc_attr = plist[j].proc_attr;
                plist[j].proc_attr = temp;

            }
            
        }
        
    }

    *index = pindex;
}
