#include "CompareCore.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace CoinRenderTests {

bool load_image(const std::string& path, Image& out, int desired_channels) {
  if (desired_channels != 4) {
    return false;
  }
  int channels = 0;
  stbi_set_flip_vertically_on_load(false);
  stbi_uc* data = stbi_load(path.c_str(), &out.width, &out.height, &channels, desired_channels);
  if (!data) {
    return false;
  }
  const size_t total = static_cast<size_t>(out.width) * out.height * 4;
  out.pixels.assign(data, data + total);
  stbi_image_free(data);
  return true;
}

bool write_image(const std::string& path, const Image& image) {
  stbi_flip_vertically_on_write(false);
  return stbi_write_png(path.c_str(), image.width, image.height, 4, image.pixels.data(), image.width * 4) != 0;
}

DiffResult compare_images(const Image& expected,
                          const Image& actual,
                          const Image* mask,
                          const CoinRenderTests::ToleranceSpec& tolerance) {
  DiffResult result;
  result.metrics.width = expected.width;
  result.metrics.height = expected.height;
  if (expected.width != actual.width || expected.height != actual.height) {
    result.metrics.pass = false;
    return result;
  }
  if (mask && (mask->width != expected.width || mask->height != expected.height)) {
    result.metrics.pass = false;
    return result;
  }
  const int width = expected.width;
  const int height = expected.height;
  const size_t total_pixels = static_cast<size_t>(width) * height;
  result.metrics.width = width;
  result.metrics.height = height;
  result.metrics.total_pixels = static_cast<int>(total_pixels);
  result.diff_pixels.assign(total_pixels * 4, 0);
  double sum_sq = 0.0;
  size_t compared_pixels = 0;
  size_t differing_pixels = 0;
  int max_abs_diff = 0;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      const size_t idx = static_cast<size_t>(y) * width + x;
      const size_t base = idx * 4;
      bool skip = false;
      if (mask) {
        const size_t mask_base = base;
        if (mask->pixels[mask_base + 3] == 0) {
          skip = true;
        }
      }
      if (skip) {
        result.diff_pixels[base + 3] = 0;
        continue;
      }
      ++compared_pixels;
      int local_max = 0;
      bool pixel_diff = false;
      for (int channel = 0; channel < 4; ++channel) {
        const int e = expected.pixels[base + channel];
        const int a = actual.pixels[base + channel];
        const int d = std::abs(e - a);
        sum_sq += static_cast<double>(d) * static_cast<double>(d);
        local_max = std::max(local_max, d);
        if (d > tolerance.per_channel) {
          pixel_diff = true;
        }
        max_abs_diff = std::max(max_abs_diff, d);
      }
      if (pixel_diff) {
        ++differing_pixels;
      }
      const int intensity = std::min(255, local_max * 4);
      result.diff_pixels[base + 0] = static_cast<uint8_t>(intensity);
      result.diff_pixels[base + 1] = static_cast<uint8_t>(intensity);
      result.diff_pixels[base + 2] = static_cast<uint8_t>(intensity);
      result.diff_pixels[base + 3] = 255;
    }
  }
  result.metrics.compared_pixels = static_cast<int>(compared_pixels);
  result.metrics.differing_pixels = static_cast<int>(differing_pixels);
  result.metrics.max_abs_diff = max_abs_diff;
  if (compared_pixels > 0) {
    result.metrics.diff_pct =
        100.0 * static_cast<double>(differing_pixels) / static_cast<double>(compared_pixels);
    result.metrics.rmse =
        std::sqrt(sum_sq / (static_cast<double>(compared_pixels) * 4.0));
    result.metrics.pass = (result.metrics.diff_pct <= tolerance.max_diff_pct) &&
                          (tolerance.rmse <= 0.0 || result.metrics.rmse <= tolerance.rmse);
  } else {
    result.metrics.diff_pct = 0.0;
    result.metrics.rmse = 0.0;
    result.metrics.pass = true;
  }
  return result;
}

bool write_metrics_json(const std::string& path, const DiffMetrics& metrics) {
  std::ofstream out(path);
  if (!out) {
    return false;
  }
  out << "{\n";
  out << "  \"width\": " << metrics.width << ",\n";
  out << "  \"height\": " << metrics.height << ",\n";
  out << "  \"total_pixels\": " << metrics.total_pixels << ",\n";
  out << "  \"compared_pixels\": " << metrics.compared_pixels << ",\n";
  out << "  \"differing_pixels\": " << metrics.differing_pixels << ",\n";
  out << std::fixed << std::setprecision(6);
  out << "  \"diff_pct\": " << metrics.diff_pct << ",\n";
  out << "  \"max_abs_diff\": " << metrics.max_abs_diff << ",\n";
  out << "  \"rmse\": " << metrics.rmse << ",\n";
  out << "  \"pass\": " << (metrics.pass ? "true" : "false") << "\n";
  out << "}\n";
  return true;
}

} // namespace CoinRenderTests
