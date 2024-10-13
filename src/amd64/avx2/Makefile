
JASMINC ?= jasminc -lazy-regalloc 
GCC ?= arch -x86_64 gcc

.DEFAULT_GOAL := all

PROGS = test_keccak1600_avx2

all: $(PROGS)


test_keccak1600_avx2: test_keccak1600_avx2.c test_imem_avx2.s test_array_avx2.s test_orig_avx2.s
	$(GCC) -o $@ $^

%.s : %.jazz
	$(JASMINC) $^ -o $@


clean:
	rm -f *.s

realclean: clean
	rm -f $(PROGS)
