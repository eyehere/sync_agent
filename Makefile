CC				= gcc
CFLAGS			= -fPIC
CFLAGS_DEBUG	=
#CFLAGS_DEBUG	= -ggdb -W -Wall -Werror -Wno-unused-parameter \
#			  	-Wunused-function -Wunused-variable -Wunused-value \
#				-D_DEBUG

MAIN_EXEC		= sync_agent

SRC_DIR			= src
LIBS			= -Llib -lpthread -llthread -leasy -lhashset -lhashmap

OBJS			= $(SRC_DIR)/agent/sync_agent.o \
				  $(SRC_DIR)/agent/sync_config.o

SRCS			= $(patsubst %.o,%.c,$(OBJS))

SRC_DIRS 		= $(dir $(SRCS))
SRC_PATH 		= $(shell echo $(SRC_DIRS) | tr -s " " "\n" | sort | uniq | tr -s "\n" ":")

VPATH 			= $(SRC_PATH)
INCLUDES		= $(patsubst %,-I%,$(subst :, ,$(VPATH)))
INCLUDES	   +=  -Isrc/thirdparty/lthread \
				   -Isrc/thirdparty/hashset \
				   -Isrc/thirdparty/hashmap \
				   -Isrc/core/base

all:lthread.a libeasy.a libhashset.a libhashmap.a $(MAIN_EXEC) 
.PHONY:all

debug:lthread.a libeasy.a libhashset.a libhashmap.a $(MAIN_EXEC)_debug 
.PHONY:debug

lthread.a:
	make -C src/thirdparty/lthread

libeasy.a:
	make -C src/core

libhashset.a:
	make -C src/thirdparty/hashset
	
libhashmap.a:
	make -C src/thirdparty/hashmap
		
$(MAIN_EXEC):$(OBJS)
	$(CC) $(CFLAGS) -O2 -o $(MAIN_EXEC) $(OBJS) $(LIBS)
	-mkdir logs
	@echo $(MAIN_EXEC) is generated [product] 

$(MAIN_EXEC)_debug:$(OBJS)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) -o $(MAIN_EXEC) $(OBJS) $(LIBS)
	-mkdir logs
	@echo $(MAIN_EXEC) is generated [debug]

$(OBJS):%.o:%.c
	$(CC) $(CFLAGS_DEBUG) $(INCLUDES) -c -o $@ $< 

.PHONY:clean
clean:
	make -C src/thirdparty/lthread clean
	make -C src/core clean
	make -C src/thirdparty/hashset clean
	make -C src/thirdparty/hashmap clean
	-rm $(OBJS)
	-rm $(MAIN_EXEC)
	-rm -rf logs
	