#ifndef MAIN_H
#define MAIN_H
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <ctype.h>

#define BUFFER_SIZE 2024
#define OS_NAME_MAX 64
#define MAX_BAR 10
#define VERSION "1.0.3"
extern unsigned long totalram;

#define RED "\033[1;31m"
#define GREEN "\033[38;5;46m"
#define YELLOW "\033[38;5;220m"
#define BLUE "\033[1;34m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define TOP_HEADER "\033[38;5;255m\033[48;5;28m\033[1m"
#define HTOP_BAR "\033[38;5;46m"
#define HTOP_TEXT "\033[38;5;248m"
#define FETCH_KEY "\033[38;5;39m"
#define FETCH_VAL "\033[38;5;255m"

struct proc_info {
	char *tasks;
	struct {
    		char *name;
    		char *state;
    		char *pid;
    		char *user;
    		int vmrss;
	};

};

struct mem_info {
	struct {
		char *ram;
		char *color;
		int percent;
		int bar;
	};

	struct {
		char *swap;
		char *swap_color;
		int swap_percent;
		int swap_bar;
	};

	struct {
		char *disk;
		char *disk_color;
		int disk_percent;
		int disk_bar;
	};

	struct {
		char *cpu_color;
		int cpu_bar;
		int cpu_percent;
	};

	char *load_avg;
};

struct sys_info {
	char *os_name;
	char *kernel;
	char *shell;
	char *user;
	char *gpu;
	char *uptime;
	char *b_capacity;
	char *b_ch_predict;
	char *temp;
	char *product;
	char *cpu;
	char *usb;
};

struct comp_info {
	struct sys_info sys_attr;
	struct mem_info mem_attr;
	struct proc_info proc_attr;
};

void proc(struct comp_info *, struct comp_info *, int *);
void print_bar(int length, const char *color);
void mem(struct comp_info *);
void load_avg(struct comp_info *);
void uptime(struct comp_info *);
void power(struct comp_info *, struct comp_info *);
void cpu(struct comp_info *);
void gpu(struct comp_info *);
void getos(struct comp_info *);
void kernel(struct comp_info *);
void get_username(struct comp_info *);
void shell(struct comp_info *);
void product_name(struct comp_info *);
void temp(struct comp_info *);
void usb(struct comp_info **, int *);
void cpu_per(struct comp_info *);

#endif
