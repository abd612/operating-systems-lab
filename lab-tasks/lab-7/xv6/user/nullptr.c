#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int *ptr = 0;
  printf(1,"Using null pointer's value in xv6\n");
  printf(1, "%d\n", *ptr);
  exit();
}
