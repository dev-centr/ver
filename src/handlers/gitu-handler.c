/*
 * Ver - Universal Version Control Interface
 * Gitu handler implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "handler.h"
#include "../core/detection.h"

/* Gitu-specific handler implementation */
/* This provides direct integration with gitu without subprocess overhead */

static int gitu_handler_init(int argc, char *argv[]) {
    printf("Initializing gitu repository...\n");
    
    /* Check if already a git repository */
    if (ver_is_git_repository()) {
        printf("Repository already initialized\n");
        return HANDLER_SUCCESS;
    }
    
    /* Execute git init */
    char *git_args[] = { "git", "init", NULL };
    return handler_execute_command("git", git_args, NULL);
}

static int gitu_handler_save(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: ver save <message>\n");
        return HANDLER_ERROR;
    }
    
    printf("Saving changes: %s\n", argv[1]);
    
    /* Stage all changes first */
    char *add_args[] = { "gitu", "stage", "all", NULL };
    int result = handler_execute_command("gitu", add_args, NULL);
    if (result != 0) {
        return result;
    }
    
    /* Commit with message */
    char *commit_args[] = { "gitu", "save", argv[1], NULL };
    return handler_execute_command("gitu", commit_args, NULL);
}

static int gitu_handler_stage(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: ver stage <file|all>\n");
        return HANDLER_ERROR;
    }
    
    printf("Staging: %s\n", argv[1]);
    
    char *stage_args[] = { "gitu", "stage", argv[1], NULL };
    return handler_execute_command("gitu", stage_args, NULL);
}

static int gitu_handler_status(int argc, char *argv[]) {
    printf("Repository status:\n");
    
    char *status_args[] = { "gitu", "status", NULL };
    return handler_execute_command("gitu", status_args, NULL);
}

static int gitu_handler_history(int argc, char *argv[]) {
    printf("Commit history:\n");
    
    char *history_args[] = { "gitu", "history", NULL };
    return handler_execute_command("gitu", history_args, NULL);
}

static int gitu_handler_sync(int argc, char *argv[]) {
    printf("Synchronizing with remote...\n");
    
    /* Pull first, then push */
    char *pull_args[] = { "gitu", "download", NULL };
    int result = handler_execute_command("gitu", pull_args, NULL);
    if (result != 0) {
        return result;
    }
    
    char *push_args[] = { "gitu", "upload", NULL };
    return handler_execute_command("gitu", push_args, NULL);
}

static int gitu_handler_branch(int argc, char *argv[]) {
    if (argc < 2) {
        /* List branches */
        char *list_args[] = { "gitu", "branch", NULL };
        return handler_execute_command("gitu", list_args, NULL);
    }
    
    if (argc == 3 && strcmp(argv[1], "create") == 0) {
        /* Create new branch */
        printf("Creating branch: %s\n", argv[2]);
        char *create_args[] = { "gitu", "branch", "create", argv[2], NULL };
        return handler_execute_command("gitu", create_args, NULL);
    }
    
    if (argc == 3 && strcmp(argv[1], "switch") == 0) {
        /* Switch to branch */
        printf("Switching to branch: %s\n", argv[2]);
        char *switch_args[] = { "gitu", "branch", "switch", argv[2], NULL };
        return handler_execute_command("gitu", switch_args, NULL);
    }
    
    fprintf(stderr, "Usage: ver branch [create|switch] <name>\n");
    return HANDLER_ERROR;
}

static int gitu_handler_merge(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: ver merge <branch>\n");
        return HANDLER_ERROR;
    }
    
    printf("Merging branch: %s\n", argv[1]);
    
    char *merge_args[] = { "gitu", "merge", argv[1], NULL };
    return handler_execute_command("gitu", merge_args, NULL);
}

static int gitu_handler_custom(int argc, char *argv[]) {
    /* Pass through to gitu directly */
    return handler_execute_command("gitu", argv, NULL);
}

/* Initialize and register the gitu handler */
ver_handler *ver_init_gitu_handler_advanced(void) {
    ver_handler *handler = ver_handler_create("gitu", "Git Unfuckified Interface");
    if (!handler) {
        return NULL;
    }
    
    handler->priority = 10;
    handler->init = gitu_handler_init;
    handler->save = gitu_handler_save;
    handler->stage = gitu_handler_stage;
    handler->status = gitu_handler_status;
    handler->history = gitu_handler_history;
    handler->sync = gitu_handler_sync;
    handler->branch = gitu_handler_branch;
    handler->merge = gitu_handler_merge;
    handler->custom = gitu_handler_custom;
    
    return handler;
}
