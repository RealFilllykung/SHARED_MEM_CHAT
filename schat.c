#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/smh.h>

#define MEM_SIZE 4096

struct shm_st
{
    int written_0;
    char data_0[BUFSIZ];
    int written_1;
    char data_1[BUFSIZ];
};

int main(int argc, char *argv[])
{
    int shmID, child;
    void *sh_mem = NULL;
    struct shm_st *sh_area;
    char buffer[BUFSIZ] = "";
    
    
}
