#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include <stdint.h>
extern void oldabsorb(uint64_t*, uint8_t*);
extern void newabsorb(uint64_t*, uint8_t*);


#define ASIZE (234+111)
#define RATE8 136


void st_print(char* str, uint64_t x[]) {
 int i, j;
 if (str!=NULL) printf("%s =\n", str);
 for (i=0; i<5; i++) {
  for (j=0; j<5; j++) {
   if (0 < j) printf(", ");
   printf("0x%016llx ", x[5*i+j]);
  }
  printf("\n");
 }
 printf("\n");
}


int test_eq(uint64_t x[], uint64_t y[]) {
  int i;
  int r = 1;
  for (i=0; i < 25; i++)
    r = r && (x[i]==y[i]);
  return r;
}

int st_check(char* str, uint64_t x[], uint64_t y[]) {
  int r = test_eq(x,y);
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
  uint64_t s1[25];
  uint64_t s2[25];
  uint8_t t[ASIZE];
  int i;
  for (i=0; i<ASIZE; i++) {
    t[i]=i;
  }
  for (i=ASIZE; i<2*ASIZE; i++) {
    t[i]=0xFF;
  }
  newabsorb(s2, t);
  oldabsorb(s1, t);
  st_check("ABSORB", s1, s2);

  return 0;
}
