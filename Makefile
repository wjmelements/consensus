CC=gcc
CFLAGS=-O3 -fdiagnostics-color=always -Iinclude/ -std=gnu11 -pthread
MKDIRS=lib bin tst/bin .pass
EXECS=$(addprefix bin/,)
TESTS=$(addprefix tst/bin/,fetchadd)
HEADERS=$(wildcard include/*.h)

.PHONY: .default all clean check distcheck

.default: all
	@true
all: $(EXECS) $(TESTS)
	@true
distcheck:
	@rm -rf .pass
	@make --no-print-directory check
clean:
	rm -rf $(MKDIRS)
check: $(addprefix .pass/, $(TESTS))
	@true
.pass/%: % | .pass
	$*
	@mkdir -p $(@D) 
	@touch $@
bin/%: %.c | bin
	$(CC) $(CFLAGS) $^ -o $@
lib/%.o: src/%.c include/%.h | lib
	$(CC) $(CFLAGS) -c $< -o $@
tst/bin/%: tst/%.c lib/%.o include/tst.h | tst/bin
	$(CC) $(CFLAGS) $(filter-out $(HEADERS),$^) -o $@
$(MKDIRS):
	@mkdir -p $@
