#ifndef _SBOCTTREE_H_
#define _SBOCTTREE_H_

#include <Inventor/SbVec3f.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbBox3f.h>

class SbSphere;
class SbOctTreeNode;
class SbPlane;

typedef struct 
{
  void (*bboxfunc)(void * const item, SbBox3f &bbox);
  SbBool (*ptinsidefunc)(void * const item, const SbVec3f &pt); 
  SbBool (*insideboxfunc)(void * const item, const SbBox3f &box);
  SbBool (*insidespherefunc)(void * const item, const SbSphere &sphere);
  SbBool (*insideplanesfunc)(void * const item, 
			     const SbPlane * const planes,
			     const int numPlanes);
} SbOctTreeFuncs;


class SbOctTree
{
public:
  SbOctTree(const SbBox3f &bbox, 
	    const SbOctTreeFuncs &itemfuncs,
	    const int maxitemspernode = 64);
  ~SbOctTree();
  
  void addItem(void * const item);
  void removeItem(void * const item);
  void findItems(const SbVec3f &pos, 
		 SbList <void*> &destarray) const;  
  void findItems(const SbBox3f &box, 
		 SbList <void*> &destarray) const;
  void findItems(const SbSphere &sphere, 
		 SbList <void*> &destarray) const;
  void findItems(const SbPlane * const planes,
		 const int numPlanes,
		 SbList <void*> &destarray);
  
  const SbBox3f &getBoundingBox() const;
  void clear();
  
private:
  friend class SbOctTreeNode;
  SbBox3f boundingBox;
  SbOctTreeNode *topnode;
  SbOctTreeFuncs itemfuncs;
  int maxItemsPerNode;
};


#endif // _SBOCTTREE_H_
