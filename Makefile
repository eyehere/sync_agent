SRC_DIR			= src

CC				= gcc
CFLAGS			= -fPIC
CFLAGS_DEBUG	=
#CFLAGS_DEBUG	= -ggdb -W -Wall -Werror -Wno-unused-parameter \
#			  	-Wunused-function -Wunused-variable -Wunused-value

MAIN_EXEC		= sync_agent

LIBS			= 

OBJS			= $(SRC_DIR)/sync_agent.o

SRCS			= $(patsubst %.o,%.c,$(OBJS))

SRC_DIRS 		= $(dir $(SRCS))
SRC_PATH 		= $(shell echo $(SRC_DIRS) | tr -s " " "\n" | sort | uniq | tr -s "\n" ":")

VPATH 			= $(SRC_PATH)
INCLUDES		= $(patsubst %,-I%,$(subst :, ,$(VPATH)))

all:$(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -O2 -o $(MAIN_EXEC) $(OBJS)
	@echo $(MAIN_EXEC) is generated [product] 

debug:$(OBJS)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) $(LIBS) -o $(MAIN_EXEC) $(OBJS)
	@echo $(MAIN_EXEC) is gegerated [debug]

$(OBJS):%.o:%.c
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) $(INCLUDES) $(LIBS) -c -o $@ $<

.PHONY:clean
clean:
	-rm $(OBJS)
	-rm $(MAIN_EXEC)


	