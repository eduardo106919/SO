/* question-8.c */

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

/* I am assuming the system calls do not give errors, just for simplicity of the code */

struct RegistoF {
    char nome[20];
    char cargo[20];
    int salário;
};


void aumentaSalarios(char* ficheiro, char* cargo, int valor, long N, int P) {
    // open the file
    int file = open(ficheiro, O_RDWR);
    struct RegistoF temp;
    ssize_t out = 0;
    long chunk = N / P;
    long count = 0;
    pid_t child = 0;

    for (int i = 0; i < P; i++) {
        child = fork();
        if (child == 0) {
            // child process

            // last process does the remaining
            if (i == (P - 1)) {
                chunk += N % P;
            }

            // go to the corresponding section of the file
            lseek(file, i * chunk, SEEK_SET);

            while (count < chunk && (out = read(file, &temp, sizeof(temp))) > 0) {
                // the employee has the same job
                if (strcmp(temp.cargo, cargo) == 0) {
                    // go back one employee
                    lseek(file, - sizeof(temp), SEEK_CUR);

                    // update the salary
                    temp.salário += valor;

                    // write back the employee info
                    write(file, &temp, sizeof(temp));
                }
                chunk++;
            }

            _exit(0);
        }
    }
}

int validaSalarios(char* ficheiro, char* cargo) {
    int fildes[2];
    pipe(fildes);
    pid_t child = fork();

    // child process
    if (child == 0) {
        close(fildes[0]); // close reading side

        dup2(fildes[1], 1); // redirect stdout to writing side of the pipe
        close(fildes[1]);

        // run the command
        execlp("filtraCargo", "filtraCargo", ficheiro, cargo, NULL);

        perror("filtraCargo");
        _exit(127);
    }

    child = fork();

    // child process
    if (child == 0) {
        close(fildes[1]); // close writing side

        dup2(fildes[0], 0); // redirect stdin to the reading side of the pipe
        close(fildes[0]);

        // run the command
        execlp("validaMin", "validaMin", NULL);

        perror("validaMin");
        _exit(127);
    }

    int status = 0;
    // wait for the child that ran "validaMin"
    waitpid(child, &status, 0);
    if (WIFEXITED(status) != 0) {
        return WEXITSTATUS(status);
    }

    return 1;
}

