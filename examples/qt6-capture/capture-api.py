#!/usr/bin/env python3
"""Render the reproducible scenes from the Coin API reference."""

import argparse
import math
import re
import subprocess
from pathlib import Path
from tempfile import TemporaryDirectory


FRONT = "1 0 0 0"
DRAGGER_VIEW = "0.76 0.65 0 0.65"

INVENTOR_EXAMPLES = (
    ("src/nodes/SoDrawStyle.cpp", "drawstyle", FRONT),
    ("src/shapenodes/SoNurbsCurve.cpp", "nurbscurve-circle", FRONT),
    ("src/shapenodes/SoNurbsSurface.cpp", "nurbssurface", "0.935986 0.246027 0.251794 1.218495"),
    ("src/nodes/SoNurbsProfile.cpp", "nurbsprofile", "0.78417615 0.15319791 0.60133033 1.52127389"),
    ("src/shapenodes/SoAsciiText.cpp", "asciitext", FRONT),
    ("src/shapenodes/SoText3.cpp", "text3", "-0.324495 0.914906 -0.240105 1.013937"),
)

# Keep the arrows front-on and expose depth where it identifies the handle.
DRAGGER_ORIENTATIONS = {
    "centerball": "1 0 0 0.04",
    "rotatecylindrical": "1 0 0 -0.38",
    "rotatedisc": "0.55 -0.83 0 0.57",
    "scale1": "0.82 -0.57 0 0.42",
    "scale2uniform": "0.82 -0.57 0 0.42",
    "tabplane": FRONT,
    "translate1": FRONT,
    "translate2": FRONT,
}

CAMERA_SCALES = {
    "centerball": "0.76",
    "directionallight": "0.78",
    "directionallightdragger_actionshot": "0.9",
    "handlebox": "0.85",
    "jack": "0.75",
    "quadmesh": "0.7",
    "rotatecylindrical": "0.83",
    "rotatedisc": "0.83",
    "rotatespherical": "0.8",
    "scale1": "0.78",
    "scale2uniform": "0.78",
    "sosfimage": "0.87",
    "spheresheet": "0.91",
    "text3": "0.78",
    "trackball": "0.79",
    "trackball-cone": "0.79",
    "transformer": "0.78",
    "transparencytype": "0.87",
}

AUTHORED_FRAMING = {
    "nurbsprofile": "position 6.1665 -9.8383 3.6840 height 6 focalDistance 12 nearDistance 1 farDistance 25",
    "nurbssurface": "position 4.9941 -10.6959 2.8805 height 8.2 focalDistance 12 nearDistance 1 farDistance 25",
}


def example_block(source, image_name, prefix):
    comment, image, _ = source.read_text(encoding="utf-8").partition(f"\\image html {image_name}.png")
    if image:
        blocks = re.findall(r"\\(code|verbatim)\b(.*?)\\end\1", comment, re.DOTALL)
        for _, block in reversed(blocks):
            if block.lstrip().startswith(prefix):
                return block.strip()
    raise ValueError(f"Example for {image_name}.png not found in {source}")


def quad_mesh(root):
    source = root / "src/shapenodes/SoQuadMesh.cpp"
    block = example_block(source, "quadmesh", "// Vertices")
    array = re.search(r"vertices\[25\]\[3\]\s*=\s*\{(.*?)\n\s*\};", block, re.DOTALL)
    if not array:
        raise ValueError(f"Quad mesh coordinates not found in {source}")
    vertices = re.findall(r"\{\s*(-?\d+)\s*,\s*(-?\d+)\s*,\s*(-?\d+)\s*\}", array[1])
    if len(vertices) != 25:
        raise ValueError(f"Expected 25 quad mesh vertices in {source}")
    points = ", ".join(" ".join(vertex) for vertex in vertices)
    return f"""
Coordinate3 {{ point [ {points} ] }}
QuadMesh {{ verticesPerRow 5 verticesPerColumn 5 }}
"""


def volume_texture():
    # Radial bands shift with depth, so the visible pattern varies along all three axes.
    size = 16
    voxels = []
    for z in range(size):
        for y in range(size):
            for x in range(size):
                radius = math.hypot(x - 7.5, y - 7.5)
                value = round(135 + 95 * math.cos(radius * 1.8 + z * 0.7))
                voxels.append(f"0x{value:02x}")
    rows = "\n".join(" ".join(voxels[i:i + size]) for i in range(0, len(voxels), size))
    return f"""
Complexity {{ textureQuality 0.6 }}
Texture3 {{ images {size} {size} {size} 1
{rows}
}}
Cube {{}}
"""


def transparency_grid():
    modes = (
        ("ADD", "Add"), ("DELAYED_ADD", "Delayed add"), ("SORTED_OBJECT_ADD", "Sorted add"),
        ("BLEND", "Blend"), ("DELAYED_BLEND", "Delayed blend"), ("SORTED_OBJECT_BLEND", "Sorted blend"),
        ("SCREEN_DOOR", "Screen door"), ("SORTED_OBJECT_SORTED_TRIANGLE_ADD", "Triangle add"),
        ("SORTED_OBJECT_SORTED_TRIANGLE_BLEND", "Triangle blend"),
    )
    cells = []
    for i, (mode, label) in enumerate(modes):
        x, y = (i % 3 - 1) * 3.2, (1 - i // 3) * 2.45
        cells.append(f"""
Separator {{
  Translation {{ translation {x} {y} 0 }}
  Separator {{
    Translation {{ translation 0 -0.95 1.2 }}
    BaseColor {{ rgb 0.92 0.94 1 }}
    LightModel {{ model BASE_COLOR }}
    AsciiText {{ string "{label}" justification CENTER }}
  }}
  TransparencyType {{ value {mode} }}
  Separator {{
    Translation {{ translation -0.28 0.25 0.2 }}
    Material {{ diffuseColor 0.77 0.28 0.5 transparency 0.38 }}
    Cube {{ width 1.25 height 1.25 depth 0.55 }}
  }}
  Separator {{
    Translation {{ translation 0.28 -0.06 -0.28 }}
    Material {{ diffuseColor 0.12 0.74 0.8 transparency 0.42 }}
    Cube {{ width 1.25 height 1.25 depth 0.55 }}
  }}
}}
""")
    return 'Font { name "DejaVu Sans" size 0.24 }\n' + "\n".join(cells)


def scenes(root, viewer):
    for source, name, orientation in INVENTOR_EXAMPLES:
        example = example_block(root / source, name, "#Inventor")
        yield name, orientation, example.split("\n", 1)[1]

    for source in sorted((root / "src/draggers").glob("So*Dragger.cpp")):
        match = re.search(
            r'\\image html (\w+)\.png "Screen Shot of Default Dragger"',
            source.read_text(encoding="utf-8"),
        )
        if match:
            name = match[1]
            orientation = DRAGGER_ORIENTATIONS.get(name, DRAGGER_VIEW)
            yield name, orientation, f"{source.stem.removeprefix('So')} {{}}"

    yield "sosfimage", "0.64 -0.76 0 0.20", example_block(
        root / "src/fields/SoSFImage.cpp", "sosfimage", "Complexity"
    )
    # These Inventor graphs correspond to the C++ examples in SoBaseKit and SoTrackballManip.
    yield "basekitexample", DRAGGER_VIEW, "ShapeKit { appearance.material Material { diffuseColor 1 0 0 } shape Cube {} }"
    yield "trackball-cone", DRAGGER_VIEW, "TrackballManip {}\nCone {}"
    yield "quadmesh", "0.788121 0.145169 -0.598157 1.131512", quad_mesh(root)

    projector = subprocess.check_output([viewer.with_name("coin-projector-example")], text=True)
    yield "spheresheet", "1 0 0 0.46", projector.split("\n", 1)[1]

    yield "directionallightdragger_actionshot", "0.23 -0.97 0 0.37", """
DirectionalLight { intensity 0.14 }
TransformSeparator {
  Translation { translation -2.5 0 0 }
  Scale { scaleFactor 0.55 0.55 0.55 }
  DirectionalLightManip { color 1 0.08 0.18 direction 1 -0.16 -0.30 }
}
Translation { translation 4.6 0.4 0 }
Cube {}
"""
    yield "texture3", DRAGGER_VIEW, volume_texture()
    yield "transparencytype", FRONT, transparency_grid()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("viewer", type=Path, help="Built coin-example-capture executable")
    parser.add_argument("output", type=Path, help="Directory for the reference PNGs")
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[2]
    viewer = args.viewer.resolve()
    args.output.mkdir(parents=True, exist_ok=True)

    with TemporaryDirectory() as temporary:
        for name, orientation, body in scenes(root, viewer):
            scene = Path(temporary) / f"{name}.iv"
            camera_type = "OrthographicCamera"
            camera = f"orientation {orientation}"
            command = [viewer, "--scene-file", scene, "--output", args.output / f"{name}.png"]
            if name in AUTHORED_FRAMING:
                # Match the legacy view axes and frame the surface rather than its control grid.
                camera += " " + AUTHORED_FRAMING[name]
                command.append("--keep-camera")
            if name == "nurbsprofile":
                body = "Complexity { value 0.8 }\n" + body
            if name in ("quadmesh", "text3"):
                camera_type = "PerspectiveCamera"
                camera += " heightAngle " + ("0.88" if name == "quadmesh" else "0.8")
            if name in CAMERA_SCALES:
                command.extend(["--camera-scale", CAMERA_SCALES[name]])
            scene.write_text(
                f"#Inventor V2.1 ascii\n\n{camera_type} {{ {camera} }}\n{body}\n",
                encoding="utf-8",
            )
            if name == "directionallightdragger_actionshot":
                command.append("--no-headlight")
            subprocess.run(command, check=True)


if __name__ == "__main__":
    main()
