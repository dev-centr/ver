/*
 * Ver - Universal Version Control Interface
 * Command dispatcher and handler management
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dispatcher.h"
#include "config.h"

static ver_handler *registered_handlers = NULL;

int ver_dispatch_command(ver_handler *handler, int argc, char *argv[]) {
    if (!handler || argc < 1) {
        return -1;
    }
    
    const char *command = argv[0];
    
    /* Map ver commands to handler functions */
    if (strcmp(command, "init") == 0) {
        if (handler->init) {
            return handler->init(argc, argv);
        }
    } else if (strcmp(command, "save") == 0) {
        if (handler->save) {
            return handler->save(argc, argv);
        }
    } else if (strcmp(command, "stage") == 0) {
        if (handler->stage) {
            return handler->stage(argc, argv);
        }
    } else if (strcmp(command, "status") == 0) {
        if (handler->status) {
            return handler->status(argc, argv);
        }
    } else if (strcmp(command, "history") == 0) {
        if (handler->history) {
            return handler->history(argc, argv);
        }
    } else if (strcmp(command, "sync") == 0) {
        if (handler->sync) {
            return handler->sync(argc, argv);
        }
    } else if (strcmp(command, "branch") == 0) {
        if (handler->branch) {
            return handler->branch(argc, argv);
        }
    } else if (strcmp(command, "merge") == 0) {
        if (handler->merge) {
            return handler->merge(argc, argv);
        }
    } else {
        /* Pass custom commands to handler */
        if (handler->custom) {
            return handler->custom(argc, argv);
        }
    }
    
    fprintf(stderr, "Error: Command '%s' not supported by %s handler\n", 
            command, handler->name);
    return -1;
}

ver_handler *ver_handler_create(const char *name, const char *description) {
    ver_handler *handler = calloc(1, sizeof(ver_handler));
    if (!handler) {
        return NULL;
    }
    
    handler->name = strdup(name);
    handler->description = strdup(description);
    handler->priority = 0;
    
    return handler;
}

void ver_handler_free(ver_handler *handler) {
    if (!handler) {
        return;
    }
    
    free(handler->name);
    free(handler->description);
    free(handler->path);
    free(handler);
}

int ver_handler_register(ver_handler *handler) {
    if (!handler) {
        return -1;
    }
    
    /* Insert handler in priority order */
    ver_handler *current = registered_handlers;
    ver_handler *prev = NULL;
    
    while (current && current->priority >= handler->priority) {
        prev = current;
        current = current->next;
    }
    
    if (prev) {
        prev->next = handler;
    } else {
        registered_handlers = handler;
    }
    
    handler->next = current;
    
    return 0;
}

ver_handler *ver_get_handlers(void) {
    return registered_handlers;
}
