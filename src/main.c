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
    struct comp_info get, getplist[2024], *get_usb[80];
    int usb_count = 0, pindex = 0;
    usb(get_usb, &usb_count);
    getos(&get);
    kernel(&get);
    get_username(&get);
    uptime(&get);
    shell(&get);
    power(&get, &get);
    gpu(&get);
    temp(&get);
    product_name(&get);
    cpu(&get);
    mem(&get);
    load_avg(&get);
    cpu_per(&get);
    proc(&get, getplist, &pindex);

    printf("\n");

    printf("  " FETCH_KEY BOLD "%s" RESET FETCH_VAL " %s", "OS\t :", get.sys_attr.os_name);
    
    printf("\t\t" HTOP_TEXT "Ram  " RESET);
    print_bar(get.mem_attr.bar, get.mem_attr.color);
    printf("%s" " %d%%" RESET HTOP_TEXT" %s\n" RESET, get.mem_attr.color, get.mem_attr.percent, get.mem_attr.ram);


    printf("  " FETCH_KEY BOLD "%s" RESET FETCH_VAL " %s", "Kernel :", get.sys_attr.kernel);
    
    printf("\t" HTOP_TEXT "Swp  " RESET);
    print_bar(get.mem_attr.swap_bar, get.mem_attr.swap_color);
    printf("%s" " %d%%" RESET HTOP_TEXT" %s\n" RESET, get.mem_attr.swap_color, get.mem_attr.swap_percent, get.mem_attr.swap);



    printf("  " FETCH_KEY BOLD "%s" RESET FETCH_VAL "%-21s", "Uptime : ", get.sys_attr.uptime);
    
    printf(HTOP_TEXT "%s" RESET, "Disk ");
    print_bar(get.mem_attr.disk_bar, get.mem_attr.disk_color);
    printf("%s" " %d%%" RESET HTOP_TEXT" %s\n" RESET, get.mem_attr.disk_color, get.mem_attr.disk_percent, get.mem_attr.disk);



    printf("  " FETCH_KEY BOLD "%-8s" RESET FETCH_VAL "%-21s", "Shell  : ", get.sys_attr.shell);
    
    printf(HTOP_TEXT "Cpu  " RESET);
    print_bar(get.mem_attr.cpu_bar, get.mem_attr.cpu_color);
    printf("%s" " %d%% " RESET HTOP_TEXT"(%s)\n" RESET, get.mem_attr.cpu_color, get.mem_attr.cpu_percent, get.sys_attr.temp);





    printf("  " FETCH_KEY BOLD "%-8s" RESET FETCH_VAL "%s", "User   : ", get.sys_attr.user);
    
    
    printf("\t" HTOP_TEXT "Load average: %s\n" RESET, get.mem_attr.load_avg);


    printf("  " FETCH_KEY BOLD "%s" RESET FETCH_VAL "%s%% %-10s", "Bat\t : ", get.sys_attr.b_capacity, get.sys_attr.b_ch_predict);
    
    
    printf("\t" HTOP_TEXT "Tasks: %s\n" RESET, get.proc_attr.tasks);
    
    
    
    printf("  " FETCH_KEY BOLD "%s" RESET FETCH_VAL "%-21s", "Gpu\t : ", get.sys_attr.gpu);
    
    
    
   // printf( HTOP_TEXT "Net: \u2191 195 kb/s \u2193 35 kb/s\n" RESET);
      printf("\n");

    printf("  " FETCH_KEY BOLD "%s" RESET FETCH_VAL "%s\n", "Host\t : ", get.sys_attr.product);
    

    
    printf("  " FETCH_KEY BOLD "%s" RESET FETCH_VAL "%s\n", "Proc\t : ", get.sys_attr.cpu);
    
    
    if(usb_count > 0)
	    printf("  " FETCH_KEY BOLD "%s" RESET FETCH_VAL "\n", "Usb");
    
    
    if (get_usb) {
        for (int i = 0; i < usb_count; i++) {
            if (get_usb[i] && get_usb[i]->sys_attr.usb) {
		     if(usb_count > 1 && i != 1)
			    printf("  ├%s", get_usb[i]->sys_attr.usb);
		     else
			    printf("  └%s", get_usb[i]->sys_attr.usb);
            }
        }
    }



    printf("\n");

    printf(TOP_HEADER "  %-10s %-10s %-15s %-10s %-10s %-10s " RESET "\n", "PID", "USER", "STATUS", "RES", "%MEM", "COMMAND");


    for(int i = 0; i < 5; i++) {

            	float pmem = ((float)getplist[i].proc_attr.vmrss / totalram) * 100;

           	printf("  %-10s %-10s %-15s %-10d %-10.1f %-10s \n",
                	getplist[i].proc_attr.pid,
                	getplist[i].proc_attr.user,
                	getplist[i].proc_attr.state,
                	getplist[i].proc_attr.vmrss,
                	pmem,
                	getplist[i].proc_attr.name);
    }

    printf("\n");


    if (get_usb) {
        for (int i = 0; i < usb_count; i++) {
            if (get_usb[i]) {
                free(get_usb[i]->sys_attr.usb);
                free(get_usb[i]);
            }
        }
    }

    free(get.sys_attr.os_name);
    free(get.sys_attr.kernel);
    free(get.sys_attr.user);
    free(get.sys_attr.uptime);
    free(get.sys_attr.shell);
    free(get.sys_attr.b_capacity);
    free(get.sys_attr.b_ch_predict);
    free(get.sys_attr.gpu);
    free(get.sys_attr.temp);
    free(get.sys_attr.product);
    free(get.sys_attr.cpu);
    free(get.mem_attr.ram);
    free(get.mem_attr.color);
    free(get.mem_attr.disk);
    free(get.mem_attr.disk_color);
    free(get.mem_attr.swap);
    free(get.mem_attr.swap_color);
    free(get.mem_attr.cpu_color);
    free(get.mem_attr.load_avg);
    free(get.proc_attr.tasks);

for (int i = 0; i < pindex; i++) {
    free(getplist[i].proc_attr.name);
    free(getplist[i].proc_attr.state);
    free(getplist[i].proc_attr.pid);
    free(getplist[i].proc_attr.user);
}


}

int main() {
    
    print_ui();
    return 0;
}

