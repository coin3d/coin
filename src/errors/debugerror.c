#include <Inventor/C/errors/debugerror.h>
#include <stdlib.h>

/* FIXME: name clash with cc_error's static members, will cause
   all-c.c compilation to fail. 20020526 mortene. */
static cc_error_cb * callback = cc_error_default_handler_cb;
static void * callback_data = NULL;

void
cc_debugerror_init(cc_debugerror * me)
{
  cc_error_init((cc_error *)me);
}

void
cc_debugerror_clean(cc_debugerror * me)
{
  cc_error_clean((cc_error *)me);
}


CC_DEBUGERROR_SEVERITY
cc_debugerror_get_severity(const cc_debugerror * me)
{
  return me->severity;
}

void
cc_debugerror_set_handler_callback(cc_error_cb * function, void * data)
{
  callback = function;
  callback_data = data;
}

cc_error_cb *
cc_debugerror_get_handler_callback(void)
{
  return callback;
}

void *
cc_debugerror_get_handler_data(void)
{
  return callback_data;
}

cc_error_cb *
cc_debugerror_get_handler(void ** data)
{
  *data = callback_data;
  return callback;
}

static void
cc_debugerror_internal_post(const char * source, cc_string * msg,
                            CC_DEBUGERROR_SEVERITY sev, const char * type)
{
  cc_debugerror deberr;

  cc_debugerror_init(&deberr);

  deberr.severity = sev;
  cc_error_set_debug_string((cc_error *)&deberr, "Coin ");
  cc_error_append_to_debug_string((cc_error *)&deberr, type);
  cc_error_append_to_debug_string((cc_error *)&deberr, " in ");
  cc_error_append_to_debug_string((cc_error *)&deberr, source);
  cc_error_append_to_debug_string((cc_error *)&deberr, "(): ");
  cc_error_append_to_debug_string((cc_error *)&deberr, cc_string_get_text(msg));
  cc_error_handle((cc_error *)&deberr);
  /* FIXME: port to C. 20020524 mortene. */
  /* check_breakpoints(source);*/

  cc_debugerror_clean(&deberr);
}

#define CC_DEBUGERROR_POST(SEVERITY, TYPE) \
  cc_string s; \
  va_list args; \
 \
  va_start(args, format); \
  cc_string_construct(&s); \
  cc_string_vsprintf(&s, format, args); \
  va_end(args); \
 \
  cc_debugerror_internal_post(source, &s, SEVERITY, TYPE); \
  cc_string_clean(&s)

void
cc_debugerror_post(const char * source, const char * format, ...)
{
  CC_DEBUGERROR_POST(CC_DEBUGERROR_ERROR, "error");
}

void
cc_debugerror_postwarning(const char * source, const char * format, ...)
{
  CC_DEBUGERROR_POST(CC_DEBUGERROR_WARNING, "warning");
}

void
cc_debugerror_postinfo(const char * source, const char * format, ...)
{
  CC_DEBUGERROR_POST(CC_DEBUGERROR_INFO, "info");
}
