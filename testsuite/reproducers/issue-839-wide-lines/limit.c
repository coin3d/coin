#include <dlfcn.h>
#include <GL/gl.h>

void GLAPIENTRY
glGetFloatv(GLenum name, GLfloat * values)
{
  void (*getFloatv)(GLenum, GLfloat *) = dlsym(RTLD_NEXT, "glGetFloatv");
  getFloatv(name, values);
  if ((name == GL_LINE_WIDTH_RANGE || name == GL_ALIASED_LINE_WIDTH_RANGE) &&
      values[1] > 7.0f) {
    values[1] = 7.0f;
  }
}
