#include <Inventor/SoDB.h>
#include "fonts/fontspec.h"
#include "fonts/glyph2d.h"
#include "fonts/glyph3d.h"
#include <atomic>
#include <thread>
#include <vector>
#include <cstdio>

int main() {
  SoDB::init();
  cc_font_specification spec;
  cc_fontspec_construct(&spec,"defaultFont",12,0.5f);
  // Start simultaneously without priming the cache, exercising initialization.
  std::atomic<int> ready(0), failures(0);
  std::atomic<bool> go(false);
  std::vector<std::thread> workers;
  for (int i=0;i<8;++i) workers.emplace_back([&] {
    ++ready;
    while (!go.load()) std::this_thread::yield();
    for (int n=0;n<20000;++n) {
      auto *a=cc_glyph2d_ref('A',&spec,0);
      auto *b=cc_glyph2d_ref('A',&spec,0);
      int size[2],offset[2];
      const auto *pixels=cc_glyph2d_getbitmap(a,size,offset);
      if (a!=b || !pixels || size[0]<=0 || size[1]<=0) ++failures;
      cc_glyph2d_unref(a);
      cc_glyph2d_unref(b);
      auto *x=cc_glyph3d_ref('A',&spec);
      auto *y=cc_glyph3d_ref('A',&spec);
      if (x!=y || !cc_glyph3d_getcoords(x)) ++failures;
      cc_glyph3d_unref(x);
      cc_glyph3d_unref(y);
    }
  });
  while (ready.load()!=8) std::this_thread::yield();
  go=true;
  for (auto &worker:workers) worker.join();
  cc_fontspec_clean(&spec);
  // Teardown is only legal after all glyph users have finished.
  SoDB::finish();
  std::printf("Font cache: 8 threads, 160000 iterations per dimension, %d failures\n",failures.load());
  return failures ? 1 : 0;
}
