#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_OF_DMESG_OUTPUT 8192

#define ERROR_COMMAND_EXEC 1

#define MODULE_STRING_UNIQUE "~~[TASK INFO]~~: procID: "

int main(int argc, char *argv[])
{
    FILE *filePointer = NULL;
    char dmesg[SIZE_OF_DMESG_OUTPUT] = { '\0' };

    pclose(popen("sudo dmesg -c", "r"));
    filePointer = popen("sudo dmesg", "r");

    if (filePointer == NULL)
    {
        printf("Error: can't execute dmesg");
        return ERROR_COMMAND_EXEC;
    }

    char checkString[512] = { "\0" };
    if (argv[1])
        snprintf(checkString, sizeof(checkString), "%s%s%s", MODULE_STRING_UNIQUE, argv[1], ",");

    while(fgets(dmesg, sizeof(dmesg), filePointer) != NULL)
    {
        if (checkString[0] == '\0' || strstr(dmesg, checkString))
        {
            printf("%s", dmesg);
        }
    }

    pclose(filePointer);

    return EXIT_SUCCESS;
}
