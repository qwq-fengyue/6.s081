#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *fileName)
{
    char buf[512], *p;
    int fd, fd1;
    struct dirent de;
    struct stat st, st1;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type)
    {
        case T_FILE:
            fprintf(2, "find: path error\n");
            break;
        case T_DIR:
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
            {
                printf("find: path too long\n");
                break;
            }

            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';

            while (read(fd, &de, sizeof(de)) == sizeof(de))
            {
                if (de.inum == 0)
                    continue;
                if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
                    continue;
                memmove(p, de.name, DIRSIZ);
                if ((fd1 = open(buf, 0)) >= 0 && (fstat(fd1, &st1)) >= 0)
                {
                    switch (st1.type)
                    {
                        case T_FILE:
                            if (!strcmp(de.name, fileName))
                                printf("%s\n", buf);
                            break;
                        case T_DIR:
                            find(buf, fileName);
                            break;
                        default:
                            break;
                    }
                    close(fd1);
                }
            }
            break;
        default:
            break;
    }

    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(2, "Usage:find <path> <fileName>\n");
        exit(0);
    }

    find(argv[1], argv[2]);
    exit(0);
}