_MAKEFILE_ABS = $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
_TARGET := $(basename $(notdir $(realpath $(lastword $(_MAKEFILE_ABS)))))

BDIR = .
ODIR = obj
SDIR = .
IDIR = -I/usr/include
LDIR = -L/usr/lib
LIBS = -lzhash
CFLAGS = -Wall -W -ggdb -std=c99 -O0 $(IDIR) $(LIBS)

_OBJS := $(patsubst %.c,%.o,$(wildcard *.c))
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

clean_filenames := $(BDIR)/$(_TARGET).exe $(ODIR)/*.o $(ODIR)/*.d
clean_files := $(strip $(foreach f,$(clean_filenames),$(wildcard $(f))))

_dummy := $(shell mkdir -p "$(BDIR)" "$(ODIR)")

all: $(OBJS) $(LIBS)
	gcc $(OBJS) $(IDIR) $(LDIR) $(LIBS) -fprofile-arcs -ftest-coverage -o $(BDIR)/$(_TARGET)

run: all
	$(BDIR)/$(_TARGET)

valgrind: all
	valgrind --error-exitcode=666 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all --track-fds=yes $(BDIR)/$(_TARGET) || gdb -batch -ex "run" -ex "bt full" --args $(BDIR)/$(_TARGET)

gcov: run
	gcov $(_TARGET).c
	bash <(curl -s https://codecov.io/bash)

# pull in dependency info for *existing* .o files
-include $(OBJS:.o=.d)

# compile and generate dependency info
$(ODIR)/%.o: $(SDIR)/%.c
	gcc -c $(CFLAGS) -fprofile-arcs -lgcov $*.c -o $(ODIR)/$*.o
	gcc -MM $(CFLAGS) $*.c > $(ODIR)/$*.d

# remove compilation products
clean:
ifneq ($(clean_files),)
	rm -f $(clean_files)
endif