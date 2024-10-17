#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#define ILEN1 101
#define ILEN2 1013
#define ILEN3 2019
#define OLEN  235

#define ILEN (ILEN1+ILEN2+ILEN3)
#define RATE8 136

extern void test_state_init_avx2(uint64_t *);
extern void test_keccakf1600_avx2(uint64_t *);
extern void test_addratebit_avx2(uint64_t*);
extern void test_addpst_avx2(uint64_t*, uint64_t*);
extern uint64_t test_pabsorb_imem_avx2(uint64_t*, uint8_t*);
extern void test_absorb_imem_avx2(uint64_t*, uint8_t*);
extern void test_shake_imem_avx2(uint64_t*, uint8_t*, uint8_t*);
extern void test_multishake_imem_avx2(uint64_t*, uint8_t*, uint8_t*);

extern void test_pabsorb_array_avx2(uint64_t*, uint8_t*);
extern void test_absorb_array_avx2(uint64_t*, uint8_t*);
extern void test_shake_array_avx2(uint64_t*, uint8_t*, uint8_t*);
extern void test_multishake_array_avx2(uint64_t*, uint8_t*, uint8_t*, uint8_t*, uint8_t*);

extern void test_absorb_orig_avx2(uint64_t*, uint8_t*);
extern void test_shake_orig_avx2(uint64_t*, uint8_t*, uint8_t*);




void buf_print(char* str, uint8_t x[]) {
 int i, j;
 if (str!=NULL) printf("%s =\n", str);
 for (i=0; i<OLEN;) {
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

int test_eqbuf(uint8_t x[], uint8_t y[]) {
  int i;
  int r = 1;
  for (i=0; i < OLEN; i++) {
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

int main() {
  uint64_t s0[28];
  uint64_t s1[28];
  uint64_t s2[28];
  uint8_t t[2*ILEN];
  uint8_t o0[2*OLEN];
  uint8_t o1[2*OLEN];
  uint8_t o2[2*OLEN];
  int i, at;
  for (i=0; i<ILEN; i++) { t[i]=(uint8_t)i; }
  for (i=ILEN; i<2*ILEN; i++) { t[i]=0xFF; }
  for (i=0; i<2*OLEN; i++) { o0[i]=0xFF; o1[i]=0xFF; o2[i]=0xFF; }

//  for (i=0; i<2000000; i++)
 test_shake_imem_avx2(s1, o1, t);
 test_shake_array_avx2(s2, o2, t);
 test_shake_orig_avx2(s0, o0, t);

//  test_state_init_avx2(s1);
//  at = test_pabsorb_imem_avx2(s1, t);
//  printf("AT=%d\n", at);
//  test_absorb_orig_avx2(s0, t);
//  test_absorb_imem_avx2(s2,t);
//  test_keccakF_avx2(s2);


  buf_check("BUF shake_imem ", o0, o1);
  st_check("STATE shake_imem ", s0, s1);
  buf_check("BUF shake_array", o0, o2);
  st_check("STATE shake_array", s0, s2);


  test_multishake_imem_avx2(s1, o1, t);
  test_multishake_array_avx2(s2, o2, t, t+ILEN1, t+(ILEN1+ILEN2));

  buf_check("BUF multishake_imem ", o0, o1);
  st_check("STATE multishake_imem ", s0, s1);
  buf_check("BUF multishake_array", o0, o2);
  st_check("STATE multishake_array", s0, s2);

  return 0;
}
