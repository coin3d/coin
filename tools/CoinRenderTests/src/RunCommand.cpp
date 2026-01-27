#include "RunCommand.h"

#include <algorithm>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits.h>
#include <sstream>
#include <set>
#include <string>
#include <vector>

#include <filesystem>
namespace fs = std::filesystem;

#include "GLProfile.h"
#include "SpecDirResolver.h"
#include "CompareCommand.h"
#include "SpecLoader.h"
#include "SnapshotCommand.h"

namespace {

using CoinRenderTests::GLProfile;

struct Options {
  std::string spec_dir = CoinRenderTests::SpecDir::computeDefaultSpecDir();
  std::string backend = "gl";
  std::string artifacts_dir;
  GLProfile profile = GLProfile::Compatibility;
  bool verbose = false;
  std::set<std::string> only_ids;
  bool update_baselines = false;
  int retries = 0;
};

using SpecEntry = std::pair<std::string, CoinRenderTests::RenderTestSpec>;

struct MetricsSummary {
  bool valid = false;
  double diff_pct = 0.0;
  double rmse = 0.0;
  int max_abs_diff = 0;
};

constexpr const char* kColorReset = "\033[0m";
constexpr const char* kColorPass = "\033[32m";
constexpr const char* kColorFail = "\033[31m";
constexpr const char* kColorInfo = "\033[34m";

struct OutputResult {
  bool success = false;
  bool baseline_updated = false;
  std::string actual;
  std::string diff;
  std::string metrics;
  MetricsSummary metrics_summary;
};

bool ensure_directory(const std::string& path) {
  if (path.empty()) {
    return true;
  }
  fs::path target(path);
  try {
    if (!fs::exists(target)) {
      fs::create_directories(target);
    }
  } catch (const fs::filesystem_error&) {
    return false;
  }
  return fs::exists(target) && fs::is_directory(target);
}

bool ensure_parent(const std::string& path) {
  fs::path parent = fs::path(path).parent_path();
  if (parent.empty()) {
    return true;
  }
  return ensure_directory(parent.string());
}

std::string extract_json_value(const std::string& line) {
  auto colon = line.find(':');
  if (colon == std::string::npos) {
    return "";
  }
  auto value = line.substr(colon + 1);
  auto start = value.find_first_not_of(" \t\r\n");
  if (start == std::string::npos) {
    return "";
  }
  auto end = value.find_last_not_of(" \t\r\n,");
  if (end == std::string::npos || end < start) {
    return "";
  }
  return value.substr(start, end - start + 1);
}

MetricsSummary parse_metrics(const std::string& path) {
  MetricsSummary summary;
  std::ifstream in(path);
  if (!in) {
    return summary;
  }
  std::string line;
  while (std::getline(in, line)) {
    if (line.find("\"diff_pct\"") != std::string::npos) {
      auto token = extract_json_value(line);
      if (!token.empty()) {
        summary.diff_pct = std::stod(token);
        summary.valid = true;
      }
    } else if (line.find("\"rmse\"") != std::string::npos) {
      auto token = extract_json_value(line);
      if (!token.empty()) {
        summary.rmse = std::stod(token);
        summary.valid = true;
      }
    } else if (line.find("\"max_abs_diff\"") != std::string::npos) {
      auto token = extract_json_value(line);
      if (!token.empty()) {
        summary.max_abs_diff = std::stoi(token);
        summary.valid = true;
      }
    }
  }
  return summary;
}

bool copy_file(const std::string& src, const std::string& dst) {
  std::ifstream in(src, std::ios::binary);
  if (!in.is_open()) {
    return false;
  }
  ensure_parent(dst);
  std::ofstream out(dst, std::ios::binary);
  if (!out.is_open()) {
    return false;
  }
  out << in.rdbuf();
  return true;
}

bool parse_options(int argc, const char** argv, Options& opts) {
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--spec-dir" && i + 1 < argc) {
      opts.spec_dir = argv[++i];
    } else if (arg == "--backend" && i + 1 < argc) {
      opts.backend = argv[++i];
    } else if (arg == "--artifacts-dir" && i + 1 < argc) {
      opts.artifacts_dir = argv[++i];
    } else if (arg == "--gl-profile" && i + 1 < argc) {
      const std::string profile = argv[++i];
      if (profile == "compat") {
        opts.profile = GLProfile::Compatibility;
      } else if (profile == "core") {
        opts.profile = GLProfile::Core;
      } else {
        std::cerr << "Unknown GL profile: " << profile << " (expected compat|core)\n";
        return false;
      }
    } else if (arg == "--verbose") {
      opts.verbose = true;
    } else if (arg == "--only" && i + 1 < argc) {
      std::string value = argv[++i];
      std::istringstream ss(value);
      std::string token;
      while (std::getline(ss, token, ',')) {
        if (!token.empty()) {
          opts.only_ids.insert(token);
        }
      }
    } else if (arg == "--update-baselines") {
      opts.update_baselines = true;
    } else if (arg == "--retries" && i + 1 < argc) {
      opts.retries = std::max(0, std::atoi(argv[++i]));
    } else {
      std::cerr << "Unknown argument: " << arg << '\n';
      return false;
    }
  }
  return true;
}

void print_usage() {
  std::cerr << "Usage: CoinRenderTests run [--spec-dir <dir>] [--backend <gl|gles|web>]"
            << " [--artifacts-dir <dir>] [--gl-profile compat|core] [--only id1,id2]"
            << " [--verbose] [--update-baselines] [--retries N]\n";
  const std::string default_artifacts = CoinRenderTests::SpecDir::computeArtifactsBase() + "/gl";
  std::cerr << "Defaults: spec-dir=" << CoinRenderTests::SpecDir::computeDefaultSpecDir()
            << " backend=gl gl-profile=compat artifacts-dir=" << default_artifacts << "\n";
}

int invoke_snapshot(const std::string& spec_path,
                    const std::string& backend,
                    const std::string& actual_path,
                    GLProfile profile,
                    bool quiet) {
  std::vector<std::string> args = {"snapshot", "--spec", spec_path, "--backend", backend, "--out", actual_path};
  if (quiet) {
    args.push_back("--quiet");
  }
  if (profile == GLProfile::Core) {
    args.push_back("--gl-profile");
    args.push_back("core");
  }
  std::vector<const char*> c_args;
  for (const std::string& token : args) {
    c_args.push_back(token.c_str());
  }
  return CoinRenderTests::RunSnapshotCommand(static_cast<int>(c_args.size()), c_args.data());
}

int invoke_compare(const std::string& expected,
                   const std::string& actual,
                   const std::string& diff,
                   const std::string& metrics,
                   const std::string& mask,
                   const CoinRenderTests::ToleranceSpec& tolerance) {
  std::vector<std::string> args = {"compare", "--expected", expected, "--actual", actual, "--quiet"};
  if (!mask.empty()) {
    args.push_back("--mask");
    args.push_back(mask);
  }
  args.push_back("--diff");
  args.push_back(diff);
  args.push_back("--metrics");
  args.push_back(metrics);
  args.push_back("--tolerance-per-channel");
  args.push_back(std::to_string(tolerance.per_channel));
  args.push_back("--max-diff-pct");
  args.push_back(std::to_string(tolerance.max_diff_pct));
  if (tolerance.rmse > 0.0) {
    args.push_back("--rmse");
    args.push_back(std::to_string(tolerance.rmse));
  }
  std::vector<const char*> c_args;
  for (const std::string& token : args) {
    c_args.push_back(token.c_str());
  }
  return CoinRenderTests::RunCompareCommand(static_cast<int>(c_args.size()), c_args.data());
}

OutputResult run_single_output(const std::string& spec_path,
                               const CoinRenderTests::RenderTestSpec& spec,
                               const CoinRenderTests::OutputSpec& output,
                               const Options& opts,
                               bool update_baselines) {
  OutputResult result;
  const std::string output_dir = opts.artifacts_dir + "/" + opts.backend;
  const std::string actual = output_dir + "/" + spec.id + ".actual.png";
  const std::string diff = output_dir + "/" + spec.id + ".diff.png";
  const std::string metrics = output_dir + "/" + spec.id + ".metrics.json";
  ensure_directory(output_dir);
  ensure_parent(actual);
  ensure_parent(diff);
  ensure_parent(metrics);

  result.actual = actual;
  result.diff = diff;
  result.metrics = metrics;
  result.baseline_updated = update_baselines;

  if (update_baselines) {
    if (invoke_snapshot(spec_path, opts.backend, actual, opts.profile, !opts.verbose) != 0) {
      std::cerr << "Snapshot failed for spec " << spec.id << '\n';
      return result;
    }
    ensure_parent(output.baseline);
    if (!copy_file(actual, output.baseline)) {
      std::cerr << "Failed to update baseline " << output.baseline << '\n';
      return result;
    }
    result.success = true;
    return result;
  }

  if (invoke_snapshot(spec_path, opts.backend, actual, opts.profile, !opts.verbose) != 0) {
    std::cerr << "Snapshot failed for spec " << spec.id << '\n';
    return result;
  }

  int compare_rc = invoke_compare(output.baseline, actual, diff, metrics, output.mask, output.tolerance);
  result.metrics_summary = parse_metrics(metrics);
  result.success = (compare_rc == 0);
  return result;
}

int run_spec(const SpecEntry& entry,
             const Options& opts,
             bool update_baselines,
             int retries,
             int& passed,
             int& failed,
             int& updated) {
  const std::string& spec_path = entry.first;
  const CoinRenderTests::RenderTestSpec& spec = entry.second;

  if (!opts.only_ids.empty() && opts.only_ids.count(spec.id) == 0) {
    return 0;
  }

  const auto* output = spec.findOutputForBackend(opts.backend);
  if (!output) {
    std::cerr << "Spec " << spec.id << " does not target backend " << opts.backend << '\n';
    return 1;
  }

  if (opts.verbose) {
    std::cout << "\n" << kColorInfo << "[CoinRenderTests] Running spec '" << spec.id
              << "' backend '" << opts.backend << "'" << kColorReset << "\n";
  }
  auto print_result_summary = [&](const OutputResult& result) {
    const char* status = result.success ? kColorPass : kColorFail;
    const char* label = result.success ? "PASS" : "FAIL";
    if (result.success && !opts.verbose) {
      std::cout << status << label << kColorReset << " " << spec.id << "\n"
                << "  actual " << result.actual << "\n";
      return;
    }

    std::ostringstream stats;
    if (result.metrics_summary.valid) {
      stats << "diff_pct=" << result.metrics_summary.diff_pct << "% "
            << "rmse=" << result.metrics_summary.rmse << " "
            << "max_diff=" << result.metrics_summary.max_abs_diff;
    } else {
      stats << "metrics=n/a";
    }

    std::cout << status << label << kColorReset << " " << spec.id << " (" << stats.str() << ")\n";
    std::cout << "  actual " << result.actual << "\n";
    std::cout << "  diff   " << result.diff << "\n";
    std::cout << "  metrics " << result.metrics << "\n";
  };

  int attempt = 0;
  while (attempt <= retries) {
    OutputResult result = run_single_output(spec_path, spec, *output, opts, update_baselines);
    if (result.baseline_updated) {
      std::cout << kColorInfo << "[CoinRenderTests] Updated baseline for spec '" << spec.id << "' backend '" << opts.backend
                << "' -> " << output->baseline << " (actual=(" << result.actual << "))" << kColorReset << "\n";
      ++updated;
      ++passed;
      return 0;
    }
    print_result_summary(result);
    if (result.success) {
      ++passed;
      return 0;
    }
    ++failed;
    ++attempt;
    if (attempt <= retries) {
      if (opts.verbose) {
        std::cerr << "Retrying " << spec.id << " (attempt " << attempt << ")\n";
      }
    }
  }
  return 1;
}

int run_with_options(int argc, const char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    const std::string arg = argv[i];
    if (arg == "--help" || arg == "-h") {
      print_usage();
      return 0;
    }
  }

  Options opts;
  if (!parse_options(argc, argv, opts)) {
    print_usage();
    return 1;
  }

  if (opts.artifacts_dir.empty()) {
    opts.artifacts_dir = CoinRenderTests::SpecDir::computeArtifactsBase();
  }

  if (!ensure_directory(opts.artifacts_dir)) {
    std::cerr << "Failed to create artifacts dir: " << opts.artifacts_dir << '\n';
    return 1;
  }

  CoinRenderTests::SpecLoader loader;
  std::vector<SpecEntry> specs;
  try {
    specs = loader.loadAllSpecs(opts.spec_dir);
  } catch (const std::exception& ex) {
    std::cerr << "Failed to load specs from " << opts.spec_dir << ": " << ex.what() << '\n';
    return 1;
  }
  if (specs.empty()) {
    std::cerr << "No specs found in " << opts.spec_dir << '\n';
    return 1;
  }

  std::sort(specs.begin(), specs.end(), [](const SpecEntry& a, const SpecEntry& b) {
    return a.second.id < b.second.id;
  });

  bool failed = false;
  int passed = 0;
  int failed_count = 0;
  int updated = 0;
  for (const auto& entry : specs) {
    if (run_spec(entry, opts, opts.update_baselines, opts.retries, passed, failed_count, updated) != 0) {
      failed = true;
      if (opts.update_baselines) {
        continue;
      }
    }
  }

  std::cout << "\n" << (failed ? kColorFail : kColorPass)
            << (failed ? "FAILED" : "PASSED") << kColorReset
            << " specs: " << passed << " passed, " << failed_count << " failed";
  if (opts.update_baselines) {
    std::cout << " (updated " << updated << ")";
  }
  std::cout << "\n";

  return failed ? 1 : 0;
}

} // namespace

namespace CoinRenderTests {

int RunRunCommand(int argc, const char* argv[]) {
  return run_with_options(argc, argv);
}

int RunRunCommand(int argc, char** argv) {
  return run_with_options(argc, const_cast<const char**>(argv));
}

} // namespace CoinRenderTests
