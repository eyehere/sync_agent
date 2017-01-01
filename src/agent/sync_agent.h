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

#ifndef _SYNC_AGENT_H_
#define _SYNC_AGENT_H_

#include "lthread.h"
#include "easy.h"
#include "set.h"
#include "hashmap.h"

#include "sync_config.h"
#include "sync_server.h"

#define WORK_DIR "./"
#define CONFIG_FILE WORK_DIR"conf/sync_server.conf"

#define LOG_BUFFER_SIZE   8096

/**
 * 全局变量申明区
 */
extern int 	_main_continue;
extern char	*_config_file;
extern sync_config_t *_config;

/**
 * 客户端连接
 */
typedef struct _connection_s {
	struct sockaddr_in cli_addr;
	int fd;
} _connection_t;

/**
 *        Name: lthread_log
 * Description: lthread里通过这个函数写日志 不要在lthread里做阻塞调用
 *   Parameter: log_level -> log level.
 *   Parameter: fmt -> log fmt.
 *      Return:
 */
void lthread_log(int log_level, const char *fmt, ...);

#endif
