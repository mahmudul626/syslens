#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 2026
#define MAX_BAR 10

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define RESET   "\033[0m"


void proc();

#endif