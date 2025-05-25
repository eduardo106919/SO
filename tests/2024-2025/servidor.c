/* servidor.c */

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/stat.h>

#define FIFO "fifo_server"

typedef struct req {
    int pid;
    char *prompt;
} Req;

int main(void) {
    mkfifo(FIFO, 0666);
    
    Req q;
    int server = 0, client = 0;
    ssize_t out = 0;
    pid_t child = 0;
    char fifoc_name[30];
    int count = 0;
    
    while ((server = open(FIFO, O_RDONLY)) > 0) {
        // receive request from client
        out = read(server, &q, sizeof(q));

        // run "SOGPT prompt"
        if (fork() == 0) {
            execlp("SOGPT", "SOGPT", q.prompt, NULL);
            perror("SOGPT");
            _exit(127);
        }
        count++;

        // send the response to the client
        sprintf(fifoc_name, "fifo_client_%d", q.pid);
        client = open(fifoc_name, O_WRONLY);
        write(client, &count, sizeof(count));
    }

    // wait for all the child processes
    for (int i = 0; i < count; i++) {
        wait(NULL);
    }

    return 0;
}
