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
#define CONFIG_FILE WORK_DIR"conf/sync_server.conf"

/**
 * 全局变量申明区
 */
int 	_main_continue 	= 1;
char	*_config_file	= CONFIG_FILE;
sync_config_t *_config 	= NULL;

/**
 * 静态函数申明区
 */
static void show_help(const char *program);
static void show_version(const char *program);
static int parse_args(int argc, char **argv);
static int create_daemon_process();
static int  install_signals(void);
static void signal_handle(int signum);

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

static int parse_args(int argc, char **argv)
{
    int opt_char = 0;
    int parsed   = 0;

    if (argc == 1) {
        parsed = 1;
    }

    while(-1 != (opt_char = getopt(argc, argv, "c:vh"))){
        parsed = 1;

        switch(opt_char){
            case 'c':
                _config_file = optarg;
                break;
            case 'v':
                show_version(argv[0]);
                exit(0);
            case 'h':
                show_help(argv[0]);
                exit(0);
            case '?':
            case ':':
            default:
                log_error("unknown arg: %c.", opt_char);
                show_help(argv[0]);
                return 0;
        }
    }

    if (parsed) {
        return 1;
    }
    else {
        log_error("unknown arg.");
        show_help(argv[0]);
        return 0;
    }
}

static int create_daemon_process()
{
    pid_t pid = 0;
    int   fd  = -1;

    pid = fork();
    /* 创建进程错误 */
    if (pid < 0) {
        return 0;
    }
    /* 父进程 */
    else if (pid > 0) {
        sync_config_free(_config);
        exit(0);
    }
    /* 子进程 */
    else {
        /* 脱离原始会话 */
        if (setsid() == -1) {
            log_error("setsid failed.");
            return 0;
        }

        /* 修改工作目录 */
        chdir("/");

        /* 重设掩码 */
        umask(0);

        fd = open("/dev/null", O_RDWR);
        if (fd == -1) {
            log_error("open /dev/null failed.");
            return 0;
        }

        /* 重定向子进程的标准输入到null设备 */
        if (dup2(fd, STDIN_FILENO) == -1) {
            log_error("dup2 STDIN to fd failed.");
            return 0;
        }

        /* 重定向子进程的标准输出到null设备 */
        if (dup2(fd, STDOUT_FILENO) == -1) {
            log_error("dup2 STDOUT to fd failed.");
            return 0;
        }

        /* 重定向子进程的标准错误到null设备 */
        if (dup2(fd, STDERR_FILENO) == -1) {
            log_error("dup2 STDERR to fd failed.");
            return 0;
        }
    }

    return 1;
}

static int install_signals(void){
    if(SIG_ERR == signal(SIGINT, signal_handle)){
        log_error("Install SIGINT fails.");
        return 0;
    }
    if(SIG_ERR == signal(SIGTERM, signal_handle)){
        log_error("Install SIGTERM fails.");
        return 0;
    }
    if(SIG_ERR == signal(SIGSEGV, signal_handle)){
        log_error("Install SIGSEGV fails.");
        return 0;
    }
    if(SIG_ERR == signal(SIGBUS, signal_handle)){
        log_error("Install SIGBUS fails.");
        return 0;
    }
    if(SIG_ERR == signal(SIGQUIT, signal_handle)){
        log_error("Install SIGQUIT fails.");
        return 0;
    }
    if(SIG_ERR == signal(SIGCHLD, signal_handle)){
        log_error("Install SIGCHLD fails.");
        return 0;
    }

    return 1;
}

static void signal_handle(int signum){
    if(SIGTERM == signum){
        log_info("recv kill signal, sync_agent will exit normally.");
        _main_continue = 0;
    }
    else if(SIGINT == signum){
        log_info("recv CTRL-C signal, sync_agent will exit normally.");
        _main_continue = 0;
    }
    else if(SIGCHLD == signum){
        log_debug("recv SIGCHLD signal[%d].", signum);
    }
    else{
        log_info("receive signal: %d", signum);
        exit(0);
    }
}

int main(int argc, char **argv)
{
	if (!parse_args(argc, argv)) {
	    return -1;
	}

	_config = sync_config_load(_config_file);
	if (NULL == _config) {
	    return -1;
	}
	log_set_level(_config->log_level);

	if (LOG_DST_FILE == _config->log_dst) {
		if (!log_set_file(_config->log_file)) {
	        log_error("log_set_file failed.");
	        return -1;
	    }
	}

	sync_config_dump(_config);

	if (_config->daemon) {
	    log_info("create daemon process.");
	    if (!create_daemon_process()) {
	        log_error("create daemon process failed.");
	        return -1;
	    }
	}

	install_signals();

	if (0 == strcasecmp(_config->mode, "server")) {//Server模式
		printf("server\n");
	}
	else if (0 == strcasecmp(_config->mode, "client")) {//Client模式
		printf("client\n");
	}
	else {
		printf("not found\n");
	}

	return 0;
}
