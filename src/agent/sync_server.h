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

#ifndef _SYNC_SERVER_H_
#define _SYNC_SERVER_H_

#define HASHMAP_PATH_NODES 1024

typedef struct _sync_server_t {
	hashmap_t *path_ip_set;      //key是client要同步的path路径 value是ipSet
} sync_server_t;

/**
 * 全局变量定义区
 */
extern sync_server_t *_server_t;

/**
 *        Name: sync_server_start
 * Description: server模式启动
 *   Parameter:
 *      Return:
 *
 */
void sync_server_start();

/**
 *        Name: sync_server_init
 * Description: sync_server 初始化
 *   Parameter:
 *      Return:
 *
 */
int sync_server_init();

/**
 *        Name: sync_server_destory
 * Description: sync_server 销毁
 *   Parameter:
 *      Return:
 *
 */
void sync_server_destroy();

#endif
