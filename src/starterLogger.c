#include <linux/sched/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <errno.h>

#define SIZE_OF_DMESG_OUTPUT 8192

#define ERROR_COMMAND_EXEC 1

#define TIMES 15
#define DELAY 10

#define MODULE_STRING_UNIQUE "~~[TASK INFO]~~: procID: "

int main(int argc, char *argv[])
{
    FILE *filePointer = NULL;
    char dmesg[SIZE_OF_DMESG_OUTPUT] = {'\0'};

    if (argv[1] != NULL)
    {
        struct sched_attr policyGetter;
        syscall(SYS_sched_getattr, atoi(argv[1]), &policyGetter, sizeof(policyGetter), 0);
        printf("%s Sched_police is: %d\n\n", strerror(errno),
               policyGetter.sched_policy); // Defines in /usr/include/bits/sched.h
    }

    system("sudo insmod taskInfoGetter.ko");
    for (int i = 0; i < TIMES; i++)
    {
        filePointer = popen("sudo dmesg -c", "r");

        if (filePointer == NULL)
        {
            printf("Error: can't execute dmesg");
            return ERROR_COMMAND_EXEC;
        }

        char checkString[512] = {"\0"};

        if (argv[1])
            snprintf(checkString, sizeof(checkString), "%s%s", MODULE_STRING_UNIQUE, argv[1]);

        while (fgets(dmesg, sizeof(dmesg), filePointer) != NULL)
        {
            if (checkString[0] == '\0' || strstr(dmesg, checkString))
            {
                printf("%s", dmesg);
                fgets(dmesg, sizeof(dmesg), filePointer);
                printf("%s", dmesg);
            }
        }

        pclose(filePointer);
        sleep(DELAY);
    }

    system("sudo rmmod taskInfoGetter");

    return EXIT_SUCCESS;
}
