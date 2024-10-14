#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include <stdint.h>

extern void test_addratebit_avx2x4(uint64_t*);
extern void test_absorb_orig_avx2(uint64_t*, uint8_t*);
extern void test_absorb_bcast_imem_avx2x4(uint64_t*, uint8_t*);
extern void test_keccakf1600_avx2x4(uint64_t *);
extern void test_shake_imem_avx2x4(uint64_t*, uint8_t*, uint8_t*, uint8_t*, uint8_t*, uint8_t*);
extern void test_shake_orig_avx2(uint64_t*, uint8_t*, uint8_t*);


#define ASIZE 1741
#define OSIZE 2329

#define RATE8 136

void buf_print(char* str, uint8_t x[]) {
 int i, j;
 if (str!=NULL) printf("%s =\n", str);
 for (i=0; i<OSIZE;) {
  j = 0; while (j++ < 8) { printf("%02x", x[i++]); } printf(" ");
  j = 0; while (j++ < 8) { printf("%02x", x[i++]); } printf(" ");
  j = 0; while (j++ < 8) { printf("%02x", x[i++]); } printf(" ");
  j = 0; while (j++ < 8) { printf("%02x", x[i++]); } printf(" ");
  j = 0; while (j++ < 8) { printf("%02x", x[i++]); } printf(" ");
  printf("\n");
 }
 printf("\n");
}

void st_print(char* str, uint64_t x[]) {
 int i, j;
 if (str!=NULL) printf("%s =\n", str);
 for (i=0; i<7; i++) {
  for (j=0; j<4; j++) {
   if (0 < j) printf(", ");
   printf("0x%016llx ", x[4*i+j]);
  }
  printf("\n");
 }
 printf("\n");
}

void stx4_print(char* str, uint64_t x[]) {
 int i, j;
 if (str!=NULL) printf("%s =\n", str);
 for (i=0; i<25; i++) {
  for (j=0; j<4; j++) {
   if (0 < j) printf(", ");
   printf("0x%016llx ", x[4*i+j]);
  }
  printf("\n");
 }
 printf("\n");
}

int test_eqbuf(uint8_t x[], uint8_t y[]) {
  int i;
  int r = 1;
  for (i=0; i < OSIZE; i++) {
    r = r && (x[i]==y[i]);
    if (!r) break;
  }
  if (!r) {
    printf("error at pos %d\n", i);
  }
  return r;
}

int test_eqst(uint64_t x[], uint64_t y[]) {
  int i;
  int r = 1;
  for (i=0; i < 28; i++) {
    r = r && (x[i]==y[i]);
    if (!r) break;
  }
  if (!r) {
    printf("error at line %d, column %d\n", i/4, i%4);
  }
  return r;
}

int buf_check(char* str, uint8_t x[], uint8_t y[]) {
  int r = test_eqbuf(x,y);
  printf("TESTING %s: \n", str);
  if (! r ) {
    printf("Expecting: ");
    buf_print("", x);
    printf(", but got: ");
    buf_print("", y);
  } else {
    printf("...OK!\n");
  }
  return r;
}

int st_check(char* str, uint64_t x[], uint64_t y[]) {
  int r = test_eqst(x,y);
  printf("TESTING %s: \n", str);
  if (! r ) {
    printf("Expecting: ");
    st_print("", x);
    printf(", but got: ");
    st_print("", y);
  } else {
    printf("...OK!\n");
  }
  return r;
}

int ststx4_check(char* str, uint64_t x[], uint64_t y[]) {
  int r = test_eqst(x,y);
  printf("TESTING %s: \n", str);
  if (! r ) {
    printf("Expecting: ");
    st_print("", x);
    printf(", but got: ");
    stx4_print("", y);
  } else {
    printf("...OK!\n");
  }
  return r;
}

int main() {
  uint64_t s0[28];
  uint64_t s1[4*25];
  uint64_t s2[4*25];
  uint8_t t[2*ASIZE];
  uint8_t o0[2*OSIZE];
  uint8_t o10[2*OSIZE], o11[2*OSIZE], o12[2*OSIZE], o13[2*OSIZE];
  uint8_t o20[2*OSIZE], o21[2*OSIZE], o22[2*OSIZE], o23[2*OSIZE];
  uint8_t o2[2*OSIZE];
  int i;
  for (i=0; i<4*25; i++) { s1[i]=0xff; s2[i]=0xff; }
  for (i=0; i<ASIZE; i++) { t[i]=(uint8_t)i; }
  for (i=ASIZE; i<2*ASIZE; i++) { t[i]=0xFF; }
  for (i=0; i<2*OSIZE; i++) { 
    o0[i]=0xFF;
    o10[i]=0xFF; o11[i]=0xFF; o12[i]=0xFF; o13[i]=0xFF;
    o20[i]=0xFF; o21[i]=0xFF; o22[i]=0xFF; o23[i]=0xFF;
  }

//test_absorb_bcast_imem_avx2x4(s1, t);
//test_keccakf1600_avx2x4(s1);
//  for (i=0; i<2000000; i++)
 test_shake_imem_avx2x4(s1, o10, o11, o12, o13, t);
//  test_shake_array_avx2x4(s2, o2, t);
 test_shake_orig_avx2(s0, o0, t);

//  test_pabsorb_avx2(s2, t);
//  test_absorb_imem_avx2(s2,t);
//  test_oldabsorb(s1, t);
//  test_keccakF_avx2(s2);
//  test_keccakF(s1);


  buf_check("BUF imem ", o0, o10);
  buf_check("BUF imem ", o0, o11);
  buf_check("BUF imem ", o0, o12);
  buf_check("BUF imem ", o0, o13);
//  ststx4_check("STATE imem ", s0, s1);
//  buf_check("BUF array", o0, o2);
//  st_check("STATE array", s0, s2);

  return 0;
}
