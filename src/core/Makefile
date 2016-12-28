CC				= gcc
CFLAGS			= 
LIBS			= -lpthread

OBJS			= base/estring.o \
				  base/elog.o \
				  base/esyslog.o \
				  base/eproperty.o	\
				  base/ememory.o \
				  base/equeue.o \
				  base/eother.o		  
				  
SRCS			= $(patsubst %.o,%.c,$(OBJS))

SRC_DIRS 		= $(dir $(SRCS))
SRC_PATH 		= $(shell echo $(SRC_DIRS) | tr -s " " "\n" | sort | uniq | tr -s "\n" ":")

VPATH 			= $(SRC_PATH)
INCLUDES		= $(patsubst %,-I%,$(subst :, ,$(VPATH)))

LIB_EASY		= libeasy.a

all:$(OBJS)
	ar rcs $(LIB_EASY) $(OBJS)
	@echo $(LIB_EASY) is generated
	-mkdir ../../lib
	-cp $(LIB_EASY) ../../lib/

$(OBJS):%.o:%.c
	$(CC) $(INCLUDES) -c -o $@ $<

.PHONY:clean
clean:
	-rm $(OBJS)
	-rm $(LIB_EASY)
	-rm -rf ../../lib
