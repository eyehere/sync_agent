/*
 * sync_config.c
 *
 *  Created on: 2016年12月11日
 *      Author: luweijun
 */

#include "easy.h"
#include "sync_config.h"

static int sync_config_item_handler(char *key, char *value, void *userp);
static char *log_dst_strs[] = {"console", "file", "syslog"};

sync_config_t *sync_config_load(char *path)
{
	sync_config_t *config = NULL;
    struct stat stat_buf;

    bzero(&stat_buf, sizeof(struct stat));
    if (-1 == stat((const char *)path, &stat_buf)) {
        log_error("config file: %s is not existed.", path);
        return NULL;
    }

    config = (sync_config_t *)malloc(sizeof(sync_config_t));
    if (NULL == config) {
        log_error("no enough memory for sync_config_t.");
        return NULL;
    }
    bzero(config, sizeof(sync_config_t));

    if (!property_read(path, sync_config_item_handler, config)) {
        log_error("parse sync config file: %s failed.", path);
        sync_config_free(config);
        return NULL;
    }

    return config;
}

void sync_config_free(sync_config_t *config)
{
    int i = 0;

    if (NULL == config) {
        return;
    }

    if (config->log_file) {
        free(config->log_file);
    }

    free(config);
}

static int sync_config_item_handler(char *key, char *value, void *userp)
{
    sync_config_t *config = (sync_config_t *)userp;

    log_debug("sync_config_item_handler, %s: %s.", key, value);
    if (0 == strcasecmp(key, "daemon")) {
        if (0 == strcasecmp(value, "yes")) {
            config->daemon = 1;
        }
        else if(0 == strcasecmp(value, "no")) {
            config->daemon = 0;
        }
        else {
            log_error("unknown dameon value: %s.", value);
            return 0;
        }
    }
    else if (0 == strcasecmp(key, "log_level")) {
        config->log_level = log_level_int(value);

        if (-1 == config->log_level) {
            log_error("unknown log level: %s.", value);
            return 0;
        }
    }
    else if (0 == strcasecmp(key, "log_dst")) {
        if (0 == strcasecmp(value, "console")) {
            config->log_dst = LOG_DST_CONSOLE;
        }
        else if (0 == strcasecmp(value, "file")) {
            config->log_dst = LOG_DST_FILE;
        }
        else {
            log_error("unknown log_dst: %s.", value);
            return 0;
        }
    }
    else if (0 == strcasecmp(key, "log_file")) {
        config->log_file = (char *)malloc(strlen(value) + 1);
        if (NULL == config->log_file) {
            log_error("no enough memory for config->log_file.");
            return 0;
        }
        bzero(config->log_file, strlen(value) + 1);
        strcpy(config->log_file, value);
    }
    else{
        log_error("unknown config, %s: %s.", key, value);
        return 0;
    }

    return 1;
}

void sync_config_dump(sync_config_t *config)
{
    int i = 0;
    printf("========================= SYNC CONFIG ===================\n");
    printf("%-30s%s\n",   "daemon: ",                 config->daemon ? "yes":"no");
    printf("%-30s%s\n",   "log_level: ",              log_level_str(config->log_level));
    printf("%-30s%s\n",   "log_dst: ",                log_dst_strs[config->log_dst]);
    printf("%-30s%s\n",   "log_file: ",               config->log_file);
    printf("===========================================================\n");
}

