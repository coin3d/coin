#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

/* FIXME: this should really test a configure check for the presence
   of the Win32 API. 20030530 mortene. */
#ifdef _WIN32

#include <assert.h>
#include <windows.h>

#include <Inventor/C/glue/win32api.h>


/* internal helper function */
void
cc_win32_print_error(const char * callerfuncname, const char * apifuncname,
                     DWORD lasterror)
{
  LPTSTR buffer;
  BOOL result = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                              FORMAT_MESSAGE_FROM_SYSTEM |
                              FORMAT_MESSAGE_IGNORE_INSERTS,
                              NULL,
                              lasterror,
                              0,
                              (LPTSTR)&buffer,
                              0,
                              NULL);
  if (!result) {
    cc_debugerror_post("cc_win32_print_error", "FormatMessage() failed!");
  }
  else {
    cc_debugerror_post(callerfuncname, "%s failed: '%s'", apifuncname, buffer);
    (void)LocalFree(buffer);
  }
}

static int WINAPI
coin_GetTextFace(HDC hdc, /* handle to device context */
                 int nCount, /* length of buffer receiving typeface name */
                 LPTSTR lpFaceName) /* pointer to buffer receiving typeface name */
{
  const int copied = GetTextFace(hdc, nCount, lpFaceName);
  if (copied == 0) {
    cc_win32_print_error("coin_GetTextFace", "GetTextFace()", GetLastError());
    assert(FALSE && "unexpected error");
  }
  return copied;
}

/* singleton access to the structure, so we can initialize it at first
   use */
const struct cc_win32_api *
cc_win32(void)
{
  static struct cc_win32_api * instance = NULL;

  if (instance) { return instance; }

  /* FIXME: one-time mem leak, clean up at exit. 20030530 mortene. */
  instance = (struct cc_win32_api *)malloc(sizeof(struct cc_win32_api));
  assert(instance != NULL);

  /* set up all function pointers */
  instance->GetTextFace = coin_GetTextFace;

  return instance;
}

#endif /* _WIN32 */
