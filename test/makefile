#based on: scottmcpeak.com/autodepend/autodepend.html
#if depencies are renamed a make clean will be required

_MAKEFILE_ABS = $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
_TARGET := $(basename $(notdir $(realpath $(lastword $(CURDIR)))))
$(info makefile=$(_MAKEFILE_ABS))
.DEFAULT_GOAL = run
_MAKEFILE_DIR := $(dir $(_MAKEFILE_ABS))


BDIR = .
ODIR = obj
SDIR = .
LDIR = ../lib
IDIR = .
INC = -I$(IDIR) -I../inc
LIB = -L$(LDIR)
LIBS = $(LDIR)/libzhash.a
LIBS_SHORT = -lzhash
CFLAGS = -static -Wall -W -ggdb -std=c99 -O0 -coverage $(INC) $(LIB)


# https://www.gnu.org/software/make/manual/html_node/Wildcard-Function.html
C_SRC := $(wildcard *.c)
_OBJS := $(patsubst %.c,%.o,$(wildcard *.c))
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))


clean_filenames := $(BDIR)/$(_TARGET).exe $(ODIR)/*.o $(ODIR)/*.d
clean_files := $(strip $(foreach f,$(clean_filenames),$(wildcard $(f))))


_dummy := $(shell mkdir -p "$(BDIR)" "$(ODIR)" "$(LDIR)")


$(info make path=$(_MAKEFILE_DIR))
$(info target=$(_TARGET))
$(info C_SRC=$(C_SRC))

all: $(OBJS) $(LIBS)
	gcc $(CFLAGS) $(C_SRC) $(LIB) $(LIBS_SHORT) -o $(BDIR)/$(_TARGET)

run: all
	$(BDIR)/$(_TARGET).exe
	gcov $*.c

# remove compilation products
clean:
ifneq ($(clean_files),)
	rm -f $(clean_files)
endif