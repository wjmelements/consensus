SHELL=/bin/bash
CC=gcc
CFLAGS=-O3 -fdiagnostics-color=always -Iinclude/ -std=gnu11 -pthread
MKDIRS=lib bin tst/bin .pass
EXECS=$(addprefix bin/,)
TESTS=$(addprefix tst/bin/,fetchadd cmpxchg)
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
	@printf "$(@F): " && $* && echo -e "\033[0;32mpass\033[0m" || (echo -e "\033[0;31mfail\033[0m" && false)
	@mkdir -p $(@D) 
	@touch $@
bin/%: %.c | bin
	$(CC) $(CFLAGS) $^ -o $@
lib/%.o: src/%.c include/%.h | lib
	$(CC) $(CFLAGS) -c $< -o $@
tst/bin/%: tst/%.c lib/%.o include/tst.h lib/logmap.o | tst/bin
	$(CC) $(CFLAGS) $(filter-out $(HEADERS),$^) -o $@
$(MKDIRS):
	@mkdir -p $@
