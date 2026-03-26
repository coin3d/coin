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

#include "SoBVHCache.h"
#include "CoinTracyConfig.h"

#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec4f.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/elements/SoShapeHintsElement.h>
#include <Inventor/nodes/SoShape.h>

#include <vector>
#include <algorithm>
#include <cstring>
#include <cassert>
#include <climits>
#include <cfloat>

// -----------------------------------------------------------------------
// BVH node: 32 bytes, cache-line friendly flat layout
// -----------------------------------------------------------------------

struct SbBVHNode {
  SbBox3f bbox;
  union {
    int32_t rightChild;     // internal node: index of right child
    int32_t firstTriangle;  // leaf node: offset into triIndices
  };
  int16_t numTriangles;     // 0 = internal, >0 = leaf
  int16_t splitAxis;        // 0=X, 1=Y, 2=Z
};

// -----------------------------------------------------------------------
// SAH binning constants
// -----------------------------------------------------------------------

static const int NUM_BINS = 16;
static const int MAX_LEAF_TRIS = 8;
static const float TRAVERSAL_COST = 1.0f;
static const float INTERSECT_COST = 1.0f;

// -----------------------------------------------------------------------
// Private implementation
// -----------------------------------------------------------------------

class SoBVHCacheP {
public:
  SoBVHCacheP()
    : vertices(NULL), normals(NULL), texcoords(NULL), colors(NULL),
      indices(NULL), numVertices(0), numTriangleIndices(0), built(FALSE) {}

  ~SoBVHCacheP() {
    delete[] vertices;
    delete[] normals;
    delete[] texcoords;
    delete[] colors;
    delete[] indices;
  }

  // Copied vertex data (independent of PVC lifetime)
  SbVec3f * vertices;
  SbVec3f * normals;
  SbVec4f * texcoords;
  uint8_t * colors;
  GLint * indices;
  int numVertices;
  int numTriangleIndices;

  // BVH structure
  std::vector<SbBVHNode> nodes;
  std::vector<int32_t> triIndices;

  SbBool built;

  // Build helpers
  void buildBVH(void);
  int buildRecursive(int begin, int end, std::vector<SbVec3f> & centroids);
  SbBox3f computeTriangleBBox(int triIdx) const;
  SbVec3f computeTriangleCentroid(int triIdx) const;
  float computeSurfaceArea(const SbBox3f & box) const;
};

// -----------------------------------------------------------------------
// SoBVHCache implementation
// -----------------------------------------------------------------------

SoBVHCache::SoBVHCache(SoState * state)
  : inherited(state)
{
  this->pimpl = new SoBVHCacheP;
}

SoBVHCache::~SoBVHCache()
{
  delete this->pimpl;
}

void
SoBVHCache::build(const SbVec3f * vertices,
                  const SbVec3f * normals,
                  const SbVec4f * texcoords,
                  const uint8_t * colors,
                  int numVertices,
                  const GLint * indices,
                  int numTriangleIndices)
{
  SoBVHCacheP * p = this->pimpl;

  // Clean up any previous data
  delete[] p->vertices;
  delete[] p->normals;
  delete[] p->texcoords;
  delete[] p->colors;
  delete[] p->indices;

  p->numVertices = numVertices;
  p->numTriangleIndices = numTriangleIndices;

  // Copy vertex data so we're independent of PVC lifetime
  p->vertices = new SbVec3f[numVertices];
  std::memcpy(p->vertices, vertices, numVertices * sizeof(SbVec3f));

  if (normals) {
    p->normals = new SbVec3f[numVertices];
    std::memcpy(p->normals, normals, numVertices * sizeof(SbVec3f));
  } else {
    p->normals = NULL;
  }

  if (texcoords) {
    p->texcoords = new SbVec4f[numVertices];
    std::memcpy(p->texcoords, texcoords, numVertices * sizeof(SbVec4f));
  } else {
    p->texcoords = NULL;
  }

  if (colors) {
    // 4 bytes per vertex (RGBA)
    p->colors = new uint8_t[numVertices * 4];
    std::memcpy(p->colors, colors, numVertices * 4);
  } else {
    p->colors = NULL;
  }

  p->indices = new GLint[numTriangleIndices];
  std::memcpy(p->indices, indices, numTriangleIndices * sizeof(GLint));

  p->buildBVH();
}

void
SoBVHCache::buildFlat(const SbVec3f * triVertices,
                      const SbVec3f * triNormals,
                      int numTriangles)
{
  SoBVHCacheP * p = this->pimpl;

  // Clean up any previous data
  delete[] p->vertices;
  delete[] p->normals;
  delete[] p->texcoords;
  delete[] p->colors;
  delete[] p->indices;

  int numVerts = numTriangles * 3;
  p->numVertices = numVerts;
  p->numTriangleIndices = numVerts;

  // Copy flat vertex data (3 vertices per triangle, no deduplication)
  p->vertices = new SbVec3f[numVerts];
  std::memcpy(p->vertices, triVertices, numVerts * sizeof(SbVec3f));

  if (triNormals) {
    p->normals = new SbVec3f[numVerts];
    std::memcpy(p->normals, triNormals, numVerts * sizeof(SbVec3f));
  } else {
    p->normals = NULL;
  }

  p->texcoords = NULL;
  p->colors = NULL;

  // Generate identity indices: [0,1,2, 3,4,5, ...]
  p->indices = new GLint[numVerts];
  for (int i = 0; i < numVerts; i++) {
    p->indices[i] = i;
  }

  p->buildBVH();
}

SbBool
SoBVHCache::rayPick(SoRayPickAction * action, SoShape * shape) const
{
  CoinZoneScopedN("SoBVHCache::rayPick");
  SoBVHCacheP * p = this->pimpl;

  if (!p->built || p->nodes.empty()) return FALSE;

  const SbBool cwFlip =
    (SoShapeHintsElement::getVertexOrdering(action->getState()) ==
     SoShapeHintsElement::CLOCKWISE);

  // Stack-based BVH traversal
  int stack[64];
  int stackPtr = 0;
  stack[stackPtr++] = 0; // root node

  while (stackPtr > 0) {
    const int nodeIdx = stack[--stackPtr];
    const SbBVHNode & node = p->nodes[nodeIdx];

    // AABB test using action->intersect(box) which handles both ray and cone picks
    SbBox3f bbox = node.bbox;
    if (!action->intersect(bbox, TRUE)) {
      continue;
    }

    if (node.numTriangles > 0) {
      // Leaf node: test triangles
      const int end = node.firstTriangle + node.numTriangles;
      for (int i = node.firstTriangle; i < end; i++) {
        const int triIdx = p->triIndices[i];
        const int baseIdx = triIdx * 3;
        const SbVec3f & v0 = p->vertices[p->indices[baseIdx]];
        const SbVec3f & v1 = p->vertices[p->indices[baseIdx + 1]];
        const SbVec3f & v2 = p->vertices[p->indices[baseIdx + 2]];

        SbVec3f intersection, barycentric;
        SbBool front;

        if (action->intersect(v0, v1, v2, intersection, barycentric, front)) {
          if (action->isBetweenPlanes(intersection)) {
            if (cwFlip) front = !front;

            SoPickedPoint * pp = action->addIntersection(intersection, front);
            if (pp) {
              // Interpolate normal using barycentric coordinates
              if (p->normals) {
                SbVec3f n =
                  p->normals[p->indices[baseIdx]]     * barycentric[0] +
                  p->normals[p->indices[baseIdx + 1]] * barycentric[1] +
                  p->normals[p->indices[baseIdx + 2]] * barycentric[2];
                n.normalize();
                pp->setObjectNormal(n);
              }

              // Interpolate texture coordinates
              if (p->texcoords) {
                SbVec4f tc =
                  p->texcoords[p->indices[baseIdx]]     * barycentric[0] +
                  p->texcoords[p->indices[baseIdx + 1]] * barycentric[1] +
                  p->texcoords[p->indices[baseIdx + 2]] * barycentric[2];
                pp->setObjectTextureCoords(tc);
              }

              // Material index: use vertex with largest barycentric weight
              float maxval = barycentric[0];
              int maxidx = p->indices[baseIdx];
              if (barycentric[1] > maxval) {
                maxval = barycentric[1];
                maxidx = p->indices[baseIdx + 1];
              }
              if (barycentric[2] > maxval) {
                maxidx = p->indices[baseIdx + 2];
              }
              pp->setMaterialIndex(maxidx);
            }
          }
        }
      }
    }
    else {
      // Internal node: push children
      // Push right child first so left child (nodeIdx+1) is processed first
      if (node.rightChild > 0) {
        if (stackPtr < 64) stack[stackPtr++] = node.rightChild;
      }
      if (stackPtr < 64) stack[stackPtr++] = nodeIdx + 1;
    }
  }

  return TRUE;
}

int
SoBVHCache::getNumTriangles(void) const
{
  return this->pimpl->numTriangleIndices / 3;
}

SbBool
SoBVHCache::isBuilt(void) const
{
  return this->pimpl->built;
}

// -----------------------------------------------------------------------
// BVH build implementation
// -----------------------------------------------------------------------

SbBox3f
SoBVHCacheP::computeTriangleBBox(int triIdx) const
{
  const int baseIdx = triIdx * 3;
  const SbVec3f & v0 = this->vertices[this->indices[baseIdx]];
  const SbVec3f & v1 = this->vertices[this->indices[baseIdx + 1]];
  const SbVec3f & v2 = this->vertices[this->indices[baseIdx + 2]];

  SbBox3f box;
  box.extendBy(v0);
  box.extendBy(v1);
  box.extendBy(v2);
  return box;
}

SbVec3f
SoBVHCacheP::computeTriangleCentroid(int triIdx) const
{
  const int baseIdx = triIdx * 3;
  const SbVec3f & v0 = this->vertices[this->indices[baseIdx]];
  const SbVec3f & v1 = this->vertices[this->indices[baseIdx + 1]];
  const SbVec3f & v2 = this->vertices[this->indices[baseIdx + 2]];

  return SbVec3f((v0[0] + v1[0] + v2[0]) / 3.0f,
                 (v0[1] + v1[1] + v2[1]) / 3.0f,
                 (v0[2] + v1[2] + v2[2]) / 3.0f);
}

float
SoBVHCacheP::computeSurfaceArea(const SbBox3f & box) const
{
  if (box.isEmpty()) return 0.0f;
  SbVec3f size = box.getMax() - box.getMin();
  return 2.0f * (size[0] * size[1] + size[0] * size[2] + size[1] * size[2]);
}

void
SoBVHCacheP::buildBVH(void)
{
  const int numTris = this->numTriangleIndices / 3;
  if (numTris == 0) {
    this->built = FALSE;
    return;
  }

  // Initialize triangle index array
  this->triIndices.resize(numTris);
  for (int i = 0; i < numTris; i++) {
    this->triIndices[i] = i;
  }

  // Precompute centroids
  std::vector<SbVec3f> centroids(numTris);
  for (int i = 0; i < numTris; i++) {
    centroids[i] = this->computeTriangleCentroid(i);
  }

  // Reserve space for nodes (roughly 2*N/leafSize nodes)
  this->nodes.clear();
  this->nodes.reserve(numTris * 2 / MAX_LEAF_TRIS + 1);

  this->buildRecursive(0, numTris, centroids);
  this->built = TRUE;
}

int
SoBVHCacheP::buildRecursive(int begin, int end,
                             std::vector<SbVec3f> & centroids)
{
  const int numTris = end - begin;
  const int nodeIdx = static_cast<int>(this->nodes.size());
  this->nodes.push_back(SbBVHNode());

  // Compute bounding box of all triangles in this range
  SbBox3f nodeBBox;
  for (int i = begin; i < end; i++) {
    SbBox3f triBBox = this->computeTriangleBBox(this->triIndices[i]);
    nodeBBox.extendBy(triBBox.getMin());
    nodeBBox.extendBy(triBBox.getMax());
  }

  this->nodes[nodeIdx].bbox = nodeBBox;

  // Create leaf if few enough triangles
  if (numTris <= MAX_LEAF_TRIS) {
    this->nodes[nodeIdx].firstTriangle = begin;
    this->nodes[nodeIdx].numTriangles = static_cast<int16_t>(numTris);
    this->nodes[nodeIdx].splitAxis = 0;
    return nodeIdx;
  }

  // Compute centroid bounding box for SAH binning
  SbBox3f centroidBBox;
  for (int i = begin; i < end; i++) {
    centroidBBox.extendBy(centroids[this->triIndices[i]]);
  }

  SbVec3f centroidExtent = centroidBBox.getMax() - centroidBBox.getMin();

  // Find longest axis
  int bestAxis = 0;
  if (centroidExtent[1] > centroidExtent[bestAxis]) bestAxis = 1;
  if (centroidExtent[2] > centroidExtent[bestAxis]) bestAxis = 2;

  // Degenerate case: all centroids at the same point
  if (centroidExtent[bestAxis] < FLT_EPSILON) {
    this->nodes[nodeIdx].firstTriangle = begin;
    this->nodes[nodeIdx].numTriangles = static_cast<int16_t>(numTris);
    this->nodes[nodeIdx].splitAxis = 0;
    return nodeIdx;
  }

  // SAH binning
  struct Bin {
    SbBox3f bbox;
    int count;
  };

  float bestCost = FLT_MAX;
  int bestSplit = -1;
  int bestSplitAxis = bestAxis;

  // Try all 3 axes, starting with the longest
  for (int axisIter = 0; axisIter < 3; axisIter++) {
    int axis = (bestAxis + axisIter) % 3;
    if (centroidExtent[axis] < FLT_EPSILON) continue;

    Bin bins[NUM_BINS];
    for (int i = 0; i < NUM_BINS; i++) {
      bins[i].count = 0;
    }

    float axisMin = centroidBBox.getMin()[axis];
    float axisRange = centroidExtent[axis];
    float invRange = (NUM_BINS - 0.001f) / axisRange;

    // Bin triangles
    for (int i = begin; i < end; i++) {
      int triIdx = this->triIndices[i];
      float c = centroids[triIdx][axis];
      int binIdx = static_cast<int>((c - axisMin) * invRange);
      if (binIdx < 0) binIdx = 0;
      if (binIdx >= NUM_BINS) binIdx = NUM_BINS - 1;

      bins[binIdx].count++;
      SbBox3f triBBox = this->computeTriangleBBox(triIdx);
      bins[binIdx].bbox.extendBy(triBBox.getMin());
      bins[binIdx].bbox.extendBy(triBBox.getMax());
    }

    // Sweep from left to compute prefix surface areas and counts
    SbBox3f leftBox;
    int leftCount[NUM_BINS];
    float leftArea[NUM_BINS];

    int runningCount = 0;
    for (int i = 0; i < NUM_BINS - 1; i++) {
      if (bins[i].count > 0 && !bins[i].bbox.isEmpty()) {
        leftBox.extendBy(bins[i].bbox.getMin());
        leftBox.extendBy(bins[i].bbox.getMax());
      }
      runningCount += bins[i].count;
      leftCount[i] = runningCount;
      leftArea[i] = this->computeSurfaceArea(leftBox);
    }

    // Sweep from right and evaluate SAH
    SbBox3f rightBox;
    runningCount = 0;
    for (int i = NUM_BINS - 1; i >= 1; i--) {
      if (bins[i].count > 0 && !bins[i].bbox.isEmpty()) {
        rightBox.extendBy(bins[i].bbox.getMin());
        rightBox.extendBy(bins[i].bbox.getMax());
      }
      runningCount += bins[i].count;

      float rightArea = this->computeSurfaceArea(rightBox);
      float cost = TRAVERSAL_COST +
        INTERSECT_COST * (leftCount[i - 1] * leftArea[i - 1] +
                          runningCount * rightArea) /
        this->computeSurfaceArea(nodeBBox);

      if (cost < bestCost && leftCount[i - 1] > 0 && runningCount > 0) {
        bestCost = cost;
        bestSplit = i;
        bestSplitAxis = axis;
      }
    }
  }

  // If SAH split isn't better than a leaf, make a leaf
  float leafCost = INTERSECT_COST * numTris;
  if (bestSplit == -1 || bestCost >= leafCost) {
    // But cap leaf size to prevent overly large leaves
    if (numTris <= MAX_LEAF_TRIS * 4) {
      this->nodes[nodeIdx].firstTriangle = begin;
      this->nodes[nodeIdx].numTriangles = static_cast<int16_t>(numTris);
      this->nodes[nodeIdx].splitAxis = 0;
      return nodeIdx;
    }
    // Force a median split on the best axis
    bestSplitAxis = bestAxis;
    bestSplit = NUM_BINS / 2;
  }

  // Partition triangles around the split
  float axisMin = centroidBBox.getMin()[bestSplitAxis];
  float axisRange = centroidExtent[bestSplitAxis];
  float invRange = (NUM_BINS - 0.001f) / axisRange;

  // Stable partition: triangles with bin < bestSplit go left
  auto midIt = std::partition(
    this->triIndices.begin() + begin,
    this->triIndices.begin() + end,
    [&](int32_t triIdx) {
      float c = centroids[triIdx][bestSplitAxis];
      int binIdx = static_cast<int>((c - axisMin) * invRange);
      if (binIdx < 0) binIdx = 0;
      if (binIdx >= NUM_BINS) binIdx = NUM_BINS - 1;
      return binIdx < bestSplit;
    }
  );

  int mid = static_cast<int>(midIt - this->triIndices.begin());

  // Safety: if partition failed to split, force a median split
  if (mid == begin || mid == end) {
    mid = (begin + end) / 2;
    std::nth_element(
      this->triIndices.begin() + begin,
      this->triIndices.begin() + mid,
      this->triIndices.begin() + end,
      [&](int32_t a, int32_t b) {
        return centroids[a][bestSplitAxis] < centroids[b][bestSplitAxis];
      }
    );
  }

  this->nodes[nodeIdx].numTriangles = 0; // internal node
  this->nodes[nodeIdx].splitAxis = static_cast<int16_t>(bestSplitAxis);

  // Build left child (will be at nodeIdx + 1 due to depth-first order)
  this->buildRecursive(begin, mid, centroids);

  // Build right child
  int rightIdx = this->buildRecursive(mid, end, centroids);
  this->nodes[nodeIdx].rightChild = rightIdx;

  return nodeIdx;
}
