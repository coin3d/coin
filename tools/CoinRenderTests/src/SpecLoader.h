#ifndef COIN_SNAPSHOT_SPEC_LOADER_H
#define COIN_SNAPSHOT_SPEC_LOADER_H

#include <array>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <Inventor/SbVec3f.h>

#include "RenderTolerance.h"

namespace CoinRenderTests {

struct CameraSpec {
  enum class Type { Perspective, Orthographic };
  Type type = Type::Perspective;
  SbVec3f position = SbVec3f(2.0f, 2.0f, 2.0f);
  SbVec3f target = SbVec3f(0.0f, 0.0f, 0.0f);
  SbVec3f up = SbVec3f(0.0f, 1.0f, 0.0f);
  float fov_deg = 45.0f;
  float near = 0.1f;
  float far = 10.0f;
};

struct ViewportSpec {
  int width = 512;
  int height = 512;
  std::array<float, 4> background = {0.2f, 0.2f, 0.2f, 1.0f};
  bool has_background = false;
};

struct StateSpec {
  bool depth_test = true;
  bool blend = false;
  bool cull_face = true;
  int msaa_samples = 0;
  bool srgb = false;
};

struct OutputSpec {
  std::string backend;
  std::string baseline;
  std::string mask;
  ToleranceSpec tolerance;
};

struct RenderTestSpec {
  std::string id;
  std::string scene;
  CameraSpec camera;
  ViewportSpec viewport;
  StateSpec state;
  std::map<std::string, bool> toggles;
  std::vector<OutputSpec> outputs;

  const OutputSpec* findOutputForBackend(const std::string& backend) const;
};

class SpecLoader {
public:
  SpecLoader() = default;
  RenderTestSpec load(const std::string& path) const;
  std::vector<std::pair<std::string, RenderTestSpec>> loadAllSpecs(const std::string& dir) const;
  bool loadSpecById(const std::string& dir,
                    const std::string& id,
                    std::pair<std::string, RenderTestSpec>& out) const;
};

} // namespace CoinRenderTests

#endif // COIN_SNAPSHOT_SPEC_LOADER_H
