#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>

#define MEM_SIZE 4096

struct shm_st
{
    int written_0;
    char data_0[BUFSIZ];
    int written_1;
    char data_1[BUFSIZ];
};

int main(int argc, char *argv[]){
    int shmID, child, running = 1;
    void *sh_mem = NULL;
    struct shm_st *sh_area;
    char buffer[BUFSIZ] = "";
    int user = atoi(argv[1]);
    
    //fork the child process
    child = fork();
    
    
    //if it is parent
    if(child){
        //We use parent as the writer
        if (user == 1)
        {
            //Get the ID of the specific key for parent
            shmID = shmget((key_t)1234,MEM_SIZE, 0666 | IPC_CREAT);
            
            //if shmget error
            if (shmID == -1){
                fprintf(stderr,"shmget failed\n");
                exit(EXIT_FAILURE);
            }
            
            //Create the share memory
            sh_mem = shmat(shmID,NULL,0);
            
            //if it failed to create the share memory
            if(sh_mem == (void*) -1){
                fprintf(stderr,"shmat failed\n");
                exit(EXIT_FAILURE);
            }
            
            //attach the sh_area to shared memory
            sh_area = (struct shm_st*)sh_mem;
            
            //While the process suppose to run
            while(running)
            {
                while(sh_area -> written_0)
                {
                    sleep(1);
                }
                fgets(buffer,BUFSIZ,stdin);
                strcpy(sh_area -> data_0, buffer);
                sh_area -> written_0 = 1;
                if (strncmp(buffer,"end chat",8) == 0){
                    kill(child,SIGINT);
                    running = 0;
                }
            }
            
            //If we cannot delete the shared memory
            if(shmdt(sh_mem) == -1){
                fprintf(stderr,"shmdt failed\n");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }
        else{
            //Get the ID of the specific key for parent
            shmID = shmget((key_t)1234,MEM_SIZE, 0666 | IPC_CREAT);
            
            //if shmget error
            if (shmID == -1){
                fprintf(stderr,"shmget failed\n");
                exit(EXIT_FAILURE);
            }
            
            //Create the share memory
            sh_mem = shmat(shmID,NULL,0);
            
            //if it failed to create the share memory
            if(sh_mem == (void*) -1){
                fprintf(stderr,"shmat failed\n");
                exit(EXIT_FAILURE);
            }
            
            //attach the sh_area to shared memory
            sh_area = (struct shm_st*)sh_mem;
            
            //While the process suppose to run
            while(running)
            {
                while(sh_area -> written_1)
                {
                    sleep(1);
                }
                fgets(buffer,BUFSIZ,stdin);
                strcpy(sh_area -> data_1, buffer);
                sh_area -> written_1 = 1;
                if (strncmp(buffer,"end chat",8) == 0){
                    kill(child,SIGINT);
                    running = 0;
                }
            }
            
            //If we cannot delete the shared memory
            if(shmdt(sh_mem) == -1){
                fprintf(stderr,"shmdt failed\n");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }
    }
    else{
        //We use child as the printer
        
        //If the user login as the user 1
        if (user == 1){
            //Get the ID of the specific key for parent
            shmID = shmget((key_t)1234,MEM_SIZE, 0666 | IPC_CREAT);
            
            //if shmget error
            if (shmID == -1){
                fprintf(stderr,"shmget failed\n");
                exit(EXIT_FAILURE);
            }
            
            //Create the share memory
            sh_mem = shmat(shmID,NULL,0);
            
            //if it failed to create the share memory
            if(sh_mem == (void*) -1){
                fprintf(stderr,"shmat failed\n");
                exit(EXIT_FAILURE);
            }
            
            //attach the sh_area to shared memory
            sh_area = (struct shm_st*)sh_mem;
            sh_area -> written_1 = 0;
            
            while(running)
            {
                if(sh_area -> written_1)
                {
                    printf("%s",sh_area -> data_1);
                    sh_area -> written_1 = 0;
                    if(strncmp(sh_area -> data_1,"end chat",8) == 0)
                    {
                        kill(getppid(),SIGINT);
                        running = 0;
                    }
                }
            }
            
            if(shmdt(sh_mem) == -1 || shmctl(shmID,IPC_RMID,0) == -1)
            {
                fprintf(stderr,"shmdt or shmctl failed\n");
                exit(EXIT_FAILURE);
            }
            
            exit(EXIT_SUCCESS);
        }
        else{
            //Get the ID of the specific key for parent
            shmID = shmget((key_t)1234,MEM_SIZE, 0666 | IPC_CREAT);
            
            //if shmget error
            if (shmID == -1){
                fprintf(stderr,"shmget failed\n");
                exit(EXIT_FAILURE);
            }
            
            //Create the share memory
            sh_mem = shmat(shmID,NULL,0);
            
            //if it failed to create the share memory
            if(sh_mem == (void*) -1){
                fprintf(stderr,"shmat failed\n");
                exit(EXIT_FAILURE);
            }
            
            //attach the sh_area to shared memory
            sh_area = (struct shm_st*)sh_mem;
            sh_area -> written_0 = 0;
            
            while(running)
            {
                if(sh_area -> written_0)
                {
                    printf("%s",sh_area -> data_0);
                    sh_area -> written_0 = 0;
                    if(strncmp(sh_area -> data_0,"end chat",8) == 0)
                    {
                        kill(getppid(),SIGINT);
                        running = 0;
                    }
                }
            }
            
            if(shmdt(sh_mem) == -1 || shmctl(shmID,IPC_RMID,0) == -1)
            {
                fprintf(stderr,"shmdt or shmctl failed\n");
                exit(EXIT_FAILURE);
            }
            
            exit(EXIT_SUCCESS);
            
        }
            
    }
}
