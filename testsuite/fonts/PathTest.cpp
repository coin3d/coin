#include <Inventor/SoDB.h>
#include <Inventor/C/tidbits.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText2.h>

#include <cstdio>
#include <cstring>

enum Result { MISSING = 0, MATCHED = 1, UNAVAILABLE = 2, FALLBACK = 4 };

static void
check_log(const SoError * error, void * data)
{
  const char * message = error->getDebugString().getString();
  unsigned int & result = *static_cast<unsigned int *>(data);
  if (std::strstr(message, "realname='CoinPathFixture Regular'")) result |= MATCHED;
  if (std::strstr(message, "FreeType library will not be used")) result |= UNAVAILABLE;
  if (std::strstr(message, "fromdefaultfont==TRUE")) result |= FALLBACK;
  std::fprintf(stderr, "%s\n", message);
}

int
main(int argc, char ** argv)
{
  if (argc != 2) return 1;

  coin_setenv("COIN_DEBUG_FONTSUPPORT", "1", 1);
  SoDB::init();
  unsigned int result = MISSING;
  SoDebugError::setHandlerCallback(check_log, &result);

  SoSeparator * root = new SoSeparator;
  root->ref();
  SoOrthographicCamera * camera = new SoOrthographicCamera;
  camera->height = 64;
  camera->farDistance = 100;
  root->addChild(camera);

  SoFont * font = new SoFont;
  font->name = argv[1];
  font->size = 8;
  root->addChild(font);

  SoText2 * text = new SoText2;
  text->string = "A";
  root->addChild(text);

  SoGetBoundingBoxAction action(SbViewportRegion(64, 64));
  action.apply(root);
  root->unref();
  SoDB::finish();

  if (result & UNAVAILABLE) return 77;
  if (result == MATCHED) return 0;

  std::fprintf(stderr, "Expected CoinPathFixture without glyph fallback: %s\n", argv[1]);
  return 1;
}
