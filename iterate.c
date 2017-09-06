#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


unsigned in2eytz(unsigned i, unsigned n) {
  while (i & 1) {
    i >>= 1;
    n >>= 1;
  }
  i >>= 1;
  n >>= 1;
  return n + i;
}

#define ffs(i) __builtin_ffs(i)

#define i2e(i, n) ((n) >> ffs(~(i))) + ((i) >> (ffs(~(i))))

unsigned in2eytz2(unsigned i, unsigned n) {
  unsigned shift = ffs(~i);
  return (n >> shift) + (i>>shift);
}

#ifdef SANITY
int main(int argc, char *argv[])
{
  int n = 31;

  for (unsigned i = 0; i < n-1; i++) {
    printf("%u %u %u, ", i, in2eytz(i, n), i2e(i, n));
  }
  printf("%u %u %u\n", n-1, in2eytz(n-1, n), i2e(n-1, n));

  return 0;
}
#endif // SANITY

unsigned straight_sum(unsigned *a, unsigned n) {
  unsigned sum = 0;
  for (int i = 0; i < n; i++) {
    sum += a[i];
  }
  return sum;
}

unsigned eytzinger_sum(unsigned *a, unsigned n) {
  unsigned sum = 0;
  unsigned h = 0;
  while ((1u << (h+1)) - 1 < n) h++;

  unsigned i = (1 << h) - 1;

  while (i < n) {
    // printf("i+1 = %d\n", i);
    sum += a[i];
    if (2*i + 2 < n) {
      // i has a right child, so find its leftmost right descendant
      i = 2*i + 2;
      while (2*i + 1 < n) i = 2*i + 1;
    } else if (! ((i+2) & (i+1))) {
      // i has no right child child and is on the rightmost path, so we're done
      i = n;
    } else {
      // go up to i's first successor
      while (i % 2 == 0) i = (i-1)/2;
      i = (i-1)/2;
    }
  }
  return sum;
}

unsigned trickseytzinger_sum(unsigned *a, unsigned n) {
  unsigned sum = 0;
  unsigned h = 0;
  while ((1u << (h+1)) - 1 < n) h++;

  unsigned i = (1 << h) - 1;

  while (i < n) {
    // printf("i+1 = %d\n", i);
    sum += a[i];
    if (2*i + 2 < n) {
      // i has a right child, so find its leftmost right descendant
      i = 2*i + 2;
      i++;
      while (2*i <= n) i = 2*i;
      i--;
    } else if (! ((i+2) & (i+1))) {
      // i has no right child child and is on the rightmost path, so we're done
      i = n;
    } else {
      // go up to i's first successor
      while (i % 2 == 0) i = (i-1)/2;
      i = (i-1)/2;
    }
  }
  return sum;
}

unsigned bfeytzinger_sum(unsigned *a, unsigned n) {
    if (((n+1) & n) != 0) {
      fprintf(stderr, "Only works for n=2^{k}-1\n");
    }
    unsigned sum = 0;
    unsigned h = 0;
    while ((1u << (h+1)) - 1 < n) h++;
    unsigned last = (1u << (h+1))-1 == n ? n : (1 << h)-1;
    unsigned i = 1 << h;
    unsigned olz = __builtin_clz(1) - h;
    unsigned leaf = ~0u;

    while (i != last)  {
      sum += a[i-1];
      unsigned oldi = i;
      i >>= leaf & __builtin_ffs(~i);
      leaf = ~leaf;
      unsigned b = leaf & 1;
      i = (i << b) | b;
      i <<= leaf & (__builtin_clz(i) - olz);
      if (__builtin_expect(oldi == n, 0)) {
        h -= 1;
        olz += 1;
        leaf = ~0u;
      }
    }
    sum += a[last-1];
    return sum;
}

unsigned fancy_sum(unsigned *a, unsigned n) {
  if (((n+1) & n) != 0) {
    fprintf(stderr, "Only works for n=2^{k}-1\n");
  }
  unsigned sum = 0;
  for (unsigned i = 0; i < n; i++) {
    unsigned shift = ffs(~i);
    sum += a[(n >> shift) + (i>>shift)];
  }
  return sum;
}

unsigned luis_sum(unsigned *a, unsigned n) {
  if (((n+1) & n) != 0) {
    fprintf(stderr, "Only works for n=2^{k}-1\n");
  }
  unsigned npp = n+1;
  unsigned stop = npp+n;
  unsigned sum = 0;
  for (unsigned i = npp; i < stop; i++) {
    unsigned shift = ffs(~i);
    sum += a[(i>>shift)-1];
  }
  return sum;
}

unsigned fancy_sum_pf(unsigned *a, unsigned n) {
  if (((n+1) & n) != 0) {
    fprintf(stderr, "Only works for n=2^{k}-1\n");
  }
  unsigned sum = 0;
  for (unsigned i = 0; i < n; i++) {
    unsigned shift = ffs(~i);
    unsigned j = (n >> shift) + (i>>shift);
    sum += a[j];
    __builtin_prefetch(a+j+16);
  }
  return sum;
}

/* This code illustrates that
 */
unsigned burgle_sum(unsigned *a, unsigned n) {
  if (((n+1) & n) != 0) {
    fprintf(stderr, "Only works for n=2^{k}-1\n");
  }
  unsigned npp = n+1;
  unsigned stop = npp+n;
  unsigned sum = 0;
  for (unsigned i = npp; i < stop; i++) {
    unsigned shift = ffs(~i);
    sum += i>>shift;
  }
  return sum;
}


unsigned fancy_sum2(unsigned *a, unsigned n) {
  if (((n+1) & n) != 0) {
    fprintf(stderr, "Only works for n=2^{k}-1\n");
  }
  unsigned sum = 0;
  for (unsigned i = 0; i < n; i++) {
    unsigned ip = i;
    unsigned np = n;
    while (ip & 1) {
      ip >>= 1;
      np >>= 1;
    }
    ip >>= 1;
    np >>= 1;
    sum += a[np+ip];
    printf("%u,", np+ip);
  }
  printf("\n");
  return sum;
}

unsigned fancy_sum3(unsigned *a, unsigned n) {
  if (((n+1) & n) != 0) {
    fprintf(stderr, "Only works for n=2^{k}-1\n");
  }
  unsigned sum = a[n/2];
  printf("%u,", n/2);
  for (unsigned i = 1; i < n; i++) {
    unsigned shift = __builtin_ctz(~i) + 1;
    sum += a[(n >> shift) + (i>>shift)];
    printf("%u,", (n >> shift) + (i>>shift));
  }
  return sum;
}

#define clz(i) __builtin_clz(i)

#define bits(k) (clz(1u) - clz(k))

#define unlikely(c) __builtin_expect((c), 0)

unsigned fancy_sum4(unsigned *a, unsigned n) {
  unsigned fullh = bits(n+1)-1;
  unsigned m = (1u << (fullh+1)) - 1;
  unsigned r = n % m;
  unsigned flag = 2*r;
  unsigned np = m+m+1;
  if (r == 0) {
      np = m;
      flag = n+1;
  }
  //printf("fullh=%u, m=%u, r=%u", fullh, m, r);
  unsigned sum = a[np/2];
  for (unsigned i = 1; i < n; i++) {
    unsigned shift = __builtin_ctz(i) + 1;
    sum += a[(np >> shift) + (i>>shift)];
    if (unlikely(i == flag)) {
      i = flag / 2;
      n -= i;
      flag = n+1;

      np = m;
    }
  }
  return sum;
}


int main(int argc, char *argv[])
{
  // p = 6000;

  unsigned n = 27;
  if (argc == 2) {
    n = atoi(argv[1]);
  }
  unsigned *a = malloc(n*sizeof(unsigned));
  for (int i = 0; i < n; i++) {
    a[i] = i+1;
  }

  clock_t start, stop;
  double elapsed;
  unsigned sum = 0;

  for (int r = 0; r < 50; r++) {
    start = clock();
    sum = 0;
    sum = straight_sum(a, n);
    stop = clock();
    elapsed = ((double)(stop-start))/CLOCKS_PER_SEC;
    printf("%22s %10u %10u %10u %lfs\n", "straight", n, sum, n*(n+1)/2, elapsed);

    start = clock();
    sum = eytzinger_sum(a, n);
    stop = clock();
    elapsed = ((double)(stop-start))/CLOCKS_PER_SEC;
    printf("%22s %10u %10u %10u %lfs\n", "eytzinger", n, sum, n*(n+1)/2, elapsed);

    start = clock();
    sum = bfeytzinger_sum(a, n);
    stop = clock();
    elapsed = ((double)(stop-start))/CLOCKS_PER_SEC;
    printf("%22s %10u %10u %10u %lfs\n", "branch-free eytzinger", n, sum, n*(n+1)/2, elapsed);

    start = clock();
    sum = fancy_sum(a, n);
    stop = clock();
    elapsed = ((double)(stop-start))/CLOCKS_PER_SEC;
    printf("%22s %10u %10u %10u %lfs\n", "fancy", n, sum, n*(n+1)/2, elapsed);

    start = clock();
    sum = fancy_sum_pf(a, n);
    stop = clock();
    elapsed = ((double)(stop-start))/CLOCKS_PER_SEC;
    printf("%22s %10u %10u %10u %lfs\n", "fancy_pf", n, sum, n*(n+1)/2, elapsed);

    start = clock();
    sum = burgle_sum(a, n);
    stop = clock();
    elapsed = ((double)(stop-start))/CLOCKS_PER_SEC;
    printf("%22s %10u %10u %10u %lfs\n", "burgle", n, sum, n*(n+1)/2, elapsed);

    start = clock();
    sum = luis_sum(a, n);
    stop = clock();
    elapsed = ((double)(stop-start))/CLOCKS_PER_SEC;
    printf("%22s %10u %10u %10u %lfs\n", "luis", n, sum, n*(n+1)/2, elapsed);

#ifdef USELESS

    start = clock();
    sum = fancy_sum2(a, n);
    stop = clock();
    elapsed = ((double)(stop-start))/CLOCKS_PER_SEC;
    printf("%22s %10u %10u %10u %lfs\n", "fancy2", n, sum, n*(n+1)/2, elapsed);

    start = clock();
    sum = fancy_sum3(a, n);
    stop = clock();
    elapsed = ((double)(stop-start))/CLOCKS_PER_SEC;
    printf("%22s %10u %10u %10u %lfs\n", "fancy3", n, sum, n*(n+1)/2, elapsed);

    start = clock();
    sum = fancy_sum4(a, n);
    stop = clock();
    elapsed = ((double)(stop-start))/CLOCKS_PER_SEC;
    printf("%22s %10u %10u %10u %lfs\n", "fancy4", n, sum, n*(n+1)/2, elapsed);

    start = clock();
    sum = trickseytzinger_sum(a, n);
    stop = clock();
    elapsed = ((double)(stop-start))/CLOCKS_PER_SEC;
    printf("%22s %10u %10u %10u %lfs\n", "bit-tricks eytzinger", n, sum, n*(n+1)/2, elapsed);

    start = clock();
    sum = eytzinger_sum(a, n);
    stop = clock();
    elapsed = ((double)(stop-start))/CLOCKS_PER_SEC;
    printf("%22s %10u %10u %10u %lfs\n", "eytzinger", n, sum, n*(n+1)/2, elapsed);
#endif //USELESS

  }
  free(a);
}
