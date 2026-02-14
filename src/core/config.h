#ifndef CONFIG_H
#define CONFIG_H

/* Configuration structure */
typedef struct ver_config {
    char *default_provider;
    char *config_file;
    int tui_enabled;
    int ai_mode;
    int debug;
    struct ver_config *next;
} ver_config;

/* Configuration functions */
ver_config *ver_config_load(void);
void ver_config_free(ver_config *config);
int ver_config_save(ver_config *config);
const char *ver_config_get_default_provider(ver_config *config);
int ver_config_get_tui_enabled(ver_config *config);

#endif /* CONFIG_H */
