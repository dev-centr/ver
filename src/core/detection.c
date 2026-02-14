/*
 * Ver - Universal Version Control Interface
 * Provider detection and handler initialization
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "detection.h"
#include "dispatcher.h"
#include "config.h"

static int directory_exists(const char *path) {
    struct stat st;
    return (stat(path, &st) == 0 && S_ISDIR(st.st_mode));
}

int ver_is_git_repository(void) {
    return directory_exists(".git");
}

int ver_is_svn_repository(void) {
    return directory_exists(".svn");
}

int ver_is_hg_repository(void) {
    return directory_exists(".hg");
}

int ver_is_ver_repository(void) {
    return directory_exists(".ver");
}

ver_handler *ver_detect_handler(ver_config *config) {
    ver_handler *handler = NULL;
    
    /* Check for repositories in order of preference */
    if (ver_is_git_repository()) {
        /* Prefer gitu over standard git */
        handler = ver_init_gitu_handler();
        if (!handler) {
            handler = ver_init_git_handler();
        }
    } else if (ver_is_svn_repository()) {
        handler = ver_init_svn_handler();
    } else if (ver_is_hg_repository()) {
        handler = ver_init_hg_handler();
    } else if (ver_is_ver_repository()) {
        handler = ver_init_local_handler();
    } else {
        /* No repository detected - use default provider */
        const char *default_provider = ver_config_get_default_provider(config);
        
        if (strcmp(default_provider, "gitu") == 0) {
            handler = ver_init_gitu_handler();
        } else if (strcmp(default_provider, "git") == 0) {
            handler = ver_init_git_handler();
        } else if (strcmp(default_provider, "local") == 0) {
            handler = ver_init_local_handler();
        }
    }
    
    return handler;
}

/* Forward declarations of handler functions */
static int gitu_init(int argc, char *argv[]);
static int gitu_save(int argc, char *argv[]);
static int gitu_stage(int argc, char *argv[]);
static int gitu_status(int argc, char *argv[]);
static int gitu_history(int argc, char *argv[]);
static int gitu_sync(int argc, char *argv[]);
static int gitu_branch(int argc, char *argv[]);
static int gitu_merge(int argc, char *argv[]);
static int gitu_custom(int argc, char *argv[]);

ver_handler *ver_init_gitu_handler(void) {
    ver_handler *handler = ver_handler_create("gitu", "Git Unfuckified Interface");
    if (!handler) {
        return NULL;
    }
    
    handler->priority = 10; /* High priority */
    handler->init = gitu_init;
    handler->save = gitu_save;
    handler->stage = gitu_stage;
    handler->status = gitu_status;
    handler->history = gitu_history;
    handler->sync = gitu_sync;
    handler->branch = gitu_branch;
    handler->merge = gitu_merge;
    handler->custom = gitu_custom;
    
    return handler;
}

/* Gitu command implementations */
static int execute_gitu_command(int argc, char *argv[]) {
    char **args = malloc((argc + 2) * sizeof(char *));
    if (!args) {
        return -1;
    }
    
    args[0] = "gitu";
    for (int i = 0; i < argc; i++) {
        args[i + 1] = argv[i];
    }
    args[argc + 1] = NULL;
    
    /* Execute gitu command */
    pid_t pid = fork();
    if (pid == 0) {
        execvp("gitu", args);
        exit(1);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        free(args);
        return WEXITSTATUS(status);
    } else {
        free(args);
        return -1;
    }
}

static int gitu_init(int argc, char *argv[]) {
    char *new_argv[] = { "init", NULL };
    return execute_gitu_command(1, new_argv);
}

static int gitu_save(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: ver save <message>\n");
        return 1;
    }
    return execute_gitu_command(argc, argv);
}

static int gitu_stage(int argc, char *argv[]) {
    return execute_gitu_command(argc, argv);
}

static int gitu_status(int argc, char *argv[]) {
    char *new_argv[] = { "status", NULL };
    return execute_gitu_command(1, new_argv);
}

static int gitu_history(int argc, char *argv[]) {
    char *new_argv[] = { "history", NULL };
    return execute_gitu_command(1, new_argv);
}

static int gitu_sync(int argc, char *argv[]) {
    char *new_argv[] = { "sync", NULL };
    return execute_gitu_command(1, new_argv);
}

static int gitu_branch(int argc, char *argv[]) {
    return execute_gitu_command(argc, argv);
}

static int gitu_merge(int argc, char *argv[]) {
    return execute_gitu_command(argc, argv);
}

static int gitu_custom(int argc, char *argv[]) {
    return execute_gitu_command(argc, argv);
}

/* Placeholder handlers for other VCS systems */
ver_handler *ver_init_git_handler(void) {
    /* TODO: Implement standard git handler */
    return NULL;
}

ver_handler *ver_init_svn_handler(void) {
    /* TODO: Implement SVN handler */
    return NULL;
}

ver_handler *ver_init_hg_handler(void) {
    /* TODO: Implement Mercurial handler */
    return NULL;
}

ver_handler *ver_init_local_handler(void) {
    /* TODO: Implement local file system handler */
    return NULL;
}
