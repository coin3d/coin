#include "GLFWBackend.h"

#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

namespace CoinRenderTests {

GLFWBackend::GLFWBackend() = default;

GLFWBackend::~GLFWBackend() {
  if (color_texture_) {
    glDeleteTextures(1, &color_texture_);
  }
  if (depth_renderbuffer_) {
    glDeleteRenderbuffers(1, &depth_renderbuffer_);
  }
  if (framebuffer_) {
    glDeleteFramebuffers(1, &framebuffer_);
  }
  if (window_) {
    glfwDestroyWindow(window_);
    glfwTerminate();
  }
}

bool GLFWBackend::init(const GLFWBackendConfig& config) {
  if (initialized_) {
    return true;
  }

  config_ = config;

  if (config_.width <= 0 || config_.height <= 0) {
    std::cerr << "Invalid framebuffer size: " << config_.width << "x" << config_.height << '\n';
    return false;
  }

  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW." << std::endl;
    return false;
  }

  glfwWindowHint(GLFW_VISIBLE, config_.visible ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_CLIENT_API, config_.gles ? GLFW_OPENGL_ES_API : GLFW_OPENGL_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config_.gl_major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config_.gl_minor);
  glfwWindowHint(GLFW_SAMPLES, config_.msaa_samples);
  if (!config_.gles) {
    if (config_.profile == GLProfile::Core) {
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    } else {
      // Coin still relies on legacy fixed-function bits; request a compatibility profile.
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    }
  }

  window_ = glfwCreateWindow(config_.width, config_.height, "CoinRenderTests", nullptr, nullptr);
  if (!window_) {
    std::cerr << "Failed to create GLFW window." << std::endl;
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window_);
  glfwSwapInterval(0);

  if (config_.offscreen) {
    glGenFramebuffers(1, &framebuffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

    glGenTextures(1, &color_texture_);
    glBindTexture(GL_TEXTURE_2D, color_texture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, config_.width, config_.height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture_, 0);

    glGenRenderbuffers(1, &depth_renderbuffer_);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_renderbuffer_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, config_.width, config_.height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_renderbuffer_);
  }

  GLenum status = GL_FRAMEBUFFER_COMPLETE;
  if (config_.offscreen) {
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "Framebuffer incomplete: " << status << std::endl;
    return false;
  }

  initialized_ = true;
  return true;
}

void GLFWBackend::bindFramebuffer() {
  if (!initialized_) {
    return;
  }
  if (config_.offscreen) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
  } else {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
  glViewport(0, 0, config_.width, config_.height);
}

bool GLFWBackend::readPixels(std::vector<uint8_t>& output) const {
  if (!initialized_) {
    return false;
  }
  const size_t stride = static_cast<size_t>(config_.width) * 4;
  output.resize(static_cast<size_t>(config_.height) * stride);
  if (config_.offscreen) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
  } else {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
  glReadPixels(0, 0, config_.width, config_.height, GL_RGBA, GL_UNSIGNED_BYTE, output.data());
  return true;
}

bool GLFWBackend::usesOffscreen() const {
  return config_.offscreen;
}

void GLFWBackend::swapBuffers() const {
  if (window_) {
    glfwSwapBuffers(window_);
  }
}

void GLFWBackend::waitForClose(const std::function<void(GLFWwindow*)>& per_frame) const {
  if (!window_) {
    return;
  }
  while (!glfwWindowShouldClose(window_)) {
    glfwPollEvents();
    if (per_frame) {
      per_frame(window_);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

} // namespace CoinRenderTests
