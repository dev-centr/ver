#ifndef DETECTION_H
#define DETECTION_H

#include "config.h"
#include "dispatcher.h"

/* Provider detection functions */
ver_handler *ver_detect_handler(ver_config *config);
int ver_is_git_repository(void);
int ver_is_svn_repository(void);
int ver_is_hg_repository(void);
int ver_is_ver_repository(void);

/* Handler initialization */
ver_handler *ver_init_gitu_handler(void);
ver_handler *ver_init_git_handler(void);
ver_handler *ver_init_svn_handler(void);
ver_handler *ver_init_hg_handler(void);
ver_handler *ver_init_local_handler(void);

#endif /* DETECTION_H */
