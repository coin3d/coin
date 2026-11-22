# Coin examples

## Examples with CMake targets

Set `COIN_BUILD_EXAMPLES=ON` in the [Coin build](../INSTALL) to configure the targets in [this directory](CMakeLists.txt). The build configures all four groups and requires their dependencies. The Qt example also has [standalone build instructions](qt/README.md).

| Example | What it demonstrates | Build requirements |
| --- | --- | --- |
| [Qt and Quarter](qt/qt_example.cpp) | Display a scene graph in a Qt application using `QuarterWidget`. | CMake 3.21 or newer, Coin, Quarter and Qt 5. |
| [GLFW](glfw/glfw.cpp) | Set up an OpenGL window and render a Coin scene from an application-owned loop. | Coin build, OpenGL and GLFW 3.3 or newer. |
| [SDL3](sdl3/sdl3.cpp) | Set up an SDL3 window and drive Coin rendering from the SDL event loop. | Coin build, OpenGL and SDL3. |
| [Dynamic extensions](dynloadextensions/README) | Build the `Switchboard` node and `SwitchboardOperator` engine as shared libraries. A [sample scene](dynloadextensions/scene.iv) accompanies them. | Coin build. Running the scene requires an application that supports loading the extensions. |

## Historical source examples

The files in [bindings](bindings/) and [misc](misc/) are outside the current CMake example build. Their notes may refer to the old `coin-config` command or toolkits. Build and runtime compatibility need to be checked before using these examples on a current setup.

| Source | What to reuse |
| --- | --- |
| [`glxiv.cpp`](bindings/glxiv.cpp) | Direct X11/GLX integration. The source documents limits in event translation and sensor processing. |
| [`glutiv.cpp`](bindings/glutiv.cpp) | Multiple GLUT windows and their Coin scene managers. The [GLFW example](glfw/glfw.cpp) is a starting point for an application-owned event loop. |
| [`CoinQtWidget.cpp`](bindings/CoinQtWidget.cpp) and [header](bindings/CoinQtWidget.h) | Historical Qt 3 integration through `QGLWidget`. For the Qt 5 integration in this tree, see the [Quarter example](qt/README.md). |
| [`glut_glinfo.cpp`](misc/glut_glinfo.cpp) | Inspect the version, vendor and extensions of an OpenGL context. The utility itself uses GLUT and OpenGL. |
| [`glut_tex.cpp`](misc/glut_tex.cpp) | Render a scene offscreen and use the image as a texture. See the [`SoOffscreenRenderer` reference](https://www.coin3d.org/coin/classSoOffscreenRenderer.html) for its public API. |
| [`ivcp.cpp`](misc/ivcp.cpp) | Read and write a scene without opening a viewer. A starting point for experimenting with scene file I/O. |

## Classic examples and tutorials

[**ivexamples**](https://github.com/coin3d/ivexamples) contains examples adapted for Coin from *The Inventor Mentor* and *The Inventor Toolmaker*. The repository lists compatibility by GUI binding. Sources such as `HelloCone` are CMake templates that select a binding during configuration.

| To learn | Start with |
| --- | --- |
| Build and display a first scene | [HelloCone](https://github.com/coin3d/ivexamples/blob/master/Mentor/02.1.HelloCone.cpp.in) |
| Understand why node order changes the result | [TransformOrdering](https://github.com/coin3d/ivexamples/blob/master/Mentor/05.6.TransformOrdering.cpp.in) |
| Represent geometry with indexed faces | [IndexedFaceSet](https://github.com/coin3d/ivexamples/blob/master/Mentor/05.2.IndexedFaceSet.cpp.in) |
| Understand how a ray finds an object in a scene | [PickAction](https://github.com/coin3d/ivexamples/blob/master/Mentor/09.4.PickAction.cpp.in) |
| Extend the library with a custom node | [Toolmaker node examples](https://github.com/coin3d/ivexamples/tree/master/Toolmaker/02.Nodes) |

The [Qt 6 and Quarter capture gallery](qt6-capture/README.md) shows these five scenes and documents how to regenerate the PNGs.

The [historical wiki index](https://github-wiki-see.page/m/coin3d/coin/wiki/Available-tutorials) lists SoWin tutorials for older Visual Studio versions, MFC integration and the Sc21 Cocoa tutorials. The [Coin documentation](https://www.coin3d.org/coin/coin_books.html) explains what each Inventor book covers.

## Coming from FreeCAD

Use the [FreeCAD scene graph guide](https://github.com/FreeCAD/FreeCAD-documentation/blob/main/wiki/Scenegraph.md) to see how FreeCAD represents its objects in Coin. To isolate a display or selection problem, start with the [Quarter example](qt/README.md), recreate the smallest relevant scene with Coin nodes, and compare the result with FreeCAD. The `IndexedFaceSet` and `PickAction` examples above show the geometry and selection steps.

[Issue #615](https://github.com/coin3d/coin/issues/615) is the discussion about user documentation and tutorials. The [standalone CMake example proposed in PR #925](https://github.com/coin3d/coin/pull/925) covers finding and linking an installed Coin package before adding a viewer.
