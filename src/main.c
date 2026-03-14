#include "../include/main.h"


void print_all() {

    printf("[ System Info ]\n");

        getos();
        kernel();
        cpu();
        uptime();
        user();
        shell();
        power();

        printf("\n");

    printf("[ Resource Usage ]\n");

        mem();
        load_avg();

        printf("\n");

    printf("[ Active Processes ]\n");

    proc();

}

int main(int argc, char *argv[]) {
    
    if (argc == 1)
    {
        print_all();
        return 0;
    }
    else if (argc == 2)
    {
        if (strcmp(argv[1], "-s") == 0)
        {
            printf("[ System Info ]\n");
            getos();
            kernel();
            cpu();
            uptime();
            user();
        }
        else if (strcmp(argv[1], "-m") == 0)
        {
            printf("[ Resource Usage ]\n");
            mem();
            load_avg();
        }
        else if (strcmp(argv[1], "-p") == 0)
        {
            printf("[ Active Processes ]\n");
            proc();
        }
        else if (strcmp(argv[1], "--version") == 0)
        {
            printf("syslens v%s\n", VERSION);
        }
        else if (strcmp(argv[1], "--help") == 0)
        {
            printf("SysLens - system information tool\n\n");
            printf("usage:\n");
            printf("syslens              show all info\n");
            printf("syslens -s           system info\n");
            printf("syslens -m           memory info\n");
            printf("syslens -p           process info\n");
            printf("syslens --help       show help\n");
            printf("syslens --version    syslens version\n");
        }
        else
        {
            printf("Invalid command!\n");
            printf("Try: syslens --help\n");
        }
    }
    else
    {
        printf("usage:\n");
        printf("syslens              show all info\n");
        printf("syslens -s           system info\n");
        printf("syslens -m           memory info\n");
        printf("syslens -p           process info\n");
        printf("syslens --help       show help\n");
        printf("syslens --version    syslens version\n");
    }
    
    return 0;
}