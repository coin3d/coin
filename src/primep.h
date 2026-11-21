#ifndef COIN_PRIMEP_H
#define COIN_PRIMEP_H

/* Prime bucket sizes used for geometric hash table growth. Bucket counts
   are unsigned int, so the final entry is the largest 32-bit prime. */
static const unsigned long coin_bucket_prime_table[] = {
  2, 5, 11, 17, 37, 67, 131, 257,
  521, 1031, 2053, 4099, 8209, 16411, 32771, 65537,
  131101, 262147, 524309, 1048583, 2097169, 4194319,
  8388617, 16777259, 33554467, 67108879, 134217757,
  268435459, 536870923, 1073741827, 2147483659UL,
  4294967291UL
};

static inline unsigned long
coin_exact_prime_at_least(unsigned long num)
{
  const unsigned long largest =
    coin_bucket_prime_table[sizeof(coin_bucket_prime_table) /
                            sizeof(coin_bucket_prime_table[0]) - 1];
  if (num <= 2) return 2;
  if (num > largest) return 0;

  for (unsigned long candidate = num | 1UL;
       candidate <= largest; candidate += 2) {
    bool prime = true;
    for (unsigned long divisor = 3;
         divisor <= candidate / divisor; divisor += 2) {
      if (candidate % divisor == 0) {
        prime = false;
        break;
      }
    }
    if (prime) return candidate;
  }
  return 0;
}

static inline unsigned long
coin_growth_prime_at_least(unsigned long num)
{
  for (unsigned int i = 0;
       i < sizeof(coin_bucket_prime_table) /
             sizeof(coin_bucket_prime_table[0]); ++i) {
    if (coin_bucket_prime_table[i] >= num) return coin_bucket_prime_table[i];
  }
  return 0;
}

#endif // COIN_PRIMEP_H
