#ifndef COIN_REPRODUCER_CLEANUP_H
#define COIN_REPRODUCER_CLEANUP_H

#include <Inventor/SoDB.h>

// Declare immediately after SoDB::init(), before local Coin objects, so
// those objects are destroyed before the library's global resources.
class CoinReproducerCleanup {
public:
  CoinReproducerCleanup() {}
  ~CoinReproducerCleanup() { SoDB::finish(); }
private:
  CoinReproducerCleanup(const CoinReproducerCleanup &);
  CoinReproducerCleanup & operator=(const CoinReproducerCleanup &);
};

#endif
