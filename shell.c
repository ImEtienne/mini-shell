#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#define BUFFER_LENGTH 1024


// Fonction pour découper la ligne en arguments
int parse_line(char *s, char **argv[]) {
    int arg_count = 0;
    char *arg;
    int space_start = 0;
 
    if (s == NULL || *s == '\0') {
        return 1; // Chaîne vide ou NULL
    }
 
    for (int i = 0; s[i] != '\0'; i++) {
        if (isspace(s[i])) {
            if (i - space_start > 0) {
                // Alloue de la mémoire pour l'argument et le copie depuis la ligne
                arg = (char *)malloc(sizeof(char) * (i - space_start + 1));
                int j = 0;
                for (char *ptr = s + space_start; j < i - space_start; j++, ptr++) {
                    arg[j] = *ptr;
                }

                // Ajoute le caractère de fin de chaîne à l'argument
                arg[i - space_start] = '\0';
 
                // Ajoute l'argument à la liste d'arguments
                (*argv)[arg_count] = arg;
                arg_count++;
 
                // Limite le nombre d'arguments pour éviter un dépassement de tableau
                if (arg_count >= 16) {
                    return -1; // Trop d'arguments
                }
            }
 
            // Met à jour la position du prochain argument dans la ligne
            space_start = i + 1;
        }
 
        // Gestion de la redirection : si on trouve '>', marque la fin des arguments
        if (s[i] == '>') {
            (*argv)[arg_count] = NULL;
            arg_count++;
        }
    }
 
    // Traite le dernier argument s'il existe
    if (space_start < strlen(s)) {
        arg = (char *)malloc(sizeof(char) * (strlen(s) - space_start + 1));
        for (int j = 0; j < strlen(s) - space_start; j++) {
            arg[j] = s[j + space_start];
        }
 
        arg[strlen(s) - space_start] = '\0';
 
        (*argv)[arg_count] = arg;
        arg_count++;
    }
 
    // Marque la fin de la liste d'arguments
    (*argv)[arg_count] = NULL;
    return arg_count; // Retourne le nombre d'arguments
}


int main() {
    char buffer[BUFFER_LENGTH];
    int bytes_read;
    int pid;
    int argument_count;
    char **command_arguments = malloc(sizeof(char *) * BUFFER_LENGTH);
    char **pipe_arguments = malloc(sizeof(char *) * BUFFER_LENGTH);
    char *path = malloc(sizeof(char *) * BUFFER_LENGTH);
     
    struct sigaction sa;
    sa.sa_handler = SIG_IGN; // Ignore le signal
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

     if (command_arguments == NULL || pipe_arguments == NULL) {
        perror("Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Erreur signal");
        return -1;
    }

    while (1)
    {//Tourne en continue
        path = getcwd(path, BUFFER_LENGTH);
        if((write(STDOUT_FILENO, path, strlen(path))) == -1){
            perror("Error");
            exit(1);
        }
        if (write(STDOUT_FILENO, " $ ", sizeof(" $ ")) == -1 ){
            perror("Error");
            exit(1);
        }

        bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
        buffer[bytes_read] = '\0';

        // Appelle la fonction de découpage de ligne
        argument_count = parse_line(buffer, &command_arguments);
        
        if ((pid = fork()) == 0) 
        { // Processus fils
            int sub_pid;
            int pipefd[2];

            sigset_t new_mask, old_mask;
            sigemptyset(&new_mask);
            sigaddset(&new_mask, SIGINT);

            // Bloquer SIGINT temporairement pendant la configuration du gestionnaire
            if (sigprocmask(SIG_BLOCK, &new_mask, &old_mask) == -1) {
                perror("Erreur SIGINT bloquant");
                return -1;
            }

            // Configurer le gestionnaire de signal pour le processus fils
            struct sigaction child_sa;
            child_sa.sa_handler = SIG_DFL; // Rétablir le gestionnaire par défaut pour les processus fils
            sigemptyset(&child_sa.sa_mask);
            child_sa.sa_flags = 0;

            if (sigaction(SIGINT, &child_sa, NULL) == -1) {
                perror("Error signal child");
                return -1;
            }

            // Restaurer le masque de signal précédent
            if (sigprocmask(SIG_SETMASK, &old_mask, NULL) == -1) {
                perror("Error signal mask");
                return -1;
            }

            if (strpbrk(buffer, "|")) 
            {//Pipe
                command_arguments[argument_count - 1] = NULL;
                 path = getcwd(path, BUFFER_LENGTH);
                 if((write(STDOUT_FILENO, path, strlen(path))) == -1 ){
                        perror("Error");
                        exit(1);
                 }
                write(STDOUT_FILENO, " > ", 3);
                int bytes_read_2 = read(STDIN_FILENO, (void *) buffer, sizeof(buffer));
                buffer[bytes_read_2] = '\0';

                parse_line(buffer, &pipe_arguments);
                pipe(pipefd);

                if ((sub_pid = fork() != 0)) {
                    wait(&sub_pid);
                    dup2(pipefd[0], STDIN_FILENO);
                    close(pipefd[0]);
                    close(pipefd[1]);
                    execvp(pipe_arguments[0], pipe_arguments);
                }
            }

            if (sub_pid == 0) 
            {// verif sub_pid important
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
            }

            if (strpbrk(buffer, ">")) 
            { // Gestion de la redirection
                int fd_out = open(command_arguments[argument_count - 1], O_RDWR | O_CREAT | O_TRUNC, 0666);
                dup2(fd_out, STDOUT_FILENO);
                close(fd_out);
            }

            execvp(command_arguments[0], command_arguments); // Exécution de la commande
        } else if (pid < 0) 
        {
            perror("Erreur Fork");
        } else 
        { // Processus parent
            wait(&pid);
        }

         if (strncmp(buffer, "exit", 4) == 0) { // Utiliser 4 comme longueur pour comparer "exit"
        // Libérer la mémoire allouée pour les arguments
            for (size_t i = 0; i < BUFFER_LENGTH; i++) {
                free(command_arguments[i]);
                free(pipe_arguments[i]);
            }

            // Libérer la mémoire allouée pour les tableaux eux-mêmes
            free(command_arguments);
            free(pipe_arguments);

            // Quitter le programme
            exit(EXIT_SUCCESS);
        }
    }

    return 0;
}
