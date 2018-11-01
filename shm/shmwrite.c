#include <stdio.h>
#include <stdlib.h>
#include "shmutil.h"

int main ()
{
  shm_write();

  /*calling the other process*/
  system("./shmread");

  return 0;
}
