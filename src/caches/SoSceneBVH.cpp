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

#include "SoSceneBVH.h"
#include "CoinTracyConfig.h"

#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/SbVec3f.h>

#include <algorithm>
#include <cfloat>
#include <map>
#include <mutex>

// SAH constants
static const int NUM_BINS = 16;
static const int MAX_LEAF_SHAPES = 4;

// -----------------------------------------------------------------------
// Global scene BVH cache: one per scene root node
// -----------------------------------------------------------------------

static std::mutex g_sceneBVHMutex;
static std::map<SoNode *, SoSceneBVH *> g_sceneBVHMap;

SoSceneBVH *
SoSceneBVH::getForRoot(SoNode * root)
{
  std::lock_guard<std::mutex> lock(g_sceneBVHMutex);
  auto it = g_sceneBVHMap.find(root);
  if (it != g_sceneBVHMap.end()) {
    return it->second;
  }
  auto * bvh = new SoSceneBVH();
  g_sceneBVHMap[root] = bvh;
  return bvh;
}

void
SoSceneBVH::invalidateForRoot(SoNode * root)
{
  std::lock_guard<std::mutex> lock(g_sceneBVHMutex);
  auto it = g_sceneBVHMap.find(root);
  if (it != g_sceneBVHMap.end()) {
    delete it->second;
    g_sceneBVHMap.erase(it);
  }
}

// -----------------------------------------------------------------------
// Thread-local state for current pick operation
// -----------------------------------------------------------------------

static std::set<SoShape *> * g_activeCandidates = NULL;
static SoSceneBVH * g_collectingBVH = NULL;

void SoSceneBVH::setActiveCandidates(std::set<SoShape *> * candidates) {
  g_activeCandidates = candidates;
}

std::set<SoShape *> * SoSceneBVH::getActiveCandidates() {
  return g_activeCandidates;
}

void SoSceneBVH::setCollecting(SoSceneBVH * bvh) {
  g_collectingBVH = bvh;
}

SoSceneBVH * SoSceneBVH::getCollecting() {
  return g_collectingBVH;
}

// -----------------------------------------------------------------------
// SoSceneBVH implementation
// -----------------------------------------------------------------------

SoSceneBVH::SoSceneBVH() = default;
SoSceneBVH::~SoSceneBVH() = default;

void
SoSceneBVH::addShape(SoShape * shape, const SbBox3f & worldBBox)
{
  if (!worldBBox.isEmpty()) {
    entries.push_back({shape, worldBBox});
  }
}

float
SoSceneBVH::surfaceArea(const SbBox3f & box)
{
  if (box.isEmpty()) return 0.0f;
  SbVec3f s = box.getMax() - box.getMin();
  return 2.0f * (s[0] * s[1] + s[0] * s[2] + s[1] * s[2]);
}

void
SoSceneBVH::build()
{
  CoinZoneScopedN("SoSceneBVH::build");

  const int n = static_cast<int>(entries.size());
  if (n == 0) {
    built = false;
    return;
  }

  entryIndices.resize(n);
  for (int i = 0; i < n; i++) {
    entryIndices[i] = i;
  }

  std::vector<SbVec3f> centroids(n);
  for (int i = 0; i < n; i++) {
    const SbBox3f & box = entries[i].worldBBox;
    centroids[i] = (box.getMin() + box.getMax()) * 0.5f;
  }

  nodes.clear();
  nodes.reserve(n * 2 + 1);

  buildRecursive(0, n, centroids);
  built = true;
}

int
SoSceneBVH::buildRecursive(int begin, int end, std::vector<SbVec3f> & centroids)
{
  const int count = end - begin;
  const int nodeIdx = static_cast<int>(nodes.size());
  nodes.push_back(SoSceneBVHNode());

  // Compute bounding box of all entries in this range
  SbBox3f nodeBBox;
  for (int i = begin; i < end; i++) {
    const SbBox3f & box = entries[entryIndices[i]].worldBBox;
    nodeBBox.extendBy(box.getMin());
    nodeBBox.extendBy(box.getMax());
  }
  nodes[nodeIdx].bbox = nodeBBox;

  // Leaf
  if (count <= MAX_LEAF_SHAPES) {
    if (count == 1) {
      nodes[nodeIdx].shapeIndex = entryIndices[begin];
      nodes[nodeIdx].rightChild = -1;
      return nodeIdx;
    }
  }

  // SAH binning
  SbBox3f centroidBBox;
  for (int i = begin; i < end; i++) {
    centroidBBox.extendBy(centroids[entryIndices[i]]);
  }

  SbVec3f extent = centroidBBox.getMax() - centroidBBox.getMin();
  int bestAxis = 0;
  if (extent[1] > extent[bestAxis]) bestAxis = 1;
  if (extent[2] > extent[bestAxis]) bestAxis = 2;

  // Degenerate
  if (extent[bestAxis] < FLT_EPSILON) {
    int mid = begin + count / 2;
    nodes[nodeIdx].shapeIndex = -1;
    buildRecursive(begin, mid, centroids);
    int rightIdx = buildRecursive(mid, end, centroids);
    nodes[nodeIdx].rightChild = rightIdx;
    return nodeIdx;
  }

  float axisMin = centroidBBox.getMin()[bestAxis];
  float axisMax = centroidBBox.getMax()[bestAxis];
  float scale = NUM_BINS / (axisMax - axisMin);

  struct Bin { SbBox3f bbox; int count = 0; };
  Bin bins[NUM_BINS];

  for (int i = begin; i < end; i++) {
    int idx = entryIndices[i];
    int binIdx = static_cast<int>((centroids[idx][bestAxis] - axisMin) * scale);
    if (binIdx >= NUM_BINS) binIdx = NUM_BINS - 1;
    if (binIdx < 0) binIdx = 0;
    const SbBox3f & box = entries[idx].worldBBox;
    bins[binIdx].bbox.extendBy(box.getMin());
    bins[binIdx].bbox.extendBy(box.getMax());
    bins[binIdx].count++;
  }

  float nodeSA = surfaceArea(nodeBBox);
  if (nodeSA < FLT_EPSILON) nodeSA = FLT_EPSILON;

  float bestCost = FLT_MAX;
  int bestSplit = -1;

  SbBox3f leftBox;
  int leftCount = 0;
  float leftAreas[NUM_BINS - 1];
  int leftCounts[NUM_BINS - 1];

  for (int i = 0; i < NUM_BINS - 1; i++) {
    if (!bins[i].bbox.isEmpty()) {
      leftBox.extendBy(bins[i].bbox.getMin());
      leftBox.extendBy(bins[i].bbox.getMax());
    }
    leftCount += bins[i].count;
    leftAreas[i] = surfaceArea(leftBox);
    leftCounts[i] = leftCount;
  }

  SbBox3f rightBox;
  int rightCount = 0;
  for (int i = NUM_BINS - 1; i >= 1; i--) {
    if (!bins[i].bbox.isEmpty()) {
      rightBox.extendBy(bins[i].bbox.getMin());
      rightBox.extendBy(bins[i].bbox.getMax());
    }
    rightCount += bins[i].count;
    float cost = 1.0f + (leftCounts[i-1] * leftAreas[i-1]
                         + rightCount * surfaceArea(rightBox)) / nodeSA;
    if (cost < bestCost && leftCounts[i-1] > 0 && rightCount > 0) {
      bestCost = cost;
      bestSplit = i;
    }
  }

  if (bestSplit < 0) {
    int mid = begin + count / 2;
    nodes[nodeIdx].shapeIndex = -1;
    buildRecursive(begin, mid, centroids);
    int rightIdx = buildRecursive(mid, end, centroids);
    nodes[nodeIdx].rightChild = rightIdx;
    return nodeIdx;
  }

  float splitPos = axisMin + bestSplit / scale;
  int mid = begin;
  for (int i = begin; i < end; i++) {
    if (centroids[entryIndices[i]][bestAxis] < splitPos) {
      std::swap(entryIndices[i], entryIndices[mid]);
      mid++;
    }
  }

  if (mid == begin || mid == end) {
    mid = begin + count / 2;
  }

  nodes[nodeIdx].shapeIndex = -1;
  buildRecursive(begin, mid, centroids);
  int rightIdx = buildRecursive(mid, end, centroids);
  nodes[nodeIdx].rightChild = rightIdx;

  return nodeIdx;
}

void
SoSceneBVH::query(SoRayPickAction * action, std::set<SoShape *> & candidates) const
{
  CoinZoneScopedN("SoSceneBVH::query");

  if (nodes.empty()) return;

  // The ray must be in world space for world-space bbox tests.
  // SoRayPickAction::intersect(SbBox3f) tests in whatever space was last
  // set via setObjectSpace(). We need to test in world space, so we
  // use the world-space ray directly.

  // Stack-based traversal
  int stack[64];
  int stackPtr = 0;
  stack[stackPtr++] = 0;

  while (stackPtr > 0) {
    int nodeIdx = stack[--stackPtr];
    const SoSceneBVHNode & node = nodes[nodeIdx];

    if (node.bbox.isEmpty()) continue;

    // Test ray against world-space AABB using the action's world-space ray
    if (!action->intersect(node.bbox, TRUE)) {
      continue;
    }

    if (node.shapeIndex >= 0) {
      // Leaf — add shape as candidate
      candidates.insert(entries[node.shapeIndex].shape);
    } else {
      // Internal — push children
      if (node.rightChild > 0 && stackPtr < 64) {
        stack[stackPtr++] = node.rightChild;
      }
      if (nodeIdx + 1 < static_cast<int>(nodes.size()) && stackPtr < 64) {
        stack[stackPtr++] = nodeIdx + 1;
      }
    }
  }
}
