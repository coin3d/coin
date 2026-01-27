#ifndef COIN_SNAPSHOT_SPEC_DIR_RESOLVER_H
#define COIN_SNAPSHOT_SPEC_DIR_RESOLVER_H

#include <limits.h>
#include <string>
#include <filesystem>

#include <whereami.h>

namespace CoinRenderTests {

namespace SpecDir {

inline std::string getExecutableDirectory() {
  char path_buf[PATH_MAX];
  int len = wai_getExecutablePath(path_buf, sizeof(path_buf), nullptr);
  if (len <= 0) {
    return ".";
  }
  std::string exe_path(path_buf, len);
  auto pos = exe_path.find_last_of("/\\");
  if (pos == std::string::npos) {
    return ".";
  }
  return exe_path.substr(0, pos);
}

inline std::string computeDefaultSpecDir() {
  namespace fs = std::filesystem;
  fs::path dir = getExecutableDirectory();
  while (!dir.empty()) {
    fs::path candidate = dir / "tests" / "render" / "specs";
    if (fs::exists(candidate) && fs::is_directory(candidate)) {
      return candidate.string();
    }
    fs::path parent = dir.parent_path();
    if (parent == dir) {
      break;
    }
    dir = parent;
  }
  return "testsuite/render/specs";
}

inline std::string computeArtifactsBase() {
  namespace fs = std::filesystem;
  fs::path dir = getExecutableDirectory();
  fs::path base = dir;
  if (!base.empty() && base.has_filename()) {
    base = base.parent_path();
  }
  if (base.empty()) {
    base = ".";
  }
  return (base / "render_artifacts").string();
}

} // namespace SpecDir

} // namespace CoinRenderTests

#endif // COIN_SNAPSHOT_SPEC_DIR_RESOLVER_H
