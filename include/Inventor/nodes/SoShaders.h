#ifndef __SO_SHADERS_H__
#define __SO_SHADERS_H__

// DLL export macro definition
#ifdef WIN32

#ifdef UMDSOSHADER_EXPORTS
// Use the UMDSOSHADER_EXPORT macro to export classes and functions
#define UMDSOSHADER_EXPORT __declspec(dllexport)
#else
// If included by external modules, exported symbols are declared as import symbols
#define UMDSOSHADER_EXPORT __declspec(dllimport)
#endif

#else
// No export declarations are necessary for non-Windows systems
#define UMDSOSHADER_EXPORT
#endif

class SoShaders
{
 public:
  static void init();
};

#endif /* __SO_SHADERS_H__ */
