_MAKEFILE_ABS = $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
_TARGET := $(basename $(notdir $(realpath $(lastword $(_MAKEFILE_ABS)))))

BDIR = .
ODIR = obj
SDIR = .
IDIR = -I/usr/include
LDIR = -L/usr/lib
LIBS = -lzhash
CFLAGS = -Wall -W -ggdb -std=c99 -O0 -coverage $(IDIR) $(LDIR) $(LIBS)

C_SRC := $(wildcard *.c)
_OBJS := $(patsubst %.c,%.o,$(wildcard *.c))
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))


clean_filenames := $(BDIR)/$(_TARGET).exe $(ODIR)/*.o $(ODIR)/*.d
clean_files := $(strip $(foreach f,$(clean_filenames),$(wildcard $(f))))


_dummy := $(shell mkdir -p "$(BDIR)" "$(ODIR)")


all:
	gcc $(CFLAGS) $(C_SRC) -o $(BDIR)/$(_TARGET)


run: all
	$(BDIR)/$(_TARGET)
	gcov $*.c


valgrind: all
	valgrind --error-exitcode=666 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all --track-fds=yes $(BDIR)/$(_TARGET)
	gcov $*.c


# remove compilation products
clean:
ifneq ($(clean_files),)
	rm -f $(clean_files)
endif