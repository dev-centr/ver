/*
 * Ver - Universal Version Control Interface
 * Common handler utilities
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "handler.h"

handler_context *handler_context_create(void) {
    handler_context *ctx = calloc(1, sizeof(handler_context));
    if (!ctx) {
        return NULL;
    }
    
    ctx->working_directory = getcwd(NULL, 0);
    ctx->verbose = 0;
    ctx->dry_run = 0;
    
    return ctx;
}

void handler_context_free(handler_context *ctx) {
    if (!ctx) {
        return;
    }
    
    free(ctx->working_directory);
    free(ctx->config_file);
    free(ctx);
}

int handler_execute_command(const char *command, char *const argv[], handler_context *ctx) {
    if (!command || !argv) {
        return -1;
    }
    
    if (ctx && ctx->dry_run) {
        printf("DRY RUN: %s", command);
        for (int i = 1; argv[i]; i++) {
            printf(" %s", argv[i]);
        }
        printf("\n");
        return 0;
    }
    
    if (ctx && ctx->verbose) {
        printf("Executing: %s", command);
        for (int i = 1; argv[i]; i++) {
            printf(" %s", argv[i]);
        }
        printf("\n");
    }
    
    pid_t pid = fork();
    if (pid == 0) {
        /* Child process */
        execvp(command, argv);
        perror("execvp failed");
        exit(127);
    } else if (pid > 0) {
        /* Parent process */
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            fprintf(stderr, "Command terminated by signal %d\n", WTERMSIG(status));
            return 128 + WTERMSIG(status);
        } else {
            fprintf(stderr, "Command terminated abnormally\n");
            return 1;
        }
    } else {
        perror("fork failed");
        return -1;
    }
}

int handler_check_repository(const char *type) {
    struct stat st;
    
    if (strcmp(type, "git") == 0) {
        return (stat(".git", &st) == 0 && S_ISDIR(st.st_mode));
    } else if (strcmp(type, "svn") == 0) {
        return (stat(".svn", &st) == 0 && S_ISDIR(st.st_mode));
    } else if (strcmp(type, "hg") == 0) {
        return (stat(".hg", &st) == 0 && S_ISDIR(st.st_mode));
    } else if (strcmp(type, "ver") == 0) {
        return (stat(".ver", &st) == 0 && S_ISDIR(st.st_mode));
    }
    
    return 0;
}
