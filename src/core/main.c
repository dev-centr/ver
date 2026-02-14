/*
 * Ver - Universal Version Control Interface
 * Main entry point and command-line interface
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dispatcher.h"
#include "config.h"
#include "detection.h"

static void show_help(void) {
    printf("Ver - Universal Version Control Interface\n\n");
    printf("Usage: ver <command> [options]\n\n");
    printf("Available commands:\n");
    printf("  init         Initialize a new repository\n");
    printf("  save <msg>   Save changes with message\n");
    printf("  stage <file> Stage files for commit\n");
    printf("  status       Show repository status\n");
    printf("  history      Show commit history\n");
    printf("  sync         Synchronize with remote\n");
    printf("  branch       Manage branches\n");
    printf("  merge        Merge branches\n");
    printf("  help         Show this help message\n");
    printf("\nProvider-specific commands are also supported.\n");
    printf("Run 'ver help <command>' for detailed help.\n");
}

static void show_version(void) {
    printf("Ver version 1.0.0\n");
    printf("Universal Version Control Interface\n");
}

int main(int argc, char *argv[]) {
    ver_config *config;
    ver_handler *handler;
    int result = 0;
    
    /* Load configuration */
    config = ver_config_load();
    if (!config) {
        fprintf(stderr, "Error: Failed to load configuration\n");
        return 1;
    }
    
    /* Handle special commands */
    if (argc < 2) {
        show_help();
        ver_config_free(config);
        return 0;
    }
    
    if (strcmp(argv[1], "help") == 0 || strcmp(argv[1], "--help") == 0) {
        show_help();
        ver_config_free(config);
        return 0;
    }
    
    if (strcmp(argv[1], "version") == 0 || strcmp(argv[1], "--version") == 0) {
        show_version();
        ver_config_free(config);
        return 0;
    }
    
    /* Detect the appropriate handler */
    handler = ver_detect_handler(config);
    if (!handler) {
        fprintf(stderr, "Error: No version control system detected\n");
        fprintf(stderr, "Use 'ver init' to initialize a repository\n");
        ver_config_free(config);
        return 1;
    }
    
    printf("Using handler: %s\n", handler->name);
    
    /* Dispatch command to handler */
    result = ver_dispatch_command(handler, argc - 1, argv + 1);
    
    /* Cleanup */
    ver_config_free(config);
    
    return result;
}
