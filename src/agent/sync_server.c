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

#include "sync_agent.h"

/**
 * 全局变量定义区
 */
sync_server_t *_server_t = NULL;

/**
 * ip_set 释放
 */
static void ip_set_free(void *ip_set);


int sync_server_init()
{
	_server_t = (sync_server_t *)malloc(sizeof(sync_server_t));
	if ( NULL == _server_t ) {
		return 0;
	}
	_server_t->path_ip_set = hashmap_new(HASHMAP_PATH_NODES, GH_COPYKEYS,
								GH_USERKEYS, ip_set_free);
	if ( NULL == _server_t->path_ip_set ) {
		return 0;
	}
	return 1;
}

void sync_server_destroy()
{
	hashmap_delete(_server_t->path_ip_set);
	free(_server_t);
}

static void ip_set_free(void *ip_set)
{
	set_destroy((simple_set*)ip_set);
}

void sync_server_listen( void *arg)
{
	while (_main_continue) {

	}
}

void sync_server_watch(void *arg)
{

}
