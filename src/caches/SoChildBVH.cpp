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

#include "SoChildBVH.h"
#include "CoinTracyConfig.h"

#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/SbVec3f.h>

#include <algorithm>
#include <cfloat>

// SAH constants
static const int NUM_BINS = 16;
static const int MAX_LEAF_CHILDREN = 4;
static const float TRAVERSAL_COST = 1.0f;
static const float INTERSECT_COST = 1.0f;

SoChildBVH::SoChildBVH() = default;
SoChildBVH::~SoChildBVH() = default;

float
SoChildBVH::surfaceArea(const SbBox3f & box)
{
  if (box.isEmpty()) return 0.0f;
  SbVec3f s = box.getMax() - box.getMin();
  return 2.0f * (s[0] * s[1] + s[0] * s[2] + s[1] * s[2]);
}

void
SoChildBVH::build(const std::vector<std::pair<int, SbBox3f>> & entries, int totalChildren)
{
  CoinZoneScopedN("SoChildBVH::build");

  builtChildCount = totalChildren;

  const int n = static_cast<int>(entries.size());
  if (n == 0) {
    nodes.clear();
    return;
  }

  // Initialize entry index permutation
  entryIndices.resize(n);
  for (int i = 0; i < n; i++) {
    entryIndices[i] = i;
  }

  // Precompute centroids (center of each child's bbox)
  std::vector<SbVec3f> centroids(n);
  for (int i = 0; i < n; i++) {
    if (!entries[i].second.isEmpty()) {
      centroids[i] = (entries[i].second.getMin() + entries[i].second.getMax()) * 0.5f;
    }
  }

  nodes.clear();
  nodes.reserve(n * 2 + 1);

  buildRecursive(0, n, entries, centroids);
}

int
SoChildBVH::buildRecursive(int begin, int end,
                            const std::vector<std::pair<int, SbBox3f>> & entries,
                            std::vector<SbVec3f> & centroids)
{
  const int count = end - begin;
  const int nodeIdx = static_cast<int>(nodes.size());
  nodes.push_back(SoChildBVHNode());

  // Compute bounding box of all entries in this range
  SbBox3f nodeBBox;
  for (int i = begin; i < end; i++) {
    const SbBox3f & box = entries[entryIndices[i]].second;
    if (!box.isEmpty()) {
      nodeBBox.extendBy(box.getMin());
      nodeBBox.extendBy(box.getMax());
    }
  }
  nodes[nodeIdx].bbox = nodeBBox;

  // Leaf node
  if (count <= MAX_LEAF_CHILDREN) {
    // For leaf, store the first child index. We'll mark it as leaf with childIndex >= 0.
    // But a leaf can have multiple entries. We handle this by making single-entry leaves
    // and letting the recursive build create them naturally.
    if (count == 1) {
      nodes[nodeIdx].childIndex = static_cast<int16_t>(entries[entryIndices[begin]].first);
      nodes[nodeIdx].rightChild = -1;
      nodes[nodeIdx].splitAxis = 0;
      return nodeIdx;
    }
    // For multi-entry leaves with few children, just split in half
    // (no SAH needed, the cost is negligible)
  }

  // SAH binning to find best split
  SbBox3f centroidBBox;
  for (int i = begin; i < end; i++) {
    centroidBBox.extendBy(centroids[entryIndices[i]]);
  }

  SbVec3f extent = centroidBBox.getMax() - centroidBBox.getMin();

  // Find longest axis
  int bestAxis = 0;
  if (extent[1] > extent[bestAxis]) bestAxis = 1;
  if (extent[2] > extent[bestAxis]) bestAxis = 2;

  // Degenerate case: all centroids at same point
  if (extent[bestAxis] < FLT_EPSILON) {
    // Split in half
    int mid = begin + count / 2;
    nodes[nodeIdx].childIndex = -1;
    nodes[nodeIdx].splitAxis = static_cast<int16_t>(bestAxis);

    // Left child (depth-first, immediately follows this node)
    buildRecursive(begin, mid, entries, centroids);
    // Right child
    int rightIdx = buildRecursive(mid, end, entries, centroids);
    nodes[nodeIdx].rightChild = rightIdx;
    return nodeIdx;
  }

  // Bin the centroids
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
    const SbBox3f & box = entries[idx].second;
    if (!box.isEmpty()) {
      bins[binIdx].bbox.extendBy(box.getMin());
      bins[binIdx].bbox.extendBy(box.getMax());
    }
    bins[binIdx].count++;
  }

  // Evaluate SAH cost for each split position
  float nodeSA = surfaceArea(nodeBBox);
  if (nodeSA < FLT_EPSILON) nodeSA = FLT_EPSILON;

  float bestCost = FLT_MAX;
  int bestSplit = -1;

  // Prefix scan from left
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

  // Suffix scan from right
  SbBox3f rightBox;
  int rightCount = 0;

  for (int i = NUM_BINS - 1; i >= 1; i--) {
    if (!bins[i].bbox.isEmpty()) {
      rightBox.extendBy(bins[i].bbox.getMin());
      rightBox.extendBy(bins[i].bbox.getMax());
    }
    rightCount += bins[i].count;
    float cost = TRAVERSAL_COST
      + INTERSECT_COST * (leftCounts[i - 1] * leftAreas[i - 1]
                          + rightCount * surfaceArea(rightBox)) / nodeSA;
    if (cost < bestCost && leftCounts[i - 1] > 0 && rightCount > 0) {
      bestCost = cost;
      bestSplit = i;
    }
  }

  // If no good split found, split in half
  if (bestSplit < 0) {
    int mid = begin + count / 2;
    nodes[nodeIdx].childIndex = -1;
    nodes[nodeIdx].splitAxis = static_cast<int16_t>(bestAxis);
    buildRecursive(begin, mid, entries, centroids);
    int rightIdx = buildRecursive(mid, end, entries, centroids);
    nodes[nodeIdx].rightChild = rightIdx;
    return nodeIdx;
  }

  // Partition entries based on best split
  float splitPos = axisMin + bestSplit / scale;
  int mid = begin;
  for (int i = begin; i < end; i++) {
    if (centroids[entryIndices[i]][bestAxis] < splitPos) {
      std::swap(entryIndices[i], entryIndices[mid]);
      std::swap(centroids[entryIndices[i]], centroids[entryIndices[mid]]);
      // Actually we swapped indices, centroids are indexed by entry index not position
      mid++;
    }
  }

  // Ensure non-empty partitions
  if (mid == begin || mid == end) {
    mid = begin + count / 2;
  }

  nodes[nodeIdx].childIndex = -1;
  nodes[nodeIdx].splitAxis = static_cast<int16_t>(bestAxis);

  // Left child (depth-first)
  buildRecursive(begin, mid, entries, centroids);
  // Right child
  int rightIdx = buildRecursive(mid, end, entries, centroids);
  nodes[nodeIdx].rightChild = rightIdx;

  return nodeIdx;
}

void
SoChildBVH::query(SoRayPickAction * action,
                   std::vector<bool> & hitChildren) const
{
  CoinZoneScopedN("SoChildBVH::query");

  if (nodes.empty()) return;

  // Stack-based BVH traversal
  int stack[64];
  int stackPtr = 0;
  stack[stackPtr++] = 0;  // root node

  while (stackPtr > 0) {
    int nodeIdx = stack[--stackPtr];
    const SoChildBVHNode & node = nodes[nodeIdx];

    // Test ray against node bbox
    if (node.bbox.isEmpty() || !action->intersect(node.bbox, TRUE)) {
      continue;
    }

    if (node.childIndex >= 0) {
      // Leaf node — mark child as hit
      hitChildren[node.childIndex] = true;
    } else {
      // Internal node — push children
      // Right child first (so left is processed first from stack)
      if (node.rightChild > 0 && stackPtr < 64) {
        stack[stackPtr++] = node.rightChild;
      }
      // Left child is always nodeIdx + 1 (depth-first layout)
      if (nodeIdx + 1 < static_cast<int>(nodes.size()) && stackPtr < 64) {
        stack[stackPtr++] = nodeIdx + 1;
      }
    }
  }
}
