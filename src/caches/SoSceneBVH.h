/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#ifndef COIN_SOSCENEBVH_H
#define COIN_SOSCENEBVH_H

#include <Inventor/SbBox3f.h>
#include <Inventor/SbMatrix.h>
#include <vector>
#include <set>

class SoNode;
class SoShape;
class SoRayPickAction;

/*!
  \class SoSceneBVH SoSceneBVH.h
  \brief Flat scene-level BVH over all shapes' world-space bounding boxes.

  Built from a collection of (shapeNode, worldSpaceBBox) pairs gathered
  during the first ray pick traversal. Subsequent picks query the BVH
  to find which shapes the ray might intersect, producing a candidate
  set. SoShape::rayPick() checks membership in this set to skip shapes
  the ray cannot hit.

  Stored as a static cache associated with a scene root node.
  Invalidated when the scene graph changes (via node notification).
*/

struct SoSceneBVHNode {
  SbBox3f bbox;          // world-space AABB
  int32_t rightChild;    // internal: right child index; leaf: -1
  int32_t shapeIndex;    // leaf: index into shapes array; internal: -1
};

struct SoSceneBVHEntry {
  SoShape * shape;
  SbBox3f worldBBox;
};

class SoSceneBVH {
public:
  SoSceneBVH();
  ~SoSceneBVH();

  /// Add a shape with its world-space bbox during collection phase
  void addShape(SoShape * shape, const SbBox3f & worldBBox);

  /// Build the BVH from collected shapes
  void build();

  /// Query: find all shapes whose world-space bbox intersects the ray.
  /// Returns a set of SoShape pointers that are candidates for picking.
  void query(SoRayPickAction * action, std::set<SoShape *> & candidates) const;

  bool isBuilt() const { return built; }
  int numShapes() const { return static_cast<int>(entries.size()); }

  /// Global scene BVH management — one BVH per scene root
  static SoSceneBVH * getForRoot(SoNode * root);
  static void invalidateForRoot(SoNode * root);

  /// Active candidate set for current pick (set by doPick, read by SoShape::rayPick)
  static void setActiveCandidates(std::set<SoShape *> * candidates);
  static std::set<SoShape *> * getActiveCandidates();

  /// Collection mode: shapes register themselves during first pick
  static void setCollecting(SoSceneBVH * bvh);
  static SoSceneBVH * getCollecting();

private:
  std::vector<SoSceneBVHEntry> entries;
  std::vector<SoSceneBVHNode> nodes;
  std::vector<int> entryIndices;
  bool built = false;
  bool collecting = false;

  int buildRecursive(int begin, int end, std::vector<SbVec3f> & centroids);
  static float surfaceArea(const SbBox3f & box);
};

#endif // COIN_SOSCENEBVH_H
