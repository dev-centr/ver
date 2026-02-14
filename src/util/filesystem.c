/*
 * Ver - Universal Version Control Interface
 * Filesystem utility functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#include "filesystem.h"

int directory_exists(const char *path) {
    struct stat st;
    return (stat(path, &st) == 0 && S_ISDIR(st.st_mode));
}

int file_exists(const char *path) {
    struct stat st;
    return (stat(path, &st) == 0);
}

int create_directory(const char *path) {
    return mkdir(path, 0755);
}

int create_directory_recursive(const char *path) {
    if (!path || !*path) return -1;
    
    char *path_copy = strdup(path);
    char *p = path_copy;
    
    /* Skip leading slash */
    if (*p == '/') p++;
    
    while (*p) {
        while (*p && *p != '/') p++;
        
        if (*p) {
            *p = '\0';
            
            if (!directory_exists(path_copy)) {
                if (create_directory(path_copy) != 0) {
                    free(path_copy);
                    return -1;
                }
            }
            
            *p = '/';
            p++;
        }
    }
    
    /* Create final directory */
    if (!directory_exists(path_copy)) {
        if (create_directory(path_copy) != 0) {
            free(path_copy);
            return -1;
        }
    }
    
    free(path_copy);
    return 0;
}

char *get_current_directory(void) {
    char cwd[4096];
    if (getcwd(cwd, sizeof(cwd))) {
        return strdup(cwd);
    }
    return NULL;
}

int change_directory(const char *path) {
    return chdir(path);
}

char *get_absolute_path(const char *path) {
    if (!path) return NULL;
    
    if (*path == '/') {
        /* Already absolute */
        return strdup(path);
    }
    
    char *cwd = get_current_directory();
    if (!cwd) return NULL;
    
    char *result = malloc(strlen(cwd) + strlen(path) + 2);
    if (result) {
        sprintf(result, "%s/%s", cwd, path);
    }
    
    free(cwd);
    return result;
}

int copy_file(const char *src, const char *dest) {
    FILE *source, *destination;
    char buffer[4096];
    size_t bytes;
    
    source = fopen(src, "rb");
    if (!source) return -1;
    
    destination = fopen(dest, "wb");
    if (!destination) {
        fclose(source);
        return -1;
    }
    
    while ((bytes = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        if (fwrite(buffer, 1, bytes, destination) != bytes) {
            fclose(source);
            fclose(destination);
            return -1;
        }
    }
    
    fclose(source);
    fclose(destination);
    return 0;
}

int copy_directory_recursive(const char *src, const char *dest) {
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char src_path[4096], dest_path[4096];
    
    /* Create destination directory */
    if (create_directory_recursive(dest) != 0) {
        return -1;
    }
    
    dir = opendir(src);
    if (!dir) return -1;
    
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        snprintf(src_path, sizeof(src_path), "%s/%s", src, entry->d_name);
        snprintf(dest_path, sizeof(dest_path), "%s/%s", dest, entry->d_name);
        
        if (stat(src_path, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                /* Recursively copy subdirectory */
                if (copy_directory_recursive(src_path, dest_path) != 0) {
                    closedir(dir);
                    return -1;
                }
            } else {
                /* Copy file */
                if (copy_file(src_path, dest_path) != 0) {
                    closedir(dir);
                    return -1;
                }
            }
        }
    }
    
    closedir(dir);
    return 0;
}

char **list_directory(const char *path, int *count) {
    DIR *dir;
    struct dirent *entry;
    char **result = NULL;
    int capacity = 16;
    
    *count = 0;
    
    dir = opendir(path);
    if (!dir) return NULL;
    
    result = malloc(capacity * sizeof(char *));
    if (!result) {
        closedir(dir);
        return NULL;
    }
    
    while ((entry = readdir(dir)) != NULL) {
        if (*count >= capacity) {
            capacity *= 2;
            char **new_result = realloc(result, capacity * sizeof(char *));
            if (!new_result) {
                closedir(dir);
                for (int i = 0; i < *count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        result[*count] = strdup(entry->d_name);
        (*count)++;
    }
    
    closedir(dir);
    return result;
}

void free_string_array(char **array, int count) {
    if (!array) return;
    
    for (int i = 0; i < count; i++) {
        free(array[i]);
    }
    free(array);
}
