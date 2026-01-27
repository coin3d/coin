#ifndef COIN_RENDER_TOLERANCE_H
#define COIN_RENDER_TOLERANCE_H

namespace CoinRenderTests {

struct ToleranceSpec {
  int per_channel = 2;
  double max_diff_pct = 0.0;
  double rmse = -1.0;
};

} // namespace CoinRenderTests

#endif // COIN_RENDER_TOLERANCE_H
