#include "../include/main.h"

void user() {
    char *ptr;
    ptr = getenv("USER");
    if (ptr != NULL) {

        FILE *file = fopen("/etc/hostname", "r");
        if(!file) return;

        char hostname[64];
        fgets(hostname, sizeof(hostname), file);
        hostname[strcspn(hostname, "\n")] = 0;


        printf("User@Host   "RED":"RESET" %s@%s\n", ptr, hostname);
        fclose(file);
    }
}

void kernel() {
    struct utsname sys;
    uname(&sys);
    printf("Kernel      "RED":"RESET" %s\n", sys.release);
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

    printf("OS          "RED":"RESET" %s %s\n", name, version);
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
        printf("Uptime      "RED":"RESET" %d hours %d mins\n", hour, minute);
    } else {
        printf("Uptime      "RED":"RESET" %d mins\n", minute);
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

    FILE *file7 = fopen("/sys/class/power_supply/BAT0/charge_full", "r");
    if (!file7) return;

    int charge_full = 0;
    char charge_capasity[120];
    fscanf(file7, "%s", charge_capasity);
    charge_full = atoi(charge_capasity);
    fclose(file7);

    FILE *file8 = fopen("/sys/class/power_supply/BAT0/charge_full_design", "r");
    if (!file8) return;

    int charge_full_design = 0;
    char charge_capasity_design[120];
    fscanf(file8, "%s", charge_capasity_design);
    charge_full_design = atoi(charge_capasity_design);
    fclose(file8);

    int health = (charge_full * 100) / charge_full_design;

    printf("Power       "RED":"RESET" %s%%"RESET" (%s) %s %s\n", capacity, status, technology, type);
    printf("Health      "RED":"RESET" %d%%\n", health);
    printf("Model       "RED":"RESET" %s %s %s\n", model_name, serial_number, manufacturer);
    
}

void shell() {
    FILE *file = fopen("/proc/self/status", "r");
    if(!file) return;

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        if (strncmp(buffer, "PPid", 4) == 0)
        {
            buffer[strcspn(buffer, "\n")] = 0;
            snprintf(buffer, sizeof(buffer), "/proc/%s/comm", buffer+6);
            char shell_name[64];
            FILE *shell_file = fopen(buffer, "r");
            if(!shell_file) return;
            fscanf(shell_file, "%s", shell_name);
            fclose (shell_file);
            printf("Shell       "RED":"RESET" %s\n", shell_name);
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
                char name_path[512];
                snprintf(name_path, sizeof(name_path), "/sys/bus/usb/devices/%s/product", a->d_name);

                FILE *name_file = fopen(name_path, "r");
                if(!name_file) continue;

                char product_name[64];
                fgets(product_name, sizeof(product_name), name_file);
                product_name[strcspn(product_name, "\n")] = 0;
                printf("USB         "RED":"RESET" %s\n", product_name);
                fclose(name_file);
            }
            fclose(file);
         }
        
    }
    closedir(d);
}