#include <Inventor/C/basic.h>

struct cc_xml_ent;

extern "C" COIN_DLL_API cc_xml_ent * cc_xml_ent_new(void);

// These are the signatures exported by Coin before the public C declarations
// and the implementation were made consistent. Existing binaries still need
// their C++-linkage symbols.
COIN_DLL_API void cc_xml_ent_delete(cc_xml_ent * ent);
COIN_DLL_API const char * cc_xml_ent_get_name(cc_xml_ent * ent);
COIN_DLL_API const char * cc_xml_ent_get_value(cc_xml_ent * ent);

int
main()
{
  cc_xml_ent * ent = cc_xml_ent_new();
  if (ent == NULL) return 1;
  if (cc_xml_ent_get_name(ent) != NULL) return 2;
  if (cc_xml_ent_get_value(ent) != NULL) return 3;
  cc_xml_ent_delete(ent);
  return 0;
}
