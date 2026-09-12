#include <Inventor/SoDB.h>
#include <Inventor/SoRenderManager.h>
#include <Inventor/SoSceneManager.h>
#include <Inventor/system/gl.h>

#if COIN_HAVE_LEGACY_GL_RENDERER != COIN_EXPECT_LEGACY_GL_RENDERER
#error Installed Coin package reports an unexpected legacy renderer capability.
#endif

/**
 * Exercise Coin strictly through its installed CMake package and public
 * headers. Besides representative Coin classes, this preserves the historical
 * OpenGL fallback contract required by consumers with older platform headers
 * and verifies the configured legacy-renderer capability.
 */
int
main()
{
  const GLenum textureUnits[] = {GL_TEXTURE0, GL_TEXTURE4, GL_TEXTURE15};
  const GLenum activeTexture = GL_ACTIVE_TEXTURE;
  const GLenum clientActiveTexture = GL_CLIENT_ACTIVE_TEXTURE;
  (void) textureUnits;
  (void) activeTexture;
  (void) clientActiveTexture;

  SoDB::init();
  {
    SoRenderManager renderManager;
    SoSceneManager sceneManager;
    (void) renderManager;
    (void) sceneManager;
  }
  SoDB::finish();
  return 0;
}
