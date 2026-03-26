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

#ifndef COIN_SOCHILDBVH_H
#define COIN_SOCHILDBVH_H

#include <Inventor/SbBox3f.h>
#include <vector>
#include <utility>

class SoRayPickAction;

/*!
  \class SoChildBVH SoChildBVH.h
  \brief BVH over a separator's direct children's bounding boxes.

  Used by SoSeparator::rayPick() to accelerate child culling. Instead
  of testing each child's bbox linearly, the ray is tested against a
  spatial tree, visiting only children whose bboxes actually intersect.

  Build from (childIndex, bbox) pairs. Query returns a bitset of which
  children the ray intersects. State-affecting children (transforms,
  materials) are not in the BVH and are always traversed.
*/

struct SoChildBVHNode {
  SbBox3f bbox;
  int32_t rightChild;   // internal: index of right child node; leaf: -1
  int16_t childIndex;   // leaf: index into SoChildList; internal: -1
  int16_t splitAxis;    // 0=X, 1=Y, 2=Z
};

class SoChildBVH {
public:
  SoChildBVH();
  ~SoChildBVH();

  /// Build from an array of (childIndex, bbox) pairs.
  /// Only children with non-empty bboxes should be included.
  /// totalChildren is the total child count for validity checking.
  void build(const std::vector<std::pair<int, SbBox3f>> & entries, int totalChildren);

  /// Query: fill hitChildren[i] = true for each child whose bbox
  /// intersects the ray. The action must have its object-space ray set.
  void query(SoRayPickAction * action,
             std::vector<bool> & hitChildren) const;

  bool isBuilt() const { return !nodes.empty(); }

  /// Check if the BVH is still valid for the given child count
  bool isValid(int currentChildCount) const {
    return !nodes.empty() && currentChildCount == builtChildCount;
  }

  int numNodes() const { return static_cast<int>(nodes.size()); }

private:
  std::vector<SoChildBVHNode> nodes;
  std::vector<int> entryIndices;  // permutation for SAH partitioning
  int builtChildCount = 0;        // child count at build time for validity

  int buildRecursive(int begin, int end,
                     const std::vector<std::pair<int, SbBox3f>> & entries,
                     std::vector<SbVec3f> & centroids);
  static float surfaceArea(const SbBox3f & box);
};

#endif // COIN_SOCHILDBVH_H
