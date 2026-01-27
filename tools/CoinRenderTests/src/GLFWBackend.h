#ifndef COIN_SNAPSHOT_GLFW_BACKEND_H
#define COIN_SNAPSHOT_GLFW_BACKEND_H

#include <cstdint>
#include <functional>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <Inventor/system/gl.h>
#include "GLProfile.h"

namespace CoinRenderTests {

struct GLFWBackendConfig {
  int width = 512;
  int height = 512;
  int msaa_samples = 0;
  bool gles = false;
  int gl_major = 3;
  int gl_minor = 3;
  bool visible = false;
  bool offscreen = true;
  GLProfile profile = GLProfile::Compatibility;
};

class GLFWBackend {
public:
  GLFWBackend();
  ~GLFWBackend();

  bool init(const GLFWBackendConfig& config);
  void bindFramebuffer();
  bool readPixels(std::vector<uint8_t>& output) const;
  bool usesOffscreen() const;
  void swapBuffers() const;
  void waitForClose(const std::function<void(GLFWwindow*)>& per_frame = {}) const;

  int width() const { return config_.width; }
  int height() const { return config_.height; }
  bool initialized() const { return initialized_; }

private:
  GLFWBackendConfig config_;
  GLFWwindow* window_ = nullptr;
  GLuint framebuffer_ = 0;
  GLuint color_texture_ = 0;
  GLuint depth_renderbuffer_ = 0;
  bool initialized_ = false;
  bool uses_offscreen_ = true;
};

} // namespace CoinRenderTests

#endif // COIN_SNAPSHOT_GLFW_BACKEND_H
