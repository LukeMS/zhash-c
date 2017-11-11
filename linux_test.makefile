_MAKEFILE_ABS = $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
_TARGET := $(basename $(notdir $(realpath $(lastword $(_MAKEFILE_ABS)))))
EXE = $(_TARGET).exe

BDIR = .
ODIR = .
SDIR = .
IDIR = -I/usr/include
LDIR = -L/usr/lib
LIBS = -lgcov -lzhash
CFLAGS = -Wall -W -ggdb -std=c99 -O0 -fprofile-arcs -ftest-coverage

_OBJS := $(patsubst %.c,%.o,$(wildcard *.c))
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

clean_filenames := $(BDIR)/$(EXE) $(ODIR)/*.o $(ODIR)/*.d
clean_files := $(strip $(foreach f,$(clean_filenames),$(wildcard $(f))))

_dummy := $(shell mkdir -p "$(BDIR)" "$(ODIR)")

all:
	gcc $(CFLAGS) $(IDIR) $(LDIR) $(LIBS) $(_TARGET).c -o $(EXE)

run: all
	$(BDIR)/$(EXE)

valgrind: all
	valgrind --error-exitcode=666 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all --track-fds=yes $(BDIR)/$(EXE) || gdb -batch -ex "run" -ex "bt full" --args $(BDIR)/$(EXE)

gcov:
	./$(EXE)
	# strings $(EXE) | grep gcda
	# ls -A ./$(_TARGET).*
	# hexdump -e '"%x\n"' -s8 -n4 $(_TARGET).gcda
	# hexdump -e '"%x\n"' -s8 -n4 $(_TARGET).gcno
	# touch $(_TARGET).gcda
	gcov ./$(_TARGET).c

# remove compilation products
clean:
ifneq ($(clean_files),)
	rm -f $(clean_files)
endif