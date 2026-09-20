# SoPath auditor callback regression

The public `insertIndex()`, `removeIndex()` and `replaceIndex()` callbacks must
leave a path unchanged for an empty path, an unrelated parent, a negative
index or a null replacement. Valid callbacks emitted by `SoChildList` retain
their existing behavior.
