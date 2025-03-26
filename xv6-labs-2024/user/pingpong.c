#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

    int main()
    {
        int p1[2], p2[2]; // Pipe 1: parent -> child, Pipe 2: child -> parent
        char buf;
        
        pipe(p1);
        pipe(p2);
        
        int pid = fork();
        
        if (pid > 0) // Parent process
        {
            close(p1[0]);
            close(p2[1]);
            
            write(p1[1], "x", 1);
            close(p1[1]);
            
            read(p2[0], &buf, 1);
            printf("%d: received pong\n", getpid());
            
            close(p2[0]);
        }
        else if (pid == 0) // Child process
        {
            close(p1[1]);
            close(p2[0]);
            
            read(p1[0], &buf, 1);
            printf("%d: received ping\n", getpid());
            
            write(p2[1], "y", 1);
            close(p2[1]);
            close(p1[0]);
        }
        else // Fork failed
        {
            printf("Fork failed\n");
            exit(1);
        }
        
        exit(0);
    }