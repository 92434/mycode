
CC=gcc
SRCDIRS=./src/

SRCS=$(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))
OBJS=$(SRCS:.c=.o)
DEPENDS=$(SRCS:.c=.d)

LIBS= -lpthread  -pthread
INCLUDES=-I
CFLAGS=-Wall -Werror $(INCLUDES) -DWORDS_BIGENDIAN -DUSE_DEBUG -g
LDFLAGS= $(LIBS)
DEPENDFLAG=-MM
PROGRAMS=A6211


%.d:%.c
	$(CC) $(DEPENDFLAG) $(CFLAGS)  $< |\
	sed "s?\\(.*\\):?$(basename $<).o $(basename $<).d :?g" \
	> $@ || $(RM) $@  

$(PROGRAMS) : $(OBJS)   
	$(CC) $(LIBS) -o $@ $(filter %.o ,$+)  


include $(DEPENDS)
clean: 
	rm $(OBJS) $(DEPENDS) $(PROGRAMS)
