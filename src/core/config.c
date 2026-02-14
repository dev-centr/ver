/*
 * Ver - Universal Version Control Interface
 * Configuration management
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#else
#include <pwd.h>
#include <unistd.h>
#endif

#include "config.h"

static char *get_config_path(void) {
    char *path = NULL;
    
#ifdef _WIN32
    char app_data[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, app_data))) {
        path = malloc(strlen(app_data) + 20);
        sprintf(path, "%s\\ver\\config.json", app_data);
    }
#else
    const char *home = getenv("HOME");
    if (!home) {
        struct passwd *pw = getpwuid(getuid());
        if (pw) {
            home = pw->pw_dir;
        }
    }
    
    if (home) {
        path = malloc(strlen(home) + 20);
        sprintf(path, "%s/.config/ver/config.json", home);
    }
#endif
    
    return path;
}

ver_config *ver_config_load(void) {
    ver_config *config = calloc(1, sizeof(ver_config));
    if (!config) {
        return NULL;
    }
    
    /* Set defaults */
    config->default_provider = strdup("gitu");
    config->tui_enabled = 1;
    config->ai_mode = 0;
    config->debug = 0;
    config->config_file = get_config_path();
    
    /* Try to load existing configuration */
    FILE *file = fopen(config->config_file, "r");
    if (file) {
        /* TODO: Parse JSON configuration */
        /* For now, just use defaults */
        fclose(file);
    }
    
    return config;
}

void ver_config_free(ver_config *config) {
    if (!config) {
        return;
    }
    
    free(config->default_provider);
    free(config->config_file);
    free(config);
}

int ver_config_save(ver_config *config) {
    if (!config || !config->config_file) {
        return -1;
    }
    
    /* Create directory if it doesn't exist */
    char *dir = strdup(config->config_file);
    char *last_slash = strrchr(dir, '/');
#ifdef _WIN32
    if (!last_slash) last_slash = strrchr(dir, '\\');
#endif
    if (last_slash) {
        *last_slash = '\0';
#ifdef _WIN32
        _mkdir(dir);
#else
        mkdir(dir, 0755);
#endif
    }
    free(dir);
    
    FILE *file = fopen(config->config_file, "w");
    if (!file) {
        return -1;
    }
    
    /* TODO: Write JSON configuration */
    fprintf(file, "{\n");
    fprintf(file, "  \"default_provider\": \"%s\",\n", config->default_provider);
    fprintf(file, "  \"tui_enabled\": %s,\n", config->tui_enabled ? "true" : "false");
    fprintf(file, "  \"ai_mode\": %s,\n", config->ai_mode ? "true" : "false");
    fprintf(file, "  \"debug\": %s\n", config->debug ? "true" : "false");
    fprintf(file, "}\n");
    
    fclose(file);
    return 0;
}

const char *ver_config_get_default_provider(ver_config *config) {
    return config ? config->default_provider : "gitu";
}

int ver_config_get_tui_enabled(ver_config *config) {
    return config ? config->tui_enabled : 1;
}
