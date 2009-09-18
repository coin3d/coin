#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
# ifdef FBX_INTERNAL
#  ifdef FBX_MAKE_DLL
#   define FBX_DLL_API __declspec(dllexport)
#  endif /* FBX_MAKE_DLL */
# else /* !FBX_INTERNAL */
#  ifdef FBX_DLL
#   ifdef FBX_NOT_DLL
#     error Define _either_ FBX_DLL _or_ FBX_NOT_DLL as appropriate for your linkage -- not both at the same time! 
#   endif /* FBX_NOT_DLL */
#   define FBX_DLL_API __declspec(dllimport)
#  else /* !FBX_DLL */
#   ifndef FBX_NOT_DLL
#    error Define either FBX_DLL or FBX_NOT_DLL as appropriate for your linkage! 
#   endif /* !FBX_NOT_DLL */
#  endif /* !FBX_DLL */
# endif /* !FBX_INTERNAL */
#endif /* Microsoft Windows */

/* Empty define to avoid errors when _not_ compiling an MSWindows DLL. */
#ifndef FBX_DLL_API
# define FBX_DLL_API
#endif /* !FBX_DLL_API */
