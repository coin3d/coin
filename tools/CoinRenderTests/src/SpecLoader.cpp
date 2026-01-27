#include "SpecLoader.h"

#include <dirent.h>
#include <sys/stat.h>
#include <algorithm>
#include <yaml-cpp/yaml.h>

#include <stdexcept>

namespace CoinRenderTests {

namespace {

std::array<float, 3> toVec3(const YAML::Node& node, const std::array<float, 3>& fallback) {
  if (!node) {
    return fallback;
  }
  std::array<float, 3> values;
  for (size_t i = 0; i < values.size(); ++i) {
    if (!node[i]) {
      throw std::runtime_error("Missing vector entry at index " + std::to_string(i));
    }
    values[i] = static_cast<float>(node[i].as<double>());
  }
  return values;
}

std::array<float, 4> toVec4(const YAML::Node& node, const std::array<float, 4>& fallback) {
  if (!node) {
    return fallback;
  }
  std::array<float, 4> values;
  for (size_t i = 0; i < values.size(); ++i) {
    if (!node[i]) {
      throw std::runtime_error("Missing vector entry at index " + std::to_string(i));
    }
    values[i] = static_cast<float>(node[i].as<double>());
  }
  return values;
}

std::string requiredString(const YAML::Node& node, const char* key) {
  auto entry = node[key];
  if (!entry) {
    throw std::runtime_error(std::string("Missing required key: ") + key);
  }
  return entry.as<std::string>();
}

int optionalInt(const YAML::Node& node, const char* key, int fallback) {
  if (!node[key]) {
    return fallback;
  }
  return node[key].as<int>();
}

bool optionalBool(const YAML::Node& node, const char* key, bool fallback) {
  if (!node[key]) {
    return fallback;
  }
  return node[key].as<bool>();
}

std::string dirName(const std::string& path) {
  auto pos = path.find_last_of("/\\");
  if (pos == std::string::npos) {
    return "";
  }
  return path.substr(0, pos);
}

bool isAbsolutePath(const std::string& value) {
  if (value.empty()) {
    return false;
  }
  if (value.front() == '/' || value.front() == '\\') {
    return true;
  }
  if (value.size() >= 2 && value[1] == ':') {
    return true;
  }
  return false;
}

std::string resolveRelative(const std::string& baseDir, const std::string& target) {
  if (target.empty() || baseDir.empty() || isAbsolutePath(target)) {
    return target;
  }
  return baseDir + "/" + target;
}

} // namespace

std::vector<std::string> gatherSpecFiles(const std::string& dir) {
  std::vector<std::string> specs;
  DIR* dp = opendir(dir.c_str());
  if (!dp) {
    return specs;
  }
  dirent* entry;
  while ((entry = readdir(dp)) != nullptr) {
    if (entry->d_type == DT_DIR) {
      continue;
    }
    std::string name(entry->d_name);
    if (name == "." || name == "..") {
      continue;
    }
    if (name.size() >= 4) {
      const std::string ext = name.substr(name.size() - 4);
      if (ext == ".yml" || (ext.size() >= 5 && name.substr(name.size() - 5) == ".yaml")) {
        specs.push_back(dir + "/" + name);
      }
    }
  }
  closedir(dp);
  std::sort(specs.begin(), specs.end());
  return specs;
}

const OutputSpec* RenderTestSpec::findOutputForBackend(const std::string& backend) const {
  for (const auto& entry : outputs) {
    if (entry.backend == backend) {
      return &entry;
    }
  }
  return nullptr;
}

std::vector<std::pair<std::string, RenderTestSpec>> SpecLoader::loadAllSpecs(const std::string& dir) const {
  std::vector<std::pair<std::string, RenderTestSpec>> entries;
  for (const auto& path : gatherSpecFiles(dir)) {
    entries.emplace_back(path, load(path));
  }
  return entries;
}

bool SpecLoader::loadSpecById(const std::string& dir,
                              const std::string& id,
                              std::pair<std::string, RenderTestSpec>& out) const {
  for (const auto& entry : gatherSpecFiles(dir)) {
    RenderTestSpec spec = load(entry);
    if (spec.id == id) {
      out = std::make_pair(entry, std::move(spec));
      return true;
    }
  }
  return false;
}

RenderTestSpec SpecLoader::load(const std::string& path) const {
  YAML::Node root = YAML::LoadFile(path);
  RenderTestSpec spec;
  const std::string spec_dir = dirName(path);

  spec.id = requiredString(root, "id");
  spec.scene = resolveRelative(spec_dir, requiredString(root, "scene"));

  if (!root["camera"]) {
    throw std::runtime_error("Missing camera section in spec");
  }
  const auto& camera = root["camera"];
  const std::string type = camera["type"].as<std::string>("perspective");
  spec.camera.type = (type == "orthographic") ? CameraSpec::Type::Orthographic : CameraSpec::Type::Perspective;
  const auto toSbVec = [](const std::array<float, 3>& values) {
    return SbVec3f(values[0], values[1], values[2]);
  };
  spec.camera.position = toSbVec(toVec3(camera["position"], {2.0f, 2.0f, 2.0f}));
  spec.camera.target = toSbVec(toVec3(camera["target"], {0.0f, 0.0f, 0.0f}));
  spec.camera.up = toSbVec(toVec3(camera["up"], {0.0f, 1.0f, 0.0f}));
  spec.camera.fov_deg = static_cast<float>(camera["fov_deg"].as<double>(45.0));
  spec.camera.near = static_cast<float>(camera["near"].as<double>(0.1));
  spec.camera.far = static_cast<float>(camera["far"].as<double>(10.0));

  if (!root["viewport"]) {
    throw std::runtime_error("Missing viewport section in spec");
  }
  const auto& viewport = root["viewport"];
  spec.viewport.width = viewport["width"].as<int>(512);
  spec.viewport.height = viewport["height"].as<int>(512);
  if (viewport["background"]) {
    spec.viewport.background = toVec4(viewport["background"], spec.viewport.background);
    spec.viewport.has_background = true;
  }

  if (root["state"]) {
    const auto& state = root["state"];
    spec.state.depth_test = optionalBool(state, "depth_test", true);
    spec.state.blend = optionalBool(state, "blend", false);
    spec.state.cull_face = optionalBool(state, "cull_face", true);
    spec.state.msaa_samples = optionalInt(state, "msaa_samples", 0);
    spec.state.srgb = optionalBool(state, "srgb", false);
  }

  if (root["toggles"]) {
    for (const auto& entry : root["toggles"]) {
      spec.toggles[entry.first.as<std::string>()] = entry.second.as<bool>();
    }
  }

  if (!root["outputs"] || !root["outputs"].IsSequence()) {
    throw std::runtime_error("Spec missing outputs");
  }
  for (const auto& output : root["outputs"]) {
    OutputSpec info;
    info.backend = requiredString(output, "backend");
    info.baseline = requiredString(output, "baseline");
    if (output["mask"]) {
      info.mask = output["mask"].as<std::string>();
    }
    if (output["tolerance"]) {
      const auto& tolerance = output["tolerance"];
      info.tolerance.per_channel = optionalInt(tolerance, "per_channel", info.tolerance.per_channel);
      info.tolerance.max_diff_pct = tolerance["max_diff_pct"].as<double>(info.tolerance.max_diff_pct);
      info.tolerance.rmse = tolerance["rmse"].as<double>(info.tolerance.rmse);
    }
    info.baseline = resolveRelative(spec_dir, info.baseline);
    if (!info.mask.empty()) {
      info.mask = resolveRelative(spec_dir, info.mask);
    }
  spec.outputs.push_back(info);
  }

  if (spec.outputs.empty()) {
    throw std::runtime_error("Spec requires at least one output");
  }

  return spec;
}

} // namespace CoinRenderTests
