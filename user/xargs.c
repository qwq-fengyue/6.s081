#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAX_LINE_LEN 128

int read_line(int fd, char *buffer, int buffer_size)
{
    int i = 0;
    char c;
    while (i < buffer_size - 1)
    {
        int n = read(fd, &c, 1);
        if (n == 0) {
            break;
        }
        if (n < 0)
        {
            fprintf(2, "read error\n");
            return -1;
        }
        if (c == '\n')
        {
            break;
        }
        buffer[i++] = c;
    }
    buffer[i] = '\0';
    return i;
}

int main(int argc, char *argv[])
{
    char line[MAX_LINE_LEN];
    char *arguments[MAXARG];

    int i;
    for (i = 0; i < argc - 1; ++i)
    {
        arguments[i] = argv[i + 1];
    }

    while (read_line(0, line, sizeof(line)) > 0)
    {
        arguments[i] = line;
        arguments[i + 1] = 0;

        int pid = fork();
        if (pid == 0)
        {
            exec(argv[1], arguments);
            exit(1);
        }
        else
        {
            wait(0);
        }
    }

    exit(0);
}