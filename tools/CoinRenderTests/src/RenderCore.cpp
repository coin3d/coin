#include "RenderCore.h"

#include "GLFWBackend.h"

#include <GLFW/glfw3.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/SbLinear.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/lists/SoPathList.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/SoPath.h>
#include <Inventor/system/gl.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <vector>

#include <stb_image_write.h>

namespace CoinRenderTests {

namespace {

int removeSceneCameras(SoNode* root) {
  if (!root) {
    return 0;
  }
  SoSearchAction search;
  search.setType(SoCamera::getClassTypeId());
  search.setSearchingAll(TRUE);
  search.setInterest(SoSearchAction::ALL);
  search.apply(root);
  const SoPathList& paths = search.getPaths();
  int removed = 0;
  for (size_t idx = 0; idx < paths.getLength(); ++idx) {
    SoPath* path = paths[idx];
    if (!path || path->getLength() < 2) {
      continue;
    }
    SoNode* camera = path->getTail();
    if (!camera) {
      continue;
    }
    SoPath* parent_path = path->copy();
    parent_path->truncate(parent_path->getLength() - 1);
    SoNode* parent = parent_path->getTail();
    if (parent && parent->getTypeId().isDerivedFrom(SoGroup::getClassTypeId())) {
      static_cast<SoGroup*>(parent)->removeChild(camera);
      ++removed;
    }
    parent_path->unref();
  }
  return removed;
}

} // namespace

struct RenderCore::Impl {
  Options options;
  GLFWBackend backend;
  SoSeparator* scene_root = nullptr;
  std::vector<uint8_t> pixels;
  SoCamera* camera = nullptr;
  SbVec3f camera_target = SbVec3f(0.0f, 0.0f, 0.0f);
  SbVec3f camera_up = SbVec3f(0.0f, 1.0f, 0.0f);
};

RenderCore::RenderCore() : impl_(new Impl()) {}

RenderCore::~RenderCore() {
  if (impl_) {
    if (impl_->scene_root) {
      impl_->scene_root->unref();
      impl_->scene_root = nullptr;
    }
    delete impl_;
  }
}

bool RenderCore::initialize(const Options& options) {
  if (!impl_) {
    impl_ = new Impl();
  }

  impl_->options = options;

  if (options.scene_file.empty() || options.output_file.empty()) {
    std::cerr << "Scene file and output path are required." << std::endl;
    return false;
  }

  GLFWBackendConfig config;
  config.width = options.width;
  config.height = options.height;
  config.msaa_samples = options.msaa_samples;
  config.gles = (options.backend == "gles");
  config.visible = options.visible_window;
  config.offscreen = !options.visible_window;
  config.profile = options.profile;
  if (config.gles) {
    config.gl_major = 2;
    config.gl_minor = 0;
  } else {
    config.gl_major = 3;
    config.gl_minor = 3;
  }

  if (!impl_->backend.init(config)) {
    return false;
  }

  SoInput input;
  if (!input.openFile(options.scene_file.c_str())) {
    std::cerr << "Unable to open scene file: " << options.scene_file << std::endl;
    return false;
  }

  SoNode* loaded = SoDB::readAll(&input);
  if (!loaded) {
    std::cerr << "Failed to parse scene file: " << options.scene_file << std::endl;
    return false;
  }

  loaded->ref();
  int removed_cameras = 0;
  if (options.spec_mode) {
    removed_cameras = removeSceneCameras(loaded);
  }
  if (removed_cameras > 0 && options.spec_mode) {
    std::cerr << "[CoinRenderTests] Spec camera overriding " << removed_cameras
              << " camera(s) defined inside the scene; they are ignored.\n";
  }

  impl_->scene_root = new SoSeparator;
  impl_->scene_root->ref();

  SoCamera* camera = nullptr;
  if (options.spec_mode && options.camera.type == CameraSpec::Type::Orthographic) {
    camera = new SoOrthographicCamera;
    static_cast<SoOrthographicCamera*>(camera)
        ->height.setValue(options.camera.fov_deg);
  } else {
    auto* perspective = new SoPerspectiveCamera;
    const float radians = options.camera.fov_deg * static_cast<float>(M_PI / 180.0);
    perspective->heightAngle.setValue(radians);
    camera = perspective;
  }
  camera->position.setValue(options.camera.position);
  camera->pointAt(options.camera.target, options.camera.up);
  camera->nearDistance.setValue(options.camera.near);
  camera->farDistance.setValue(options.camera.far);
  impl_->scene_root->addChild(camera);
  impl_->camera = camera;
  impl_->camera_target = options.camera.target;
  impl_->camera_up = options.camera.up;

  if (options.add_default_light) {
    auto light = new SoDirectionalLight;
    light->direction.setValue(SbVec3f(-0.5f, -1.0f, -0.5f));
    impl_->scene_root->addChild(light);
    if (!options.quiet) {
      std::cout << "[CoinRenderTests] Using default directional light\n";
    }
  } else if (!options.quiet) {
    std::cout << "[CoinRenderTests] Scene lighting only (default light disabled)\n";
  }
  impl_->scene_root->addChild(loaded);
  loaded->unref();

  return true;
}

bool RenderCore::renderScene(bool capture) {
  if (!impl_ || !impl_->scene_root) {
    return false;
  }

  impl_->backend.bindFramebuffer();
  glViewport(0, 0, impl_->options.width, impl_->options.height);
  if (impl_->options.state.depth_test) {
    glEnable(GL_DEPTH_TEST);
  } else {
    glDisable(GL_DEPTH_TEST);
  }
  if (impl_->options.state.blend) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  } else {
    glDisable(GL_BLEND);
  }
  if (impl_->options.state.cull_face) {
    glEnable(GL_CULL_FACE);
  } else {
    glDisable(GL_CULL_FACE);
  }
  if (impl_->options.state.srgb) {
    glEnable(GL_FRAMEBUFFER_SRGB);
  } else {
    glDisable(GL_FRAMEBUFFER_SRGB);
  }
  const auto& bg = impl_->options.background_set ? impl_->options.clear_color
                                                  : std::array<float, 4>{0.2f, 0.2f, 0.2f, 1.0f};
  glClearColor(bg[0], bg[1], bg[2], bg[3]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  SoGLRenderAction action(SbViewportRegion(impl_->options.width, impl_->options.height));
  action.apply(impl_->scene_root);
  if (!impl_->backend.usesOffscreen()) {
    impl_->backend.swapBuffers();
  }

  if (!capture) {
    return true;
  }

  impl_->pixels.clear();
  if (!impl_->backend.readPixels(impl_->pixels)) {
    std::cerr << "Failed to read pixels from the framebuffer." << std::endl;
    return false;
  }

  const int stride = impl_->options.width * 4;
  const int height = impl_->options.height;
  for (int row = 0; row < height / 2; ++row) {
    auto* top = impl_->pixels.data() + static_cast<size_t>(row) * stride;
    auto* bottom = impl_->pixels.data() + static_cast<size_t>(height - row - 1) * stride;
    for (int i = 0; i < stride; ++i) {
      std::swap(top[i], bottom[i]);
    }
  }

  if (!stbi_write_png(impl_->options.output_file.c_str(),
                      impl_->options.width,
                      impl_->options.height,
                      4,
                      impl_->pixels.data(),
                      stride)) {
    std::cerr << "Failed to write PNG output: " << impl_->options.output_file << std::endl;
    return false;
  }

  return true;
}

bool RenderCore::run() {
  if (!impl_ || !impl_->scene_root) {
    return false;
  }

  const bool capture = !(impl_->options.visible_window && impl_->options.wait_for_close);
  if (!renderScene(capture)) {
    return false;
  }

  if (impl_->options.visible_window && impl_->options.wait_for_close) {
    impl_->backend.waitForClose([this](GLFWwindow* window) {
      handleCameraInput(window);
      renderScene(false);
    });
  }

  impl_->scene_root->unref();
  impl_->scene_root = nullptr;
  impl_->camera = nullptr;

  if (!impl_->options.quiet) {
    std::cout << "Rendered scene to " << impl_->options.output_file << '\n';
  }
  return true;
}

void RenderCore::handleCameraInput(GLFWwindow* window) {
  if (!impl_ || !impl_->camera || !window) {
    return;
  }
  const float move_speed = 0.05f;
  SbVec3f position = impl_->camera->position.getValue();
  SbVec3f target = impl_->camera_target;
  SbVec3f forward = target - position;
  if (forward.normalize() == 0.0f) {
    return;
  }
  SbVec3f right = forward.cross(impl_->camera_up);
  if (right.normalize() == 0.0f) {
    return;
  }
  SbVec3f delta(0.0f, 0.0f, 0.0f);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    delta += forward * move_speed;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    delta -= forward * move_speed;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    delta += right * move_speed;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    delta -= right * move_speed;
  }
  if (delta == SbVec3f(0.0f, 0.0f, 0.0f)) {
    return;
  }
  position += delta;
  target += delta;
  impl_->camera->position.setValue(position);
  impl_->camera_target = target;
  impl_->camera->pointAt(target);
  auto round_vec3 = [](const SbVec3f& v) {
    SbVec3f out = v;
    for (int i = 0; i < 3; ++i) {
      out[i] = std::round(out[i] * 100.0f) / 100.0f;
    }
    return out;
  };
  SbVec3f log_pos = round_vec3(position);
  SbVec3f log_target = round_vec3(target);
  std::cout << "[CoinRenderTests] WASD move delta=(" << delta.getValue()[0] << "," << delta.getValue()[1]
            << "," << delta.getValue()[2] << ") pos=(" << log_pos.getValue()[0] << ","
            << log_pos.getValue()[1] << "," << log_pos.getValue()[2] << ") target=("
            << log_target.getValue()[0] << "," << log_target.getValue()[1] << "," << log_target.getValue()[2] << ")\n";
}

} // namespace CoinRenderTests
