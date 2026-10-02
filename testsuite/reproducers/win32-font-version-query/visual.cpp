/**************************************************************************\
 * Copyright (c) 2026 FreeCAD contributors
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

#include <Inventor/SoDB.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoFont.h>
#include <cstdio>
#include <cstdlib>

int main(int argc, char **argv) {
  if (argc < 2 || argc > 3) return 1;
  SoDB::init();
  const char scene[] =
    "#Inventor V2.1 ascii\n"
    "Separator { OrthographicCamera { position 4 0.2 18 height 5 nearDistance 1 farDistance 50 } "
    "DirectionalLight { direction -0.3 -0.4 -1 } "
    "Material { diffuseColor 0.9 0.7 0.25 } "
    "RotationXYZ { axis Y angle 0.25 } "
    "Font { name \"Arial\" size 1 } "
    "Text3 { string [\"OB8 agpq\", \"Windows GDI\"] parts ALL } }";
  SoSeparator *root = NULL;
  {
    SoInput input;
    input.setBuffer(scene, sizeof(scene)-1);
    root = SoDB::readAll(&input);
  }
  if (!root) return 2;
  root->ref();
  if (argc == 3) static_cast<SoFont *>(root->getChild(4))->name = argv[2];
  int result = 0;
  {
    const int width = 1000, height = 500;
    SoOffscreenRenderer renderer(SbViewportRegion(width, height));
    renderer.setComponents(SoOffscreenRenderer::RGB);
    renderer.setBackgroundColor(SbColor(0.05f, 0.07f, 0.12f));
    if (!renderer.render(root)) result = 3;
    else {
      const unsigned char *buffer = renderer.getBuffer();
      size_t foreground = 0;
      for (int i = 0; i < width*height; ++i)
        if (buffer[3*i] > 80 || buffer[3*i+1] > 80 || buffer[3*i+2] > 80) ++foreground;
      std::printf("Foreground pixels: %zu\n", foreground);
      if (foreground < 1000) result = 6;
      FILE *out = std::fopen(argv[1], "wb");
      if (!out) result = 4;
      else {
        std::fprintf(out, "P6\n%d %d\n255\n", width, height);
        for (int y = height-1; y >= 0; --y)
          if (std::fwrite(buffer + y*width*3, 3, width, out) != width) result = 5;
        std::fclose(out);
      }
    }
  }
  root->unref();
  SoDB::finish();
  return result;
}
