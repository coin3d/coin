#include <cstddef>
#include <cstdint>
#include <climits>

#include <Inventor/C/XML/attribute.h>
#include <Inventor/C/XML/document.h>
#include <Inventor/C/XML/element.h>

extern "C" int
LLVMFuzzerTestOneInput(const uint8_t * data, size_t size)
{
  // Keep standalone fuzz invocations bounded too, not only the CTest smoke.
  if (size > 4096 || size > static_cast<size_t>(INT_MAX)) return 0;

  cc_xml_doc * doc = cc_xml_doc_new();
  if (!doc) return 0;

  if (cc_xml_doc_read_buffer_x(
        doc, reinterpret_cast<const char *>(data), size)) {
    cc_xml_elt * root = cc_xml_doc_get_root(doc);
    if (root) {
      cc_xml_doc * clone_doc = cc_xml_doc_new();
      cc_xml_elt * clone = clone_doc ? cc_xml_elt_clone(root) : NULL;
      if (!clone) {
        cc_xml_doc_delete_x(clone_doc);
        cc_xml_doc_delete_x(doc);
        return 0;
      }
      cc_xml_doc_set_root_x(clone_doc, clone);

      const int numattributes = cc_xml_elt_get_num_attributes(clone);
      if (numattributes > 0) {
        const cc_xml_attr ** attributes = cc_xml_elt_get_attributes(clone);
        cc_xml_elt_set_attribute_x(clone, cc_xml_attr_clone(attributes[0]));
      }

      const int numchildren = cc_xml_elt_get_num_children(clone);
      if (numchildren > 0) {
        cc_xml_elt * child = cc_xml_elt_get_child(clone, 0);
        cc_xml_elt_remove_child_x(clone, child);
        cc_xml_elt_insert_child_x(clone, child,
                                  cc_xml_elt_get_num_children(clone));
        cc_xml_elt_add_child_x(child, clone);
      }

      cc_xml_elt * released = cc_xml_doc_release_root_x(clone_doc);
      cc_xml_doc_set_root_x(clone_doc, released);

      char * serialized = NULL;
      size_t serialized_size = 0;
      if (cc_xml_doc_write_to_buffer(
            clone_doc, &serialized, &serialized_size) && serialized) {
        cc_xml_doc * reparsed = cc_xml_doc_new();
        cc_xml_doc_read_buffer_x(reparsed, serialized, serialized_size);
        cc_xml_doc_delete_x(reparsed);
        delete [] serialized;
      }
      cc_xml_doc_delete_x(clone_doc);
    }
  }

  cc_xml_doc_delete_x(doc);
  return 0;
}
