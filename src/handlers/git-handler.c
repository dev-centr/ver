/*
 * Ver - Universal Version Control Interface
 * Standard Git handler implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "handler.h"

/* Standard git handler implementation */

static int git_handler_init(int argc, char *argv[]) {
    printf("Initializing git repository...\n");
    
    char *git_args[] = { "git", "init", NULL };
    return handler_execute_command("git", git_args, NULL);
}

static int git_handler_save(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: ver save <message>\n");
        return HANDLER_ERROR;
    }
    
    printf("Committing changes: %s\n", argv[1]);
    
    char *commit_args[] = { "git", "commit", "-m", argv[1], NULL };
    return handler_execute_command("git", commit_args, NULL);
}

static int git_handler_stage(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: ver stage <file|all>\n");
        return HANDLER_ERROR;
    }
    
    if (strcmp(argv[1], "all") == 0) {
        printf("Staging all changes...\n");
        char *add_args[] = { "git", "add", ".", NULL };
        return handler_execute_command("git", add_args, NULL);
    } else {
        printf("Staging file: %s\n", argv[1]);
        char *add_args[] = { "git", "add", argv[1], NULL };
        return handler_execute_command("git", add_args, NULL);
    }
}

static int git_handler_status(int argc, char *argv[]) {
    printf("Repository status:\n");
    
    char *status_args[] = { "git", "status", NULL };
    return handler_execute_command("git", status_args, NULL);
}

static int git_handler_history(int argc, char *argv[]) {
    printf("Commit history:\n");
    
    char *log_args[] = { "git", "log", "--oneline", "-10", NULL };
    return handler_execute_command("git", log_args, NULL);
}

static int git_handler_sync(int argc, char *argv[]) {
    printf("Synchronizing with remote...\n");
    
    /* Pull first */
    char *pull_args[] = { "git", "pull", NULL };
    int result = handler_execute_command("git", pull_args, NULL);
    if (result != 0) {
        return result;
    }
    
    /* Then push */
    char *push_args[] = { "git", "push", NULL };
    return handler_execute_command("git", push_args, NULL);
}

static int git_handler_branch(int argc, char *argv[]) {
    if (argc < 2) {
        /* List branches */
        char *branch_args[] = { "git", "branch", "-a", NULL };
        return handler_execute_command("git", branch_args, NULL);
    }
    
    if (argc == 3 && strcmp(argv[1], "create") == 0) {
        /* Create and switch to new branch */
        printf("Creating and switching to branch: %s\n", argv[2]);
        char *checkout_args[] = { "git", "checkout", "-b", argv[2], NULL };
        return handler_execute_command("git", checkout_args, NULL);
    }
    
    if (argc == 3 && strcmp(argv[1], "switch") == 0) {
        /* Switch to existing branch */
        printf("Switching to branch: %s\n", argv[2]);
        char *checkout_args[] = { "git", "checkout", argv[2], NULL };
        return handler_execute_command("git", checkout_args, NULL);
    }
    
    fprintf(stderr, "Usage: ver branch [create|switch] <name>\n");
    return HANDLER_ERROR;
}

static int git_handler_merge(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: ver merge <branch>\n");
        return HANDLER_ERROR;
    }
    
    printf("Merging branch: %s\n", argv[1]);
    
    char *merge_args[] = { "git", "merge", argv[1], NULL };
    return handler_execute_command("git", merge_args, NULL);
}

static int git_handler_custom(int argc, char *argv[]) {
    /* Pass through to git directly */
    return handler_execute_command("git", argv, NULL);
}
