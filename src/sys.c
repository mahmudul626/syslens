#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include "../include/main.h"
#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 64
#endif

void get_username(struct comp_info *buf)
{
	char hostname[HOST_NAME_MAX + 1] = "unknown";
	char *user = getenv("USER");
	if (!buf || !user)
		return;

	FILE *file = fopen("/etc/hostname", "r");
        if (file) {
		if (fgets(hostname, sizeof(hostname), file))
			hostname[strcspn(hostname, "\n")] = '\0';
		fclose(file);
	}
	asprintf(&buf->sys_attr.user, "%s@%s", user, hostname);
}

void kernel(struct comp_info *buf)
{
	struct utsname sys;

	if (!buf || uname(&sys) != 0) 
		return;

	buf->sys_attr.kernel = strdup(sys.release);
	if (buf->sys_attr.kernel == NULL) 
		buf->sys_attr.kernel = strdup("unknown");
}

void getos(struct comp_info *buf)
{
	if (!buf)
		return;

	char buffer[OS_NAME_MAX];
	char name[32] = "", version[32] = "", final_name[256] = "Linux";

    	FILE *file = fopen("/etc/os-release", "r");
    	if (!file)
		return;

	while (fgets(buffer, sizeof(buffer), file)) {
        	if (strncmp(buffer, "NAME=", 5) == 0) {
            		char *str_n = buffer + 5;
			if (*str_n == '"')
				str_n++;

			size_t len_n = strcspn(str_n, "\"\n");
			snprintf(name, sizeof(name), "%.*s", (int)len_n, str_n);
        	}

		if (strncmp(buffer, "VERSION_ID=", 11) == 0) {
                        char *str_v = buffer + 11;
                        if (*str_v == '"')
                                str_v++;

                        size_t len_v = strcspn(str_v, "\"\n");
                        snprintf(version, sizeof(version), "%.*s", (int)len_v, str_v);
                }

		if (name[0] != '\0' && version[0] != '\0')
			break;
	}
	fclose(file);

	if (name[0] != '\0') {
		if (version[0] != '\0')
			snprintf(final_name, sizeof(final_name), "%s %s", name, version);
		else
			snprintf(final_name, sizeof(final_name), "%s", name);
	}
    	buf->sys_attr.os_name = strdup(final_name);
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




void uptime(struct comp_info *buf)
{
	if (!buf)
		return;

    	FILE *file = fopen("/proc/uptime", "r");
    	if (!file)
		return;

    	char buffer[BUFFER_SIZE];
    	fscanf(file, "%s", buffer);
    	float second = atof(buffer);
    
    	float totalmin = second / 60;
    	int hour = totalmin / 60;
    	float gap = hour * 60;
    	int minute = totalmin - gap;

    	if (hour > 0)
		asprintf(&buf->sys_attr.uptime, "%dh %dm", hour, minute);
	else
		asprintf(&buf->sys_attr.uptime, "%dm", minute);

    	fclose(file);
}


void power(struct comp_info *charge, struct comp_info *predict)
{
	if (charge == NULL || predict == NULL)
		return;

    	FILE *file = fopen("/sys/class/power_supply/BAT0/capacity", "r");
    	if (!file)
		return;

    	char capacity[24];
    	fscanf(file, "%s", capacity);
    	fclose(file);

	asprintf(&charge->sys_attr.b_capacity, "%s", capacity);

    	FILE *file1 = fopen("/sys/class/power_supply/BAT0/status", "r");
    	if (!file1)
		return;

    	char status[64];
    	fscanf(file1, "%s", status);
    	fclose(file1);

    	FILE *file7 = fopen("/sys/class/power_supply/BAT0/charge_full", "r");
    	if (!file7)
		return;

    	int charge_full = 0;
    	char charge_capasity[120];
    	fscanf(file7, "%s", charge_capasity);
    	charge_full = atoi(charge_capasity);
    	fclose(file7);
    
    	FILE *file9 = fopen("/sys/class/power_supply/BAT0/charge_now", "r");
    	if (!file9)
		return;

    	int charge_now = 0;
    	char charge_now_str[120];
    	fscanf(file9, "%s", charge_now_str);
    	charge_now = atoi(charge_now_str);
    	fclose(file9);

    	FILE *file10 = fopen("/sys/class/power_supply/BAT0/current_now", "r");
    	if (!file10)
		return;
    
    	int current_now = 0;
    	char current_now_str[120];
    	fscanf(file10, "%s", current_now_str);
    	current_now = atoi(current_now_str);
    	fclose(file10);

    	if (current_now <= 0)
		return;
    
    	int rem_charge = charge_full - charge_now;
    	float rem_time = ((float)rem_charge/ current_now) * 60;
    	float backup = ((float)charge_now / current_now) * 60;

    	//until charge full
    
    	int rem_total_time = (int)rem_time;
    	int rem_hour = rem_total_time / 60;
    	int rem_min = rem_total_time % 60;

    	//remain charge

    	int backup_time = (int)backup;
    	int backup_hour = backup_time / 60;
    	int backup_min = backup_time % 60;

    	if (strcmp(status, "Charging") == 0) {
		if (rem_hour > 0)
			asprintf(&predict->sys_attr.b_ch_predict, "%dh %dm to full", rem_hour, rem_min);
		else
	    		asprintf(&predict->sys_attr.b_ch_predict, "%dm to full", rem_min);
    	} else if (strcmp(status, "Discharging") == 0) {
		if (backup_hour > 0)
			asprintf(&predict->sys_attr.b_ch_predict, "%dh %dm left", backup_hour, backup_min);
		else
	    		asprintf(&predict->sys_attr.b_ch_predict, "%dm left", backup_min);
    	} else if (strcmp(status, "full") == 0) {
		asprintf(&predict->sys_attr.b_ch_predict, "%s", status);
	}
}

void shell(struct comp_info *buf)
{
	if (buf == NULL)
		return;

    	FILE *file = fopen("/proc/self/status", "r");
    	if (!file)
		return;

    	char buffer[1024];
    	while (fgets(buffer, sizeof(buffer), file) != NULL) {
        	if (strncmp(buffer, "PPid", 4) == 0) {
           		buffer[strcspn(buffer, "\n")] = 0;
            		snprintf(buffer, sizeof(buffer), "/proc/%s/comm", buffer+6);

            		char shell_name[64];

            		FILE *shell_file = fopen(buffer, "r");
            		if(!shell_file)
				return;

            		fscanf(shell_file, "%s", shell_name);
            		fclose (shell_file);
            		buf->sys_attr.shell = strdup(shell_name);
            		break;
        	}    
    	}
	fclose(file); 
}

void product_name() {
    FILE *file = fopen("/sys/devices/virtual/dmi/id/product_name", "r");
    if(!file) return;

    char name[512];
    fgets(name, sizeof(name), file);
    name[strcspn(name, "\n")] = 0;
    fclose(file);
    printf("Host        "RED":"RESET" %s\n", name);
}

void temp() {

    int mcls = 0;
    FILE *file = fopen("/sys/class/hwmon/hwmon4/temp1_input", "r");
    if(!file) return;

    fscanf(file, "%d", &mcls);
    fclose(file);

    int c = mcls / 1000;
    printf("Temp        "RED":"RESET" %d°C\n", c);
}

void gpu() {

    FILE *file = fopen("/sys/class/drm/card1/device/vendor", "r");
    if(!file) return;
    char vendor[64];
    fscanf(file, "%s", vendor);
    char gpu[64];
    if (strcmp(vendor, "0x8086") == 0)
    {
        strcpy(gpu, "Intel");
    } else if (strcmp(vendor, "0x10de") == 0)
    {
        strcpy(gpu, "NVIDIA");
    } else if (strcmp(vendor, "0x1002") == 0)
    {
        strcpy(gpu, "AMD");
    } else {
        strcpy(gpu, "unknown");
    }
    
    printf("GPU         "RED":"RESET" %s\n", gpu);

    fclose(file);
    
}

void usb() {
    DIR *d = opendir("/sys/bus/usb/devices/");
    if(!d) return;

    struct dirent *a;
    int count = 0;

    while ((a = readdir(d)) != NULL)
    {
        if (isdigit(a->d_name[0]))
        {
            char path[512];
            snprintf(path, sizeof(path), "/sys/bus/usb/devices/%s/removable", a->d_name);

            FILE *file = fopen(path, "r");
            if(!file) {
                continue;
            }

            char attachment[64];
            fscanf(file, "%s", attachment);

            if (strcmp(attachment, "removable") == 0)
            {
		if(count == 0) {
			printf("USB         "RED":"RESET" ");
		}
                char name_path[512];
                snprintf(name_path, sizeof(name_path), "/sys/bus/usb/devices/%s/product", a->d_name);
		count++;

                FILE *name_file = fopen(name_path, "r");
                if(!name_file) continue;

                char product_name[64];
                fgets(product_name, sizeof(product_name), name_file);
                product_name[strcspn(product_name, "\n")] = 0;
		if(count > 1) {
			printf("| %s", product_name);
		} else {
                	printf("%s ", product_name);
		}
                fclose(name_file);
            }
            fclose(file);
         }
        
    }
    if(count > 0) printf("\n");
    closedir(d);
}

