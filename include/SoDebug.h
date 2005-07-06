#ifndef COIN_SODEBUG_H
#define COIN_SODEBUG_H

class SoBase;
class SoNode;
class SoField;

#include <Inventor/SbBasic.h>

class COIN_DLL_API SoDebug {
public:
  static const char * GetEnv(const char * var);
  static void RTPrintf(const char * formatstr, ...);
  static void NamePtr(const char * name, void * ptr);
  static const char * PtrName(void * ptr);
  static void write(SoNode * node);
  static void writeToFile(SoNode * node, const char * filename);
  static void writeField(SoField * field);
  static void printName(SoBase * base);
}; // SoDebug

#endif // !COIN_SODEBUG_H
