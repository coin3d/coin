#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/nodes/SoInfo.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSeparator.h>

#include <cstdio>
#include <cstdlib>

static bool
check(const bool condition, const char * message)
{
  if (!condition) std::fprintf(stderr, "%s\n", message);
  return condition;
}

static void *
resizeOutputBuffer(void * pointer, size_t size)
{
  return std::realloc(pointer, size);
}

static bool
exerciseReferenceRoundTrip(const SbBool binary)
{
  SoSeparator * source = new SoSeparator;
  source->ref();
  SoSeparator * unnamed = new SoSeparator;
  SoSeparator * named = new SoSeparator;
  named->setName("shared,param");
  SoSeparator * terminal = new SoSeparator;
  terminal->setName("terminal,");
  SoSeparator * nested = new SoSeparator;
  SoSeparator * plain = new SoSeparator;
  nested->addChild(plain);
  nested->addChild(new SoInfo);
  nested->addChild(plain);

  source->addChild(unnamed);
  source->addChild(unnamed);
  source->addChild(named);
  source->addChild(named);
  source->addChild(terminal);
  source->addChild(terminal);
  source->addChild(nested);
  source->addChild(terminal);

  void * buffer = std::malloc(1024);
  size_t bytes = 0;
  bool valid = check(buffer != NULL,
                     "could not allocate reference serialization buffer");
  if (buffer) {
    SoOutput output;
    output.setBuffer(buffer, 1024, resizeOutputBuffer);
    output.setBinary(binary);
    SoWriteAction writer(&output);
    writer.apply(source);
    valid &= check(output.getBuffer(buffer, bytes) && bytes > 0,
                   "reference serialization produced no output");
  }
  source->unref();

  SoSeparator * parsed = NULL;
  if (valid) {
    SoInput input;
    input.setBuffer(buffer, bytes);
    SoNode * node = NULL;
    const bool readok = SoDB::read(&input, node);
    const bool read = readok && node != NULL &&
      node->getTypeId() == SoSeparator::getClassTypeId();
    valid &= check(read, "shared reference graph could not be read");
    if (read) parsed = static_cast<SoSeparator *>(node);
    else if (node) {
      node->ref();
      node->unref();
    }
  }
  std::free(buffer);

  if (!parsed) return false;
  parsed->ref();
  valid &= check(parsed->getNumChildren() == 8,
                 "shared reference child count changed");
  if (parsed->getNumChildren() == 8) {
    valid &= check(parsed->getChild(0) == parsed->getChild(1),
                   "anonymous comma-delimited USE identity changed");
    valid &= check(parsed->getChild(2) == parsed->getChild(3),
                   "named comma-delimited USE identity changed");
    valid &= check(parsed->getChild(2)->getName() == "shared,param",
                   "internal comma in reference name changed");
    valid &= check(parsed->getChild(4) == parsed->getChild(5) &&
                   parsed->getChild(4) == parsed->getChild(7),
                   "terminal-comma reference identity changed");
    valid &= check(parsed->getChild(4)->getName() == "terminal,",
                   "terminal comma in reference name changed");
    SoNode * nestednode = parsed->getChild(6);
    const bool nestedtype = nestednode->getTypeId() ==
      SoSeparator::getClassTypeId();
    valid &= check(nestedtype, "ordinary USE container type changed");
    if (nestedtype) {
      SoSeparator * parsednested = static_cast<SoSeparator *>(nestednode);
      valid &= check(parsednested->getNumChildren() == 3 &&
                     parsednested->getChild(0) == parsednested->getChild(2),
                     "ordinary final USE identity changed");
    }
  }
  parsed->unref();
  return valid;
}

static bool
exerciseDelimitedFieldReferences(void)
{
  const char delimited[] =
    "[ DEF internal,name Separator { } USE internal,name, "
    "DEF terminal Separator { } DEF terminal, Separator { } "
    "USE terminal,, USE terminal, ]";
  SoInput input;
  input.setBuffer(delimited, sizeof(delimited) - 1);
  SoMFNode field;
  bool valid = check(field.read(&input, SbName("nodes")) &&
                       field.getNum() == 6,
                     "comma-delimited MFNode references could not be read");
  if (field.getNum() == 6) {
    valid &= check(field[0] == field[1] &&
                   field[0]->getName() == "internal,name",
                   "internal-comma MFNode reference changed");
    valid &= check(field[3] == field[4] && field[3] == field[5] &&
                   field[2] != field[3] &&
                   field[3]->getName() == "terminal,",
                   "terminal-comma MFNode reference changed");
  }

  const char ordinary[] =
    "[ DEF plain Separator { } USE plain ]";
  SoInput plaininput;
  plaininput.setBuffer(ordinary, sizeof(ordinary) - 1);
  SoMFNode plainfield;
  valid &= check(plainfield.read(&plaininput, SbName("nodes")) &&
                 plainfield.getNum() == 2 &&
                 plainfield[0] == plainfield[1],
                 "ordinary MFNode reference changed");
  return valid;
}

static bool
exerciseFieldReference(void)
{
  const char data[] =
    "#Inventor V2.1 ascii\n"
    "Separator {\n"
    "  DEF camera PerspectiveCamera { }\n"
    "  Rotation { rotation = USE camera.orientation }\n"
    "}\n";
  SoInput input;
  input.setBuffer(data, sizeof(data) - 1);
  SoNode * node = NULL;
  const bool readok = SoDB::read(&input, node);
  bool valid = check(readok && node != NULL &&
                     node->getTypeId() == SoSeparator::getClassTypeId(),
                     "field-reference graph could not be read");
  if (!valid) {
    if (node) {
      node->ref();
      node->unref();
    }
    return false;
  }

  SoSeparator * root = static_cast<SoSeparator *>(node);
  root->ref();
  valid &= check(root->getNumChildren() == 2,
                 "field-reference child count changed");
  if (root->getNumChildren() == 2) {
    SoNode * cameranode = root->getChild(0);
    SoNode * rotationnode = root->getChild(1);
    const bool types =
      cameranode->getTypeId() == SoPerspectiveCamera::getClassTypeId() &&
      rotationnode->getTypeId() == SoRotation::getClassTypeId();
    valid &= check(types, "field-reference node types changed");
    if (types) {
      SoPerspectiveCamera * camera =
        static_cast<SoPerspectiveCamera *>(cameranode);
      SoRotation * rotation = static_cast<SoRotation *>(rotationnode);
      SoField * master = NULL;
      valid &= check(rotation->rotation.getConnectedField(master) &&
                     master == &camera->orientation,
                     "USE name.field connection changed");
    }
  }
  root->unref();
  return valid;
}

int
main(void)
{
  SoDB::init();
  const bool valid = exerciseReferenceRoundTrip(FALSE) &&
    exerciseReferenceRoundTrip(TRUE) &&
    exerciseDelimitedFieldReferences() && exerciseFieldReference();
  SoDB::finish();
  return valid ? 0 : 1;
}
