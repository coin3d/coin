#ifndef COIN_SOMANIPKITPATH_H
#define COIN_SOMANIPKITPATH_H

#include <Inventor/SoPath.h>
#include <Inventor/nodekits/SoBaseKit.h>

// SoPath::getTail() stops at the first node with hidden children. A manipulator
// may belong to a later nodekit in the complete route.
static inline SoBaseKit *
coin_lastKitInPath(const SoPath * path)
{
  const SoFullPathView route = path->fullPath();
  for (int i = 0; i < route.getLength(); i++) {
    SoNode * node = route.getNodeFromTail(i);
    if (node != NULL && node->isOfType(SoBaseKit::getClassTypeId())) {
      return static_cast<SoBaseKit *>(node);
    }
  }
  return NULL;
}

#endif // COIN_SOMANIPKITPATH_H
