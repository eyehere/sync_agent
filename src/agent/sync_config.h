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
#ifndef _SYNC_CONFIG_H_
#define _SYNC_CONFIG_H_

#define LOG_DST_CONSOLE 0
#define LOG_DST_FILE    1
#define LOG_DST_SYSLOG  2

typedef struct _sync_config_t {
    int    daemon;                               //是否以daemon进程方式运行
    int    log_level;                            //日志级别
    int    log_dst;                              //日志输出方式，支持控制台，文件，syslog三种方式
    char  *log_file;                             //日志文件
    char  *mode;							     //agent的工作模式(server|client)
    int    port;								 //源服务器端口
    char  *watch_path;							 //监测目录(server模式)
    char  *subscribe_path;                       //订阅目录(client模式)
} sync_config_t;

/**
 *        Name: sync_config_load
 * Description: load sync config
 *   Parameter: path -> sync config file path
 *      Return: the instance of sync config
 *              load failedly, return NULL
 */
sync_config_t *sync_config_load(char *path);

/**
 *        Name: sync_config_free
 * Description: release sync config
 *   Parameter: config -> the instance of sync config
 *      Return:
 *
 */
void sync_config_free(sync_config_t *config);

/**
 *        Name: sync_config_dump
 * Description: dump sync config information
 *   Parameter: config -> the instance of sync config
 *      Return:
 *
 */
void sync_config_dump(sync_config_t *config);

#endif
