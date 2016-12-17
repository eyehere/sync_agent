CC				= gcc
CFLAGS			= 
LIBS			= 

OBJS			= hashset.o \
				  hashset_itr.o \
				  set.o
				  
SRCS			= $(patsubst %.o,%.c,$(OBJS))

SRC_DIRS 		= $(dir $(SRCS))
SRC_PATH 		= $(shell echo $(SRC_DIRS) | tr -s " " "\n" | sort | uniq | tr -s "\n" ":")

VPATH 			= $(SRC_PATH)
INCLUDES		= $(patsubst %,-I%,$(subst :, ,$(VPATH)))

LIB_HASHSET	= libhashset.a

all:$(OBJS)
	ar rcs $(LIB_HASHSET) $(OBJS)
	@echo $(LIB_HASHSET) is generated
	-mkdir ../../../lib 
	-cp $(LIB_HASHSET) ../../../lib/

$(OBJS):%.o:%.c
	$(CC) $(INCLUDES) -c -o $@ $<

.PHONY:clean
clean:
	-rm $(OBJS)
	-rm $(LIB_HASHSET)
	-rm -r ../../../lib