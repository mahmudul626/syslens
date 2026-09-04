#include <stdio.h>
#include "../include/main.h"

#define RESET           "\033[0m"
#define BOLD            "\033[1m"
#define TOP_HEADER      "\033[38;5;255m\033[48;5;28m\033[1m" 
#define HTOP_BAR        "\033[38;5;46m"                      
#define HTOP_TEXT       "\033[38;5;248m"                     
#define FETCH_KEY       "\033[38;5;39m"                      
#define FETCH_VAL       "\033[38;5;255m"

void print_ui() {
    struct comp_info get;
    getos(&get);
    kernel(&get);
    get_username(&get);

    printf("\n");

    printf("  " FETCH_KEY BOLD "%s" RESET FETCH_VAL " %s", "OS\t :", get.sys_attr.os_name);
    
    printf("\t\t" HTOP_TEXT "Ram  " RESET);
    print_bar(5, HTOP_BAR);
    printf(HTOP_BAR " 50%%" RESET HTOP_TEXT" (3.6GiB/7.6GiB)\n" RESET);


    printf("  " FETCH_KEY BOLD "%s" RESET FETCH_VAL " %s", "Kernel :", get.sys_attr.kernel);
    
    printf("\t" HTOP_TEXT "Swp  " RESET);
    print_bar(2, HTOP_BAR);
    printf(HTOP_BAR " 20%%" RESET HTOP_TEXT" (0.0GiB/2.0GiB)\n" RESET);



    printf("  " FETCH_KEY BOLD "%s" RESET FETCH_VAL " 10 hours, 24 mins", "Uptime :");
    
    printf("\t" HTOP_TEXT "Disk " RESET);
    print_bar(7, HTOP_BAR);
    printf(HTOP_BAR " 70%%" RESET HTOP_TEXT" (3.6GiB/7.6GiB)\n" RESET);



    printf("  " FETCH_KEY BOLD "%-8s" RESET FETCH_VAL " bash 5.2.21", "Shell  :");
    
    printf("\t\t" HTOP_TEXT "Cpu  " RESET);
    print_bar(1, HTOP_BAR);
    printf(HTOP_BAR " 10%% " RESET HTOP_TEXT"(40°C)\n" RESET);





    printf("  " FETCH_KEY BOLD "%-8s" RESET FETCH_VAL "%s", "User   : ", get.sys_attr.user);
    
    
    
    
    char load_buffer[50];
    snprintf(load_buffer, sizeof(load_buffer), "Load average: 0.45 0.61 0.55\n");
    printf("\t" HTOP_TEXT "%s" RESET, load_buffer);






    printf("  " FETCH_KEY BOLD "%s" RESET FETCH_VAL "93%% (34m to full)", "Bat\t : ");
    
    
    char task_buffer[50];
    snprintf(task_buffer, sizeof(task_buffer), "Tasks: 142, 1 R, 141 S, 0 Z\n");
    printf("\t" HTOP_TEXT "%s" RESET, task_buffer);
    
    
    
    printf("  " FETCH_KEY BOLD "%s" RESET FETCH_VAL "i5-5300U", "Proc\t : ");
    
    
    
    printf( HTOP_TEXT "\t\tNet: \u2191 195 kb/s \u2193 35 kb/s\n" RESET);


    printf("  " FETCH_KEY BOLD "%s" RESET FETCH_VAL "EliteBook 840 G2\n", "Host\t : ");
    

    
    printf("  " FETCH_KEY BOLD "%s" RESET FETCH_VAL "Intel\n", "Gpu\t : ");
    
    
    
    printf("  " FETCH_KEY BOLD "%s" RESET FETCH_VAL "\n", "Usb");
    
    
    printf("  ├─ ttyACM0 ── Arduino Uno\n");
    printf("  └─ ttyUSB0 ── RPi Pico\n");



    printf("\n");

    printf(TOP_HEADER "  %-10s %-10s %-15s %-10s %-10s %-15s " RESET "\n", "PID", "PPID", "STATUS", "RES", "%MEM", "COMMAND");
    
    printf("  %-10s %-10s %-15s %-10s %-10s %-15s \n", "1234", "3214", "S (Sleeping)", "1500", "1.5%", "systemd");
    
    printf("  %-10s %-10s %-15s %-10s %-10s %-15s \n", "1234", "3214", "S (Sleeping)", "1500", "1.5%", "systemd");
    
    printf("  %-10s %-10s %-15s %-10s %-10s %-15s \n", "1234", "3214", "S (Sleeping)", "1500", "1.5%", "systemd");
    
    printf("  %-10s %-10s %-15s %-10s %-10s %-15s \n", "1234", "3214", "S (Sleeping)", "1500", "1.5%", "systemd");
    
    printf("  %-10s %-10s %-15s %-10s %-10s %-15s \n", "1234", "3214", "S (Sleeping)", "1500", "1.5%", "systemd");
    
    printf("\n");

    free(get.sys_attr.os_name);
    free(get.sys_attr.kernel);
    free(get.sys_attr.user);

}

int main() {
    
    print_ui();
    return 0;
}

