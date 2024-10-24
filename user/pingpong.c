#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  int pid;
  int pfds[2];
  int cfds[2];
  char buf[8];
  
  pipe(pfds);
  pipe(cfds);

  pid = fork();
  if (pid == 0)
  {
    read(pfds[0], buf, 4);
    printf("%d: received %s\n", getpid(), buf);
    write(cfds[1], "pong", 4);
  }
  else
  {
    write(pfds[1], "ping", 4);
    read(cfds[0], buf, 4);
    printf("%d: received %s\n", getpid(), buf);
  }

  exit(0);
}