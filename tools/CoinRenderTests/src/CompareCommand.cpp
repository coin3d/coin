#include "CompareCommand.h"

#include <iostream>
#include <sstream>
#include <string>

#include "CompareCore.h"

namespace {

struct Config {
  std::string expected;
  std::string actual;
  std::string mask;
  std::string diff;
  std::string metrics;
  bool quiet = false;
  CoinRenderTests::ToleranceSpec tolerance;
};

void log_error(const Config& cfg, const std::string& msg) {
  if (cfg.quiet) {
    std::cerr << "  " << msg << '\n';
  } else {
    std::cerr << msg << '\n';
  }
}

void print_usage() {
  std::cout << "Usage: CoinRenderTests compare --expected <png> --actual <png> --diff <png> --metrics <json>"
            << " [--mask <png>] [--tolerance-per-channel N] [--max-diff-pct P] [--rmse R] [--quiet]\n";
}

bool parse_args(int argc, const char* argv[], Config& cfg) {
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--expected" && i + 1 < argc) {
      cfg.expected = argv[++i];
    } else if (arg == "--actual" && i + 1 < argc) {
      cfg.actual = argv[++i];
    } else if (arg == "--mask" && i + 1 < argc) {
      cfg.mask = argv[++i];
    } else if (arg == "--diff" && i + 1 < argc) {
      cfg.diff = argv[++i];
    } else if (arg == "--metrics" && i + 1 < argc) {
      cfg.metrics = argv[++i];
    } else if (arg == "--tolerance-per-channel" && i + 1 < argc) {
      cfg.tolerance.per_channel = std::stoi(argv[++i]);
    } else if (arg == "--max-diff-pct" && i + 1 < argc) {
      cfg.tolerance.max_diff_pct = std::stod(argv[++i]);
    } else if (arg == "--rmse" && i + 1 < argc) {
      cfg.tolerance.rmse = std::stod(argv[++i]);
    } else if (arg == "--quiet") {
      cfg.quiet = true;
    } else {
      std::cerr << "Unknown argument: " << arg << '\n';
      return false;
    }
  }
  return !cfg.expected.empty() && !cfg.actual.empty();
}

std::string render_result(const CoinRenderTests::DiffMetrics& metrics) {
  std::ostringstream out;
  out << "diff_pct=" << metrics.diff_pct << "% "
      << "rmse=" << metrics.rmse << " "
      << "max_diff=" << metrics.max_abs_diff << " "
      << "failures=" << metrics.differing_pixels;
  return out.str();
}

int run_with_args(int argc, const char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    const std::string arg = argv[i];
    if (arg == "--help" || arg == "-h") {
      print_usage();
      return 0;
    }
  }

  Config cfg;
  if (!parse_args(argc, argv, cfg)) {
    print_usage();
    return 2;
  }

  CoinRenderTests::Image expected;
  CoinRenderTests::Image actual;
  CoinRenderTests::Image mask;
  if (!CoinRenderTests::load_image(cfg.expected, expected)) {
    log_error(cfg, std::string("Failed to load expected image: ") + cfg.expected);
    return 2;
  }
  if (!CoinRenderTests::load_image(cfg.actual, actual)) {
    log_error(cfg, std::string("Failed to load actual image: ") + cfg.actual);
    return 2;
  }
  CoinRenderTests::Image* mask_ptr = nullptr;
  if (!cfg.mask.empty()) {
    if (!CoinRenderTests::load_image(cfg.mask, mask)) {
      log_error(cfg, std::string("Failed to load mask: ") + cfg.mask);
      return 2;
    }
    mask_ptr = &mask;
  }

  auto result = CoinRenderTests::compare_images(expected, actual, mask_ptr, cfg.tolerance);
  if (!cfg.quiet) {
    std::cout << "Compare result: " << render_result(result.metrics) << '\n';
  }

  if (!cfg.diff.empty()) {
    CoinRenderTests::Image diff;
    diff.width = result.metrics.width;
    diff.height = result.metrics.height;
    diff.pixels = result.diff_pixels;
    if (!CoinRenderTests::write_image(cfg.diff, diff)) {
      log_error(cfg, std::string("Failed to write diff image: ") + cfg.diff);
      return 2;
    }
  }
  if (!cfg.metrics.empty()) {
    if (!CoinRenderTests::write_metrics_json(cfg.metrics, result.metrics)) {
      log_error(cfg, std::string("Failed to write metrics JSON: ") + cfg.metrics);
      return 2;
    }
  }

  return result.metrics.pass ? 0 : 1;
}

} // namespace

namespace CoinRenderTests {

int RunCompareCommand(int argc, const char* argv[]) {
  return run_with_args(argc, argv);
}

int RunCompareCommand(int argc, char** argv) {
  return run_with_args(argc, const_cast<const char**>(argv));
}

} // namespace CoinRenderTests
