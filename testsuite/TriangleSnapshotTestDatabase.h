#ifndef COIN_TRIANGLE_SNAPSHOT_TEST_DATABASE_H
#define COIN_TRIANGLE_SNAPSHOT_TEST_DATABASE_H

#include <Inventor/SoDB.h>

// Declare before test fixtures so shutdown follows their destruction, including
// early returns and exceptions. Coin's global fields require explicit cleanup.
struct SnapshotTestDatabase {
  SnapshotTestDatabase() { SoDB::init(); }
  ~SnapshotTestDatabase() { SoDB::finish(); }
  SnapshotTestDatabase(const SnapshotTestDatabase &) = delete;
  SnapshotTestDatabase & operator=(const SnapshotTestDatabase &) = delete;
};

#endif
