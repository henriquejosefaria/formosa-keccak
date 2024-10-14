
JASMINC ?= jasminc -lazy-regalloc 
GCC ?= arch -x86_64 gcc

.DEFAULT_GOAL := all

PROGS = test_keccak1600_avx2 test_keccak1600_avx2x4

all: $(PROGS)


test_keccak1600_avx2: test_keccak1600_avx2.c test_imem_avx2.s test_array_avx2.s test_orig_avx2.s
	$(GCC) -o $@ $^

test_keccak1600_avx2x4: test_keccak1600_avx2x4.c test_imem_avx2x4.s test_orig_avx2.s
	$(GCC) -o $@ $^

%.s : %.jazz
	$(JASMINC) $< -o $@

test_imem_avx2.s: keccak1600_avx2.jinc keccak1600_imem_avx2.jinc keccak1600_orig_avx2.jinc

test_imem_avx2x4.s: keccak1600x4_avx2.jinc keccak1600x4_imem_avx2.jinc keccak1600_orig_avx2.jinc

clean:
	rm -f *.s

realclean: clean
	rm -f $(PROGS) *~
