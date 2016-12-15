/*
  +----------------------------------------------------------------------+
  | sync_agent                                                           |
  +----------------------------------------------------------------------+
  | this source file is subject to version 2.0 of the apache license,    |
  | that is bundled with this package in the file license, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.apache.org/licenses/license-2.0.html                      |
  | if you did not receive a copy of the apache2.0 license and are unable|
  | to obtain it through the world-wide-web, please send a note to       |
  | yiming_6weijun@163.com so we can mail you a copy immediately.        |
  +----------------------------------------------------------------------+
  | author: weijun lu  <yiming_6weijun@163.com>                          |
  +----------------------------------------------------------------------+
*/

#include "easy.h"
#include "sync_config.h"

static char *log_dst_strs[] = {"console", "file", "syslog"};

static int sync_config_item_handler(char *key, char *value, void *userp);
static hashset_t watch_set_handler(char *watch_path);
static hashmap_t* subscribe_map_handler(char *subscribe_path);
void watch_set_free(void *data);

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

    if (config->mode) {
    	free(config->mode);
    }

    if (config->watch_path) {
    	free(config->watch_path);
    }

    if (config->subscribe_path) {
    	free(config->subscribe_path);
    }

    if (config->watch_set) {
    	watch_set_free(config->watch_set);
    }

    if (config->subscribe_map) {
    	hashmap_delete(config->subscribe_map);
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
    else if (0 == strcasecmp(key, "mode")) {
    	config->mode = (char *)malloc(strlen(value) + 1);
    	if ( NULL == config->mode ) {
    		log_error("malloc memory for config->mode error.");
    		return 0;
    	}
    	bzero(config->mode, strlen(value) + 1);
    	strcpy(config->mode, value);
    }
    else if (0 == strcasecmp(key, "port")) {
    	config->port = atoi(value);
    }
    else if (0 == strcasecmp(key, "watch_path")) {
    	config->watch_path = (char*)malloc(strlen(value) + 1);
    	if (NULL == config->watch_path) {
    		log_error("malloc memory for config->sync_path error.");
    		return 0;
    	}
    	bzero(config->watch_path, strlen(value) + 1);
    	strcpy(config->watch_path, value);
    	if (strlen(config->watch_path) > 0) {
    		config->watch_set = watch_set_handler(config->watch_path);
    	}
    }
    else if (0 == strcasecmp(key, "subscribe_path")) {
        config->subscribe_path = (char*)malloc(strlen(value) + 1);
        if (NULL == config->subscribe_path) {
        	log_error("malloc memory for config->subscribe_path error.");
        	return 0;
        }
        bzero(config->subscribe_path, strlen(value) + 1);
        strcpy(config->subscribe_path, value);
        if (strlen(config->subscribe_path) > 0) {
        	config->subscribe_map = subscribe_map_handler(config->subscribe_path);
        }
    }
    else{
        log_error("unknown config, %s: %s.", key, value);
        return 0;
    }

    return 1;
}

static hashset_t watch_set_handler(char *watch_path)
{
	if ( NULL == watch_path ) {
		return NULL;
	}

	hashset_t watch_set = hashset_create();
	char *last = watch_path;
	char *end  = watch_path + strlen(watch_path);
	char *pos = strstr(last,",");
	while( pos != end || pos == NULL ) {
		if (pos == NULL) {
			pos = end;
		}
		int len       = pos - last;
		char *element = (char*)malloc(len+1);
		bzero(element, len);
		strncpy(element, last, len);
		hashset_add(watch_set, element);

		if (pos != end) {
			last = pos+1;
			pos = strstr(last,",");
		}
	}

	return watch_set;
}

void watch_set_free(void *data)
{
	hashset_t watch_set = (hashset_t)data;
	hashset_itr_t watch_itr = hashset_iterator(watch_set);
	while (hashset_iterator_has_next(watch_itr)) {
	    char *watch_path = (char*)hashset_iterator_value(watch_itr);
	    free(watch_path);
	    hashset_iterator_next(watch_itr);
	}
	hashset_destroy(watch_set);
	free(watch_itr);
}

static hashmap_t* subscribe_map_handler(char *subscribe_path)
{
	hashmap_t *subscribe_map = hashmap_new(HASHMAP_NODES, 0, GH_USERKEYS, watch_set_free);

	char *last = subscribe_path;
	char *end  = subscribe_path + strlen(subscribe_path);

	char *pos = strstr(last,",");
	while ( NULL != pos || end != last ) {
		if (last == end) break;
		if (NULL == pos) pos = end;

		char *path_begin = strstr(last,":");
		char *host = (char*)malloc(path_begin - last);
		strncpy(host, last, path_begin - last);
		*(host + (path_begin - last)) = '\0';
		hashset_t path_set = (hashset_t)hashmap_find(subscribe_map, (void*)host);
		if (NULL == path_set) {
			path_set = hashset_create();
		}

		path_begin++;
		char *path_pos = strstr(path_begin, "|");
		while( NULL != path_pos && path_pos < pos ) {
			char *path = (char*)malloc(path_pos - path_begin);
			strncpy(path, path_begin, (path_pos - path_begin));
			*(path + (path_pos - path_begin)) = '\0';
			hashset_add(path_set, path);
			path_begin = path_pos + 1;
			path_pos = strstr(path_begin, "|");
		}

		if ( NULL == path_pos || path_pos > pos ) {
			path_pos = pos;
			char *path = (char*)malloc(path_pos - path_begin);
			strncpy(path, path_begin, (path_pos - path_begin));
			*(path + (path_pos - path_begin)) = '\0';
			hashset_add(path_set, path);
		}

		hashmap_add(subscribe_map, host, path_set);

		if (pos == end) {
			last = end;
		} else {
			last = pos + 1;
			pos = strstr(last,",");
		}
	}

	return subscribe_map;
}

void sync_config_dump(sync_config_t *config)
{
    int i = 0;
    printf("========================= SYNC CONFIG ===================\n");
    printf("%-30s%s\n",   "daemon: ",                 config->daemon ? "yes":"no");
    printf("%-30s%s\n",   "log_level: ",              log_level_str(config->log_level));
    printf("%-30s%s\n",   "log_dst: ",                log_dst_strs[config->log_dst]);
    printf("%-30s%s\n",   "log_file: ",               config->log_file);
    printf("%-30s%s\n",   "mode: ",                   config->mode);
    printf("%-30s%d\n",   "port: ",                   config->port);
    printf("%-30s%s\n",   "watch_path: ",             config->watch_path);
    printf("%-30s%s\n",   "subscribe_path: ",         config->subscribe_path);
    printf("%-30s\n",     "watch_set:");

    if ( config->watch_set != NULL ) {
    	hashset_itr_t watch_itr = hashset_iterator(config->watch_set);
    	while (hashset_iterator_has_next(watch_itr)) {
    		char *watch_path = (char*)hashset_iterator_value(watch_itr);
    		printf("%-10s%s\n",   " ", watch_path);
    		hashset_iterator_next(watch_itr);
    	}
    	free(watch_itr);
    }
    printf("%-30s\n",     "subscribe_map:");
    if (config->subscribe_map != NULL) {
    	hashmap_node_t *node = hashmap_findfirst(config->subscribe_map);
    	while (node != NULL) {
    		printf("%-10s%s => \n",     " ", (char*)node->key);
    		hashset_itr_t itr = hashset_iterator((hashset_t)node->data);
    		while (hashset_iterator_has_next(itr)) {
    			char *path = (char*)hashset_iterator_value(itr);
    			printf("%-20s%s\n"," ", path);
    			hashset_iterator_next(itr);
    		}
    		node = hashmap_findnext(config->subscribe_map);
    		free(itr);
    	}
    }
    printf("===========================================================\n");
}

