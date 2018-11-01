#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include "shmutil.h"

#define SHM_KEY 0xDEADBEEF
#define SHM_SIZE 0x100000

void shm_read()
{
  int shm_id;
  char* ptr;
  int i;

  /* Allocate a shared memory segment. */
  shm_id = shmget (SHM_KEY, SHM_SIZE, IPC_CREAT | S_IRUSR | S_IWUSR);

  if (shm_id == -1)
    return;

  /* Attach the shared memory segment. */
  ptr = (char*) shmat (shm_id, 0, 0);

  for (i=0; i< SHM_SIZE; i++)
  {
    if (*ptr == '\0')
      break;
    printf("%c", *ptr++);
  }
  printf("\n");

  /* Detach the shared memory segment. */
  shmdt (ptr);
}


void shm_write()
{
  int shm_id;
  char *ptr;

  /* Allocate a shared memory segment. */
  shm_id = shmget (SHM_KEY, SHM_SIZE, IPC_CREAT | S_IRUSR | S_IWUSR);

  if (shm_id == -1)
  {
    perror("shm_write");
  }
  else
  {
    /* Attach the shared memory segment. */
    ptr= (char*) shmat (shm_id, 0, 0);

    /* Start to write data. */
    sprintf(ptr, "Life is beautiful!");

    /* Detach the shared memory segment. */
    shmdt (ptr);
  }
}
