/* question-9.c */

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

/* I am assuming the system calls do not give errors, just for simplicity of the code */

void traduz_e_filtra(char* caminho_ficheiro, char* palavra_chave) {
    int file = open(caminho_ficheiro, O_RDONLY);
    int fildes[2];
    pipe(fildes);

    pid_t child = fork();

    // child process
    if (child == 0) {
        close(fildes[0]); // close reading side of the pipe

        // redirect the stdin to the file
        dup2(file, 0);
        close(file);

        // redirect stdout to the writting side of the pipe
        dup2(fildes[1], 1);
        close(fildes[1]);

        execlp("tradutor", "tradutor", NULL);
        perror("execlp");
        _exit(127);
    }
    close(fildes[1]); // close writting side of the pipe

    child = fork();

    // child process
    if (child == 0) {
        // redirect stdin to the reading side of the pipe
        dup2(fildes[0], 0);
        close(fildes[0]);

        execlp("grep", "grep", palavra_chave, NULL);
        perror("execlp");
        _exit(127);
    }
    close(fildes[0]); // close reading side of the pipe
}

#define N 10

void filtraN(char* caminhos_ficheiros[], int total_ficheiros, char* palavra_chave) {
    int chunk = total_ficheiros / N;
    int i, j = 0;
    
    for (i = 0; i < N; i++) {
        if (fork() == 0) {
            // last process does the remaining
            if (i == (N - 1)) {
                chunk += total_ficheiros % N;
            }

            while (j < chunk) {
                // iterate over the given chunk
                traduz_e_filtra(caminhos_ficheiros[i * chunk + j], palavra_chave);
            }
            _exit(0);
        }
    }

    // wait for all the child processes
    for (i = 0; i < N; i++) {
        wait(NULL);
    }
}
