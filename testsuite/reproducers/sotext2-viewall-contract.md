# SoText2: one-call viewAll contract still open

PR #774 remains draft. Its current bounds repair does not satisfy a complete
single-call framing contract for screen-space text. The earlier statement that
#417 was fully closed must be qualified by this multi-label regression.

## Reproduction and oracle

`SoText2ViewAllAudit.cpp` reproduces BUGS.txt item 219: `XXX` at y=0, `YYY` at
y=100 and `ZZZ` at y=200, with cumulative translations. Red/green/blue distinguish
the labels without changing geometry. The oracle first renders all labels with
a manually placed camera that contains them, then counts each color's coverage.
Each post-fit label must have the same pixel count as that fully visible
reference; merely drawing one pixel is not success. Both counts are reported.

The 12 cases use perspective/orthographic cameras, builtin font sizes 10/24/96
and slack 1/1.1, in a 640x480 viewport. The initial framing camera is (0,0,1).
A separate culling control uses (0,0,5), away from the exact near-plane boundary,
expects only XXX to draw, and rejects a ray aimed at offscreen ZZZ.

Against unchanged master, one call renders one label. Against repaired #774,
one call renders two and the second renders all three. All 12 single-call
contract cases fail on both libraries. Culling controls pass on both. These are
Linux/software-GLX/builtin-font results, not a general convergence guarantee.

## Proposed final contract before implementation

For finite supported scene/viewport data, with each fixed-pixel text footprint
small enough to fit its rendered viewport, one public `viewAll()` call should
frame the full visible pixel footprint of every eligible SoText2 under the
resulting camera, along with ordinary scene geometry. Camera orientation must
remain unchanged. Drawing and picking must still reject out-of-view text before
fitting; text bounds should not silently shrink the set of labels to those
visible under the old camera.

The acceptance test is containment under the **final** camera, not a nonempty
bbox under the initial camera. Raster rounding needs an explicit pixel margin.
The existing near/far and `slack` behavior must be preserved or deliberately
specified: current concrete cameras use slack for clipping planes, not lateral
camera distance/height. Raising slack to 1.1 did not fix any case here.

Cases that cannot fit (a fixed-pixel label larger than its viewport), multiple
cameras/subviewports, camera outside the measured scene, transformed anchors,
labels behind/on the eye plane, camera subclasses, pathological projection
parameters and path overloads need explicit policy before claiming a universal
contract. The current void API has no failure result; do not invent a provisional
public API just to hide these questions.

## Comparing approaches

| Approach | Evidence | Open requirements |
| --- | --- | --- |
| Current #774: one bbox measured with initial camera | 2/3 after one call, 3/3 after two | Does not check the final-camera footprint |
| Anchor-only fitting | Same 2/3 in all 12 cases; later repetitions do not change the camera | Anchors at the edge do not reserve glyph pixels; cannot be the complete policy |
| A dedicated screen-space framing envelope | A viable design direction, not implemented here | Combine world anchors and pixel extents using final projection; handle transforms, camera subclasses, multiple viewports and impossible fits |
| Bounded iterative fitting | Repeating the current public call twice passes these 12 cases | Must validate containment, define margin and iteration/failure bounds, avoid observer-visible intermediate camera changes, preserve ordinary-geometry behavior |

Eight repeated calls do not reach identical camera values: truncation of screen
positions produces small oscillations, even while the three labels remain fully
rendered. Therefore neither exact camera equality nor a hard-coded two-call
wrapper is a sufficient stopping contract. A solver must stop when the final
footprints fit with the chosen margin, and handle failure explicitly in its
internal policy. This audit does not change SoCamera or select an algorithm.

## Effect on the 2009 culling repair

Commit 54c3cdc81fd9443add461d8c86b2ec27b38f44d2 excluded text quads outside the
current frustum from bounding boxes (COIN-4). #774 passes `visibleonly=FALSE`
from public SoText2::computeBBox, but GLRender uses the old private wrapper
(`visibleonly=TRUE`) and then SoCullElement::cullTest; ray picking also uses the
visibility-filtered quad. The focused offscreen draw/pick controls still pass.

The general bounding-box behavior nevertheless changes: offscreen labels now
contribute to aggregate bounding caches. Preserving immediate draw rejection
does not prove identical separator-level culling cost/cache behavior, or close
the original COIN-4 context. A framing-only bounds mode could keep ordinary
bbox policy, but it must be private or designed as the final public contract;
adding a temporary public mode repeats the design problem highlighted by #714.
Before publishing, test separator bbox caches, partial-frustum labels, transforms,
near/far boundaries and full scene traversal. The study does not claim those
broader checks complete.

## Build and run

This is an explicitly failing regression audit, excluded from the default build
and not registered as a passing CTest until the contract is implemented:

```sh
cmake --build /tmp/pr774-abi-fix-build --target CoinSoText2ViewAllAudit -j4
COIN_GLX_PIXMAP_DIRECT_RENDERING=1 LIBGL_ALWAYS_SOFTWARE=1 \
  xvfb-run -a /tmp/pr774-abi-fix-build/bin/CoinSoText2ViewAllAudit /tmp/viewall
# Current #774: exit 1; all 12 single-call contract cases fail.
# Use --diagnostic as the second argument to collect all results with exit 0.
# An optional third argument --anchor-only selects the anchor-only comparison.
```

Local rendering/log evidence is in `/tmp/coin-viewall-audit`. Production source,
ABI and FreeCAD are unchanged in this audit. No commits were pushed and no
GitHub comments were posted. Keep #774 draft until the contract, implementation
and single-call regression are resolved.

Sources:
- https://github.com/coin3d/coin/blob/674e74267df863dbaf50416c477bc7f918a826d8/docs/BUGS.txt#L2326
- https://github.com/coin3d/coin/commit/54c3cdc81fd9443add461d8c86b2ec27b38f44d2
- https://github.com/coin3d/coin/pull/774
- https://github.com/coin3d/coin/pull/714
