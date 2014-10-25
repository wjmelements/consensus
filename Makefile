SHELL=/bin/bash
export SHELLOPTS = pipefail
CC=gcc
CFLAGS=-O3 -fdiagnostics-color=always -Iinclude/ -std=gnu11 -pthread
MKDIRS=lib bin tst/bin .pass
EXECS=$(addprefix bin/,)
CASES=cmpxchg fetchadd mtmxchg queue stack xchg
TESTS=$(addprefix tst/bin/seq,$(CASES)) $(addprefix tst/bin/log,$(CASES)) $(addprefix tst/bin/lin,$(CASES)) $(addprefix tst/bin/pool,$(CASES))
HEADERS=$(wildcard include/*.h)

.PHONY: .default execs tests all clean again check distcheck
.SECONDARY:

.default: all
all: execs tests
	@true
execs: $(EXECS)
tests: $(TESTS)
distcheck:
	@rm -rf .pass
	@make --no-print-directory check
clean:
	rm -rf $(MKDIRS)
again: clean all
check: $(addprefix .pass/, $(TESTS))
	@true
.pass/%: % | .pass
	@printf "$(@F): ";\
    /usr/bin/time -p $* 2>&1 | grep real | cut -d " " -f2 | tr -d "\n" \
    && echo -e " \033[0;32mpass\033[0m"\
    || (echo -e " \033[0;31mfail\033[0m" && false)
	@mkdir -p $(@D) 
	@touch $@
bin/%: %.c | bin
	$(CC) $(CFLAGS) $^ -o $@
lib/%.o: src/%.c include/%.h | lib
	$(CC) $(CFLAGS) -c $< -o $@
tst/bin/lin%: tst/%.c lib/%.o include/tst.h lib/linmap.o | tst/bin
	$(CC) $(CFLAGS) $(filter-out $(HEADERS),$^) -Dmap=linmap -o $@
tst/bin/log%: tst/%.c lib/%.o include/tst.h lib/logmap.o | tst/bin
	$(CC) $(CFLAGS) $(filter-out $(HEADERS),$^) -Dmap=logmap -o $@
tst/bin/pool%: tst/%.c lib/%.o include/tst.h lib/logmap.o lib/poolmap.o | tst/bin
	$(CC) $(CFLAGS) $(filter-out $(HEADERS),$^) -Dmap=poolmap -o $@
tst/bin/seq%: tst/%.c lib/%.o include/tst.h lib/seqmap.o | tst/bin
	$(CC) $(CFLAGS) $(filter-out $(HEADERS),$^) -Dmap=seqmap -o $@
$(MKDIRS):
	@mkdir -p $@
