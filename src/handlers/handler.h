#ifndef HANDLER_H
#define HANDLER_H

/* Common handler interface and utilities */

/* Handler function return codes */
#define HANDLER_SUCCESS 0
#define HANDLER_ERROR 1
#define HANDLER_NOT_IMPLEMENTED 2

/* Common handler context */
typedef struct handler_context {
    char *working_directory;
    char *config_file;
    int verbose;
    int dry_run;
} handler_context;

/* Handler utility functions */
handler_context *handler_context_create(void);
void handler_context_free(handler_context *ctx);
int handler_execute_command(const char *command, char *const argv[], handler_context *ctx);
int handler_check_repository(const char *type);

#endif /* HANDLER_H */
