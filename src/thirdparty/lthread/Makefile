CC				= gcc
CFLAGS			= 
LIBS			= -lpthread

OBJS			= lthread_compute.o \
				  lthread_io.o \
				  lthread_poller.o \
				  lthread_sched.o \
				  lthread_socket.o \
				  lthread.o
				  
SRCS			= $(patsubst %.o,%.c,$(OBJS))

SRC_DIRS 		= $(dir $(SRCS))
SRC_PATH 		= $(shell echo $(SRC_DIRS) | tr -s " " "\n" | sort | uniq | tr -s "\n" ":")

VPATH 			= $(SRC_PATH)
INCLUDES		= $(patsubst %,-I%,$(subst :, ,$(VPATH)))

LIB_LTHREAD		= liblthread.a

all:$(OBJS)
	ar rcs $(LIB_LTHREAD) $(OBJS)
	@echo $(LIB_LTHREAD) is generated
	-mkdir ../../../lib 
	-cp $(LIB_LTHREAD) ../../../lib/

$(OBJS):%.o:%.c
	$(CC) $(INCLUDES) -c -o $@ $<

.PHONY:clean
clean:
	-rm $(OBJS)
	-rm $(LIB_LTHREAD)
	-rm -r ../../../lib