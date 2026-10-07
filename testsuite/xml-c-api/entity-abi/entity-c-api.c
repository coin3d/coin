#include <Inventor/C/XML/entity.h>

#ifdef __cplusplus
#error "This regression client must be compiled as C"
#endif

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
