#ifndef FILESYSTEM_H
#define FILESYSTEM_H

/* Filesystem utility functions */

int directory_exists(const char *path);
int file_exists(const char *path);
int create_directory(const char *path);
int create_directory_recursive(const char *path);

char *get_current_directory(void);
int change_directory(const char *path);
char *get_absolute_path(const char *path);

int copy_file(const char *src, const char *dest);
int copy_directory_recursive(const char *src, const char *dest);

char **list_directory(const char *path, int *count);
void free_string_array(char **array, int count);

#endif /* FILESYSTEM_H */
