CC				= gcc
CFLAGS			= -fPIC
CFLAGS_DEBUG	=
#CFLAGS_DEBUG	= -ggdb -W -Wall -Werror -Wno-unused-parameter \
#			  	-Wunused-function -Wunused-variable -Wunused-value \
#				-D_DEBUG

MAIN_EXEC		= sync_agent

SRC_DIR			= src
LIBS			= -Llib -lpthread -llthread

OBJS			= $(SRC_DIR)/agent/sync_agent.o

SRCS			= $(patsubst %.o,%.c,$(OBJS))

SRC_DIRS 		= $(dir $(SRCS))
SRC_PATH 		= $(shell echo $(SRC_DIRS) | tr -s " " "\n" | sort | uniq | tr -s "\n" ":")

VPATH 			= $(SRC_PATH)
INCLUDES		= $(patsubst %,-I%,$(subst :, ,$(VPATH)))
INCLUDES	   +=  -Isrc/thirdparty/lthread

all:lthread.a $(MAIN_EXEC) 
.PHONY:all

debug:lthread.so $(MAIN_EXEC)_debug 
.PHONY:all

lthread.a:
	make -C src/thirdparty/lthread

$(MAIN_EXEC):$(OBJS)
	$(CC) $(CFLAGS) -O2 -o $(MAIN_EXEC) $(OBJS) $(LIBS)
	@echo $(MAIN_EXEC) is generated [product] 

$(MAIN_EXEC)_debug:$(OBJS)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) -o $(MAIN_EXEC) $(OBJS) $(LIBS)
	@echo $(MAIN_EXEC) is generated [debug]

$(OBJS):%.o:%.c
	$(CC) $(CFLAGS_DEBUG) $(INCLUDES) -c -o $@ $< 

.PHONY:clean
clean:
	make -C src/thirdparty/lthread clean
	-rm $(OBJS)
	-rm $(MAIN_EXEC)
	