#include "SnapshotCommand.h"

#include <Inventor/SoDB.h>

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

#include "SpecDirResolver.h"
#include "SpecLoader.h"

namespace fs = std::filesystem;

namespace {

bool looksLikeSceneFile(const std::string& value) {
  if (value.find('/') == 0 || value.find('\\') == 0) {
    // absolute paths are treated as scenes.
    return true;
  }
  if (value.find('/') != std::string::npos || value.find('\\') != std::string::npos) {
    return true;
  }
  auto dot = value.find_last_of('.');
  if (dot == std::string::npos) {
    return false;
  }
  std::string suffix = value.substr(dot);
  std::transform(suffix.begin(), suffix.end(), suffix.begin(), [](unsigned char c) { return std::tolower(c); });
  return suffix == ".iv";
}

struct SnapshotArgs {
  std::string spec_dir = CoinRenderTests::SpecDir::computeDefaultSpecDir();
  std::string spec_id;
  std::string spec_path;
};

std::string default_output_file(const SnapshotArgs& args) {
  std::string base = "CoinRenderTests_snapshot";
  if (!args.spec_id.empty()) {
    base = args.spec_id;
  } else if (!args.spec_path.empty()) {
    base = fs::path(args.spec_path).stem().string();
  } else if (!args.spec_dir.empty()) {
    base = "snapshot";
  }
  fs::path path;
  try {
    path = fs::current_path();
    path /= (base + ".png");
    return path.string();
  } catch (const fs::filesystem_error&) {
    return base + ".png";
  }
}

void print_usage() {
  std::cout << "Usage: CoinRenderTests snapshot [<spec_id>|<scene.iv>]"
            << " [--spec <spec.yml>] [--spec-dir <dir>]"
            << " [--visible-window] [--out <image.png>] [--backend gl|gles]"
            << " [--gl-profile compat|core] [--wait] [--quiet]"
            << " [--width N] [--height N] [--msaa N]\n";
}

bool parse_args(int argc, const char* argv[], SnapshotArgs& args, CoinRenderTests::RenderCore::Options& opts) {
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--scene" && i + 1 < argc) {
      opts.scene_file = argv[++i];
    } else if (arg == "--out" && i + 1 < argc) {
      opts.output_file = argv[++i];
    } else if (arg == "--backend" && i + 1 < argc) {
      opts.backend = argv[++i];
      if (opts.backend != "gl" && opts.backend != "gles") {
        std::cerr << "Unsupported backend: " << opts.backend << '\n';
        return false;
      }
    } else if (arg == "--width" && i + 1 < argc) {
      const char* value = argv[++i];
      try {
        opts.width = std::max(8, std::stoi(value));
        opts.width_override = true;
      } catch (...) {
        std::cerr << "Invalid width value: " << value << '\n';
        return false;
      }
    } else if (arg == "--height" && i + 1 < argc) {
      const char* value = argv[++i];
      try {
        opts.height = std::max(8, std::stoi(value));
        opts.height_override = true;
      } catch (...) {
        std::cerr << "Invalid height value: " << value << '\n';
        return false;
      }
    } else if (arg == "--msaa" && i + 1 < argc) {
      const char* value = argv[++i];
      try {
        opts.msaa_samples = std::max(0, std::stoi(value));
        opts.msaa_override = true;
      } catch (...) {
        std::cerr << "Invalid msaa value: " << value << '\n';
        return false;
      }
    } else if (arg == "--spec" && i + 1 < argc) {
      args.spec_path = argv[++i];
    } else if (arg == "--spec-dir" && i + 1 < argc) {
      args.spec_dir = argv[++i];
    } else if (arg == "--gl-profile" && i + 1 < argc) {
      const std::string profile = argv[++i];
      if (profile == "compat") {
        opts.profile = CoinRenderTests::GLProfile::Compatibility;
      } else if (profile == "core") {
        opts.profile = CoinRenderTests::GLProfile::Core;
      } else {
        std::cerr << "Unknown GL profile: " << profile << " (expected compat|core)\n";
        return false;
      }
    } else if (arg == "--quiet") {
      opts.quiet = true;
    } else if (arg == "--visible-window") {
      opts.visible_window = true;
    } else if (arg == "--wait") {
      opts.wait_for_close = true;
    } else if (arg.rfind("--", 0) == 0) {
      std::cerr << "Unknown argument: " << arg << '\n';
      return false;
    } else {
      if (!args.spec_id.empty()) {
        std::cerr << "Only one positional spec identifier is allowed.\n";
        return false;
      }
      if (looksLikeSceneFile(arg)) {
        opts.scene_file = arg;
      } else {
        args.spec_id = arg;
      }
    }
  }

  return true;
}

bool configure_spec(const std::string& spec_path,
                    CoinRenderTests::RenderCore::Options& opts,
                    std::unique_ptr<CoinRenderTests::RenderTestSpec>& spec) {
  try {
    if (!spec) {
      spec.reset(new CoinRenderTests::RenderTestSpec(CoinRenderTests::SpecLoader().load(spec_path)));
    }
  } catch (const std::exception& ex) {
    std::cerr << "Failed to load spec: " << ex.what() << '\n';
    return false;
  }
  const auto* output = spec->findOutputForBackend(opts.backend);
  if (!output) {
    std::cerr << "Spec '" << spec->id << "' does not define backend '" << opts.backend << "'\n";
    return false;
  }
  opts.scene_file = spec->scene;
  if (!opts.width_override) {
    opts.width = std::max(8, spec->viewport.width);
  }
  if (!opts.height_override) {
    opts.height = std::max(8, spec->viewport.height);
  }
  if (!opts.msaa_override) {
    opts.msaa_samples = spec->state.msaa_samples;
  }
  opts.spec_mode = true;
  opts.camera = spec->camera;
  opts.viewport = spec->viewport;
  opts.state = spec->state;
  const auto toggle_it = spec->toggles.find("lighting");
  if (toggle_it != spec->toggles.end()) {
    opts.add_default_light = toggle_it->second;
  }
  if (spec->viewport.has_background) {
    opts.clear_color = spec->viewport.background;
    opts.background_set = true;
  }
  return true;
}

int run_with_args(int argc, const char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    const std::string arg = argv[i];
    if (arg == "--help" || arg == "-h") {
      print_usage();
      return 0;
    }
  }

  CoinRenderTests::RenderCore::Options opts;
  SnapshotArgs args;
  if (!parse_args(argc, argv, args, opts)) {
    print_usage();
    return 1;
  }

  if (opts.output_file.empty()) {
    opts.output_file = default_output_file(args);
  }

  CoinRenderTests::SpecLoader loader;
  std::unique_ptr<CoinRenderTests::RenderTestSpec> spec;
  std::string resolved_spec_path;
  if (!args.spec_id.empty()) {
    std::pair<std::string, CoinRenderTests::RenderTestSpec> entry;
    if (!loader.loadSpecById(args.spec_dir, args.spec_id, entry)) {
      std::cerr << "Spec '" << args.spec_id << "' not found in " << args.spec_dir << '\n';
      return 1;
    }
    resolved_spec_path = entry.first;
    spec.reset(new CoinRenderTests::RenderTestSpec(std::move(entry.second)));
  } else if (!args.spec_path.empty()) {
    resolved_spec_path = args.spec_path;
  }

  if (!resolved_spec_path.empty()) {
    if (!configure_spec(resolved_spec_path, opts, spec)) {
      return 1;
    }
  } else if (opts.scene_file.empty()) {
    print_usage();
    return 1;
  }

  SoDB::init();

  CoinRenderTests::RenderCore core;
  if (!core.initialize(opts)) {
    return 1;
  }

  if (!core.run()) {
    return 1;
  }

  return 0;
}

} // namespace

namespace CoinRenderTests {

int RunSnapshotCommand(int argc, const char* argv[]) {
  return run_with_args(argc, argv);
}

int RunSnapshotCommand(int argc, char** argv) {
  return run_with_args(argc, const_cast<const char**>(argv));
}

} // namespace CoinRenderTests
