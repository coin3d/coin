// ScXMLEventTarget::getEventTarget() must return NULL for a registered
// target name when the requested session does not exist.
#include <Inventor/SoDB.h>
#include <Inventor/scxml/ScXMLEventTarget.h>

#include <cstdio>

#if defined(_MSC_VER) && defined(_DEBUG)
#include <crtdbg.h>
#endif

namespace {

class LookupTarget : public ScXMLEventTarget {
  SCXML_OBJECT_HEADER(LookupTarget)

public:
  LookupTarget(void) { this->setEventTargetType("scxml"); }

  static void initClass(void)
  {
    SCXML_OBJECT_INIT_CLASS(LookupTarget, ScXMLEventTarget, "ScXMLEventTarget");
  }

  static void cleanClass(void)
  {
    LookupTarget::classTypeId = SoType::badType();
  }

  static ScXMLEventTarget * lookup(const char * type, const char * name,
                                   const char * session = NULL)
  {
    return ScXMLEventTarget::getEventTarget(type, name, session);
  }
};

SCXML_OBJECT_SOURCE(LookupTarget)

bool
check(bool condition, const char * message)
{
  if (!condition) std::fprintf(stderr, "%s\n", message);
  return condition;
}

int
run(void)
{
  LookupTarget target;
  target.setEventTargetName("lookup-target");
  bool valid = check(LookupTarget::lookup("scxml", "lookup-target") == &target,
                     "default-session lookup failed");
  valid &= check(LookupTarget::lookup("absent-type", "lookup-target") == NULL,
                 "missing type was not rejected");
  valid &= check(LookupTarget::lookup("scxml", "absent-name") == NULL,
                 "missing name was not rejected");
  valid &= check(LookupTarget::lookup("scxml", "lookup-target", "absent-session") == NULL,
                 "missing session was not rejected");
  valid &= check(LookupTarget::lookup("scxml", "lookup-target") == &target,
                 "lookup failed after a rejected session");
  return valid ? 0 : 1;
}

} // namespace

int
main(void)
{
#if defined(_MSC_VER) && defined(_DEBUG)
  // Report checked-iterator failures on stderr instead of a dialog.
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif
  SoDB::init();
  LookupTarget::initClass();
  const int result = run();
  LookupTarget::cleanClass();
  SoDB::finish();
  if (result == 0) std::printf("event target lookup rejects missing sessions\n");
  return result;
}
