#include <stdio.h>
#include "../include/main.h"

#define RESET           "\033[0m"
#define BOLD            "\033[1m"
#define TOP_HEADER      "\033[38;5;255m\033[48;5;28m\033[1m" 
#define HTOP_BAR        "\033[38;5;46m"                      
#define HTOP_TEXT       "\033[38;5;248m"                     
#define FETCH_KEY       "\033[38;5;39m"                      
#define FETCH_VAL       "\033[38;5;255m"

int main() {
    struct sysinfo get;
    getos(&get);
    kernel(&get);

    printf("\n");

    printf("  " HTOP_TEXT "Mem" BOLD "[" HTOP_BAR "|||||||||||               " RESET HTOP_TEXT "3.21G/15.8G" BOLD "]" RESET);
    printf("      " FETCH_KEY BOLD "%-8s" RESET FETCH_VAL "%s\n", "OS:", get.os_name);

    printf("  " HTOP_TEXT "Swp" BOLD "[" HTOP_BAR "||                        " RESET HTOP_TEXT "512M/8.00G" BOLD "]" RESET);
    printf("       " FETCH_KEY BOLD "%-8s" RESET FETCH_VAL "%s\n", "Kernel:", get.kernel);

    char task_buffer[50];
    snprintf(task_buffer, sizeof(task_buffer), "Tasks: 142, 1 running");
    printf("  " HTOP_TEXT "%-43s" RESET, task_buffer);
    printf("     " FETCH_KEY BOLD "%-8s" RESET FETCH_VAL "10 hours, 24 mins\n", "Uptime:");

    char load_buffer[50];
    snprintf(load_buffer, sizeof(load_buffer), "Load average: 0.45 0.61 0.55");
    printf("  " HTOP_TEXT "%-43s" RESET, load_buffer);
    printf("     " FETCH_KEY BOLD "%-8s" RESET FETCH_VAL "bash 5.2.21\n", "Shell:");

    printf("\n");

    printf(TOP_HEADER "  %-10s %-10s %-15s %-10s %-10s %-15s " RESET "\n", "PID", "PPID", "STATUS", "RES", "%MEM", "COMMAND");
    
    printf("  %-10s %-10s %-15s %-10s %-10s %-15s \n", "1234", "3214", "S (Sleeping)", "1500", "1.5%", "systemd");
    //printf("  %-8s %-6s %-8s %-30s \n", "5678", "12.8%", "25.3%", "chrome");
    //printf("  %-8s %-6s %-8s %-30s \n", "9101", "2.1%", "0.0%", "fastfetch");
    
    printf("\n");
    return 0;
}

