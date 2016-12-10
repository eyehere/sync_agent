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

#define WORK_DIR "./"
#define CONFIG_FILE WORK_DIR"conf/sync_agent.conf"

/**
 * 全局变量申明区
 */
int 	_main_continue 	= 1;
char	*_config_file	= CONFIG_FILE;

/**
 * 静态函数申明区
 */
static void show_help(const char *program);
static void show_version(const char *program);

static void show_help(const char *program)
{
	printf("Usage: %s [-c config_file]\n", program);
	printf("       %s -v\n", program);
	printf("       %s -h\n", program);
	printf("       -c: 指定配置文件，默认./conf/sync_agent.conf \n");
	printf("       -v: 显示版本号。\n");
	printf("       -h: 显示命令帮助。\n\n");
}

static void show_version(const char *program)
{
	printf("%s, Version 1.0.0\n\n", program);
}

int main(int argc, char **argv)
{

}
