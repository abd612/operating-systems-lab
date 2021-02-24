#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int c = getsyscallinfo();
  printf(1, "%s: %d\n", "System call count", c);

  int n = 166;
  int m = geteval(n);
  printf(1, "%s: %d, %s: %d\n", "Number", n, "Double", m);
  exit();
}