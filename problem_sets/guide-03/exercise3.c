#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int better_system(const char *command) {
    int res = -1;
    if (command != NULL) {
        pid_t child = fork();
        int out = 0;

        if (child == 0) {
            char *line = strdup(command);
            
            char **tokens = malloc(sizeof(char *));
            int i = 0;
            while ((tokens[i] = strsep(&line, " ")) != NULL) {
                i++;
                tokens = realloc(tokens, sizeof(int) * (i + 2));
            }
            i++;

            execvp(tokens[0], tokens);

            // error handling
            perror(tokens[0]);
            _exit(127);
        } else {
            int status = 0;
            child = wait(&status);
            if (child == -1) {
                perror("wait()");
                return res;
            }

            if (WIFEXITED(status) != 0) {
                // exec() went wrong
                res = WEXITSTATUS(status);
                
                if (res == 127) {
                    printf("Something went wrong with process %d\n", child);
                }
            }
        }
    }

    return res;
}

int main(void) {

    char comando1[] = "ls -l -a -h";
    char comando2[] = "sleeep 30";
	char comando3[] = "sleep 10";
	char comando4[] = "ps";
	int ret;

	printf("\nCOMANDO 1: a executar better_system para %s\n",comando1);
	ret = better_system(comando1);
	printf("COMANDO 1: ret = %d\n",ret);

	printf("\nCOMANDO 2: a executar better_system para %s\n",comando2);
    ret = better_system(comando2);
    printf("COMANDO 2: ret = %d\n",ret);

	printf("\nCOMANDO 3: a executar better_system para %s\n",comando3);
	ret = better_system(comando3);
    printf("COMANDO 3: ret = %d\n",ret);

    printf("\nCOMANDO 4: a executar better_system para %s\n",comando4);
	ret = better_system(comando4);
    printf("COMANDO 4: ret = %d\n",ret);

    return 0;
}