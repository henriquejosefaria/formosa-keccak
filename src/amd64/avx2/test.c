#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include <stdint.h>
extern void test_oldabsorb(uint64_t*, uint8_t*);
extern void test_pabsorb_avx2(uint64_t*, uint8_t*);
extern void test_absorb_imem_avx2(uint64_t*, uint8_t*);
extern void test_keccakF_avx2(uint64_t *);
extern void test_keccakF(uint64_t *);
extern void test_shake_imem_avx2(uint64_t*, uint8_t*, uint8_t*);
extern void test_oldshake(uint64_t*, uint8_t*, uint8_t*);


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

int main() {
  uint64_t s1[28];
  uint64_t s2[28];
  uint8_t t[2*ASIZE];
  uint8_t o1[2*OSIZE];
  uint8_t o2[2*OSIZE];
  int i;
  for (i=0; i<ASIZE; i++) { t[i]=(uint8_t)i; }
  for (i=ASIZE; i<2*ASIZE; i++) { t[i]=0xFF; }
  for (i=0; i<2*OSIZE; i++) { o1[i]=0xFF; o2[i]=0xFF; }

  test_shake_imem_avx2(s2, o2, t);
  for (i=0; i<2000000; i++)
  test_oldshake(s1, o1, t);

//  test_pabsorb_avx2(s2, t);
//  test_absorb_imem_avx2(s2,t);
//  test_oldabsorb(s1, t);
//  test_keccakF_avx2(s2);
//  test_keccakF(s1);


  buf_check("BUF", o1, o2);
  st_check("STATE", s1, s2);

  return 0;
}
