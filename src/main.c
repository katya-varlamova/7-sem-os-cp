#include <linux/sched/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <errno.h>

#define SIZE_OF_LOG 8192

#define ERROR_COMMAND_EXEC 1

#define TIMES 100
#define DELAY 1

int main(int argc, char *argv[])
{
    FILE *filePointer = NULL;
    char log[SIZE_OF_LOG] = {'\0'};

    //system("sudo insmod md.ko");
    
    for (; ; )
    {
        filePointer = popen("cat /proc/analyzer", "r");

        if (filePointer == NULL)
        {
            printf("Error: can't execute cat for process analyzer");
            return ERROR_COMMAND_EXEC;
        }

        while (fgets(log, sizeof(log), filePointer) != NULL)
        {
            printf("%s", log);
            fgets(log, sizeof(log), filePointer);
            printf("%s", log);
        }

        pclose(filePointer);
        //sleep(DELAY);
    }

    //system("sudo rmmod md.ko");

    return EXIT_SUCCESS;
}
