/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#include <Inventor/SbOctTree.h>
#include <Inventor/SbSphere.h>
#include <Inventor/SbPlane.h>
#include <float.h>
#include <stddef.h>
#include <math.h>
#include <Inventor/misc/SoBasic.h> // COIN_STUB()

/*!
  \class SbOctTree SbOctTree.h Inventor/SbOctTree.h
  \brief The SbOctTree class defines a generic oct tree for fast geometry searches.
  \ingroup base

  FIXME: doc

  Note: SbOctTree is an extension versus the Open Inventor API.
*/

/*!
  \struct SbOctTreeFuncs SbOctTree.h Inventor/SbOctTree.h
  The SbOctTreeFuncs struct is used to specify callback functions for
  items SbOcTree.

  The only function that must be set is the \e bboxfunc. The other functions
  must be set if you intend to use the corresponding find methods in
  SbOctTree.  */

/*!
  \var SbOctTreeFuncs::bboxfunc
  FIXME: write doc
 */
/*!
  \var SbOctTreeFuncs::ptinsidefunc
  FIXME: write doc
 */
/*!
  \var SbOctTreeFuncs::insideboxfunc
  FIXME: write doc
 */
/*!
  \var SbOctTreeFuncs::insidespherefunc
  FIXME: write doc
 */
/*!
  \var SbOctTreeFuncs::insideplanesfunc
  FIXME: write doc
 */

//
// got to have unique intersection funcs, therefore the standard
// Inventor intersection functions won't do. E.g. SbBox3f::pointInside()
// will return TRUE for all eight child-boxes if the center point of the
// parent box is tested, which is correct, but not really usable for an
// oct tree.
//

static SbBool
intersect_box_sphere(const SbBox3f &box,
                     const SbSphere &sphere)
{
  const SbVec3f &C = sphere.getCenter();
  const SbVec3f &Bmin = box.getMin();
  const SbVec3f &Bmax = box.getMax();
  float dmin = 0;
  for (int i = 0; i < 3; i++) {
    if (C[i] < Bmin[i]) dmin += SbSqr(C[i] - Bmin[i]);
    else if (C[i] > Bmax[i]) dmin += SbSqr(C[i] - Bmax[i]);
  }
  return (dmin <= SbSqr(sphere.getRadius()));
}

static SbBool
intersect_box_box(const SbBox3f &box1, const SbBox3f &box2)
{
  return ! (box1.getMin()[0] >= box2.getMax()[0] ||
            box1.getMax()[0] < box2.getMin()[0] ||
            box1.getMin()[1] >= box2.getMax()[1] ||
            box1.getMax()[1] < box2.getMin()[1] ||
            box1.getMin()[2] >= box2.getMax()[2] ||
            box1.getMax()[2] < box2.getMin()[2]);
}

static SbBool
point_inside_box(const SbVec3f &pt, const SbBox3f &box)
{
  return ! (pt[0] < box.getMin()[0] ||
            pt[0] >= box.getMax()[0] ||
            pt[1] < box.getMin()[1] ||
            pt[1] >= box.getMax()[1] ||
            pt[2] < box.getMin()[2] ||
            pt[2] >= box.getMax()[2]);
}

static SbBool
box_inside_planes(const SbBox3f &box, const SbPlane * const planes,
                  const int numPlanes)
{
  //
  // "better safe than sorry" for speed
  //
  SbVec3f size = (box.getMax() - box.getMin()) * 0.5f;
  float radius = (float)sqrt(size[0]*size[0] + size[1]*size[1] + size[2]*size[2]);

  SbVec3f center = (box.getMin() + box.getMax()) * 0.5f;

  for (int i = 0; i < numPlanes; i++) {
    if (planes[i].getDistance(center) < -radius) return FALSE;
  }

  return TRUE;
}

class SbOctTreeNode
{
public:

  SbOctTreeNode();
  ~SbOctTreeNode();

  void addItem(void * const item,
               const SbOctTreeFuncs &itemfuncs,
               const int maxitems,
               const SbBox3f &nodesize);
  void removeItem(void * const item,
                  const SbOctTreeFuncs &itemfuncs,
                  const SbBox3f &nodesize);
  void findItems(const SbVec3f &pos,
                 SbList <void*> &destarray,
                 const SbOctTreeFuncs &itemfuncs,
                 const SbBox3f &nodesize) const;
  void findItems(const SbBox3f &box,
                 SbList <void*> &destarray,
                 const SbOctTreeFuncs &itemfuncs,
                 const SbBox3f &nodesize) const;
  void findItems(const SbSphere &sphere,
                 SbList <void*> &destarray,
                 const SbOctTreeFuncs &itemfuncs,
                 const SbBox3f &nodesize) const;
  void findItems(const SbPlane * const planes,
                 const int numPlanes,
                 SbList <void*> &destarray,
                 const SbOctTreeFuncs &itemfuncs,
                 const SbBox3f &nodesize) const;

private:

  void splitBox(const SbBox3f &box, SbBox3f *destarray) const;
  SbBool splitNode(const SbBox3f &nodesize, const SbOctTreeFuncs &funcs);

  SbOctTreeNode *children[8];
  SbList <void*> items;
};

static void
add_to_array(SbList <void*> &array, void *ptr)
{
  int i, n= array.getLength();
  for (i = 0; i < n; i++) {
    if (array[i] == ptr) return;
  }
  array.append(ptr);
}

SbOctTreeNode::SbOctTreeNode()
{
  for (int i = 0; i < 8; i++)
    this->children[i] = NULL;
}

SbOctTreeNode::~SbOctTreeNode()
{
  if (this->children[0]) {
    for (int i = 0; i < 8; i++) delete children[i];
  }
}

void
SbOctTreeNode::addItem(void * const item,
                    const SbOctTreeFuncs &itemfuncs,
                    const int maxitems,
                    const SbBox3f &nodesize)
{
  if (this->children[0]) { // node has been split
    SbBox3f childbox[8];
    splitBox(nodesize, childbox);
    for (int i = 0; i < 8; i++) {
      if (itemfuncs.insideboxfunc(item, childbox[i])) {
        this->children[i]->addItem(item, itemfuncs,
                                   maxitems, childbox[i]);
      }
    }
  }
  else if (this->items.getLength() >= maxitems) { // node is full
    if (splitNode(nodesize, itemfuncs)) {
      addItem(item, itemfuncs, maxitems, nodesize); // try again
    }
    else {
      this->items.append(item);
    }
  }
  else {
    this->items.append(item);
  }
}

void
SbOctTreeNode::removeItem(void *const item,
                          const SbOctTreeFuncs &itemfuncs,
                          const SbBox3f &nodesize)
{
  if (children[0]) {
    SbBox3f childbox[8];
    splitBox(nodesize, childbox);
    int i;
    for (i = 0; i < 8; i++) {
      if (itemfuncs.insideboxfunc(item, childbox[i])) {
        this->children[i]->removeItem(item, itemfuncs,
                                      childbox[i]);
      }
    }
  }
  else {
    int n = this->items.getLength();
    int i;

    for (i = 0; i < n; i++) {
      if (this->items[i] == item) {
        this->items.removeFast(i);
        n--;
      }
    }
  }
}

void
SbOctTreeNode::findItems(const SbVec3f &pos,
                         SbList <void*> &destarray,
                         const SbOctTreeFuncs &itemfuncs,
                         const SbBox3f &nodesize) const
{
  if (this->children[0]) {
    SbBox3f childbox[8];
    splitBox(nodesize, childbox);
    for (int i = 0; i < 8; i++) {
      if (point_inside_box(pos, childbox[i])) {
        children[i]->findItems(pos, destarray,
                               itemfuncs, childbox[i]);
      }
    }
  }
  else {
    int n = this->items.getLength();
    for (int i = 0; i < n; i++) {
      void *item = this->items[i];
      if (itemfuncs.ptinsidefunc(item, pos)) {
        add_to_array(destarray, item);
      }
    }
  }
}

void
SbOctTreeNode::findItems(const SbBox3f &box,
                         SbList <void*> &destarray,
                         const SbOctTreeFuncs &itemfuncs,
                         const SbBox3f &nodesize) const
{
  if (this->children[0]) {
    SbBox3f childbox[8];
    splitBox(nodesize, childbox);
    for (int i = 0; i < 8; i++) {
      if (intersect_box_box(box, childbox[i]))
        this->children[i]->findItems(box, destarray,
                                     itemfuncs, childbox[i]);
    }
  }
  else {
    int n = this->items.getLength();
    for (int i = 0; i < n; i++) {
      void *item = this->items[i];
      if (itemfuncs.insideboxfunc(item, box)) {
        add_to_array(destarray, item);
      }
    }
  }
}

void
SbOctTreeNode::findItems(const SbSphere &sphere,
                         SbList <void*> &destarray,
                         const SbOctTreeFuncs &itemfuncs,
                         const SbBox3f &nodesize) const
{
  if (this->children[0]) {
    SbBox3f childbox[8];
    splitBox(nodesize, childbox);
    for (int i = 0; i < 8; i++) {
      if (intersect_box_sphere(childbox[i],sphere))
        this->children[i]->findItems(sphere, destarray,
                                     itemfuncs, childbox[i]);
    }
  }
  else {
    int n = this->items.getLength();
    for (int i = 0; i < n; i++) {
      void *item = this->items[i];
      if (itemfuncs.insidespherefunc(item, sphere)) {
        add_to_array(destarray, item);
      }
    }
  }
}

void
SbOctTreeNode::findItems(const SbPlane * const planes,
                         const int numPlanes,
                         SbList <void*> &destarray,
                         const SbOctTreeFuncs &itemfuncs,
                         const SbBox3f &nodesize) const
{
  if (this->children[0]) {
    SbBox3f childbox[8];
    splitBox(nodesize, childbox);
    for (int i = 0; i < 8; i++) {
      if (box_inside_planes(childbox[i], planes, numPlanes)) {
        this->children[i]->findItems(planes,
                                     numPlanes,
                                     destarray,
                                     itemfuncs,
                                     childbox[i]);
      }
    }
  }
  else {
    int n = this->items.getLength();
    for (int i = 0; i < n; i++) {
      void *item = this->items[i];
      if (itemfuncs.insideplanesfunc(item, planes, numPlanes)) {
        add_to_array(destarray, item);
      }
    }
  }
}

void
SbOctTreeNode::splitBox(const SbBox3f &box, SbBox3f *dest) const
{
  SbVec3f mid = (box.getMin() + box.getMax()) * 0.5f;

  for (int i = 0; i < 8; i++) {
    dest[i].setBounds((i & 4) ? box.getMin()[0] : mid[0],
                      (i & 2) ? box.getMin()[1] : mid[1],
                      (i & 1) ? box.getMin()[2] : mid[2],
                      (i & 4) ? mid[0] : box.getMax()[0],
                      (i & 2) ? mid[1] : box.getMax()[1],
                      (i & 1) ? mid[2] : box.getMax()[2]);
  }
}

SbBool
SbOctTreeNode::splitNode(const SbBox3f &nodesize,
                         const SbOctTreeFuncs &itemfuncs)
{
  int i;
  for (i = 0; i < 8; i++)
    children[i] = new SbOctTreeNode();

  SbBox3f childbox[8];
  splitBox(nodesize, childbox);

  int n = this->items.getLength();
  for (i = 0; i < n; i++) {
    void *item = this->items[i];
    for (int j = 0; j < 8; j++) {
      if (itemfuncs.insideboxfunc(item, childbox[j])) {
        children[j]->items.append(item);
      }
    }
  }
  for (i = 0; i < 8; i++) {
    if (children[i]->items.getLength() == n) break;
  }
  if (i < 8) {
    for (i = 0; i < 8; i++) {
      delete children[i];
      children[i] = NULL;
    }
    return FALSE;
  }
  else {
    this->items.clear(1);
    return TRUE;
  }
  return TRUE;
}


/*!
  Constructor.
 */
SbOctTree::SbOctTree(const SbBox3f &bbox,
                     const SbOctTreeFuncs &itemfuncs,
                     const int maxitems)
{
  this->maxItemsPerNode = maxitems;
  this->topnode = new SbOctTreeNode();
  this->boundingBox = bbox;
  this->itemfuncs = itemfuncs;
}

/*!
  Destructor.
*/
SbOctTree::~SbOctTree()
{
  delete this->topnode;
}

/*!
  Frees all memory used by this octtree.
*/
void
SbOctTree::clear()
{
  delete this->topnode;
  this->topnode = NULL;
  this->topnode = new SbOctTreeNode();
}

/*!
  Adds an item to this oct tree.
*/
void
SbOctTree::addItem(void * const item)
{
  this->topnode->addItem(item, this->itemfuncs,
                         this->maxItemsPerNode,
                         this->boundingBox);
}

/*!
  Removes the item from the octtree.
*/
void
SbOctTree::removeItem(void * const item)
{
  this->topnode->removeItem(item, this->itemfuncs,
                            this->boundingBox);
}

/*!
  Finds all items which contains the point \a pos. Items
  are returned in \a destarray.
*/
void
SbOctTree::findItems(const SbVec3f &pos,
                     SbList <void*> &destarray) const
{
  assert(this->itemfuncs.ptinsidefunc);
  topnode->findItems(pos, destarray,
                     this->itemfuncs,
                     this->boundingBox);
}

/*!
  Finds all items inside \a box. Items are returned in \a destarray.
*/
void
SbOctTree::findItems(const SbBox3f &box, SbList <void*>&destarray) const
{
  assert(this->itemfuncs.insideboxfunc);
  this->topnode->findItems(box, destarray,
                           this->itemfuncs,
                           this->boundingBox);
}

/*!
  Finds all items inside \a sphere. Items are returned in \a destarray.
*/
void
SbOctTree::findItems(const SbSphere &sphere,
                     SbList <void*>&destarray) const
{
  assert(this->itemfuncs.insidespherefunc);
  this->topnode->findItems(sphere, destarray,
                           this->itemfuncs,
                           this->boundingBox);
}

/*!
  Finds all items inside \a planes. The method SbPlane::isInHalfSpace()
  should be used, and only items which is inside \e all planes are
  returned. Items are returned in \a destarray.
*/
void
SbOctTree::findItems(const SbPlane * const planes,
                     const int numPlanes,
                     SbList <void*> &destarray)
{
  assert(this->itemfuncs.insideplanesfunc);
  this->topnode->findItems(planes, numPlanes, destarray,
                           this->itemfuncs, this->boundingBox);
}

/*!
  Returns a bounding box enclosing all the elements in the tree.
 */
const SbBox3f &
SbOctTree::getBoundingBox() const
{
  COIN_STUB();
  static SbBox3f b;
  return b;
}
