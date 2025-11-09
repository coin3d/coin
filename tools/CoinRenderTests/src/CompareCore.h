#ifndef COIN_COMPARE_CORE_H
#define COIN_COMPARE_CORE_H

#include <cstdint>
#include <string>
#include <vector>

#include "RenderTolerance.h"

namespace CoinRenderTests {

struct Image {
  int width = 0;
  int height = 0;
  std::vector<uint8_t> pixels;
};

struct DiffMetrics {
  int width = 0;
  int height = 0;
  int total_pixels = 0;
  int compared_pixels = 0;
  int differing_pixels = 0;
  double diff_pct = 0.0;
  int max_abs_diff = 0;
  double rmse = 0.0;
  bool pass = false;
};

struct DiffResult {
  DiffMetrics metrics;
  std::vector<uint8_t> diff_pixels;
};

bool load_image(const std::string& path, Image& out, int desired_channels = 4);
bool write_image(const std::string& path, const Image& image);
DiffResult compare_images(const Image& expected,
                          const Image& actual,
                          const Image* mask,
                          const CoinRenderTests::ToleranceSpec& tolerance);
bool write_metrics_json(const std::string& path, const DiffMetrics& metrics);

} // namespace CoinRenderTests

#endif // COIN_COMPARE_CORE_H
