#based on: scottmcpeak.com/autodepend/autodepend.html
#if depencies are renamed a make clean will be required

_MAKEFILE_ABS = $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
_TARGET := $(basename $(notdir $(realpath $(lastword $(CURDIR)))))
$(info makefile=$(_MAKEFILE_ABS))
.DEFAULT_GOAL = run
_MAKEFILE_DIR := $(dir $(_MAKEFILE_ABS))


$(info make path=$(_MAKEFILE_DIR))
$(info target=$(_TARGET))
BDIR = .
ODIR = obj
SDIR = .
LDIR = ../lib
IDIR = .
INC = -I$(IDIR) -I../inc
LIB = -L$(LDIR)
LIBS = $(LDIR)/libzhash.a
LIBS_SHORT = -lzhash
CFLAGS = -static -Wall -W -ggdb -std=c99 $(INC) $(LIB)


# https://www.gnu.org/software/make/manual/html_node/Wildcard-Function.html
_OBJS := $(patsubst %.c,%.o,$(wildcard *.c))
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))


clean_filenames := $(BDIR)/$(_TARGET).exe $(ODIR)/*.o $(ODIR)/*.d
clean_files := $(strip $(foreach f,$(clean_filenames),$(wildcard $(f))))


_dummy := $(shell mkdir -p "$(BDIR)" "$(ODIR)" "$(LDIR)")


all: $(OBJS) $(LIBS)
	gcc $(OBJS) $(LIB) $(LIBS_SHORT) -o $(BDIR)/$(_TARGET)

rundrm: all
	C:/dr_m/bin/drmemory -batch -- $(BDIR)/$(_TARGET).exe

run: all
	$(BDIR)/$(_TARGET).exe

# pull in dependency info for *existing* .o files
-include $(OBJS:.o=.d)

# compile and generate dependency info
$(ODIR)/%.o: $(SDIR)/%.c
	gcc -c $(CFLAGS) $*.c -o $(ODIR)/$*.o
	gcc -MM $(CFLAGS) $*.c > $(ODIR)/$*.d

$(LDIR)/libzhash.a:
	mingw32-make -C ../ --file=makefile lib DEST=$(LDIR) lib

# remove compilation products
clean:
ifneq ($(clean_files),)
	rm -f $(clean_files)
endif