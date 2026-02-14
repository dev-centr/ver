#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "config.h"

/* Handler interface structure */
typedef struct ver_handler {
    char *name;
    char *description;
    char *path;
    int priority;
    
    /* Handler functions */
    int (*init)(int argc, char *argv[]);
    int (*save)(int argc, char *argv[]);
    int (*stage)(int argc, char *argv[]);
    int (*status)(int argc, char *argv[]);
    int (*history)(int argc, char *argv[]);
    int (*sync)(int argc, char *argv[]);
    int (*branch)(int argc, char *argv[]);
    int (*merge)(int argc, char *argv[]);
    int (*custom)(int argc, char *argv[]);
    
    struct ver_handler *next;
} ver_handler;

/* Command dispatch function */
int ver_dispatch_command(ver_handler *handler, int argc, char *argv[]);

/* Handler management */
ver_handler *ver_handler_create(const char *name, const char *description);
void ver_handler_free(ver_handler *handler);
int ver_handler_register(ver_handler *handler);

#endif /* DISPATCHER_H */
