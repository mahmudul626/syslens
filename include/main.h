#ifndef MAIN_H
#define MAIN_H

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

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define RESET   "\033[0m"

struct sys_info {
	char *os_name;
	char *kernel;
	char *shell;
	char *user;
	char *product;
};

struct comp_info {
	struct sys_info sys_attr;
};

void proc();
void print_bar(int length, const char *color);
void mem();
void load_avg();
void uptime();
void power();
void cpu();
void gpu();
void getos(struct comp_info *);
void kernel(struct comp_info *);
void get_username(struct comp_info *);
void shell();
void product_name();
void temp();
void usb();
void cpu_per();

#endif
