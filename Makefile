CC=gcc
CFLAGS=-O3 -fdiagnostics-color=always -Iinclude/ -std=gnu11 -pthread
MKDIRS=lib bin tst/bin .pass
EXECS=$(addprefix bin/,)
TESTS=$(addprefix tst/bin/,fetchadd)

.PHONY: .default all clean check

.default: all
	@true
all: $(EXECS) $(TESTS)
	@true
clean:
	rm -rf $(MKDIRS)
check: $(addprefix .pass/, $(TESTS))
	@true
.pass/%: % | .pass
	$*
	touch $@
bin/%: %.c | bin
	$(CC) $(CFLAGS) $^ -o $@
lib/%.o: src/%.c include/%.h | lib
	$(CC) $(CFLAGS) -c $< -o $@
tst/bin/%: tst/%.c lib/%.o | tst/bin
	$(CC) $(CFLAGS) $^ -o $@
$(MKDIRS):
	mkdir -p $@
