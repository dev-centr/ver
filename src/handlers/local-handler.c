/*
 * Ver - Universal Version Control Interface
 * Local file system handler implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>

#include "handler.h"

/* Local handler - manages file snapshots without remote */

static int local_handler_init(int argc, char *argv[]) {
    printf("Initializing local version control...\n");
    
    /* Create .ver directory */
    if (mkdir(".ver", 0755) != 0) {
        if (errno == EEXIST) {
            printf("Repository already initialized\n");
            return HANDLER_SUCCESS;
        } else {
            perror("Failed to create .ver directory");
            return HANDLER_ERROR;
        }
    }
    
    /* Create subdirectories */
    mkdir(".ver/snapshots", 0755);
    mkdir(".ver/metadata", 0755);
    
    /* Create initial metadata */
    FILE *meta = fopen(".ver/metadata/info", "w");
    if (meta) {
        fprintf(meta, "created: %ld\n", time(NULL));
        fprintf(meta, "type: local\n");
        fclose(meta);
    }
    
    printf("Local repository initialized\n");
    return HANDLER_SUCCESS;
}

static int local_handler_save(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: ver save <message>\n");
        return HANDLER_ERROR;
    }
    
    if (!directory_exists(".ver")) {
        fprintf(stderr, "Not a local repository. Use 'ver init' first.\n");
        return HANDLER_ERROR;
    }
    
    /* Create snapshot with timestamp */
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char snapshot_name[256];
    strftime(snapshot_name, sizeof(snapshot_name), "%Y%m%d_%H%M%S", tm_info);
    
    char snapshot_dir[512];
    snprintf(snapshot_dir, sizeof(snapshot_dir), ".ver/snapshots/%s", snapshot_name);
    
    if (mkdir(snapshot_dir, 0755) != 0) {
        perror("Failed to create snapshot directory");
        return HANDLER_ERROR;
    }
    
    /* Save snapshot metadata */
    char meta_file[512];
    snprintf(meta_file, sizeof(meta_file), "%s/metadata.txt", snapshot_dir);
    
    FILE *meta = fopen(meta_file, "w");
    if (meta) {
        fprintf(meta, "timestamp: %ld\n", now);
        fprintf(meta, "message: %s\n", argv[1]);
        fclose(meta);
    }
    
    /* TODO: Copy current files to snapshot directory */
    printf("Created snapshot: %s\n", snapshot_name);
    printf("Message: %s\n", argv[1]);
    
    return HANDLER_SUCCESS;
}

static int local_handler_stage(int argc, char *argv[]) {
    printf("Local handler doesn't use staging. All files are saved automatically.\n");
    return HANDLER_SUCCESS;
}

static int local_handler_status(int argc, char *argv[]) {
    if (!directory_exists(".ver")) {
        printf("Not a local repository.\n");
        return HANDLER_ERROR;
    }
    
    printf("Local repository status:\n");
    printf("Type: Local file snapshots\n");
    
    /* Count snapshots */
    int snapshot_count = 0;
    DIR *dir = opendir(".ver/snapshots");
    if (dir) {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                snapshot_count++;
            }
        }
        closedir(dir);
    }
    
    printf("Snapshots: %d\n", snapshot_count);
    return HANDLER_SUCCESS;
}

static int local_handler_history(int argc, char *argv[]) {
    if (!directory_exists(".ver")) {
        printf("Not a local repository.\n");
        return HANDLER_ERROR;
    }
    
    printf("Snapshot history:\n");
    printf("================\n");
    
    /* List snapshots with metadata */
    DIR *dir = opendir(".ver/snapshots");
    if (dir) {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                char meta_file[512];
                snprintf(meta_file, sizeof(meta_file), ".ver/snapshots/%s/metadata.txt", entry->d_name);
                
                FILE *meta = fopen(meta_file, "r");
                if (meta) {
                    char line[256];
                    printf("Snapshot: %s\n", entry->d_name);
                    while (fgets(line, sizeof(line), meta)) {
                        printf("  %s", line);
                    }
                    printf("\n");
                    fclose(meta);
                }
            }
        }
        closedir(dir);
    }
    
    return HANDLER_SUCCESS;
}

static int local_handler_sync(int argc, char *argv[]) {
    printf("Local handler doesn't support synchronization (no remote).\n");
    return HANDLER_SUCCESS;
}

static int local_handler_branch(int argc, char *argv[]) {
    printf("Local handler doesn't use branches.\n");
    return HANDLER_SUCCESS;
}

static int local_handler_merge(int argc, char *argv[]) {
    printf("Local handler doesn't support merging.\n");
    return HANDLER_SUCCESS;
}

static int local_handler_custom(int argc, char *argv[]) {
    printf("Local handler doesn't support custom commands.\n");
    return HANDLER_NOT_IMPLEMENTED;
}
