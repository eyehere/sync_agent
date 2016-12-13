CC				= gcc
CFLAGS			= 
LIBS			= 

OBJS			= hash_fcn.o \
				  prime_table.o \
				  hashmap.o
				  
SRCS			= $(patsubst %.o,%.c,$(OBJS))

SRC_DIRS 		= $(dir $(SRCS))
SRC_PATH 		= $(shell echo $(SRC_DIRS) | tr -s " " "\n" | sort | uniq | tr -s "\n" ":")

VPATH 			= $(SRC_PATH)
INCLUDES		= $(patsubst %,-I%,$(subst :, ,$(VPATH)))

LIB_HASHMAP	= libhashmap.a

all:$(OBJS)
	ar rcs $(LIB_HASHMAP) $(OBJS)
	@echo $(LIB_HASHMAP) is generated
	-mkdir ../../../lib 
	-cp $(LIB_HASHMAP) ../../../lib/

$(OBJS):%.o:%.c
	$(CC) $(INCLUDES) -c -o $@ $<

.PHONY:clean
clean:
	-rm $(OBJS)
	-rm $(LIB_HASHMAP)
	-rm -r ../../../lib