#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int p[2])
{
  int prime;
  int num;
  int p1[2];

  if (read(p[0], &prime, 4))
  {
    printf("prime %d\n", prime);
    pipe(p1);
    if (fork() == 0)
    {
      close(p[0]);
      close(p1[1]);
      primes(p1);
    }
    else
    {
      close(p1[0]);
      while (read(p[0], &num, 4))
      {
        if (num % prime != 0)
        {
          write(p1[1], &num, 4);
        }
      }

      close(p[0]);
      close(p1[1]);
      wait(0);
    }
  }
  else
  {
    close(p[0]);
  }
  exit(0);
}

int main(int argc, char *argv[])
{
  int p[2];
  pipe(p);

  for (int i = 2; i <= 35; i++)
  {
    write(p[1], &i, 4);
  }

  close(p[1]);
  primes(p);
  exit(0);
}