#include <Inventor/C/basic.h>

#ifdef __cplusplus
#error "This regression client must be compiled as C"
#endif

typedef struct cc_xml_ent cc_xml_ent;

COIN_DLL_API cc_xml_ent * cc_xml_ent_new(void);
COIN_DLL_API void cc_xml_ent_delete_x(cc_xml_ent * ent);
COIN_DLL_API const char * cc_xml_ent_get_name(const cc_xml_ent * ent);
COIN_DLL_API const char * cc_xml_ent_get_value(const cc_xml_ent * ent);

int
main(void)
{
  cc_xml_ent * ent = cc_xml_ent_new();
  if (ent == NULL) return 1;
  if (cc_xml_ent_get_name(ent) != NULL) return 2;
  if (cc_xml_ent_get_value(ent) != NULL) return 3;
  cc_xml_ent_delete_x(ent);
  return 0;
}
